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
##
## The above calls ***MUST*** be implimented in a Host specific fashion...
##
##	Host and Target Systems are both CPU and OS combinations
##		expansion hardware is included in OS driver simulation within plugins!
##
##	Thanks go out to
##	 "roxfan" of "##asm" on freenode
##	 "Kas1e" of OS4coding.net/AmigaWorld.net/Amigans.net in writing ""The Hacking Way: Part 1""
##
##	The calling convention upheld by this code would not be possible without the above!
##
##		Calling Convention for the ECALL_*() routines in AmigaOS4.x
##
## * r0,r1,r2,r13,r31 are reserved for system usage
## * r3-r7 are defined as volatile
## * r8-r12 are defined as ECPU call restricted access
## * r14 is defined as the ECPU/ESYS combined Flags register
## * r15-r30 are defined as ESYS private state (volatile to use in plugins)
##
##	Anything Not Defined Here is unusable within the following code!
##
## * r3		= InterfacePtr		=
## * r4-r7  = SysVargV			= Standard Argument Vector register constraints
##
## * r8	 = OpCode			= Current Instruction Selection
## * r9	 = InstructionPtr	= Instruction Address to Interpret from
## * r10	= ExceptionVector	= Plugin Provided "EXTS" Interface Vector, Exception Operations
## * r11	= OpCodeVector		= Plugin Provided "ESYS" Interface Vector, Standard Operations
## * r12	= EprocVector		= Polymorph.Library plugin Interface
## * r14	= EprocMachWord		= Shared with Plugins ("ECPU" and "ESYS" Interfaces overlap for this register only)
##

##
##	ECALL_*()s are defined below
##
##	ALL THE FOLLOWING ECALL_*() functions use the following register constraints for PowerPC Hosts
##
##		r0-r2,r13,r31	are System Reserved
##		r3-r7			are SysV compliant Argument Vector
##		r8-r12			are Core State Machine
##		r14				are Core & Plugin Shared State Machine Flags
##		r14-r30			are Plugin State Machine Reserved
##
##		r14 has 16 bit Flags defined (by Enumeration)
##
##		0x00000001		Zero/Non-Zero
##		0x00000002
##		0x00000004
##		0x00000008
##		0x00000010
##		0x00000020
##		0x00000040
##		0x00000080
##		0x00000100
##		0x00000200
##		0x00000400
##		0x00000800
##		0x00001000
##		0x00002000
##		0x00004000
##		0x00008000
##		0x00010000
##		0x00020000
##		0x00040000
##		0x00080000
##		0x00100000
##		0x00200000
##		0x00400000
##		0x00800000
##		0x01000000
##		0x02000000
##		0x04000000
##		0x08000000
##		0x10000000
##		0x20000000
##		0x40000000
##		0x80000000		Negative
##
*/

asm("\n\
##																					\n\
##	Inline Naked Functionality for GCC												\n\
##																					\n\
																					\n\
##	ECALL_ReadOctet():														  		\n\
##																			  		\n\
ECALL_ReadOctet:																	\n\
	lbz %r4,0(%r4);				##											  		\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_WriteOctet():														 		\n\
##																			  		\n\
ECALL_WriteOctet:															   		\n\
	stb %r5,0(%r4);				##											  		\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_ReadShortBE():															\n\
##																			  		\n\
ECALL_ReadShortBE:															  		\n\
	lhz %r4,0(%r4);				##											  		\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_WriteShortBE():													   		\n\
##																			  		\n\
ECALL_WriteShortBE:															 		\n\
	sth %r5,0(%r4);				##											  		\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_ReadShortLE():															\n\
##																			  		\n\
ECALL_ReadShortLE:															  		\n\
	lhbrx %r4,0,%r4;			##											  		\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_WriteShortLE():													   		\n\
##																			  		\n\
ECALL_WriteShortLE:																	\n\
	sthbrx %r5,0,%r4;			##											 		\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_ReadLongBE():														 		\n\
##																			  		\n\
ECALL_ReadLongBE:															   		\n\
	lwz %r4,0(%r4);				##											  		\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_WriteLongBE():															\n\
##																			  		\n\
ECALL_WriteLongBE:															  		\n\
	stw %r5,0(%r4);				##											  		\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_ReadLongLE():														 		\n\
##																			  		\n\
ECALL_ReadLongLE:															   		\n\
	lwbrx %r4,0,%r4;			##  An Address in r4 is replaced by the contents at that address\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_WriteLongLE():															\n\
##																			  		\n\
ECALL_WriteLongLE:															  		\n\
	stwbrx %r5,0,%r4;			##  Write from r5 to Memory using an address in r4	\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_ExecShort();																\n\
##																					\n\
ECALL_ExecShort:																	\n\
	stwu %r1,-16(%r1);			##  Enter the Sandman...to make a nightmare?		\n\
	mflr %r0;					##													\n\
	lhzu %r8,0(%r9);			##	Load the Short to Interpret and update the IXP	\n\
	stw %r0,20(%r1);			##													\n\
	rlwinm %r6,2,%r5,2,18;		##	r6 = (r4 << 2) && 0x3FFFC ARG:offset			\n\
	rlwinm %r7,2,%r8,2,18;		##	r7 = (r8 << 2) && 0x3FFFC OpCode				\n\
	add %r5,%r6,%r7;			##	OpCode+Offset after shift and mask				\n\
	lwzx %r5,%r5,%r11;			##	Load OpCodeVector[OpCode+Offset]				\n\
	mtctr %r5;					##													\n\
	bctr;						##	ECPU OpCodeVector() Execution					\n\
	lwz %r0,20(%r1);			##													\n\
	addi %r1,%r1,16;			##													\n\
	eieio;						##	Input & Output Sync								\n\
	mtlr %r0;					##													\n\
	isync;						##	Instruction Sync								\n\
	blr;						##	return();										\n\
																					\n\
##	ECALL_ExecOctet();																\n\
##																					\n\
ECALL_ExecOctet:																	\n\
	stwu %r1,-16(%r1);			##  Entering the Dreamers Nightmares...				\n\
	mflr %r0;					##													\n\
	lbzu %r8,0(%r9);			##	Load the Octet to Interpret and update the IXP	\n\
	stw %r0,20(%r1);			##													\n\
	rlwinm %r6,2,%r4,2,18;		##	r6 = (r4 << 2) && 0x3FFFC ARG:offset			\n\
	rlwinm %r7,2,%r8,2,10;		##	r7 = (r8 << 2) && 0x3FC OpCode					\n\
	add %r5,%r6,%r7;			##	OpCode+Offset after shift and mask				\n\
	lwzx %r5,%r5,%r11;			##	Load OpCodeVector[OpCode+Offset]				\n\
	mtctr %r5;					##													\n\
	bctr;						##	ECPU OpCodeVector() Execution					\n\
	lwz %r0,20(%r1);			##													\n\
	addi %r1,%r1,16;			##													\n\
	eieio;						##	Input & Output Sync								\n\
	mtlr %r0;					##													\n\
	isync;						##	Instruction Sync								\n\
	blr;						##	return();										\n\
																					\n\
##	ECALL_ExceptVector();															\n\
##																					\n\
##	This Vector is used for resolution of an Interrupt/Exception Event				\n\
##																					\n\
ECALL_ExceptVector:																	\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_ExternVector();															\n\
##																					\n\
##	This Vector is used for Emulator External Call Execution (EECE_)				\n\
##																					\n\
ECALL_ExternVector:																	\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_InterpretVector():														\n\
##																			  		\n\
##	Used for resolving Addresses within Interpretive Emulation			  			\n\
##																			  		\n\
ECALL_InterpretVector:														  		\n\
	blr;						##	return();								   		\n\
																					\n\
##	ECALL_DynamicVector():													  		\n\
##																			  		\n\
##	Used for resolving Addresses within Dynamic JIT-style Emulation	  				\n\
##																			  		\n\
ECALL_DynamicVector:																\n\
	blr;						##	return();								   		\n\
																					\n\
");

/**/
