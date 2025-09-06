//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Tile
//------------------------------------------------------------------------------

#ifndef TILE_H
#define TILE_H

//------------------------------------------------------------------------------

/* tiles, the function is differently interpreted wether the tile type
 * size = 1 byte
 */

/* basic tile structure */
typedef struct
{
	unsigned char function:4;
	unsigned char object:1;
	unsigned char type:3;
} dm_tile;

typedef dm_tile* tile_p;

typedef struct 
{
	unsigned char imaginary:1;
	unsigned char unk1:1; // no obvious meaning
	unsigned char invisible:1; // cumulative with imaginary
	unsigned char open:1; // 0 is closed
	unsigned char object:1;
	unsigned char type:3; // should be = 2
} dm_tile_pit;

typedef dm_tile_pit* tile_pit_p;

typedef struct 
{
	unsigned char unk2:2;
	unsigned char leading:1; // 0 is down
	unsigned char facing:1; // 0 is west/east
	unsigned char object:1;
	unsigned char type:3; // should be = 3
} dm_tile_stairs;

typedef dm_tile_stairs* tile_stairs_p;

typedef struct
{
	unsigned char function:2; // different meanings
	unsigned char closed:1; // 0 is open
	unsigned char facing:1; // 0 is west/east
	unsigned char object:1;
	unsigned char type:3; // should be = 4
} dm_tile_door;

typedef dm_tile_door* tile_door_p;

typedef struct 
{
	unsigned char unk2:2;
	unsigned char bluehaze:1;
	unsigned char open:1; // 0 is closed 
	unsigned char object:1;
	unsigned char type:3; // should be = 5
} dm_tile_teleport;

typedef dm_tile_teleport* tile_teleport_p;

typedef struct
{
	unsigned char imaginary:1;
	unsigned char unk2:1;
	unsigned char open:1; // 0 is closed 
	unsigned char unk1:1;
	unsigned char object:1;
	unsigned char type:3; // should be = 6
} dm_tile_trickwall;

typedef dm_tile_trickwall* tile_trickwall_p;

//------------------------------------------------------------------------------

extern tile_p getTile (char x, char y, char l);
extern tile_p getCurrentTile ();

extern void putDoorTile (char x, char y, char l);

extern void switchTile ();
extern void switchTileFunction (tile_p tile, unsigned char function);

#endif