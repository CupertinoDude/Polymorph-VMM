/*
\\		"Polymorph"
*/
#include "polymorph.h"

/*  Tags and Table structures to Publish this Interface for Applications to use
*/
CONST struct TagItem LibEmulationTags[] =
{
		{ MIT_Name,		(ULONG)"ESYS"   			},
		{ MIT_VectorTable, (ULONG)EmulationTable	},
		{ MIT_Version,	 1						},
		{ TAG_END,		 0						}
};

CONST APTR EmulationTable[] =
{
  (APTR)&MCALL_Obtain,
  (APTR)&MCALL_Release,
  NULL,
  NULL,
  (APTR)&LCALL_Open,
  (APTR)&LCALL_Close,
  (APTR)&LCALL_Expunge,
  (APTR)&LCALL_Reserved,
/**/
  (APTR)&ECALL_ReadOctet,
  (APTR)&ECALL_WriteOctet,
  (APTR)&ECALL_ReadShortBE,
  (APTR)&ECALL_WriteShortBE,
  (APTR)&ECALL_ReadShortLE,
  (APTR)&ECALL_WriteShortLE,
  (APTR)&ECALL_ReadLongBE,
  (APTR)&ECALL_WriteLongBE,
  (APTR)&ECALL_ReadLongLE,
  (APTR)&ECALL_WriteLongLE,
/**/
  (APTR)&ECALL_ExecShort,
  (APTR)&ECALL_ExecOctet,
  (APTR)&ECALL_ExceptVector,
  (APTR)&ECALL_ExternVector,
  (APTR)&ECALL_InterpretVector,
  (APTR)&ECALL_DynamicVector,
/**/
  (APTR)-1
};

/*
#
## The above calls ***MUST*** be implimented in a Host specific fashion...
#
##	Host and Target Systems are both CPU and OS combinations
##		expansion hardware is included in OS driver simulation within plugins!
#
##	Thanks go out to
##	 "roxfan" of "##asm" on freenode
##	 "Kas1e" of OS4coding.net/AmigaWorld.net/Amigans.net in writing ""The Hacking Way: Part 1""
#
##	The calling convention upheld by this code would not be possible without the above!
#
##		Calling Convention for the ECALL_*() routines in AmigaOS4.x
#
#   *	THIS IS THE SECOND DEFINITION FOR THE PPC
#
#	*	r0	=	RESERVED 							(SYSTEM)
#	*	r1	=	RESERVED 							(SYSTEM)
#	*	r2	=	RESERVED 							(SYSTEM)
#	*	r3	=	SYSV ARG(0)	*scratch*				(Shared SYSV)
#	*	r4	=	SYSV ARG(1)	*scratch*				(Shared SYSV)
#   *	r5	=	SYSV ARG(2)	*scratch*				(Shared SYSV)
#   *	r6	=	SYSV ARG(3)	*scratch*				(Shared SYSV)
#   *	r7	=	SYSV ARG(4)	*scratch*				(Shared SYSV)
#   *	r8	=	SYSV ARG(5)	*scratch*				(Shared SYSV)
#	*	r9	=   SYSV ARG(6)	*scratch*				(Shared SYSV)
#   *	r10	=	ECALL Microcode Vector				(Polymorph Library)
#   *	r11	=	ECPU Exception Vector				(Plugin Processor)
#   *	r12	=	ECPU Execution Vector				(Plugin Processor)
#   *	r13	=	RESERVED							(SYSTEM)
#   *	r14	=   ECALL IXP InstructionPtr			(Core)
#	*	r15	=	ECPU Register[0]                    (Plugin)
#	*	r16	=	ECPU Register[1]                    (Plugin)
#	*	r17	=	ECPU Register[2]                    (Plugin)
#	*	r18	=	ECPU Register[3]                    (Plugin)
#	*	r19	=	ECPU Register[4]                    (Plugin)
#	*	r20	=	ECPU Register[5]                    (Plugin)
#	*	r21	=	ECPU Register[6]                    (Plugin)
#	*	r22	=	ECPU Register[7]                    (Plugin)
#	*	r23	=	ECPU Register[8]                    (Plugin)
#	*	r24	=	ECPU Register[9]                    (Plugin)
#	*	r25	=	ECPU Register[A]                    (Plugin)
#	*	r26	=	ECPU Register[B]                    (Plugin)
#	*	r27	=	ECPU Register[C]                    (Plugin)
#	*	r28	=	ECPU Register[D]                    (Plugin)
#	*	r29	=	ECPU Register[E]                    (Plugin)
#   *   r30	=	ECPU Register[F]                    (Plugin)
#   *	r31	=	RESERVED 							(SYSTEM)
#
##	ECALL_*() are usable directly and the SYSV ARG[] range of registers are *scratch*
##      RESERVED registers are Host system related
##		ECPU marked registers from r15 through to r30 are Plugin Defined
##		r10,r11,r12,r14 are fixed use within the Polymorph defined Environment
##		r3 through to r9 are SysV calling convention and *scratch* registers.
##
##		Any "ECPU Register[n]" register unused within a plugin can be *scratch* as well.
#
##
##	ECALL_*()s are defined below
##
*/

/*	ECALL_ReadOctet(r5:addr):
*/
asm("\n\
ECALL_ReadOctet:																	\n\
	lbz %r3,0(%r5);				##	An Address in r7 has the content loaded to r3	\n\
	blr;						##	return();										\n\
");

/*	ECALL_WriteOctet(r5:addr,r6:data):
*/
asm("\n\
ECALL_WriteOctet:																	\n\
	stb %r6,0(%r5);				##  Write from r8 to Memory using an address in r7	\n\
	blr;						##	return();										\n\
");

/*	ECALL_ReadShortBE(r5:addr):
*/
asm("\n\
ECALL_ReadShortBE:																	\n\
	lhz %r3,0(%r5);				##	An Address in r7 has the content loaded to r3	\n\
	blr;						##	return();										\n\
");

/*	ECALL_WriteShortBE(r5:addr,r6:data):
*/
asm("\n\
ECALL_WriteShortBE:																	\n\
	sth %r6,0(%r5);				##  Write from r8 to Memory using an address in r7	\n\
	blr;						##	return();										\n\
");

/*	ECALL_ReadShortLE(r5:addr):
*/
asm("\n\
ECALL_ReadShortLE:																	\n\
	lhbrx %r3,0,%r5;			##	An Address in r7 has the content loaded to r3	\n\
	blr;						##	return();										\n\
");

/*	ECALL_WriteShortLE(r5:addr,r6:data):
*/
asm("\n\
ECALL_WriteShortLE:																	\n\
	sthbrx %r6,0,%r5;			##  Write from r8 to Memory using an address in r7	\n\
	blr;						##	return();										\n\
");

/*	ECALL_ReadLongBE(r5:addr):
*/
asm("\n\
ECALL_ReadLongBE:																	\n\
	lwz %r3,0(%r5);				##	An Address in r7 has the content loaded to r3	\n\
	blr;						##	return();										\n\
");

/*	ECALL_WriteLongBE(r5:addr,r6:data):
*/
asm("\n\
ECALL_WriteLongBE:																	\n\
	stw %r6,0(%r5);				##  Write from r8 to Memory using an address in r7	\n\
	blr;						##	return();										\n\
");

/*	ECALL_ReadLongLE(r5:addr):
*/
asm("\n\
ECALL_ReadLongLE:																	\n\
	lwbrx %r3,0,%r5;			##  An Address in r7 has the content loaded to r3   \n\
	blr;						##	return();										\n\
");

/*	ECALL_WriteLongLE(r5:addr,r6:data):
*/
asm("\n\
ECALL_WriteLongLE:																	\n\
	stwbrx %r6,0,%r5;			##  Write from r8 to Memory using an address in r7	\n\
	blr;						##	return();										\n\
");

/*
**	ECALL EntryPoint for ECPU EntryPoint Execution	( 65536 OpCodes Restriction )
**
**	ECALL_ExecShort(MachineWord, ECPU[0], ECPU[1], ECPU[2], ECPU[3], offset):
**	ECPU EntryPoint mirrored
#	*	r3	=	SYSV ARG(0) = MachineWord
#	*	r4	=	SYSV ARG(1) = ECPU[0]
#   *	r5	=	SYSV ARG(2) = ECPU[1]
#   *	r6	=	SYSV ARG(3) = ECPU[2]
#   *	r7	=	SYSV ARG(4) = ECPU[3]
#	ECALL Extended
#   *	r8	=	SYSV ARG(5) = offset
#	*	r9	=   OpWord internally loaded
*/
asm("\n\
ECALL_ExecShort:																	\n\
	stwu	%r1,-16(%r1);		##	Enter the Sandman...to make a nightmare...		\n\
	mflr	%r0;				##													\n\
	stw		%r0,20(%r1);		##	Prologue...										\n\
##								##													\n\
	lhzu	%r9,0(%r14);			##	r9 = OpCode = [IXP]								\n\
	rlwinm	%r8,2,%r5,2,18;		##	r8 = (ARG:offset << 2) && 0x3FFFC				\n\
	rlwinm	%r9,2,%r9,2,18;		##	r9 = (OpCode << 2) && 0x3FFFC					\n\
	add		%r9,%r8,%r9;		##	r9 = r9+r8										\n\
	lwzx	%r9,%r9,%r12;		##	r8:OpCodeFunc() = %r8:Function[%r12:Interface]	\n\
	mtctr	%r8;				##	Set For Calling									\n\
	bctr;						##	Execute!										\n\
##								##													\n\
	isync;						##													\n\
	eieio;						##													\n\
	lwz		%r0,20(%r1);		##	Epilogue...										\n\
	addi	%r1,%r1,16;			##													\n\
	mtlr	%r0;				##													\n\
	blr;						##	return();										\n\
");

/*
**	ECALL EntryPoint for ECPU EntryPoint Execution	( 256 OpCodes Restriction )
**
**	ECALL_ExecShort(MachineWord, ECPU[0], ECPU[1], ECPU[2], ECPU[3], offset):
#	*	r3	=	SYSV ARG(0) = MachineWord       **ECPU**
#	*	r4	=	SYSV ARG(1) = ECPU[0]           **ECPU**
#   *	r5	=	SYSV ARG(2) = ECPU[1]           **ECPU**
#   *	r6	=	SYSV ARG(3) = ECPU[2]           **ECPU**
#   *	r7	=	SYSV ARG(4) = ECPU[3]           **ECPU**
#   *	r8	=	SYSV ARG(5) = offset            **ECALL**
#	*	r9	=   OpWord internally loaded        **ECALL**
*/
asm("\n\
ECALL_ExecOctet:																	\n\
	stwu	%r1,-16(%r1);		##	The Dreamer sleeps soundly one step at a time...\n\
	mflr	%r0;				##													\n\
	stw		%r0,20(%r1);		##	Prologue...										\n\
##								##													\n\
	lbzu	%r9,0(%r14);		##	r9 = OpCode = [IXP]								\n\
	rlwinm	%r8,2,%r5,2,18;		##	r8 = (ARG:offset << 2) && 0x3FFFC				\n\
	rlwinm	%r9,2,%r9,2,10;		##	r9 = (OpCode << 2) && 0x3FFFC					\n\
	add		%r9,%r8,%r9;		##	r9 = r9+r8										\n\
	lwzx	%r9,%r9,%r12;		##	r8:OpCodeFunc() = %r8:Function[%r12:Interface]	\n\
	mtctr	%r9;				##	Set For Calling									\n\
	bctr;						##	Execute!										\n\
##								##													\n\
	isync;						##													\n\
	eieio;						##													\n\
	lwz		%r0,20(%r1);		##	Epilogue...										\n\
	addi	%r1,%r1,16;			##													\n\
	mtlr	%r0;				##													\n\
	blr;						##	return();										\n\
");

/*
**	ECALL EntryPoint for ECPU EntryPoint Execution	( 256 Vectors Restriction )
**
**	ECALL_ExceptVector(r3:MachineWord,r4:ExceptionID,r5:offset):
#	*	r3	=	SYSV ARG(0) = MachineWord       **ECPU**
#	*	r4	=	SYSV ARG(1) = ExceptionID		**ECALL**
#   *	r5	=	SYSV ARG(2) = ExceptionOffset   **ECALL**
*/
asm("\n\
ECALL_ExceptVector:																	\n\
	stwu	%r1,-16(%r1);		##	To Ride Pegasus and fly through space...		\n\
	mflr	%r0;				##													\n\
	stw		%r0,20(%r1);		##	Prologue...										\n\
##								##													\n\
	rlwinm	%r6,2,%r4,2,18;		##	r6 = (ARG:exception << 2) && 0x3FC				\n\
	rlwinm	%r7,2,%r5,2,10;		##	r7 = (ARG:offset << 2) && 0x3FFFC				\n\
	add		%r6,%r6,%r7;		##	r6 = r6+r7										\n\
	lwzx	%r6,%r6,%r12;		##	r8:OpCodeFunc() = %r8:Function[%r12:Interface]	\n\
	mtctr	%r6;				##	Set For Calling									\n\
	bctr;						##	Execute!										\n\
##								##													\n\
	isync;						##													\n\
	eieio;						##													\n\
	lwz		%r0,20(%r1);		##	Epilogue...										\n\
	addi	%r1,%r1,16;			##													\n\
	mtlr	%r0;				##													\n\
	blr;						##	return();										\n\
");

/*
**	External Calling Convention (FFI specification here)
**
**	ECALL_ExternVector():
*/
asm("\n\
ECALL_ExternVector:																	\n\
	stwu	%r1,-96(%r1)		##													\n\
	mflr	%r0					##													\n\
	stw		%r0,100(%r1)		##													\n\
##								##													\n\
##								##													\n\
	isync;						##													\n\
	eieio;						##													\n\
	lwz		%r0,100(%r1)		##													\n\
	addi	%r1,%r1,96			##													\n\
	mtlr	%r0					##													\n\
	blr;						##	return();										\n\
");

/*
**	ECALL_InterpretVector():		Update the IXP to a given Address, Also Sets the IXP
*/
asm("\n\
ECALL_InterpretVector:																\n\
	stwu	%r1,-16(%r1)		##													\n\
	mflr	%r0					##													\n\
	stw		%r0,20(%r1)			##													\n\
##								##													\n\
	eieio;						##													\n\
	lwz		%r0,20(%r1)			##													\n\
	addi	%r1,%r1,16			##													\n\
	mtlr	%r0					##													\n\
	blr;						##	return();										\n\
");

/*
**	ECALL_DynamicVector():			Return the given Address Calculated
*/
asm("\n\
ECALL_DynamicVector:																\n\
	stwu	%r1,-16(%r1)		##													\n\
	mflr	%r0					##													\n\
	stw		%r0,20(%r1)			##													\n\
##								##													\n\
##								##													\n\
	eieio;						##													\n\
	lwz		%r0,20(%r1)			##													\n\
	addi	%r1,%r1,16			##													\n\
	mtlr	%r0					##													\n\
	blr;						##	return();										\n\
");

/**/
