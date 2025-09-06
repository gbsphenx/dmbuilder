//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Display
//------------------------------------------------------------------------------

#ifndef DISPLAY_H
#define DISPLAY_H

#define __TILE_SZ_BASE__	16
#define __TILE_GL_BASE__	32			// GL here needs x2 size for display, keep SZ as logical size
#define __TILE_SCALE__		1.5f

#define __STD_STACK_SIZE__	128			// old was 64


#define __DISP_MAP_START_X__	80
#define __DISP_MAP_START_Y__	80


extern void redrawScreen ();

#endif