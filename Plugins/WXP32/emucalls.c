/*
\\		"Polymorph"
*/
#include "plugin.h"

CONST struct TagItem LibEmulationTags[] =
{
	    { MIT_Name,        (ULONG)"ESYS"   			},
	    { MIT_VectorTable, (ULONG)EmulationTable	},
	    { MIT_Version,     1                    	},
	    { TAG_END,         0                    	}
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
/*Polymorph ESYS Interface* /
  (APTR)&LCALL_Reserved,
  (APTR)&LCALL_Reserved,
  (APTR)&LCALL_Reserved,
  (APTR)&LCALL_Reserved,
  (APTR)&LCALL_Reserved,
  (APTR)&LCALL_Reserved,
  (APTR)&LCALL_Reserved,
  (APTR)&LCALL_Reserved,
/ **/
  (APTR)-1
};

/*  This is the "Emulation Processor" Interface,
 *   Deliberately Defined as a Second Interface for Modularity and context seperation purposes.
 *
 *   Emulation Processors are defined using an OpWord value, and OpCodes table,
 *     the OpCodes table is similar to this interface
 *
 *   Octet is  8bits, Short is 16bits, Long  is 32bits for this implimentation
 *
 *  Polymorph will remain compliant for simulating 32bit environments at this time.
 *
 *   All Operations on this Interface are defined to use only "volatile" registers for a given host
 *     on the PPC this means r3 through to r7 exclusively
 *     on other processors this requires expanded documentation
 *
 *  PowerPC Register Usage for ALL of the following functions follows this layout!
 *
 * r0,r1,r2,r13,r31 are reserved for system usage
 * r3-r7 are defined as volatile
 * r8-r12 are defined as ECPU call restricted access (SysV Argument Registers)
 * r14 is defined as the ECPU/ESYS combined Flags register
 * r15-r30 are defined as ESYS private state (volatile to use in plugins)
 *
 * r8  = OpCode           = Current Instruction Selection
 * r9  = InstructionPtr   = Instruction Address to Interpret from
 * r10 = ExceptionVector  = Plugin Special Operations (This is usable for Interrupt Emulations)
 * r11 = OpCodeVector     = Plugin Provided "ESYS" Interface Vector
 * r12 = EprocVector      = Polymorph.Library plugin Interface
 *
 * r14 = EprocMachWord	  = Emulation Processor Conditions (Shared with the Host Software)
 *		r14 is special in being the only exclusively "shared" material for both the Polymorph and Plugin libraries when operating
 *
 * r15 = PluginRegister   =
 * r16 = PluginRegister   =
 * r17 = PluginRegister   =
 * r18 = PluginRegister   =
 * r19 = PluginRegister   =
 * r20 = PluginRegister   =
 * r21 = PluginRegister   =
 * r22 = PluginRegister   =
 * r23 = PluginRegister   =
 * r24 = PluginRegister   =
 * r25 = PluginRegister   =
 * r26 = PluginRegister   =
 * r27 = PluginRegister   =
 * r28 = PluginRegister   =
 * r29 = PluginRegister   =
 * r30 = PluginRegister   =
 *
 * NO FUNCTIONS ARE CALLED OUTSIDE POLYMORPH WITHOUT PROTECTING THE ABOVE STATE!!!
 *  Polymorph functions on the basis of the above data layout,
 * Primary Operations of any functions below **MUST RETAIN MODIFIED STATE**
 *  The compiler "optimizing" away changes will actively break this designated volatile state!
*/

extern void ECALL_AddEbGb(void);
extern void ECALL_AddEvGv(void);
extern void ECALL_AddGbEb(void);
extern void ECALL_AddGvEv(void);
extern void ECALL_AddALib(void);
extern void ECALL_AddEAXlz(void);
extern void ECALL_OrEbGb(void);
extern void ECALL_OrEvGv(void);
extern void ECALL_OrGbEb(void);
extern void ECALL_OrGvEv(void);
extern void ECALL_OrALib(void);
extern void ECALL_OrEAXlz(void);
extern void ECALL_Expand0F(void);
extern void ECALL_AdcEbGb(void);
extern void ECALL_AdcEvGv(void);
extern void ECALL_AdcGbEb(void);
extern void ECALL_AdcGvEv(void);
extern void ECALL_AdcALib(void);
extern void ECALL_AdcEAXlz(void);
extern void ECALL_SbbEbGb(void);
extern void ECALL_SbbEvGv(void);
extern void ECALL_SbbGbEb(void);
extern void ECALL_SbbGvEv(void);
extern void ECALL_SbbALib(void);
extern void ECALL_SbbEAXlz(void);
extern void ECALL_AndEbGb(void);
extern void ECALL_AndEvGv(void);
extern void ECALL_AndGbEb(void);
extern void ECALL_AndGvEv(void);
extern void ECALL_AndALib(void);
extern void ECALL_AndEAXlz(void);
extern void ECALL_ES(void);
extern void ECALL_SubEbGb(void);
extern void ECALL_SubEvGv(void);
extern void ECALL_SubGbEb(void);
extern void ECALL_SubGvEv(void);
extern void ECALL_SubALib(void);
extern void ECALL_SubEAXlz(void);
extern void ECALL_CS(void);
extern void ECALL_XorEbGb(void);
extern void ECALL_XorEvGv(void);
extern void ECALL_XorGbEb(void);
extern void ECALL_XorGvEv(void);
extern void ECALL_XorALib(void);
extern void ECALL_XorEAXlz(void);
extern void ECALL_SS(void);
extern void ECALL_CmpEbGb(void);
extern void ECALL_CmpEvGv(void);
extern void ECALL_CmpGbEb(void);
extern void ECALL_CmpGvEv(void);
extern void ECALL_CmpALib(void);
extern void ECALL_CmpEAXlz(void);
extern void ECALL_DS(void);
extern void ECALL_IncEAX(void);
extern void ECALL_IncECX(void);
extern void ECALL_IncEDX(void);
extern void ECALL_IncEBX(void);
extern void ECALL_IncESP(void);
extern void ECALL_IncEBP(void);
extern void ECALL_IncESI(void);
extern void ECALL_IncEDI(void);
extern void ECALL_DecEAX(void);
extern void ECALL_DecECX(void);
extern void ECALL_DecEDX(void);
extern void ECALL_DecEBX(void);
extern void ECALL_DecESP(void);
extern void ECALL_DecEBP(void);
extern void ECALL_DecESI(void);
extern void ECALL_DecEDI(void);
extern void ECALL_PushEAX(void);
extern void ECALL_PushECX(void);
extern void ECALL_PushEDX(void);
extern void ECALL_PushEBX(void);
extern void ECALL_PushESP(void);
extern void ECALL_PushEBP(void);
extern void ECALL_PushESI(void);
extern void ECALL_PushEDI(void);
extern void ECALL_PopEAX(void);
extern void ECALL_PopECX(void);
extern void ECALL_PopEDX(void);
extern void ECALL_PopEBX(void);
extern void ECALL_PopESP(void);
extern void ECALL_PopEBP(void);
extern void ECALL_PopESI(void);
extern void ECALL_PopEDI(void);
extern void ECALL_PushA(void);
extern void ECALL_PopA(void);
extern void ECALL_FS(void);
extern void ECALL_GS(void);
extern void ECALL_OPSIZE(void);
extern void ECALL_ADSIZE(void);
extern void ECALL_PushIz(void);
extern void ECALL_ImulGvEvIz(void);
extern void ECALL_PushIb(void);
extern void ECALL_ImulGvEvIb(void);
extern void ECALL_InSYbDX(void);
extern void ECALL_InSYzDX(void);
extern void ECALL_OutSDXXb(void);
extern void ECALL_OutSDXXz(void);
extern void ECALL_JmpOjb(void);
extern void ECALL_JmpNOjb(void);
extern void ECALL_JmpBjb(void);
extern void ECALL_JmpNBjb(void);
extern void ECALL_JmpZjb(void);
extern void ECALL_JmpNZjb(void);
extern void ECALL_JmpEjb(void);
extern void ECALL_JmpNEjb(void);
extern void ECALL_JmpSjb(void);
extern void ECALL_JmpNSjb(void);
extern void ECALL_JmpPjb(void);
extern void ECALL_JmpNPjb(void);
extern void ECALL_JmpLjb(void);
extern void ECALL_JmpNLjb(void);
extern void ECALL_JmpNGjb(void);
extern void ECALL_JmpGjb(void);
extern void ECALL_TestEbGb(void);
extern void ECALL_TestEvGv(void);
extern void ECALL_XchgEbGb(void);
extern void ECALL_XchgEvGv(void);
extern void ECALL_MoveEbGb(void);
extern void ECALL_MoveEvGv(void);
extern void ECALL_MoveGbEb(void);
extern void ECALL_MoveGvEv(void);
extern void ECALL_MoveMwSw(void);
extern void ECALL_LeaGvM(void);
extern void ECALL_MoveSwMw(void);
extern void ECALL_NOP(void);
extern void ECALL_XchgCXAX(void);
extern void ECALL_XchgDXAX(void);
extern void ECALL_XchgBXAX(void);
extern void ECALL_XchgSPAX(void);
extern void ECALL_XchgBPAX(void);
extern void ECALL_XchgSIAX(void);
extern void ECALL_XchgDIAX(void);
extern void ECALL_CWDE(void);
extern void ECALL_CDQ(void);
extern void ECALL_CallApWZ(void);
extern void ECALL_Wait(void);
extern void ECALL_PushFFv(void);
extern void ECALL_PopFFv(void);
extern void ECALL_SAHF(void);
extern void ECALL_LAHF(void);
extern void ECALL_MoveALOv(void);
extern void ECALL_MoveAXOv(void);
extern void ECALL_MoveOvAL(void);
extern void ECALL_MoveOvAX(void);
extern void ECALL_MovSYbXb(void);
extern void ECALL_MovSYvXv(void);
extern void ECALL_CmpSYbXb(void);
extern void ECALL_CmpSYvXv(void);
extern void ECALL_TestALib(void);
extern void ECALL_TestAXiz(void);
extern void ECALL_StosYbAL(void);
extern void ECALL_StosYvAX(void);
extern void ECALL_LodsALXb(void);
extern void ECALL_LodsAXXv(void);
extern void ECALL_ScasYbAL(void);
extern void ECALL_ScasYvAX(void);
extern void ECALL_MoveALib(void);
extern void ECALL_MoveCLib(void);
extern void ECALL_MoveDLib(void);
extern void ECALL_MoveBLib(void);
extern void ECALL_MoveAHib(void);
extern void ECALL_MoveCHib(void);
extern void ECALL_MoveDHib(void);
extern void ECALL_MoveBHib(void);
extern void ECALL_MoveAXib(void);
extern void ECALL_MoveCXib(void);
extern void ECALL_MoveDXib(void);
extern void ECALL_MoveBXib(void);
extern void ECALL_MoveSPib(void);
extern void ECALL_MoveBPib(void);
extern void ECALL_MoveSIib(void);
extern void ECALL_MoveDIib(void);
extern void ECALL_RetNearIw(void);
extern void ECALL_RetNear(void);
extern void ECALL_EnterIwIb(void);
extern void ECALL_Leave(void);
extern void ECALL_RetFarIw(void);
extern void ECALL_RetFar(void);
extern void ECALL_Int03(void);
extern void ECALL_SoftIntb(void);
extern void ECALL_INTO(void);
extern void ECALL_IReturn(void);
extern void ECALL_Xlat(void);
extern void ECALL_EFPU0ib(void);
extern void ECALL_EFPU1ib(void);
extern void ECALL_EFPU2ib(void);
extern void ECALL_EFPU3ib(void);
extern void ECALL_EFPU4ib(void);
extern void ECALL_EFPU5ib(void);
extern void ECALL_EFPU6ib(void);
extern void ECALL_EFPU7ib(void);
extern void ECALL_LoopNEqual(void);
extern void ECALL_LoopEqual(void);
extern void ECALL_Loop(void);
extern void ECALL_InALib(void);
extern void ECALL_InEAXib(void);
extern void ECALL_OutIbAL(void);
extern void ECALL_OutIbEAX(void);
extern void ECALL_Call(void);
extern void ECALL_Jmpjz(void);
extern void ECALL_JmpApWZ(void);
extern void ECALL_JumpJb(void);
extern void ECALL_InALDX(void);
extern void ECALL_InEAXDX(void);
extern void ECALL_OutDXAL(void);
extern void ECALL_OutDXEAX(void);
extern void ECALL_Lock(void);
extern void ECALL_Int01(void);
extern void ECALL_RepNE(void);
extern void ECALL_RepEq(void);
extern void ECALL_Halt(void);
extern void ECALL_CMC(void);
extern void ECALL_CLC(void);
extern void ECALL_STC(void);
extern void ECALL_CLI(void);
extern void ECALL_STI(void);
extern void ECALL_CLD(void);
extern void ECALL_STD(void);
extern void ECALL_LarGvEw(void);
extern void ECALL_LslGwEw(void);
extern void ECALL_CLTS(void);
extern void ECALL_JmpOjz(void);
extern void ECALL_JmpNOjz(void);
extern void ECALL_JmpBjz(void);
extern void ECALL_JmpNBjz(void);
extern void ECALL_JmpZjz(void);
extern void ECALL_JmpNZjz(void);
extern void ECALL_JmpBEjz(void);
extern void ECALL_JmpNBEjz(void);
extern void ECALL_JmpSjz(void);
extern void ECALL_JmpNSjz(void);
extern void ECALL_JmpPjz(void);
extern void ECALL_JmpNPjz(void);
extern void ECALL_JmpLjz(void);
extern void ECALL_JmpNLjz(void);
extern void ECALL_JmpLEjz(void);
extern void ECALL_JmpNLEjz(void);

/*	ECALL_Reserved Placeholder dummy function
*/
void ECALL_Reserved(void)
{
	return;
}

/*
	Undefined/Reserved Operation at the CPU level,  Also doubles as "No Operation" for any particular bytecode

	Modify this function specifically to find any **illegal** or otherwise "missing" operation codes for a particular processor,

	You *CAN*READ* the Polymorph System Registers (That is why it is part of the *published* specification)
		the PSRs are read only and available for plugin debugging/operational state management
	You *MUST*NOT* Write to the PSR register set (this WILL damage your state)

	Reserved Doubles as NOP and Template Function
*/
asm("\n\
##                                                                                              \n\
##	Inline Naked Functionality for GCC                                                          \n\
##                                                                                              \n\
ECALL_AddEbGb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AddEvGv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AddGbEb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AddGvEv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AddALib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AddEAXlz:                                                                                 \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_OrEbGb:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_OrEvGv:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_OrGbEb:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_OrGvEv:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_OrALib:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_OrEAXlz:                                                                                  \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_Expand0F:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_AdcEbGb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AdcEvGv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AdcGbEb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AdcGvEv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AdcALib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AdcEAXlz:                                                                                 \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_SbbEbGb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_SbbEvGv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_SbbGbEb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_SbbGvEv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_SbbALib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_SbbEAXlz:                                                                                 \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_AndEbGb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AndEvGv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AndGbEb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AndGvEv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AndALib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_AndEAXlz:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_ES:                                                                                       \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_SubEbGb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_SubEvGv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_SubGbEb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_SubGvEv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_SubALib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_SubEAXlz:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_CS:                                                                                       \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_XorEbGb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_XorEvGv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_XorGbEb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_XorGvEv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_XorALib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_XorEAXlz:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_SS:                                                                                       \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_CmpEbGb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_CmpEvGv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_CmpGbEb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_CmpGvEv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_CmpALib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_CmpEAXlz:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_DS:                                                                                       \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_IncEAX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_IncECX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_IncEDX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_IncEBX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_IncESP:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_IncEBP:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_IncESI:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_IncEDI:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_DecEAX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_DecECX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_DecEDX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_DecEBX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_DecESP:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_DecEBP:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_DecESI:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_DecEDI:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_PushEAX:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_PushECX:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_PushEDX:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_PushEBX:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_PushESP:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_PushEBP:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_PushESI:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_PushEDI:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_PopEAX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_PopECX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_PopEDX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_PopEBX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_PopESP:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_PopEBP:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_PopESI:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_PopEDI:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_PushA:                                                                                    \n\
	blr;                                                                                        \n\
ECALL_PopA:                                                                                     \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_FS:                                                                                       \n\
	blr;                                                                                        \n\
ECALL_GS:                                                                                       \n\
	blr;                                                                                        \n\
ECALL_OPSIZE:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_ADSIZE:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_PushIz:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_ImulGvEvIz:                                                                               \n\
	blr;                                                                                        \n\
ECALL_PushIb:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_ImulGvEvIb:                                                                               \n\
	blr;                                                                                        \n\
ECALL_InSYbDX:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_InSYzDX:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_OutSDXXb:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_OutSDXXz:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_JmpOjb:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNOjb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpBjb:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNBjb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpZjb:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNZjb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpEjb:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNEjb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpSjb:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNSjb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpPjb:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNPjb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpLjb:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNLjb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpNGjb:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpGjb:                                                                                   \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_TestEbGb:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_TestEvGv:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_XchgEbGb:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_XchgEvGv:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveEbGb:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveEvGv:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveGbEb:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveGvEv:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveMwSw:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_LeaGvM:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_MoveSwMw:                                                                                 \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_NOP:                                                                                      \n\
	blr;                                                                                        \n\
ECALL_XchgCXAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_XchgDXAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_XchgBXAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_XchgSPAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_XchgBPAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_XchgSIAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_XchgDIAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_CWDE:                                                                                     \n\
	blr;                                                                                        \n\
ECALL_CDQ:                                                                                      \n\
	blr;                                                                                        \n\
ECALL_CallApWZ:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_Wait:                                                                                     \n\
	blr;                                                                                        \n\
ECALL_PushFFv:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_PopFFv:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_SAHF:                                                                                     \n\
	blr;                                                                                        \n\
ECALL_LAHF:                                                                                     \n\
	blr;                                                                                        \n\
ECALL_MoveALOv:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveAXOv:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveOvAL:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveOvAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MovSYbXb:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MovSYvXv:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_CmpSYbXb:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_CmpSYvXv:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_TestALib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_TestAXiz:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_StosYbAL:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_StosYvAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_LodsALXb:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_LodsAXXv:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_ScasYbAL:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_ScasYvAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveALib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveCLib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveDLib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveBLib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveAHib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveCHib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveDHib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveBHib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveAXib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveCXib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveDXib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveBXib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveSPib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveBPib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveSIib:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_MoveDIib:                                                                                 \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_RetNearIw:                                                                                \n\
	blr;                                                                                        \n\
ECALL_RetNear:                                                                                  \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_EnterIwIb:                                                                                \n\
	blr;                                                                                        \n\
ECALL_Leave:                                                                                    \n\
	blr;                                                                                        \n\
ECALL_RetFarIw:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_RetFar:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_Int03:                                                                                    \n\
	blr;                                                                                        \n\
ECALL_SoftIntb:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_INTO:                                                                                     \n\
	blr;                                                                                        \n\
ECALL_IReturn:                                                                                  \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_Xlat:                                                                                     \n\
	blr;                                                                                        \n\
ECALL_EFPU0ib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_EFPU1ib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_EFPU2ib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_EFPU3ib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_EFPU4ib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_EFPU5ib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_EFPU6ib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_EFPU7ib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_LoopNEqual:                                                                               \n\
	blr;                                                                                        \n\
ECALL_LoopEqual:                                                                                \n\
	blr;                                                                                        \n\
ECALL_Loop:                                                                                     \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_InALib:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_InEAXib:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_OutIbAL:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_OutIbEAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_Call:                                                                                     \n\
	blr;                                                                                        \n\
ECALL_Jmpjz:                                                                                    \n\
	blr;                                                                                        \n\
ECALL_JmpApWZ:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JumpJb:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_InALDX:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_InEAXDX:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_OutDXAL:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_OutDXEAX:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_Lock:                                                                                     \n\
	blr;                                                                                        \n\
ECALL_Int01:                                                                                    \n\
	blr;                                                                                        \n\
ECALL_RepNE:                                                                                    \n\
	blr;                                                                                        \n\
ECALL_RepEq:                                                                                    \n\
	blr;                                                                                        \n\
ECALL_Halt:                                                                                     \n\
	blr;                                                                                        \n\
ECALL_CMC:                                                                                      \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_CLC:                                                                                      \n\
	blr;                                                                                        \n\
ECALL_STC:                                                                                      \n\
	blr;                                                                                        \n\
ECALL_CLI:                                                                                      \n\
	blr;                                                                                        \n\
ECALL_STI:                                                                                      \n\
	blr;                                                                                        \n\
ECALL_CLD:                                                                                      \n\
	blr;                                                                                        \n\
ECALL_STD:                                                                                      \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_LarGvEw:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_LslGwEw:                                                                                  \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_CLTS:                                                                                     \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
ECALL_JmpOjz:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNOjz:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpBjz:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNBjz:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpZjz:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNZjz:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpBEjz:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpNBEjz:                                                                                 \n\
	blr;                                                                                        \n\
ECALL_JmpSjz:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNSjz:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpPjz:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNPjz:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpLjz:                                                                                   \n\
	blr;                                                                                        \n\
ECALL_JmpNLjz:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpLEjz:                                                                                  \n\
	blr;                                                                                        \n\
ECALL_JmpNLEjz:                                                                                 \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
## ECALL_Reserved:                                                                              \n\
	blr;                                                                                        \n\
");

/*
 *   Emulator Call Table,  Used for Running the x86 simulation,  Each Entry in this table is a *decoded* bytecode
 *
 *   sandpile.org notation is used for name schema with other sites material for disambiguation
*/
CONST APTR LibEprocTableA[] =
{
  (APTR)&ECALL_AddEbGb,   (APTR)&ECALL_AddEvGv,   (APTR)&ECALL_AddGbEb,   (APTR)&ECALL_AddGvEv,   (APTR)&ECALL_AddALib,   (APTR)&ECALL_AddEAXlz,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_OrEbGb,    (APTR)&ECALL_OrEvGv,    (APTR)&ECALL_OrGbEb,    (APTR)&ECALL_OrGvEv,    (APTR)&ECALL_OrALib,    (APTR)&ECALL_OrEAXlz,   (APTR)&ECALL_Reserved,  (APTR)&ECALL_Expand0F,
  (APTR)&ECALL_AdcEbGb,   (APTR)&ECALL_AdcEvGv,   (APTR)&ECALL_AdcGbEb,   (APTR)&ECALL_AdcGvEv,   (APTR)&ECALL_AdcALib,   (APTR)&ECALL_AdcEAXlz,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_SbbEbGb,   (APTR)&ECALL_SbbEvGv,   (APTR)&ECALL_SbbGbEb,   (APTR)&ECALL_SbbGvEv,   (APTR)&ECALL_SbbALib,   (APTR)&ECALL_SbbEAXlz,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_AndEbGb,   (APTR)&ECALL_AndEvGv,   (APTR)&ECALL_AndGbEb,   (APTR)&ECALL_AndGvEv,   (APTR)&ECALL_AndALib,   (APTR)&ECALL_AndEAXlz,  (APTR)&ECALL_ES,        (APTR)&ECALL_Reserved,  (APTR)&ECALL_SubEbGb,   (APTR)&ECALL_SubEvGv,   (APTR)&ECALL_SubGbEb,   (APTR)&ECALL_SubGvEv,   (APTR)&ECALL_SubALib,   (APTR)&ECALL_SubEAXlz,  (APTR)&ECALL_CS,        (APTR)&ECALL_Reserved,
  (APTR)&ECALL_XorEbGb,   (APTR)&ECALL_XorEvGv,   (APTR)&ECALL_XorGbEb,   (APTR)&ECALL_XorGvEv,   (APTR)&ECALL_XorALib,   (APTR)&ECALL_XorEAXlz,  (APTR)&ECALL_SS,        (APTR)&ECALL_Reserved,  (APTR)&ECALL_CmpEbGb,   (APTR)&ECALL_CmpEvGv,   (APTR)&ECALL_CmpGbEb,   (APTR)&ECALL_CmpGvEv,   (APTR)&ECALL_CmpALib,   (APTR)&ECALL_CmpEAXlz,  (APTR)&ECALL_DS,        (APTR)&ECALL_Reserved,
  (APTR)&ECALL_IncEAX,    (APTR)&ECALL_IncECX,    (APTR)&ECALL_IncEDX,    (APTR)&ECALL_IncEBX,    (APTR)&ECALL_IncESP,    (APTR)&ECALL_IncEBP,    (APTR)&ECALL_IncESI,    (APTR)&ECALL_IncEDI,    (APTR)&ECALL_DecEAX,    (APTR)&ECALL_DecECX,    (APTR)&ECALL_DecEDX,    (APTR)&ECALL_DecEBX,    (APTR)&ECALL_DecESP,    (APTR)&ECALL_DecEBP,    (APTR)&ECALL_DecESI,    (APTR)&ECALL_DecEDI,
  (APTR)&ECALL_PushEAX,   (APTR)&ECALL_PushECX,   (APTR)&ECALL_PushEDX,   (APTR)&ECALL_PushEBX,   (APTR)&ECALL_PushESP,   (APTR)&ECALL_PushEBP,   (APTR)&ECALL_PushESI,   (APTR)&ECALL_PushEDI,   (APTR)&ECALL_PopEAX,    (APTR)&ECALL_PopECX,    (APTR)&ECALL_PopEDX,    (APTR)&ECALL_PopEBX,    (APTR)&ECALL_PopESP,    (APTR)&ECALL_PopEBP,    (APTR)&ECALL_PopESI,    (APTR)&ECALL_PopEDI,
  (APTR)&ECALL_PushA,	  (APTR)&ECALL_PopA,	  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_FS,        (APTR)&ECALL_GS,        (APTR)&ECALL_OPSIZE,    (APTR)&ECALL_ADSIZE,    (APTR)&ECALL_PushIz,    (APTR)&ECALL_ImulGvEvIz,(APTR)&ECALL_PushIb,    (APTR)&ECALL_ImulGvEvIb,(APTR)&ECALL_InSYbDX,   (APTR)&ECALL_InSYzDX,   (APTR)&ECALL_OutSDXXb,  (APTR)&ECALL_OutSDXXz,
  (APTR)&ECALL_JmpOjb,    (APTR)&ECALL_JmpNOjb,   (APTR)&ECALL_JmpBjb,    (APTR)&ECALL_JmpNBjb,   (APTR)&ECALL_JmpZjb,    (APTR)&ECALL_JmpNZjb,   (APTR)&ECALL_JmpEjb,    (APTR)&ECALL_JmpNEjb,   (APTR)&ECALL_JmpSjb,    (APTR)&ECALL_JmpNSjb,   (APTR)&ECALL_JmpPjb,    (APTR)&ECALL_JmpNPjb,   (APTR)&ECALL_JmpLjb,    (APTR)&ECALL_JmpNLjb,   (APTR)&ECALL_JmpNGjb,   (APTR)&ECALL_JmpGjb,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_TestEbGb,  (APTR)&ECALL_TestEvGv,  (APTR)&ECALL_XchgEbGb,  (APTR)&ECALL_XchgEvGv,  (APTR)&ECALL_MoveEbGb,  (APTR)&ECALL_MoveEvGv,  (APTR)&ECALL_MoveGbEb,  (APTR)&ECALL_MoveGvEv,  (APTR)&ECALL_MoveMwSw,  (APTR)&ECALL_LeaGvM,    (APTR)&ECALL_MoveSwMw,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_NOP,		  (APTR)&ECALL_XchgCXAX,  (APTR)&ECALL_XchgDXAX,  (APTR)&ECALL_XchgBXAX,  (APTR)&ECALL_XchgSPAX,  (APTR)&ECALL_XchgBPAX,  (APTR)&ECALL_XchgSIAX,  (APTR)&ECALL_XchgDIAX,  (APTR)&ECALL_CWDE,      (APTR)&ECALL_CDQ,       (APTR)&ECALL_CallApWZ,  (APTR)&ECALL_Wait,      (APTR)&ECALL_PushFFv,   (APTR)&ECALL_PopFFv,    (APTR)&ECALL_SAHF,      (APTR)&ECALL_LAHF,
  (APTR)&ECALL_MoveALOv,  (APTR)&ECALL_MoveAXOv,  (APTR)&ECALL_MoveOvAL,  (APTR)&ECALL_MoveOvAX,  (APTR)&ECALL_MovSYbXb,  (APTR)&ECALL_MovSYvXv,  (APTR)&ECALL_CmpSYbXb,  (APTR)&ECALL_CmpSYvXv,  (APTR)&ECALL_TestALib,  (APTR)&ECALL_TestAXiz,  (APTR)&ECALL_StosYbAL,  (APTR)&ECALL_StosYvAX,  (APTR)&ECALL_LodsALXb,  (APTR)&ECALL_LodsAXXv,  (APTR)&ECALL_ScasYbAL,  (APTR)&ECALL_ScasYvAX,
  (APTR)&ECALL_MoveALib,  (APTR)&ECALL_MoveCLib,  (APTR)&ECALL_MoveDLib,  (APTR)&ECALL_MoveBLib,  (APTR)&ECALL_MoveAHib,  (APTR)&ECALL_MoveCHib,  (APTR)&ECALL_MoveDHib,  (APTR)&ECALL_MoveBHib,  (APTR)&ECALL_MoveAXib,  (APTR)&ECALL_MoveCXib,  (APTR)&ECALL_MoveDXib,  (APTR)&ECALL_MoveBXib,  (APTR)&ECALL_MoveSPib,  (APTR)&ECALL_MoveBPib,  (APTR)&ECALL_MoveSIib,  (APTR)&ECALL_MoveDIib,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_RetNearIw, (APTR)&ECALL_RetNear,   (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_EnterIwIb, (APTR)&ECALL_Leave,     (APTR)&ECALL_RetFarIw,  (APTR)&ECALL_RetFar,    (APTR)&ECALL_Int03,     (APTR)&ECALL_SoftIntb,  (APTR)&ECALL_INTO,      (APTR)&ECALL_IReturn,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Xlat,      (APTR)&ECALL_EFPU0ib,   (APTR)&ECALL_EFPU1ib,   (APTR)&ECALL_EFPU2ib,   (APTR)&ECALL_EFPU3ib,   (APTR)&ECALL_EFPU4ib,   (APTR)&ECALL_EFPU5ib,   (APTR)&ECALL_EFPU6ib,   (APTR)&ECALL_EFPU7ib,
  (APTR)&ECALL_LoopNEqual,(APTR)&ECALL_LoopEqual, (APTR)&ECALL_Loop,      (APTR)&ECALL_Reserved,  (APTR)&ECALL_InALib,    (APTR)&ECALL_InEAXib,   (APTR)&ECALL_OutIbAL,   (APTR)&ECALL_OutIbEAX,  (APTR)&ECALL_Call,      (APTR)&ECALL_Jmpjz,     (APTR)&ECALL_JmpApWZ,   (APTR)&ECALL_JumpJb,    (APTR)&ECALL_InALDX,    (APTR)&ECALL_InEAXDX,   (APTR)&ECALL_OutDXAL,   (APTR)&ECALL_OutDXEAX,
  (APTR)&ECALL_Lock,      (APTR)&ECALL_Int01,     (APTR)&ECALL_RepNE,     (APTR)&ECALL_RepEq,     (APTR)&ECALL_Halt,      (APTR)&ECALL_CMC,       (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_CLC,       (APTR)&ECALL_STC,       (APTR)&ECALL_CLI,       (APTR)&ECALL_STI,       (APTR)&ECALL_CLD,       (APTR)&ECALL_STD,       (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved
};

/*  Emulator Call Table,  Used for Running the x86 simulation,  Each Entry in this table is a *decoded* bytecode, 0F prefixed
*
CONST APTR LibEprocTableB[] =
{
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_LarGvEw,   (APTR)&ECALL_LslGwEw,   (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_CLTS    ,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_JmpOjz,    (APTR)&ECALL_JmpNOjz,   (APTR)&ECALL_JmpBjz,    (APTR)&ECALL_JmpNBjz,   (APTR)&ECALL_JmpZjz,    (APTR)&ECALL_JmpNZjz,   (APTR)&ECALL_JmpBEjz,   (APTR)&ECALL_JmpNBEjz,  (APTR)&ECALL_JmpSjz,    (APTR)&ECALL_JmpNSjz,   (APTR)&ECALL_JmpPjz,    (APTR)&ECALL_JmpNPjz,   (APTR)&ECALL_JmpLjz,    (APTR)&ECALL_JmpNLjz,   (APTR)&ECALL_JmpLEjz,   (APTR)&ECALL_JmpNLEjz,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved
};
*/

/**/
