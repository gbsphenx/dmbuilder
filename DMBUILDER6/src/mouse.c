//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Mouse Management
//------------------------------------------------------------------------------

#include <mouse.h>
#include <editor.h>
#include <tile.h>
#include <display.h>

#include <stdio.h>

#ifdef __MINGW__
	#include <GL/freeglut.h>
#else
	#include <GL/glut.h>	// include glut last
#endif


mouse_t Mouse;

#define MOUSE_STATE_RELEASED 1
#define MOUSE_STATE_PUSHED 0

void
Mouse_Init ()
{
	Mouse.button = 0;
	Mouse.state = 0;
	Mouse.x = 0;
	Mouse.y = 0;
}

//------------------------------------------------------------------------------

void
Mouse_CheckCell ()
{
	int iHalfTile = (__TILE_SZ_BASE__*__TILE_SCALE__)/2;
	int iFullTile = (__TILE_SZ_BASE__*__TILE_SCALE__);
	Mouse.tile_x = -1;
	{
		// __DISP_MAP_START_X__ and __DISP_MAP_START_Y__ tells the CENTER POINT of the first tile on map, then SX and SY have to be adapted to check the top left pixel

		int sx = __DISP_MAP_START_X__ - iHalfTile;
		int sy = __DISP_MAP_START_Y__ - iHalfTile;
		
		if ((Mouse.x >= sx) && (Mouse.x <= (sx + 32*iFullTile))
			&& (Mouse.y >= sy) && (Mouse.y <= (sy + 32*iFullTile)))
		{
			Mouse.tile_x = (Mouse.x - sx)/iFullTile;
			Mouse.tile_y = (Mouse.y - sy)/iFullTile;
		}

		//printf("SX = %d / SY = %d || TileX = %d / TileY = %d\n", sx, sy, Mouse.tile_x, Mouse.tile_y);
		
	}
}

void
Mouse_PaintScreen ()
{
	Mouse.pixel_x = -1;

	if (Mouse.x >= 96 && Mouse.x <= (96+17*32) && Mouse.y >= 158 && Mouse.y <= (158+17*29))
	{
		Mouse.pixel_x = (Mouse.x - 96)/17;
		Mouse.pixel_y = (Mouse.y - 158)/17;
	}
}

//------------------------------------------------------------------------------

extern unsigned tileset;



void
Mouse_Handler (int button, int state, int x, int y)
{
	//printf ("Button = %d / State = %d / (x,y) = (%d,%d) \n", button, state, x, y);
	Mouse.state = state;
	Mouse.button = button;
	Mouse.x = x;
	Mouse.y = y;
	Mouse_CheckCell ();

	if (Mouse.state == MOUSE_STATE_PUSHED)
	{
		// TILE EDITING CHECKING
		if (Mouse.tile_x != -1)
		{
			tile_p cell = getTile ((char)Mouse.tile_x, (char)Mouse.tile_y, getEditCursor (cursor_L));
			if (Mouse.button == 0) // left button
			{
				if (cell->type == 0)
				cell->type = 1; // floor
				else if (cell->type == 2)
					((tile_pit_p) cell)->open += 8;
				else if (cell->type == 3)
					((tile_stairs_p) cell)->leading += 1;
				else if (cell->type == 4)
					((tile_door_p) cell)->closed += 1;
				else if (cell->type == 5)
					((tile_teleport_p) cell)->open += 1;
				else if (cell->type == 6)
					((tile_trickwall_p) cell)->open += 1;
			}
			else if (Mouse.button == 2) // right button
			{	
				if (cell->type == 1)
					cell->type = 0; // wall
			}
			Mouse.is_filling_tiles = 1;
			setEditCursor (cursor_X, (char)Mouse.tile_x);
			setEditCursor (cursor_Y, (char)Mouse.tile_y);
		}
		// SELECT A TILE FOR FILLING
//		if (Mouse.tile_filler != -1 && !Mouse.is_filling_tiles)
//			Editor.TileFiller = Mouse.tile_filler;

	}
	else if (Mouse.state == MOUSE_STATE_RELEASED)
		Mouse.is_filling_tiles = 0;
	glutPostRedisplay ();
}

void
Mouse_Motion (int x, int y)
{
	//printf ("From Motion: x = %d, y = %d\n", x, y);
	if (Mouse.tile_x != -1);
	setEditCursor (cursor_X, (char)Mouse.tile_x);
	setEditCursor (cursor_Y, (char)Mouse.tile_y);
	Mouse_Handler (Mouse.button, Mouse.state, x, y);
}

void
Mouse_Motion_Passive (int x, int y)
{
	//printf ("From Passive Motion: x = %d, y = %d\n", x, y);
	Mouse.x = x;
	Mouse.y = y;

	// update tile under mouse selection only if not in edit tile (lock) mode
	if (!isEditingTile ())
	{

		Mouse_CheckCell();
		//if (Mouse.tile_x != -1);
		setEditCursor (cursor_X, (char)Mouse.tile_x);
		setEditCursor (cursor_Y, (char)Mouse.tile_y);
	}

	//setEditCursor (cursor_X, (char) (getEditCursor (cursor_X) + 1)); break;

	//Mouse_Handler (Mouse.button, Mouse.state, x, y);
	glutPostRedisplay ();
}

