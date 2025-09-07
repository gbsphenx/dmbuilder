//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Level Data
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <assert.h>
#include <level.h>
#include <dm.h>
//#include <dms.h>

level_p LEVELS;

char MColors[MAX_LEVELS];

extern int SKULLKEEP;

// current dungeon type max
int dt_Door_Max = door_Max;	// door max
int dt_Monster_Max = monster_Max;
int dt_Wall_Max = wall_Max;
int dt_Floor_Max = floor_Max;
int dt_Ornate_Max = ornate_Max;

int dt_Weapons_Max = weapon_Max;
int dt_Clothing_Max = clothing_Max;
int dt_Misc_Max = misc_Max;
int dt_Containers_Max = 1;

void 
setLevelsFromPointer(level_p pLevels)
{
	assert (pLevels != NULL);
	LEVELS = pLevels;
}

level_p
getLevels ()
{
	assert (LEVELS != NULL);
	return LEVELS;
}

level_p
getLevel(int m)
{
	assert (LEVELS != NULL);
	return (level_p) &(getLevels()[m]);
}

void
flushLevelLayout (char level)
{
	char i, j;
	for (i = 0; i < 32; i ++)
		for (j = 0; j < 32; j ++)
			*((char*) (&LEVELS[level].map[i][j])) = 0x00;
}

void
__TestRandomFloorsTiles (char level)
{
	char i, j;
	for (i = 0; i < 32; i ++)
		for (j = 0; j < 32; j ++)
			*((char*) (&LEVELS[level].map[i][j])) = 0x28;
}

void
flushLevelHeader (char level)
{
	dm_level_header* header = (dm_level_header*) &(LEVELS[level].header);
	header->xOffset = 0;
	header->yOffset = 0;
	header->xDim = 31;
	header->yDim = 31;
	header->level = level;		// stacks the current level under the previous one
	header->depth = 0;
	header->door1 = door_IronGate;	// wooden
	header->door2 = door_Wooden;	// iron gate
	header->nMonsters = 0;
	header->nWalls = 0;
	header->nOrnates = 0;
	header->nFloors = 0;
	header->rFloors = 0;
	header->rWalls = 0;
}

void
startLevels ()
{
	size_t i;
	LEVELS = (level_p) calloc (MAX_LEVELS, sizeof (struct level_t));

	for (i = 0; i < MAX_LEVELS; i++)
	{	flushLevelHeader ((char) i);
		flushLevelLayout ((char) i);
		MColors[i] = 7;
	}
}

void
cycleLevelGraphics (int bank, char position, char step, char curlevel)
{
	level_p level = (level_p) &(getLevels()[curlevel]);
	int maxtable[] = {
		dt_Monster_Max
		, dt_Wall_Max
		, dt_Floor_Max
		, dt_Ornate_Max
		, dt_Door_Max
	};
	unsigned char doors[] = {level->header.door1, level->header.door2};
	unsigned char* lists[] = {level->monsters, level->walls, level->floors, level->ornates, doors};

	//--- Door max adjustment
	/*if (SKULLKEEP)
	{
		maxtable[4] = 11;
	}
	else
	{
		maxtable[4] = 4;
	}*/

	lists[bank][position] = (lists[bank][position] + maxtable[bank] + step)%maxtable[bank];
	if (bank == 4)
	{
		level->header.door1 = doors[0];
		level->header.door2 = doors[1];
	}
}

void
modifyLevelNumberGraphics (int bank, char l, char add)
{
	level_p level = (level_p) &(getLevels()[l]);
	char number[] = { level->header.nMonsters, level->header.nWalls, level->header.nFloors,
	level->header.nOrnates};
	static int max[] = { 15, 15, 15, 15};

	if (bank > 3) return;
	else
		number[bank] += add;
	if (number[bank] < 0) number[bank] = 0;
	else if (number[bank] > max[bank]) number[bank] = max[bank];
	level->header.nMonsters = number[0];
	level->header.nWalls = number[1];
	level->header.nFloors = number[2];
	level->header.nOrnates = number[3];
}

/*
void
autoSetLevelMonsters (char level)
{
	char i, j;
	char monsters_flag[monster_Max];
	int m = 0;
	
	for (i = 0; i < monster_Max; i++)
		monsters_flag[i] = 0;

	for (i = 0; i < LEVELS[level].level.yDim+1; i++)
		for (j = 0; j < LEVELS[level].level.yDim+1; j++)
		{
			short ref = getReference (level, i, j);
			if (ref != -2)
			{
				struct dm_reference* refp = (struct dm_reference*) &(ref);
				if (refp->category == category_Monster)
				{
					struct dm_monster* monster;
					short *item = getItem (ref);
					monster = (struct dm_monster*) (item+1);
					monsters_flag[monster->type] = 1;
				}
			}
		}


	for (i = 0; i < monster_Max; i++)
	{
		if (monsters_flag[i] && m < 15)
		{	
			LEVELS[level].monsters[m] = i;
			m++;
		}
	}
	LEVELS[level].level.nMonsters = m;
}

*/

int iMusicMax = 0x1D;

int
cycleMapMusic (int iMap, int iStep)
{
	level_p pLevel = getLevel(iMap);
	pLevel->iMusicID = (pLevel->iMusicID + iStep + iMusicMax)%iMusicMax;
	return 1;
}

