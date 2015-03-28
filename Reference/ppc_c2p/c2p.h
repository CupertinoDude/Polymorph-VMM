/*
**  Chunky 2 Planar routine (C) 2009 Fredrik Wikstrom
**/

#ifndef C2P_H
#define C2P_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

void chunky2planar (uint8 *chunky, uint16 *planar, uint32 plane_size,
	uint32 width, uint32 height, uint32 depth);

#endif
