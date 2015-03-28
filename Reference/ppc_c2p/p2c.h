/*
**  Planar 2 Chunky routine (C) 2009 Fredrik Wikstrom
**/

#ifndef P2C_H
#define P2C_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

void planar2chunky (uint16 *planar, uint32 plane_size, uint8 *chunky,
	uint32 width, uint32 height, uint32 depth);

#endif
