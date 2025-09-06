//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Flush Data
//------------------------------------------------------------------------------

#include <flush.h>

#include <editor.h>
#include <level.h>
#include <item.h>
#include <text.h>
#include <reference.h>

void
flushDungeon ()
{
	dm_dungeon_header* header = getDungeon ();
	
	header->f_start = 0;
	header->l_start = 0;
	header->nLevels = 1;
	header->randomGraphicsSeed = 0;
	header->x_start = 0;
	header->y_start = 0;

	

	startLevels ();
	startItems ();
	startTexts ();
	startGroundReferences ();
}

void
flushDungeonKeepStacks ()
{
	dm_dungeon_header* header = getDungeon ();
	
	header->f_start = 0;
	header->l_start = 0;
	header->nLevels = 1;
	header->randomGraphicsSeed = 0;
	header->x_start = 0;
	header->y_start = 0;

	

	startLevels ();
	startItems ();
	startTexts ();
	startGroundReferencesKeepStacks ();
}

void
flushMap(int map)
{
	flushLevelLayout(map);
	flushLevelHeader(map);

}