//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Level Data
//------------------------------------------------------------------------------


#ifndef LEVEL_H
#define LEVEL_H

#include <dms.h>
#include <tile.h>

// Increased from 16 (DM) to 64 (SK)
#define MAX_LEVELS	64	

struct level_t
{
	//dm_level_header header;
	sk_level_header header;	// switch to SK for more generic and tileset handling
	dm_tile map[32][32];
	unsigned char monsters[15];	
	unsigned char walls[15];		
	unsigned char floors[15];		// only 9 different gfx
	unsigned char ornates[15];	// only 12 different ornates gfx
//	char color;
	unsigned int startoffset;	// offset from beginning of dungeon.dat file
	unsigned int endoffset;		// end offset within dungeon.dat file

	unsigned int iMusicID;		// to sync with SONGLIST.DAT
};

extern char MColors[MAX_LEVELS];

typedef struct level_t* level_p;

extern void startLevels ();
extern void setLevelsFromPointer(level_p pLevels);	// used when switching context

extern level_p getLevels ();
extern level_p getLevel (int m);

extern int cycleMapMusic (int iMap, int iStep);

extern void flushLevelHeader (char level);
extern void flushLevelLayout (char level);
extern void cycleLevelGraphics (int bank, char position, char step, char curlevel);
extern void modifyLevelNumberGraphics (int bank, char l, char add);

extern void __TestRandomFloorsTiles (char level);

#endif