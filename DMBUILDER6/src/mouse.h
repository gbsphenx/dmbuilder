//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Mouse Management
//------------------------------------------------------------------------------

#ifndef MOUSE_H
#define MOUSE_H


typedef struct
{
	// first 4 mouse stats
	int button;
	int state;
	int x;
	int y;
	// then other useful stuff
	short tile_x;	// -1 means that mouse doesn't point any tile.
	short tile_y;
	short select_map; // -1 if mouse is not on the numbers
	short tile_filler;	// select a tile

	// paint
	short pixel_x;
	short pixel_y;
	char pixel_color;
	char pixel_erasor_color;


	// booleans
	int is_filling_tiles:1;	// when mouse goes on the level selection,
							// this will prevent to change the level during editing.
	int is_filling_pixels:1;

} mouse_t;

extern mouse_t Mouse;

extern void Mouse_Init ();
extern void Mouse_Handler (int button, int state, int x, int y);

extern void Mouse_Motion (int x, int y);
extern void Mouse_Motion_Passive (int x, int y);


#endif
