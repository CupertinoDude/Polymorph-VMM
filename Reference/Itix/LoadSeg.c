static struct hunk *AddEmulHunk(struct LoaderLibrary *LibBase, ULONG *funcarray)
{
	struct emulhunk	*emul;
	struct DosLibrary	*DOSBase;

#if 0
	REG_D0	= sizeof(struct emulhunk);
	REG_D1	= MEMF_ANY;
	REG_A6	= (ULONG)LibBase->MySysBase;
	emul	= (struct emulhunk *)MyEmulHandle->CallDirect68k(funcarray[1]);
#else
	emul	= AllocMem(sizeof(struct emulhunk), MEMF_ANY);
#endif

	if (emul)
	{
#ifdef DEBUG
		NewRawDoFmt("[OS4LOADER] Emul hunk allocated from 0x%8x\n", (APTR)1, NULL, emul);
#endif
		emul->next						= 0;
		emul->size						= sizeof(struct emulhunk);
		emul->Trap						= TRAP_LIB << 16;
		emul->TrapAddr					= &emul->Code;
		emul->Launcher					= &Launcher;
#if 0
		emul->opcode					= 0x227c;			// move.l #xxxx,a1
		emul->trap						= TRAP_LIB;
		emul->trapext					= 0;
		emul->addr						= (ULONG)&Launcher;
#endif
		emul->RomTag.rt_MatchWord	= 0;
		emul->RomTag.rt_Init			= NULL;
		CopyMemQuick(&launcher_patch, &emul->Code, LAUNCHER_PATCH_SIZE);
		return (struct hunk *)emul;
	}

	DOSBase	= LibBase->MyDOSBase;

	SetIoErr(ERROR_NO_FREE_STORE);
	return NULL;
}

/**********************************************************************
	LIB_LoadSeglist
**********************************************************************/

BPTR LIB_LoadSeglist(struct LoaderLibrary *LibBase, BPTR file, LONG *funcarray, ULONG array, ULONG stack)
{
	struct elfheader  eh;
	struct sheader   *sh;
	BPTR   hunks;

	// Read ELF headers

	sh		= LoadHeaders(LibBase, file, &eh);
	hunks = 0;

	if (sh)
	{
		struct DosLibrary	*DOSBase = LibBase->MyDOSBase;
		struct hunk	*link, *start, *next;

		// Create 68k stub

		link = AddEmulHunk(LibBase, funcarray);

		if (link)
		{
			struct emulhunk	*emul;
			ULONG	mainentry, i;
			LONG	startaddr;

			startaddr	 = -1;
			mainentry	 = 0;
			start			 = link;
			emul			 = (struct emulhunk *)link;
			hunks			 = (BPTR)MKBADDR(link);
			hunks			+= (BPTR)MKBADDR(4);

			// Load execution related sections first

			for (i = 0; i < eh.shnum; i++)
			{
				struct sheader	*s	= &sh[i];

				D(bug("Section type is 0x%08.8lx, size 0x%08.8lx, flags 0x%08.8lx, index %ld\n", s->type, s->size, s->flags, i));

				// Section is either symbol table, string table, or reloc

				if ((s->flags & SHF_ALLOC) == 0 && (s->type == SHT_SYMTAB || s->type == SHT_STRTAB || s->type == SHT_RELA || s->type == SHT_REL))
				{
					APTR addr;

					addr	= AllocTaskPooled(s->size);

					if (!addr)
					{
						D(bug("out of memory\n"));
						SetIoErr(ERROR_NO_FREE_STORE);
						goto error;
					}

					s->addr = addr;

					if (ReadBlock(file, s->offset, addr, s->size, DOSBase) == 0)
						goto error;
				}
				else	if (s->flags & SHF_ALLOC)
				{
					D(bug("sh[i].addr 0x%08.8lx\n", (ULONG)s->addr));

					if (startaddr < 0)
					{
						startaddr	= (ULONG)s->addr;
						D(bug("start addr is 0x%08.8lx\n", (ULONG)&startaddr));
					}

					if (s->size)
					{
						struct hunk *newhunk;

						newhunk	= LoadHunk(file, s, DOSBase);

						if (newhunk == NULL)
							goto error;

						link->next	= (BPTR)newhunk;
						link			= newhunk;
					}
				}
			}

			// Relocate sections
			D(bug("Relocate()\n"));

			for (i = 0; i < eh.shnum; i++)
			{
				struct sheader	*s	= &sh[i];

				if (s->type == SHT_RELA || s->type == SHT_REL)
				{
					ULONG	ok = relocate(&eh, sh, i, DOSBase);

					if (ok == 0)
					{
						goto error;
					}
				}
			}

			// find _start()
			D(bug("find _start()\n"));
			for (i = 0; !mainentry && i < eh.shnum; i++)
			{
				struct sheader	*s	= &sh[i];

				if (s->type == SHT_SYMTAB)
				{
					struct symbol *symtab   = (struct symbol *)s->addr;
					ULONG	size, j;

					size	= s->size / 16;

					for (j = 0; j < size; j++)
					{
						if ((symtab->info >> 4) == 1)
						{
							ULONG	type = symtab->info & 0xf;

							if (type == 0 || type == 2) 		// i.e. <_start>
							{
								STRPTR name	= sh[ s->link ].addr + symtab->name;

								//D(bug("DEF: %s\n", name));

								if (!strcmp(name, "_start"))
								{
									mainentry = symtab->value - startaddr;
									D(bug("Patch startup code\n"));
									patch_startup(sh, eh.shnum, (UWORD *)(start->next + 8 + mainentry));
									break;
								}
							}
						}

						symtab++;
					}
				}
			}

			D(bug("Check ramlib task\n"));

			if (FindTask(NULL) == LibBase->RamLibTask)
			{
				struct OS4_Resident	*r	= NULL;

				for (i = 0; i < eh.shnum; i++)
				{
					struct sheader	*s	= &sh[i];

					if (s->type == SHT_PROGBITS)
					{
						D(bug("Check romtag\n"));
						r	= OS4_FindLibHeader68k(s->addr, s->size);

						if (r)
						{
							if (r->rt_Flags & RTF_OS4_AUTOINIT)
							{
								D(bug("Found OS4 library.\n"));
								if (OS4_BuildLibraryTable(LibBase, file, emul, r) == 0)
								{
									D(bug("Couldnt build library\n"));
									goto error;
								}

								break;
							}
							else
							{
								D(bug("Only autoinit libraries supported.\n"));
								r	= NULL;
							}
						}
					}
				}

				if (r == NULL)
				{
					D(bug("no resident tag\n"));
					goto error;
				}
			}

			next	= (struct hunk *)start->next;

			D(bug("Base start address is 0x%08.8lx\n", (ULONG)&start->next));

			if (!mainentry)
			{
				mainentry = (ULONG)sh[0].addr;
			}

			emul->Starter	= (APTR)start->next + 8 + mainentry;

			//patch_startup(sh, eh.shnum, (UWORD *)emul->Starter);

			link	= next;

			while (next)
			{
				start->next	 = (BPTR)MKBADDR(next) + 1;
				start			 = next;
				next			 = (struct hunk *)start->next;
			}

			OS4_AddNativeSegment(LibBase, (IPTR)&link->data, link->size);
			CacheFlushDataInstArea(emul->Code, LAUNCHER_PATCH_SIZE);
			D(bug("Execute from 0x%08.8lx\n", (ULONG)&emul->Starter));

			LIB_FlushCaches(sh, eh.shnum);
			SetIoErr(0);
			goto end;

error:
			if (hunks)
			{
				// Fix hunk pointers

				struct hunk	*next = (struct hunk *)start->next;

				while (next)
				{
					start->next	 = (BPTR)MKBADDR(next) + 1;
					start			 = next;
					next			 = (struct hunk *)start->next;
				}
			}

			LIB_UnLoadSeg(LibBase, hunks);
			hunks = 0;

end:
			for (i = 0; i < eh.shnum; i++)
			{
				struct sheader	*s	= &sh[i];

				if (s->addr)
				{
					if (sh[i].type == SHT_SYMTAB || sh[i].type == SHT_STRTAB || sh[i].type == SHT_RELA || sh[i].type == SHT_REL)
					{
						FreeTaskPooled(sh[i].addr, sh[i].size);
					}
				}
			}
		}

		FreeVecTaskPooled(sh);
	}

	return hunks;
}
