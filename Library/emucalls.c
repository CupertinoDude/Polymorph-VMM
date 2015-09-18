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
#	*	r3	=	SYSV ARG(0) OpWord/Result           (Shared SYSV)
#	*	r4	=	SYSV ARG(1) ECALL Interface (HOST)  (Shared SYSV)
#	*	r5	=	SYSV ARG(2) ECALL Interface (ECPU)  (Shared SYSV)
#   *	r6	=	SYSV ARG(3) ECALL Interface (ESYS)  (Shared SYSV)
#   *	r7	=	SYSV ARG(4) Input[0]                (Shared SYSV)
#   *	r8	=	SYSV ARG(5) Input[1]                (Shared SYSV)
#   *	r9	=	SYSV ARG(6) Input[2]                (Shared SYSV)
#   *	r10	=	SYSV ARG(7) Input[3]                (Shared SYSV)
#   *	r11	=	ECPU Procedure Vector				(Core)
#   *	r12	=	ECPU Exception Vector               (Core)
#   *	r13	=	RESERVED 							(SYSTEM)
#   *	r14	=   InstructionPtr						(Core)
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
##
##	ECALL_*()s are defined below
##
*/

/*	ECALL_ReadOctet():
*/
asm("\n\
ECALL_ReadOctet:																	\n\
	lbz %r3,0(%r7);				##	An Address in r7 has the content loaded to r3	\n\
	blr;						##	return();										\n\
");

/*	ECALL_WriteOctet():
*/
asm("\n\
ECALL_WriteOctet:																	\n\
	stb %r8,0(%r7);				##  Write from r8 to Memory using an address in r7	\n\
	blr;						##	return();										\n\
");

/*	ECALL_ReadShortBE():
*/
asm("\n\
ECALL_ReadShortBE:																	\n\
	lhz %r3,0(%r7);				##	An Address in r7 has the content loaded to r3	\n\
	blr;						##	return();										\n\
");

/*	ECALL_WriteShortBE():
*/
asm("\n\
ECALL_WriteShortBE:																	\n\
	sth %r8,0(%r7);				##  Write from r8 to Memory using an address in r7	\n\
	blr;						##	return();										\n\
");

/*	ECALL_ReadShortLE():
*/
asm("\n\
ECALL_ReadShortLE:																	\n\
	lhbrx %r3,0(%r7);			##	An Address in r7 has the content loaded to r3	\n\
	blr;						##	return();										\n\
");

/*	ECALL_WriteShortLE():
*/
asm("\n\
ECALL_WriteShortLE:																	\n\
	sthbrx %r8,0(%r7);			##  Write from r8 to Memory using an address in r7	\n\
	blr;						##	return();										\n\
");

/*	ECALL_ReadLongBE():
*/
asm("\n\
ECALL_ReadLongBE:																	\n\
	lwz %r3,0(%r7);				##	An Address in r7 has the content loaded to r3	\n\
	blr;						##	return();										\n\
");

/*	ECALL_WriteLongBE():
*/
asm("\n\
ECALL_WriteLongBE:																	\n\
	stw %r8,0(%r7);				##  Write from r8 to Memory using an address in r7	\n\
	blr;						##	return();										\n\
");

/*	ECALL_ReadLongLE():
*/
asm("\n\
ECALL_ReadLongLE:																	\n\
	lwbrx %r3,0(%r7);			##  An Address in r7 has the content loaded to r3   \n\
	blr;						##	return();										\n\
");

/*	ECALL_WriteLongLE():
*/
asm("\n\
ECALL_WriteLongLE:																	\n\
	stwbrx %r8,0(%r7);			##  Write from r8 to Memory using an address in r7	\n\
	blr;						##	return();										\n\
");

/*	ECALL_ExecShort():
*/
asm("\n\
ECALL_ExecShort:																	\n\
	stwu	%r1,-16(%r1)		##	Enter the Sandman...to make a nightmare...		\n\
	mflr	%r0					##													\n\
	stw		%r0,20(%r1)			##	Prologue...										\n\
##								##													\n\
	lhzu	%r3,0(%r14)			##													\n\
##								##													\n\
	lwz		%r0,20(%r1)			##	Epilogue...										\n\
	addi	%r1,%r1,16			##													\n\
	eieio;						##													\n\
	mtlr	%r0					##													\n\
	isync;						##													\n\
	blr;						##	return();										\n\
");

/*	ECALL_ExecOctet():
*/
asm("\n\
ECALL_ExecOctet:																	\n\
	stwu	%r1,-16(%r1)		##	The Dreamer sleeps soundly one step at a time...\n\
	mflr	%r0					##													\n\
	stw		%r0,20(%r1)			##	Prologue...										\n\
##								##													\n\
	lbzu	%r3,0(%r14)			##													\n\
##								##													\n\
	lwz		%r0,20(%r1)			##	Epilogue...										\n\
	addi	%r1,%r1,16			##													\n\
	eieio;						##													\n\
	mtlr	%r0					##													\n\
	isync;						##													\n\
	blr;						##	return();										\n\
");

/*	ECALL_ExceptVector():
*/
asm("\n\
ECALL_ExceptVector:																	\n\
	stwu	%r1,-16(%r1)		##													\n\
	mflr	%r0					##													\n\
	stw		%r0,20(%r1)			##													\n\
##								##													\n\
	lwz		%r0,20(%r1)			##													\n\
	addi	%r1,%r1,16			##													\n\
	eieio;						##													\n\
	mtlr	%r0					##													\n\
	isync;						##													\n\
	blr;						##	return();										\n\
");

/*	ECALL_ExternVector():
*/
asm("\n\
ECALL_ExternVector:																	\n\
	stwu	%r1,-16(%r1)		##													\n\
	mflr	%r0					##													\n\
	stw		%r0,20(%r1)			##													\n\
##								##													\n\
	lwz		%r0,20(%r1)			##													\n\
	addi	%r1,%r1,16			##													\n\
	eieio;						##													\n\
	mtlr	%r0					##													\n\
	isync;						##													\n\
	blr;						##	return();										\n\
");

/*	ECALL_InterpretVector():
*/
asm("\n\
ECALL_InterpretVector:																\n\
	stwu	%r1,-16(%r1)		##													\n\
	mflr	%r0					##													\n\
	stw		%r0,20(%r1)			##													\n\
##								##													\n\
	lwz		%r0,20(%r1)			##													\n\
	addi	%r1,%r1,16			##													\n\
	eieio;						##													\n\
	mtlr	%r0					##													\n\
	isync;						##													\n\
	blr;						##	return();										\n\
");

/*	ECALL_DynamicVector():
*/
asm("\n\
ECALL_DynamicVector:																\n\
	stwu	%r1,-16(%r1)		##													\n\
	mflr	%r0					##													\n\
	stw		%r0,20(%r1)			##													\n\
##								##													\n\
	lwz		%r0,20(%r1)			##													\n\
	addi	%r1,%r1,16			##													\n\
	eieio;						##													\n\
	mtlr	%r0					##													\n\
	isync;						##													\n\
	blr;						##	return();										\n\
");

/**/
