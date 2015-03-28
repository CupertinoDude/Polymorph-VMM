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
  (APTR)&ECALL_LoadProgramFile,		// Load an Executable to Memory
  (APTR)&ECALL_SaveProgramFile,		// Save an Executable to Disk
  (APTR)&ECALL_InitEmulation,		// Create the Emulation Environment
  (APTR)&ECALL_ExitEmulation,		// Delete the Emulation Environment
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

/*
	Undefined/Reserved Operation at the CPU level,  Also doubles as "No Operation" for any particular bytecode

	Modify this function specifically to find any **illegal** or otherwise "missing" operation codes for a particular processor,

	You *CAN*READ* the Polymorph System Registers (That is why it is part of the *published* specification,
		the PSRs are read only and available for plugin debugging/operational state management
*/
void ECALL_Reserved(void)
{
	return;
}

/*
 *   Emulator Call Table,  Used for Running the Java Processor ByteCodes (**DECODED** ByteCodes are selected from this table!!)
*/
CONST APTR LibEprocTableA[] =
{
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,   (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,   (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,   (APTR)&ECALL_Reserved,   (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,  (APTR)&ECALL_Reserved,
};

/* Template Function for Operation basics
*/
void ECALL_Template(void)
{
	__asm("nop;\n\t"
		  "nop;\n\t"
		);
	return;
}

/*
*/
