//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor
//------------------------------------------------------------------------------

#include <skullexe.h>
#include <loaddungeon.h>

#include <editor.h>
#include <loadgfx.h>

#include <displaytext.h>
#include <level.h>
#include <item.h>
#include <reference.h>

#include <dms.h>
#include <dm.h>
#include <data.h>
#include <actuator.h>

#include <text.h>
#include <flush.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

unsigned char* SAVEHEADER = NULL;
long savecsum = 0; // only tells if the current dungeon is a savegame or not.
long shsize = 0; // size of the savegame header (all data before dungeon.dat)


dm_dungeon_header DUNGEON;

extern int SKULLKEEP;
extern int THERONSQUEST;
extern int TELOS;

// Context
int iGlbCurrentContext = 0;
t_DungeonContext xTblDunSaveContexts[4];

/*
	cursor_X = 0,	// x cursor on map
	cursor_Y,		// y cursor on map
	cursor_L,		// current level
	cursor_TX,		// target x destination
	cursor_TY,		// target y destination
	cursor_TM,		// target current level (for teleports)
	cursor_NewItem,		// selected new category item cursor
	cursor_Stack,		// stack cursor

  cursor_LevelSpec,
*/

char* currentFileName = NULL;

static char cursors_limit[] = {
	  31	// x
	, 31	// y
	, 63	// level
	, 31	// target x
	, 31	// target y
	, 63	// target level
	, 10	// new item
	, 64	// stack cursor
	, 4		// selected level gfx
	, 14	// graphics index
	, 8		// index in chest
	, 2		// level spec
	, 62	// AI Index
	, 35	// AI Attribute
	};
static const size_t cursor_number = cursor_AI_attribute + 1;
char cursors[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0};

int txtcursors[] = {0, 0};
static const size_t text_cursor_number = 2;

int screen = screen_Map;

int editing_tile = 0;
int select_new = 0;
int editing_gfx = 0;
int editing_text = 0;
int second_fct = 0;	// 0 = standard edit : main item // 1 = second fonction // 2 = third fonction
int editing_target = 0;
int level_spec = 0;

int selectFile = 0;

int tActivatorItemTranscoListDM1toDM2[][3] = {
	{0, category_Miscs, 5},
	{1, category_Miscs, 5},
	{2, category_Miscs, 5},
	{3, category_Miscs, 5},

	{4, category_Weapon, 2},
	{5, category_Weapon, 2},
	{6, category_Weapon, 2},
	{7, category_Weapon, 2},

	{8, category_Miscs, 1},
	{9, category_Miscs, 1},
	{10, category_Miscs, 2},
	{11, category_Miscs, 2},
	{12, category_Miscs, 3},
	{13, category_Miscs, 3},
	{14, category_Weapon, 3},
	{15, category_Weapon, 3},
	{16, category_Weapon, 0},
	{17, category_Weapon, 0},
	{18, category_Weapon, 1},
	{19, category_Weapon, 1},
	{20, category_Weapon, 4},
	{21, category_Weapon, 4},
	{22, category_Weapon, 4},
	{23, category_Weapon, 5},
	{24, category_Weapon, 5},
	{25, category_Weapon, 6},
	{26, category_Weapon, 6},
	{27, category_Weapon, 7},
	{28, category_Weapon, 45},
	{29, category_Weapon, 7},
	{30, category_Scroll, 0},
	{31, category_Scroll, 0},

	{32, category_Weapon, 8},
	{33, category_Weapon, 9},
	{34, category_Weapon, 10},
	{35, category_Weapon, 11},
	{36, category_Weapon, 12},
	{37, category_Weapon, 13},
	{38, category_Weapon, 14},
	{39, category_Weapon, 15},
	{40, category_Weapon, 16},
	{41, category_Weapon, 17},

	{42, category_Weapon, 18},
	{43, category_Weapon, 19},
	{44, category_Weapon, 20},
	{45, category_Weapon, 21},
	{46, category_Weapon, 22},
	{47, category_Weapon, 23},
	{48, category_Weapon, 24},
	{49, category_Weapon, 25},
	{50, category_Weapon, 26},
	{51, category_Weapon, 27},

	{52, category_Weapon, 28},
	{53, category_Weapon, 29},
	{54, category_Weapon, 30},
	{55, category_Weapon, 31},
	{56, category_Weapon, 32},
	{57, category_Weapon, 33},
	{58, category_Weapon, 34},
	{59, category_Weapon, 35},
	{60, category_Weapon, 36},
	{61, category_Weapon, 37},

	{62, category_Weapon, 38},
	{63, category_Weapon, 39},
	{64, category_Weapon, 40},
	{65, category_Weapon, 41},
	{66, category_Weapon, 42},

	{67, category_Clothing, 5},
	{68, category_Clothing, 7},
	{69, category_Clothing, 9},
	{70, category_Clothing, 10},
	{71, category_Clothing, 13},
	{72, category_Clothing, 16},
	{73, category_Clothing, 20},
	{74, category_Clothing, 21},
	{75, category_Clothing, 32},
	{76, category_Clothing, 34},
	{77, category_Clothing, 39},
	{78, category_Clothing, 44},
	{79, category_Clothing, 49},

	{80, category_Clothing, 0},
	{81, category_Clothing, 1},
	{82, category_Clothing, 2},
	{83, category_Clothing, 6},
	{84, category_Clothing, 8},
	{85, category_Clothing, 11},
	{86, category_Clothing, 12},
	{87, category_Clothing, 14},
	{88, category_Clothing, 17},
	{89, category_Clothing, 19},
	{90, category_Clothing, 22},
	{91, category_Clothing, 33},
	{92, category_Clothing, 35},
	{93, category_Clothing, 40},
	{94, category_Clothing, 45},
	{95, category_Clothing, 50},
	{96, category_Clothing, 25},
	{97, category_Clothing, 26},
	{98, category_Clothing, 27},
	{99, category_Clothing, 36},
	{100, category_Clothing, 38},
	{101, category_Clothing, 43},
	{102, category_Clothing, 48},
	{103, category_Clothing, 23},
	{104, category_Clothing, 24},
	{105, category_Clothing, 28},
	{106, category_Clothing, 29},
	{107, category_Clothing, 31},	// ? inversion
	{108, category_Clothing, 30},	// ? inversion
	{109, category_Clothing, 42},
	{110, category_Clothing, 47},
	{111, category_Clothing, 52},
	{112, category_Clothing, 3},
	{113, category_Clothing, 18},
	{114, category_Clothing, 4},
	{115, category_Clothing, 37},
	{116, category_Clothing, 41},
	{117, category_Clothing, 46},
	{118, category_Clothing, 51},
	{119, category_Clothing, 15},

	{120, category_Miscs, 37},
	{121, category_Miscs, 38},
	{122, category_Miscs, 39},
	{123, category_Miscs, 40},
	{124, category_Miscs, 41},
	{125, category_Miscs, 6},
	{126, category_Miscs, 7},
	{127, category_Miscs, 8},
	{128, category_Miscs, 25},
	{129, category_Miscs, 26},
	{130, category_Miscs, 27},
	{131, category_Miscs, 28},
	{132, category_Miscs, 42},
	{133, category_Miscs, 43},
	{134, category_Miscs, 44},
	{135, category_Weapon, 43},
	{136, category_Miscs, 45},
	{137, category_Miscs, 46},
	{138, category_Miscs, 47},
	{139, category_Miscs, 48},

	{140, category_Clothing, 53},
	{141, category_Clothing, 54},
	{142, category_Clothing, 55},
	{143, category_Weapon, 44},

	{144, category_Chest, 0},
	{145, category_Chest, 0},

	{146, category_Miscs, 4},
	{147, category_Miscs, 0},

	{148, category_Potion, 0},
	{149, category_Potion, 1},
	{150, category_Potion, 2},
	{151, category_Potion, 3},
	{152, category_Potion, 4},
	{153, category_Potion, 5},
	{154, category_Potion, 6},
	{155, category_Potion, 7},
	{156, category_Potion, 8},
	{157, category_Potion, 9},
	{158, category_Potion, 10},
	{159, category_Potion, 11},
	{160, category_Potion, 12},
	{161, category_Potion, 13},
	{162, category_Potion, 14},
	{163, category_Potion, 15},
	{164, category_Potion, 16},
	{165, category_Potion, 17},
	{166, category_Potion, 18},
	{167, category_Potion, 19},

	{168, category_Miscs, 29},
	{169, category_Miscs, 30},
	{170, category_Miscs, 31},
	{171, category_Miscs, 32},
	{172, category_Miscs, 33},
	{173, category_Miscs, 34},
	{174, category_Miscs, 35},
	{175, category_Miscs, 36},
	{176, category_Miscs, 9},
	{177, category_Miscs, 10},
	{178, category_Miscs, 11},
	{179, category_Miscs, 12},
	{180, category_Miscs, 13},
	{181, category_Miscs, 14},
	{182, category_Miscs, 15},
	{183, category_Miscs, 16},
	{184, category_Miscs, 17},
	{185, category_Miscs, 18},
	{186, category_Miscs, 19},
	{187, category_Miscs, 20},
	{188, category_Miscs, 21},
	{189, category_Miscs, 22},
	{190, category_Miscs, 23},
	{191, category_Miscs, 24},
	{192, category_Miscs, 49},
	{193, category_Miscs, 50},
	{194, category_Clothing, 56},
	{195, category_Miscs, 20},
	{196, category_Clothing, 57},
	{197, category_Miscs, 51},
	{198, category_Miscs, 52},
};


void
retrieveDM2ActivatorInfo(int iDM1ItemID, int* iCategory, int* iItemID)
{
	if (iDM1ItemID >= 0 && iDM1ItemID < 200)
	{
		*iCategory = tActivatorItemTranscoListDM1toDM2[iDM1ItemID][1];
		*iItemID = tActivatorItemTranscoListDM1toDM2[iDM1ItemID][2];
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

void Context_Init()
{
	unsigned int i = 0;
	for (i = 0; i < 4; i++)
	{
		xTblDunSaveContexts[i].iInitialized = 0;
		xTblDunSaveContexts[i].pMapsFullDefinition = NULL;
		//xTblDunSaveContexts[i].xGlobalDungeonHeader;
		xTblDunSaveContexts[i].pFloorRefs = NULL;
	}
}

//item.c
extern short* ITEMS[16]; // Item Data
extern int nItems[16];	// Number of items per category
extern int lastItem[16]; // Last item ID attributed for a category

void Context_PushCurrentToStore(int iContext)
{
	unsigned int i = 0;
	if (getLevels() != NULL)
	{
		xTblDunSaveContexts[iContext].iInitialized = 1;
		xTblDunSaveContexts[iContext].pMapsFullDefinition = getLevels();
		xTblDunSaveContexts[iContext].xGlobalDungeonHeader.randomGraphicsSeed = DUNGEON.randomGraphicsSeed;
		xTblDunSaveContexts[iContext].xGlobalDungeonHeader.mapDataSize = DUNGEON.mapDataSize;
		xTblDunSaveContexts[iContext].xGlobalDungeonHeader.nLevels = DUNGEON.nLevels;
		xTblDunSaveContexts[iContext].xGlobalDungeonHeader.null = DUNGEON.null;
		xTblDunSaveContexts[iContext].xGlobalDungeonHeader.textsDataSize = DUNGEON.textsDataSize;
		xTblDunSaveContexts[iContext].xGlobalDungeonHeader.x_start = DUNGEON.x_start;
		xTblDunSaveContexts[iContext].xGlobalDungeonHeader.y_start = DUNGEON.y_start;
		xTblDunSaveContexts[iContext].xGlobalDungeonHeader.f_start = DUNGEON.f_start;
		xTblDunSaveContexts[iContext].xGlobalDungeonHeader.l_start = DUNGEON.l_start;
		xTblDunSaveContexts[iContext].xGlobalDungeonHeader.itemListSize = DUNGEON.itemListSize;
		for (i = 0; i < 16; i++)
			xTblDunSaveContexts[iContext].xGlobalDungeonHeader.nObjects[i] = DUNGEON.nObjects[i];
		xTblDunSaveContexts[iContext].pFloorRefs = Ref_GetFloorsPointer();

		for (i = 0; i < 16; i++)
		{
			xTblDunSaveContexts[iContext].nItems[i] = nItems[i];
			xTblDunSaveContexts[iContext].lastItem[i] = lastItem[i];
			xTblDunSaveContexts[iContext].xItemsData[i] = ITEMS[i];
		}

		xTblDunSaveContexts[iContext].nTotalTexts = totalTexts;
		for (i = 0; i < 1024; i++)
		{
			xTblDunSaveContexts[iContext].xTextStrings[i] = TEXTS[i];
			xTblDunSaveContexts[iContext].iTblTextTypes[i] = TXTTYPE[i];
			xTblDunSaveContexts[iContext].iTblOffsetsAddresses[i] = adresses[i];
		}

		xTblDunSaveContexts[iContext].iIsSkullkeep = SKULLKEEP;
		xTblDunSaveContexts[iContext].iIsTheronsQuest = THERONSQUEST;
		xTblDunSaveContexts[iContext].iIsTELOS = TELOS;
		xTblDunSaveContexts[iContext].currentFileName = currentFileName;
	}
}

void Context_GetToCurrent(int iContext)
{
	unsigned int i = 0;
	flushDungeonKeepStacks();
	if (xTblDunSaveContexts[iContext].iInitialized == 0)
	{
		DUNGEON.randomGraphicsSeed = 0;
		DUNGEON.mapDataSize = 0;
		DUNGEON.nLevels = 1;
		DUNGEON.null = 0;
		DUNGEON.textsDataSize = 0;
		DUNGEON.x_start = 0;
		DUNGEON.y_start = 0;
		DUNGEON.f_start = 0;
		DUNGEON.l_start = 0;
		for (i = 0; i < 16; i++)
			DUNGEON.nObjects[i] = 0;
		startLevels();
		startItems();
		startTexts();
		SKULLKEEP = 0;
		THERONSQUEST = 0;
		TELOS = 0;
		currentFileName = NULL;
	}
	else
	{
		DUNGEON.randomGraphicsSeed = xTblDunSaveContexts[iContext].xGlobalDungeonHeader.randomGraphicsSeed;
		DUNGEON.mapDataSize = xTblDunSaveContexts[iContext].xGlobalDungeonHeader.mapDataSize;
		DUNGEON.nLevels = xTblDunSaveContexts[iContext].xGlobalDungeonHeader.nLevels;
		DUNGEON.null = xTblDunSaveContexts[iContext].xGlobalDungeonHeader.null;
		DUNGEON.textsDataSize = xTblDunSaveContexts[iContext].xGlobalDungeonHeader.textsDataSize;
		DUNGEON.x_start = xTblDunSaveContexts[iContext].xGlobalDungeonHeader.x_start;
		DUNGEON.y_start = xTblDunSaveContexts[iContext].xGlobalDungeonHeader.y_start;
		DUNGEON.f_start = xTblDunSaveContexts[iContext].xGlobalDungeonHeader.f_start;
		DUNGEON.l_start = xTblDunSaveContexts[iContext].xGlobalDungeonHeader.l_start;
		for (i = 0; i < 16; i++)
			DUNGEON.nObjects[i] = xTblDunSaveContexts[iContext].xGlobalDungeonHeader.nObjects[i];
		setLevelsFromPointer(xTblDunSaveContexts[iContext].pMapsFullDefinition);
		Ref_SetFloorsPointer(xTblDunSaveContexts[iContext].pFloorRefs);
		for (i = 0; i < 16; i++)
		{
			nItems[i] = xTblDunSaveContexts[iContext].nItems[i];
			lastItem[i] = xTblDunSaveContexts[iContext].lastItem[i];
			ITEMS[i] = xTblDunSaveContexts[iContext].xItemsData[i];
		}
		totalTexts = xTblDunSaveContexts[iContext].nTotalTexts;
		for (i = 0; i < 1024; i++)
		{
			TEXTS[i] = xTblDunSaveContexts[iContext].xTextStrings[i];
			TXTTYPE[i] = xTblDunSaveContexts[iContext].iTblTextTypes[i];
			adresses[i] = xTblDunSaveContexts[iContext].iTblOffsetsAddresses[i];
		}

		SKULLKEEP = xTblDunSaveContexts[iContext].iIsSkullkeep;
		THERONSQUEST = xTblDunSaveContexts[iContext].iIsTheronsQuest;
		TELOS = xTblDunSaveContexts[iContext].iIsTELOS;
		currentFileName = xTblDunSaveContexts[iContext].currentFileName;
	}
}

void switchContextFromTo(int iPreviousContext, int iContext)
{
	//printf("SWITCHING DUNGEON CONTEXT FROM %d TO %d\n", iPreviousContext, iContext);
	if (iPreviousContext == iContext)
		return;
	Context_PushCurrentToStore(iPreviousContext);
	Context_GetToCurrent(iContext);
	iGlbCurrentContext = iContext;
}

void switchContextTo(int iContext)
{
	if (iContext == iGlbCurrentContext)
		return;
	switchContextFromTo(iGlbCurrentContext, iContext);
}

/*
typedef struct
{
	unsigned int iInitialized;
	level_p pMapsFullDefinition; // level_p LEVELS; initialized by startLevels
	dm_dungeon_header xGlobalDungeonHeader;

} t_DungeonContext;*/

void switchContext()
{
	int iPreviousContext = iGlbCurrentContext;
	iGlbCurrentContext = (iGlbCurrentContext + 1)%4;
	switchContextFromTo(iPreviousContext, iGlbCurrentContext);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

void
switchModifyLevelSpecs ()
{	level_spec = !level_spec;}

void updateCursor (enum cursorType type)
{
	cursors_limit[2] = cursors_limit[5] = (getDungeon ()->nLevels)-1;
	cursors_limit[7] = getStackSize()-1;

	if (type == cursor_X || type == cursor_Y || type == cursor_L)
		cursors[cursor_Stack] = 0;

	if (cursors[type] > cursors_limit[type]) cursors[type] = cursors_limit[type];
	else if (cursors[type] < 0) cursors[type] = 0;

	//if (cursors[cursor_Graphics] > 2 && cursors[cursor_Index] > 6)
	//	cursors[cursor_Index] = 6;
	else if (cursors[cursor_Graphics] > 3 && cursors[cursor_Index] > 1)
		cursors[cursor_Index] = 1;

//	printf("CURSORS: X%02d Y%02d M%02d TX%02d TY%02d TM%02d NI%02d SC%02d\n", 
//		cursors[cursor_X], cursors[cursor_Y], cursors[cursor_L],
//		cursors[cursor_TX], cursors[cursor_TY], cursors[cursor_TM],
//		cursors[cursor_NewItem], cursors[cursor_Stack]);

	importStack (cursors[cursor_X], cursors[cursor_Y], cursors[cursor_L]);
}

void updateAll ()
{
	size_t i;
	for (i = 0; i < cursor_number; i++)
		updateCursor (i);
}

int
isEditingTile ()
{
	return editing_tile;
}

void
switchEditingTile ()
{
	editing_tile = !editing_tile;
}

int
isEditingGraphics ()
{
	return editing_gfx;
}

void
switchEditingGraphics ()
{
	editing_gfx = !editing_gfx;
}


int
isEditingText ()
{
	return editing_text;
}

void
switchEditingText ()
{
	editing_text = !editing_text;
}


int
isSecondFunction ()
{	return second_fct;}

void
switchSecondFunction ()
{	second_fct = (second_fct+1)%3;}


int
isSelectingNewItem ()
{
	return select_new;
}

void
setSelectingNewItem (int boolean)
{
	select_new = boolean;
}


int
isEditingTarget ()
{
	return editing_target;
}

void
setEditingTarget (int boolean)
{
	editing_target = boolean;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

int
getTextCursor (enum cursorText type)
{
	assert ((size_t) type < text_cursor_number);
	return txtcursors[type];
}

void
setTextCursor (enum cursorText type, int new_value)
{
	assert ((size_t) type < text_cursor_number);
	txtcursors[type] = new_value;
	if (type == cursor_Text && new_value < 0)
		txtcursors[type] = 0;
	else if (type == cursor_Text && new_value >= totalTexts)	// from text.h
		txtcursors[type] = totalTexts-1;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

char
getEditCursor (enum cursorType type)
{
	assert ((size_t) type < cursor_number);
	return cursors[type];
}

void
setEditCursor (enum cursorType type, int new_value)
{
	assert ((size_t) type < cursor_number);
	cursors[type] = new_value;
	updateCursor (type);
}

int
recomputeActuatorsUsage()
{
	tCompanionActuator* xact = NULL;

	dm_dungeon_header* xDungeonHeader = NULL;
	int iNbMaps = 0;
	int m = 0;
	int i = 0;
	int j = 0;
	level_p xCurrentMap = NULL;
	dm_level_header* xDMLevelHeader = NULL;
	sk_level_header* xSKLevelHeader = NULL;


	for (i = 0; i < 1024; i++)
	{
		xact = &xActuatorTable[i];
		xact->id = i;
		xact->map = -1;
		xact->used = 0;
		xact->wact = 0;
	}

	//--- There, search through the entire dungeon for all actuators
	xDungeonHeader = getDungeon();
	iNbMaps = xDungeonHeader->nLevels;

	for (m = 0; m < iNbMaps; m++)
	{
		short xRefItem = -2;
		int iTileType = 0;
		reference_p ref = NULL;
		//xCurrentMap = (level_p) &(getLevels()[m]);
		for (j = 0; j < 32; j++)
		{
			for (i = 0; i < 32; i++)
			{
				iTileType = getTile (i,j,m)->type;
				ref = getGroundReference (i, j, m);
				xRefItem = *((short*)ref);
				while (xRefItem != -2)
				{
					int iItemValue = 0;
					int iListBase = 0;
					int iActivatorID = 0;
					int iItemCategory = 0;
					int iItemIndexInCat = 0;

					if (ref->category == category_Actuator)
					{
						xact = &xActuatorTable[ref->id];
						xact->id = ref->id;
						xact->used = 1;
						xact->map = m;

						if (iTileType == tile_Wall)	// convert Wall actuators
						{
							xact->wact = 1;
						}
						else	// floor and other tiles
						{
							xact->wact = 0;
						}
					}
					ref = getNextItem (ref);
					xRefItem = *((short*)ref);
				}
			}
		}
	}


	return 0;
}


int
getScreen ()
{
	return screen;
}

void
setScreen (int val)
{
	screen = val;

	if (screen == screen_ListsActuators)
		recomputeActuatorsUsage();
}

//------------------------------------------------------------------------------

void startEditor ()
{
	loadGraphics ();
	loadTextFont ();
	DUNGEON.nLevels = 1;
	flushDungeon ();
	beginTextOutput ();
}

//------------------------------------------------------------------------------

dm_dungeon_header*
getDungeon ()
{
	return &DUNGEON;
}

//------------------------------------------------------------------------------

void
goTarget (reference_p ref)
{
	if (ref->category == category_Teleport)
	{
		teleport_p teleport = (teleport_p) getItem (ref);
		setEditCursor (cursor_X, (char) teleport->xdest);
		setEditCursor (cursor_Y, (char) teleport->ydest);
		setEditCursor (cursor_L, (char) teleport->ldest);
	}
}










//------------------------------------------------------------------------------

long 
checksumSE (FILE* fp, long start, long end)
{
	long i;
	long total = 0;
	unsigned short tmp = 0;
	long address = ftell (fp);
	fseek (fp , start, SEEK_SET);
	for (i = 0; i < end; i++)
	{
		fread (&tmp, 1, 1, fp);
		total += (unsigned long) tmp;
	}
	fseek (fp, address, SEEK_SET);
	return (unsigned short) total;
}

short
checksumS (FILE* fp, long start)
{
	long i;
	long total = 0;
	unsigned short tmp = 0;
	long address = ftell (fp);
	fseek (fp , start, SEEK_SET);
	for (i = 0; i < address-start; i++)
	{
		fread (&tmp, 1, 1, fp);
		total += (unsigned long) tmp;
	}
	fseek (fp, address, SEEK_SET);
	return (unsigned short) total;
}


short
checksum (FILE* fp) // checksum DM, 2 bytes big end
{
	long i;
	long total = 0;
	unsigned short tmp = 0;
	long address = ftell (fp);
	fseek (fp , 0x00000000, SEEK_SET);
	for (i = 0; i < address; i++)
	{
		fread (&tmp, 1, 1, fp);
		total += (unsigned long) tmp;
	}
	fseek (fp, address, SEEK_SET);
	return (unsigned short) total;
}


int DM1_tbl_ColorPriorities[27] = {
	6, 5, 7, 6, 4, 4, 7, 7,
	7, 7, 7, 7, 7, 5, 4, 3,
	2, 7, 7, 1, 2, 1, 0, 7, // 8 Chaos? ou 1?
	0, 0, 0
};

void
updatePriorityColors ()
{
	int i;
	for (i = 0; i < getDungeon()->nLevels; i++)
	{
//		LEVEL* level = (LEVEL*) &Dungeon->Levels[i];
		level_p level = (level_p) &(getLevels()[i]);
		int priority = 7;
		int j;
		for (j = level->header.nMonsters-1; j >= 0; j--)
		{
			int color = DM1_tbl_ColorPriorities[level->monsters[j]];
			if (color < priority)
			{
				MColors[i] = color;
				break;
			}
		}
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
