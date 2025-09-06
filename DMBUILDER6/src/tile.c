//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Tile
//------------------------------------------------------------------------------

#include <tile.h>
#include <level.h>
#include <editor.h>
#include <dm.h>

static char static_wall = 0x00; // has to be interpreted then as a dm_tile

tile_p
getTile (char x, char y, char l)
{
	if (x < 0 || x > getLevels()[l].header.xDim ||
		y < 0 || y > getLevels()[l].header.yDim)
		return (tile_p) &static_wall;
	else
		return &(getLevels()[l]).map[x][y];
}

tile_p
getCurrentTile ()
{
	return getTile (getEditCursor (cursor_X), getEditCursor (cursor_Y), getEditCursor (cursor_L));
}

void
switchTile ()
{
	tile_p tile = getTile (getEditCursor(cursor_X), getEditCursor(cursor_Y), getEditCursor(cursor_L));
	switch (tile->type)
	{
	case tile_Wall:
		tile->type = tile_Floor; break;
	case tile_Floor:
		tile->type = tile_Wall; break;
	case tile_Door:
		((tile_door_p) tile)->closed += 1; break;
	
	}
}

void
switchTileFunction (tile_p tile, unsigned char function)
{
	if (tile->function & function)
		tile->function -= function;
	else
		tile->function += function;
}

void
putDoorTile (char x, char y, char l)
{
	tile_door_p door = (tile_door_p) getTile (x, y, l);
	door->type = tile_Door;
	door->closed = 1;
	door->object = 0;
	door->function = 0;
	/*if (getTile (x, y-1, l)->type == tile_Wall)
		door->facing = 1;
	else door->facing = 0;
	*/door->facing = (getTile (x, (char) (y-1), l)->type == tile_Wall)?0:1;

}