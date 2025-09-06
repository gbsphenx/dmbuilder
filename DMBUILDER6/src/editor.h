//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor
//------------------------------------------------------------------------------


#ifndef EDITOR_H
#define EDITOR_H

#include <reference.h>

#define MSG_LOAD_FAILURE	0x0001
#define MSG_SAVE_FAILURE	0x0002
#define MSG_LOAD_SUCCESS	0x0004
#define MSG_SAVE_SUCCESS	0x0008
#define MSG_LOAD_SG_SUCCESS 0x0010
#define MSG_SAVE_SG_SUCCESS 0x0020

#define MSG_PRINT_VERSION	0x8000

/* priority is given to first */
#define MSG_DUNGEON_LOCKED			0x0700
#define MSG_DUNGEON_QUICKVIEW		0x0300
#define MSG_DUNGEON_EDITING_LOCK	0x0100

#define MSG_INVALID_DUNGEON			0x1000	/* bad checksum */

extern unsigned char* SAVEHEADER;
extern long shsize;
extern long savecsum;

extern char* currentFileName;


/* header structure for the dungeon.dat file 
 * size = 42 bytes
 */
typedef struct
{
	short randomGraphicsSeed; // 2 bytes for the randomized graphics for floors and walls.
	unsigned short mapDataSize;
	unsigned char nLevels;
	unsigned char null; // is 0 in DM, Prison and CSB. Has a role?
	unsigned short textsDataSize; // number of bytes used for the dungeon coded texts
	unsigned short x_start:5;
	unsigned short y_start:5;
	unsigned short f_start:2; // starting facing
	unsigned short l_start:4; // I suppose this was for the level start, but it doesn't work
	unsigned short itemListSize;
	unsigned short nObjects[16];
} dm_dungeon_header;


enum cursorType
{
	cursor_X = 0,	// x cursor on map
	cursor_Y,		// y cursor on map
	cursor_L,		// current map
	cursor_TX,		// target x destination
	cursor_TY,		// target y destination
	cursor_TM,		// target map destination (for teleports)
	cursor_NewItem,		// selected new category item cursor
	cursor_Stack,		// stack cursor
	cursor_Graphics,	// selected level gfx
	cursor_Index,		// selected graphics index
	cursor_Chested,		// used for chested objects and various items attributes
	cursor_LevelSpec,	// the one that choose between depth and x,y map offsets.

	cursor_AI_index,
	cursor_AI_attribute,
};

enum screenType
{
	screen_Map = 0,
	screen_MainHeader,
	screen_Level,
	screen_ListsCreatures,
	screen_ListsItems,
	screen_ListsActuators,
	screen_DM2AI,
	screen_LoadFile,
	screen_SaveFile,
};


extern int screen;
extern int editing_tile;
extern int select_new;
extern int editing_gfx;
extern int second_fct;
extern int editing_target;
extern int level_spec;
extern int selectFile;

//extern tSkullExe xSkullExe;

//------------------------------------------------------------------------------

#include <level.h>
#include <item.h>
#include <dms.h>
#include <dm.h>
#include <reference.h>

typedef struct
{
	unsigned int iInitialized;
	level_p pMapsFullDefinition; // level_p LEVELS; initialized by startLevels
	dm_dungeon_header xGlobalDungeonHeader;
	short*	pFloorRefs;

	// ITEM DATA
	short*	xItemsData[16];	// Item Data
	int nItems[16];	// Number of items per category
	int lastItem[16]; // Last item ID attributed for a category

	// TEXT DATA
	char* xTextStrings[1024];	// Converted plain text, 1 per different text
	int nTotalTexts;
	short iTblOffsetsAddresses[1024];	// Table of beginning of each text in raw data

	// Special flags of game mode
	int iIsSkullkeep;
	int iIsTheronsQuest;
	int iIsTELOS;

} t_DungeonContext;

extern t_DungeonContext xTblDunSaveContexts[4];
extern int iGlbCurrentContext;

//------------------------------------------------------------------------------

extern void startEditor ();

extern char getEditCursor (enum cursorType type);
extern void setEditCursor (enum cursorType type, char new_value);

extern int getScreen ();
extern void setScreen (int val);

extern int isEditingTile ();
extern void switchEditingTile ();

extern int isSelectingNewItem ();
extern void setSelectingNewItem (int boolean);

extern int isSecondFunction ();
extern void switchSecondFunction ();

extern int isEditingTarget ();
extern void setEditingTarget (int boolean);

extern int isEditingGraphics ();
extern void switchEditingGraphics ();
extern void goTarget (reference_p ref);

extern void switchModifyLevelSpecs ();

extern dm_dungeon_header* getDungeon ();

extern void updatePriorityColors ();

void DMB_AutoEdit_CreateTestDungeon ();

void switchContext();
void Context_Init();

#include <stdio.h>
extern short checksum (FILE* fp);
extern long checksumSE (FILE* fp, long start, long end);
extern short checksumS (FILE* fp, long start);
#endif

