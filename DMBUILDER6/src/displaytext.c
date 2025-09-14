//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Display Text
//------------------------------------------------------------------------------

#include <glTexFont.h>
#include <tga.h>

#include <displaytext.h>
#include <editor.h>
#include <level.h>

#include <item.h>
#include <dm.h>
#include <text.h>
#include <tile.h>
#include <reference.h>
#include <actuator.h>
#include <monster.h>
#include <string.h>
#include <assert.h>
#include <skullexe.h>
#include <data.h>

#include <math.h>
#include <stdarg.h>
#include <stdio.h>

extern int winW;
extern int winH;

extern tSkullExe xSkullExe;

#include <display.h>	// for define

//------------------------------------------------------------------------------

int iInfoX = 1200;
int iInfoY = 925;
int iInfoYNeg = 1050 - 925 + 30;
int iInfoFntSize = 15;
	//x = winW-370;
	//y = winH/2;

//------------------------------------------------------------------------------

static const char* txt_monsters[] = {
	"Scorpion", "Slime", "Giggler", "Beholder",
	"Rat", "Ruster", "Screamer", "Rockpile", "Ghost", 
	"Stone Golem", "Mummy", "Black Flame", "Skeleton",
	"Couatl", "Jawa", "Magenta Worm", "Trolin",
	"Wasp", "Animated Armour", "Fader", "Water Elemental",
	"Oitu", "Demon", "Lord Chaos", "Red Dragon",
	"Evil Lord", "Grey Lord",
};

static const char* txt_monsters_csb[] = {
	"Scorpion", "Slime Devil", "Giggler", "Gazer",
	"Hell Hound", "Ruster", "Screamer", "Rockpile", "Rive", 
	"Stone Golem", "Mummy", "Black Flame", "Skeleton",
	"Couatl", "Vexirk", "Armoured Worm", "Ant Man",
	"Muncher", "Deth Knight", "Zytaz", "Water Elemental",
	"Greater Oitu", "Viper Demon", "Lord Chaos", "Red Dragon",
	"Evil Lord", "Grey Lord",
};


static const char* txt_monsters_sk[] = {
	"Thorn Demon", "Cavern Table", "Vortex", "Amplifier", // 00
	"Bush", "Pillar", "Stalagmite", "Boulder", // 04
	"Face Pillar", "Glop", "0A", "0B",	// 08
	"Rocky", "Giggler", "Thicket Thief", "Tiger Striped Worm",	// 0C
	"Spectre",	"Power Crystal", "Tree Gorgon", "Fountain", // 10
	"Tree", "15", "Skull Brazier", "Dragoth",
	"Cave In", "Wood Table", "Magik Cauldron", "Evil Attack Minion",
	"Tower Bat", "Archer-Guard", "1E", "Vexirk King",
	"Dark Vexirk", "Pit Ghost", "Merchant", "Cavern Bat",
	"Spiked Capstan", "Evil Fountain", "Mummy", "Pipe Shaft",
	"28", "Lightning Rod", "SarDes Minion", "Evil Scout Minion",
	"Merchant Guard", "Barrel", "Magik Reflector", "Boiler",
	"Scout Minion", "Attack Minion", "Carry Minion", "Fetch Minion", // 30
	"Guard Minion", "U-Haul Minion", "36", "Flame Orb",
	"Digger Worm", "Tombstone", "Merchant Tomb", "Trading Table (Sells)",
	"Trading Table (Buys)", "Obelisk", "Obelisk", "Obelisk",
	"40", "Skeleton", "Axeman", "Bad Merchant",
	"Magic Merchant", "Evil Guard Minion", "Void Door", "Steam Engine",
	"Axeman Thief", "Ghost", "Wolf", "4B",
	"Dragoth Attack Minion", "Cavern Table", "Door Ghost", "4F",
	"50", "Spiked Wall", "Dru Tan", "Cavern Table",
	"Flying Chest", "Pedistal", "56", "Evil Attack Minion",
	// 58
};


static float text_cat_colors[][3] =
{
	{.75, .5, .25}, {.25, .75, 1}, {.8,.7, .9}, {.75, .75, .75},	// door, teleporter, text, actuator
	{1, 0, 0}, {1, 1, 1}, {1, .6, 0}, {.5, 1, 1},		// creature, weapon, clothing, scroll,
	{.2, .6, 1}, {1, 1, 0}, {.25, 1, .25}, {1, 0, 0},		// potion, chest, misc, ?
	{1, 1, 0}, {0, 1, 1}, {1, 0, 1}, {.3,.3,.3}			// ?, ?, missile, cloud
};


static const char* txt_dm2_ai_attributes[] =
{
	"FLAGS 00", "FLAGS 01", "ARMOR CLASS", "B03",
	"BASE HP (LOW)", "BASE HP (HIGH)", "ATTACK STRENGTH", "POISON DAMAGE",
	"DEFENSE", "FLAGS 09", "B10", "B11",
	"B12", "B13", "SPECIAL ATTACKS", "SPELLS 2",
	"B16", "B17", "SWITCH TRIGGER", "B21",
	"B20", "B21", "RESISTANCE 1", "RESISTANCE 2",
	"B24", "B25", "B26", "B27",
	"B28", "WEIGHT", "B30", "B31",
	"B32", "B33", "B34", "SMOKE SIZE"
};


static const char* txt_dm2_sact_extuse[] =
{
	"STILL IMAGE", "CREATURE TRANSFORMER", "ANIMATED IMAGE", "3?",
	"ITEM TOGGLER", "ENGINE MONITOR", "LADDER (ENABLED)", "LADDER (OFF)",
	"ITEM RESPAWNER", "9?", "SWAMP MARSH", "11?",
	"12?", "2-STATE ORNATE", "GDAT TXT", "TELEPORTER MARKER",

	"TELEPORTER + FLAG", "17?", "18?", "CREATURE SPAWN ON TRIGGER",
	"20?", "CREATURE SOMETHING ?", "22?", "23?",
	"24?", "25?", "26?", "27?",
	"28?", "29?", "30?", "31?"
};


static const char* txt_generators[] =
{
	"CRASH!!", "1*", "2*", "3*",
	"4*", "1?(2-3?)", "1?(2-3?)", "1?(2-3?)",
	"CRASH2!!", "1?", "1-2*", "1-3*",
	"1-4*", "1-4?", "1-4?", "1-4?"
};

static const char* txt_objects[] =
{
	"Door", "Teleport", "Text", "Activator",
	"Monster", "Weapon", "Clothing", "Scroll",
	"Potion", "Chest", "Misc", "-1",
	"-2", "-3", "Missile", "Cloud"
};

static const char* txt_DM2_objects[] =
{
	"Door", "Teleport", "Ornate/Activator", "Activator",
	"Monster/Object", "Weapon", "Clothing", "Scroll",
	"Potion", "Container/Map", "Misc", "-1",
	"-2", "-3", "Missile", "Cloud"
};

static const char* txt_ObjectCategory_Display[] =
{
	"DOOR", "TELEPORT", "TXT/SIMPLE.ACT.", "ACTUATOR",
	"MONSTER/OBJ.", "WEAPON", "CLOTHING", "SCROLL",
	"POTION", "CONTAINER/MAP", "MISCELLANEOUS", "EXT-1",
	"EXT-2", "EXT-3", "MISSILE", "CLOUD"
};


static const char* txt_complete_wall_actuators[] =
{
	"CHAMPION MIRROR",
	"DISABLED (NOTHING)",	"PUSH BUTTON",	"ALCOVE ITEM",	"ITEM",
	"ITEM EATER",	"TRIGGER",	"COUNTING PAD",	"7?",
	"SPELL SHOOTER",	"WEAPON SHOOTER",	"DOUBLE SPELL SHOOTER",	"ITEM EATER TOGGLER 1",
	"MOUSE LAST TOGGLER",	"REMOVE ITEM TOGGLER",	"GENERAL ITEM SHOOTER",	"DOUBLE GENERAL ITEM SHOOTER",
	"ACT. TOGGLER",	"EATER TOGGLER 2",	"END PAD",
};

static const char* txt_complete_floor_actuators[] = 
{
	"Nothing",	"Activated by Everything",	"2?",	"Activated by Party",
	"Activated by Item",	"5?",	"Monster Generator",	"Activated by Monster",
	"Activated by Item Carried",	"9?",	"10?",	"11?",
	"12?",	"13?",	"14?",	"15?",
	"16?",	"17?",	"18?",	"19?",
};

static const char* txt_dm2_wall_actuators[] = 
{
	"Nothing", "1?", "2?", "Item Watcher",
	"4?", "5?", "6?", "7?",

	"Spell Shooter ?", "9?", "Double Spell Shooter ?", "11?",

	"12?",	"13?",	"General Item Shooter",	"15?",
	"16?",	"17?",	"End Pad",	"19?",

	"20?",	"Charged Item Watcher",	"Cross Map",	"2-State Switch",
	"Button Switch",	"25?",	"Key Hole",	"27?",
	"Old Shop Panel?",	"Counter",	"Tick Generator",	"31?",
	"Relay 1", "Arrival & Departure", "Flying Item Catcher", "Item Teleporter (Flying)",
	"36?", "37?", "Switch Sign for Creature", "39?",
	"40?", "41?", "Alcove Item", "43?",
	"Infinite Ornate Animator", "Finite Activator Relay", "Creature Generator", "47?",
	"48?", "Work Timer", "Ornate Animator", "51?",
	"52?", "53?", "54?", "55?",
	"56?", "57?", "58?", "Item Teleporter (Placed Item)",
	"Item Generator", "Relay 2", "62?", "Shop Panel",

	"Item Recycler", "Ornate Step Animator", "66?", "Inverse Flag",
	"Test Flag", "Relay 3", "Button Switch 2", "Item Capture",
	"72?", "73?", "74?", "75?",
	"76?", "77?", "78?", "79?",
};



static const char* txt_dm2_floor_actuators[] = 
{
	"Nothing", "Activated by Everything", "2?", "Activated by Party",
	"Activated by Item", "5?", "6?", "Activated by Monster",
	"Activated by Item Carried", "9?", "Shooter 2", "Creature Killer",
	"12?", "13?", "14?", "15?",
	"16?", "17?", "18?", "19?",
	"20?", "21?", "Cross Map Activator", "23?",
	"24?", "25?", "26?", "27?",
	"28?", "Counter", "Infinite Tick Generator", "31?",

	"Relay 1", "Arrival & Departure", "34?", "35?",
	"36?", "37?", "Missile Explosion", "Map Link",
	"40?", "41?", "Item in Alcove", "43?",
	"Infinite Ornate Animator", "Finite Activator Relay", "Squad Teleporter", "47?",
	"Shop Exhibition", "49?", "Ornate Animator", "51?",
	"52?", "53?", "54?", "55?",
	"56?", "57?", "Creature Animator", "Item Teleporter (by Item)",
	"60?", "Relay 2", "62?", "63?",
	"Item Recycler (by Creature)", "65?", "Set Creature Facing Direction", "67?",
	"68?", "Relay 3", "70?", "71?",
	"72?", "Item Capture from Creature", "74?", "75?",
	"76?", "77?", "78?", "79?",

};

static const char* txt_actuator_effect[] = {
	"STEP ON: OPEN/SET",
	"STEP ON: CLOSE/CLEAR",
	"STEP ON: TOGGLE",
	"STEP ON: OPEN/SET / STEP OUT: CLOSE/CLEAR",

	"STEP OUT: OPEN/SET",	
	"STEP OUT: CLOSE/CLEAR",	
	"STEP OUT: TOGGLE",
	"STEP OUT: OPEN/SET / STEP ON: CLOSE/CLEAR"};

static const char* txt_position_ortho[] = {
	"NORTH", "EAST", "SOUTH", "WEST" };

static const char* txt_position_quarter[] = {
	"NORTH/WEST", "NORTH/EAST", "SOUTH/EAST", "SOUTH/WEST" };

//char **txt_act_walls = (char**) &(txt_complete_wall_actuators[1]);
//char **txt_act_floors = (char**) &(txt_complete_floor_actuators[0]);

static const char* txt_door_names [] = {
	"IRON GRATE", "WOODEN DOOR", "METALLIC DOOR", "RA DOOR" };

static const char* txt_tileset_names [] = {
	"VOID", "THICKET", "CAVE", "KEEP", "KEEP ROOF", "FOG THICKET" };


static const char* txt_skull_ai_groupnames [] = {
"TREE", "LAB. TABLE", "STONE ALTAR", "BUSH",
"PILLARS / TABLE / ROD", "STALAGMITE", "BOULDER", "FOUNTAIN",
"OBELISKS / TOMBSTONE", "WOOD TABLE", "MAGIK CAULDRON", "SKULL BRAZIER",
"TRADING TABLE", "SCOUT MINION", "ATTACK MINION", "CARRY MINION",
"FETCH MINION", "GUARD MINION", "U-HAUL MINION", "THORN DEMON",

"OBELISK / SIGN DIRECTIONS", "VORTEX", "FLAME ORB", "CAVERN BAT",
"GLOP", "ROCKY", "GIGGLER", "THICKET THIEF",
"TIGER STRIPED WORM", "TREANT", "LORD DRAGOTH", "DRU TAN",
"CAVE IN", "MERCHANT", "???", "TOWER BAT", "ARCHER GUARD",
"MAGICK REFLECTOR", "POWER CRYSTAL", "EVIL FOUNTAIN", "SPIKED WALL",
"SPECTRE", "VEGMOUTH", "EVIL ATK. MINION", "AXEMAN",
"STONE TABLE / WALL HOLE", "MUMMY", "VOID DOOR", "DARK VEXIRK",
"EVIL GUARD MINION", "SKELETON", "AMPLIFIER", "WOLF",
"PIT GHOST", "DOOR GHOST", "VEXIRK KING", "OBELISK?",
"AXEMAN THIEF", "FLYING CHEST", "BARREL", "PEDISTAL",

"GHOST", "MINION?", "MINION?", "DEFAULT?"
	};


static const char* txt_dm2_musics[] = {
	"NOSTALGIC (AXE GIANTS)", "ACTION THEME", "THE VAULT", "CAVES (BATS)",
	"BATTERING RAMS", "SKULLKEEP MARCH", "UNDERGROUNDS (FIRE ORB)", "MYSTERIOUS CEMETERY",
	"WORMS TRAP", "SUN CLAN WILDERNESS", "GUITAR THEME 1", "GUITAR THEME 2",
	"SKULLKEEP ENTRANCE", "SKULLKEEP CORE ROOM", "FOREST: WOLVES", "VEXIRK CHANT",
	"DRU TAN'S CAVES", "SUN CLAN VILLAGE", "MYSTERIOUS CHOIRS (GNOMUS)", "UNDERGROUNDS (SKELETON)",
	"SUN CLAN RA CULT (VORTEX)", "BYDLO", "THICKET THIEF", "MYSTERIOUS (TREANTS)",
	"CRYSTAL ROOM 1", "CRYSTAL ROOM 2", "CRYSTAL ROOM 3", "CRYSTAL ROOM 4",
	"FINAL BATTLE (DRAGOTH)", "x1D", "x1E", "x1F"
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

char **txt_act_walls = NULL;
char **txt_act_floors = NULL;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

int txtX, txtY;
int line, column;
float textr;
float textg;
float textb;
int textsize;

int iStdFntSize = 20; // original was 14 

void
properFlushTextStrings(char*** txt_table)
{
	int i = 0;

	if (*txt_table != NULL)
	{
		for (i = 0; i < 128; i++)
		{
			if ((*txt_table)[i] != NULL)
				free((*txt_table)[i]);
		}
		free(*txt_table);
		*txt_table = NULL;
	}
}

void
allocSafeTextStrings(char*** txt_table)
{
	int i = 0;

	if (*txt_table != NULL)
		properFlushTextStrings(txt_table);

	*txt_table = (char**) calloc (128, sizeof(char*));
	{
		for (i = 0; i < 128; i++)
		{
			(*txt_table)[i] = (char*) calloc(128, sizeof(char));
			sprintf((*txt_table)[i], "act. %02d", i);
		}
	}
}

void
reinitTextStrings()
{
	int i = 0;

	properFlushTextStrings(&txt_act_walls);
	properFlushTextStrings(&txt_act_floors);
	allocSafeTextStrings(&txt_act_walls);
	allocSafeTextStrings(&txt_act_floors);

	if (!SKULLKEEP)
	{
		for (i = 0; i < 18; i++)
			sprintf(txt_act_floors[i], txt_complete_floor_actuators[i]);
		for (i = 0; i < 19; i++)
			sprintf(txt_act_walls[i], txt_complete_wall_actuators[i+1]);
		sprintf(txt_act_walls[127], txt_complete_wall_actuators[0]);
	}
	else if (SKULLKEEP)
	{
		for (i = 0; i < 76; i++)
			sprintf(txt_act_floors[i], txt_dm2_floor_actuators[i]);

		for (i = 0; i < 76; i++)	
			sprintf(txt_act_walls[i], txt_dm2_wall_actuators[i]);
		sprintf(txt_act_walls[126], "Cryo Cell Resurrector");
	}
}

void
beginTextOutput ()
{
	line = 0;
	column = 0;
}	

void
setTextProperties (int size, double r, double g, double b)
{
	textsize = size;
	textr = (float) r;
	textg = (float) g;
	textb = (float) b;
	fontSize (size);
	fontColor ((float) r, (float) g, (float) b);
}

void
setWarningText ()
{
	setTextProperties (11, 1, .3, .3);
}

void
outputTextLineAt (int x, int y, char* string, ...)
{
	char buffer[300];
	va_list args;
	column = 0;
	va_start (args, string);
	vsprintf (buffer, string, args);
	fontSize (textsize);
	fontColor ((float) textr, (float) textg, (float) textb);
	fontDrawString (x, y, buffer);
	va_end (args);
}


void
outputTextLine (char* string, ...)
{
	char buffer[300];
	va_list args;
	column = 0;
	va_start (args, string);
	vsprintf (buffer, string, args);
	fontSize (textsize);
	fontColor ((float) textr, (float) textg, (float) textb);
	fontDrawString (txtX, txtY-12*line, buffer);
	va_end (args);
	line ++;
}

void
outputTextString (char* string, ...)
{
	char buffer[300];
	va_list args;
	va_start (args, string);
	vsprintf (buffer, string, args);
	fontSize (textsize);
	fontColor ((float) textr, (float) textg, (float) textb);
	fontDrawString (txtX+12*column, txtY-12*line, buffer);
	va_end (args);
	column += strlen (string);
}

//------------------------------------------------------------------------------

void
loadTextFont ()
{
	fontLoad ("font.tga");
}

//------------------------------------------------------------------------------

// monster priority colors
static double monsterColors[][3] =
{
	{.8, .2, 0},	// red
	{1, .5, 0},		// orange
	{0, .5, 1},		// blue
	{.9, .2, 1},	// purple
	{.7, .4, 0},	// brown
	{.1, .8, .2},	// green
	{1, .9, .1},	// yellow
	{.8, .8, .8},	// no specific
};

char* txtMonsterColors[8] = {
	"Dark Red", "Orange", "Blue", "Purple",
	"Brown", "Green", "Golden", "Colorless"
};

static void
printCoordinates ()
{	

	unsigned char color = MColors[getEditCursor (cursor_L)];

	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, winH-iStdFntSize, "F1: HELP");

	if (SKULLKEEP == 1)
		setTextProperties (iStdFntSize, .5, .9, 1);
	else
		setTextProperties (iStdFntSize, .7, .7, .7);
	fontDrawString (22 + (iStdFntSize*15), winH-iStdFntSize, "DM MODE : (%d) [%s]",
		SKULLKEEP, (SKULLKEEP == 1) ? "SKULLKEEP" : "CLASSIC DM");

	if (currentFileName != NULL) 
	{
		setTextProperties (iStdFntSize, 1, 1, .5);
		fontDrawString (22 + (iStdFntSize*42), winH-iStdFntSize, "DUNGEON : [%s]", currentFileName);
	}
	
	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, winH-38, "COORDS : R (%02d,%02d,%02d)",
		getEditCursor (cursor_L), getEditCursor (cursor_X), getEditCursor (cursor_Y));
	
	setTextProperties (iStdFntSize, .8, .6, .95);
	fontDrawString (22 + (21*iStdFntSize), winH-38, " A (%02d,%02d,%02d) / DEPTH: %02d",
		(getLevels()[getEditCursor (cursor_L)]).header.level,
		(getLevels()[getEditCursor (cursor_L)]).header.xOffset + getEditCursor (cursor_X),
		(getLevels()[getEditCursor (cursor_L)]).header.yOffset + getEditCursor (cursor_Y),
		(getLevels()[getEditCursor (cursor_L)]).header.depth); 

	if (!SKULLKEEP)
	{
		setTextProperties (iStdFntSize, monsterColors[color][0], monsterColors[color][1], monsterColors[color][2]);
		fontDrawString (22 + (50*iStdFntSize), winH-38, "COLOR : %d", color);
	}
}

static void
printLevelStack ()
{
	int iOffsetX = 11; // old = 6
	int iFntSize = 14; // old = 11
	unsigned int i;
	for (i = 0; i < MAX_LEVELS; i++)
	{
		int iVFntSize = iFntSize;
		if (isEditingTarget () && getEditCursor (cursor_TM) == (char) i)
			setTextProperties (iVFntSize, .4, .9, 1);
		
		else if (i == (unsigned int)getEditCursor (cursor_L))
		{
			iVFntSize += 2;
			setTextProperties (iVFntSize, 1, 1, .8);
		}
		else if (i >= getDungeon()->nLevels)
			setTextProperties (iVFntSize, .4, .35, .35);
		else 
			setTextProperties (iVFntSize, .7, .7, .7);
		fontDrawString (iOffsetX, winH-50-(iFntSize+1)*i, "%02d", i);
	}
}

int iTileInfo_OffsetX = 860;
int iTileInfo_OffsetY = __DISP_MAP_START_Y__;

static void
printTileValue ()
{
	static char* sStairsLead[] = { "DOWN", "UP" };
	static char* sStairsFacing[] = { "WEST/EAST", "NORTH/SOUTH" };
	unsigned char* tile = (unsigned char*) getCurrentTile ();
	tile_p ptile = (tile_p) tile;
	tile_stairs_p stairs = (tile_stairs_p) tile;

	setTextProperties (17, .5, .7, 1);
	fontDrawString (iTileInfo_OffsetX, winH-iTileInfo_OffsetY, "TILE: (%02X)", *tile);

	if (ptile->type == tile_Stairs)
	{
		setTextProperties (17, .6, .8, 1);
		fontDrawString (iTileInfo_OffsetX, winH-iTileInfo_OffsetY, "TILE: (%02X) L=%d F=%d > %s : %s", *tile, stairs->leading, stairs->facing,
			sStairsLead[stairs->leading], sStairsFacing[stairs->facing]);
	}

	//fontDrawString (winW - 200, winH-(32*19), "TILE: (%02X)", *tile);
}

static void
printReferenceValue ()
{
	int iCategoryRef = 0;
	int iPosition = 0;
	reference_p ref = getStackReference (getEditCursor (cursor_Stack));
	tile_p tile;

	iCategoryRef = ref->category;
	iPosition = ref->position;

	tile = getCurrentTile(),//->type == tile_Wall)


	setTextProperties (17, .5, .7, 1);
	fontDrawString (iTileInfo_OffsetX, winH-iTileInfo_OffsetY-19, "REF: (%04X) / CAT:(%1X)", *((unsigned short*) ref), iCategoryRef);
	//fontDrawString (winW - 200, winH-(33*19), "REF: (%04X)", *((unsigned short*) ref));
	
	setTextProperties (17, .5, .7, 1);
	setTextProperties (17, text_cat_colors[iCategoryRef][0], text_cat_colors[iCategoryRef][1], text_cat_colors[iCategoryRef][2]);

	if (tile->type == tile_Wall)
		fontDrawString (iTileInfo_OffsetX, winH-iTileInfo_OffsetY-19-19, "ID: (%03X) / POS: (%1d-%s)", ref->id, iPosition, txt_position_ortho[iPosition]);
	else
		fontDrawString (iTileInfo_OffsetX, winH-iTileInfo_OffsetY-19-19, "ID: (%03X) / POS: (%1d-%s)", ref->id, iPosition, txt_position_quarter[iPosition]);

	//fontDrawString (winW - 200, winH-(34*19), "ID: (%03X)", ref->id);
}


//------------------------------------------------------------------------------

void
printGeneralMessage (char *message)
{
	setTextProperties (27, .2, .5, 1);
	fontDrawString (100, winH-27, "{%s}", message);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

extern int level_spec;



void
getDoorName(char* sDest, int iDoorNumber)
{
	sprintf(sDest, "UNKNOWN DOOR %02d", iDoorNumber);
	if (iDoorNumber >= 0 && iDoorNumber < 4)
		strcpy(sDest, txt_door_names[iDoorNumber]);
}


void
getTilesetName(char* sDest, int iTilesetNumber)
{
	sprintf(sDest, "UNKNOWN TILESET %02d", iTilesetNumber);

	if (SKULLKEEP)
	{
		if (iTilesetNumber >= 0 && iTilesetNumber < 6)
			strcpy(sDest, txt_tileset_names[iTilesetNumber]);
	}
	else
		strcpy(sDest, "CLASSIC DUNGEON");
}


int iBlinkAngle = 0;

void
printAIDetailsInfos ()
{
	double fsinv = 0;
	double rad = 0;

	int B = 0;
	int iFntSize = 24;
	int iFntSmaller = 16;
	int iFntHexSize = 24;
	char sHelpObjectText[256];

	int x = 75;//winW-370;
	int y = winH - 75;
	int i = 0;
	int iRowText = 0;
	int iStepText = iFntSize+2;
	char cSep = '-';

	int iExeSizeLoaded = xSkullExe.iExeDataSize;
	int iSizeOfAIDef = 0;
	tSKEXE_AI_Definition* xAIDef = NULL;
	unsigned char* xpAIDef = NULL;
	int iNbAI = 0;

	int iCursorAIIndex = 0;
	int iCursorAIAttribute = 0;
	int iCurrentValue = 0;
	float fValueColor[3] = { .9, .9, .9 };

	memset(sHelpObjectText, 0, 256);

	iSizeOfAIDef = sizeof(tSKEXE_AI_Definition);

	//printf("SkullExe (print AI) : %08x\n", &xSkullExe);

	if (iExeSizeLoaded > 0)
	{
		xAIDef = &(xSkullExe.xAI[0]);
		xpAIDef = (unsigned char*) xAIDef;
		iNbAI = xSkullExe.iNumberOfAI;
	}

	iCursorAIIndex = getEditCursor (cursor_AI_index);
	iCursorAIAttribute = getEditCursor (cursor_AI_attribute);


	{
		iBlinkAngle = iBlinkAngle + 1;
		if (iBlinkAngle > 360)
			iBlinkAngle = iBlinkAngle%360;
		rad = ((double)iBlinkAngle) / 360 * (2*3.1415f) * 6;
		fsinv = (double)cos(rad);
		fsinv = (fsinv/2) + 0.75f;
		//printf("iBlink = %d / rad = %f / fsinv = %f\n", iBlinkAngle, rad, fsinv);


		//if (edit != 1)
		//	fsinv = 1;

		//2.0f, r*fsinv, g*fsinv, b*fsinv, target);
	}




	setTextProperties (iFntSize, .9, .7, .7);
	y -= iStepText;
	fontDrawString (x, y, "SKULL.EXE SIZE: %06d", iExeSizeLoaded);
	//outputTextLineAt (200, winH-70, "Size of Skull.exe : %06d", iExeSizeLoaded);

	setTextProperties (iFntSize, .9, .7, .7);
	y -= iStepText;
	fontDrawString (x, y, "SIZE OF AI DEF: %d", iSizeOfAIDef);

	iFntHexSize = (winW/2) / (iSizeOfAIDef*2);

	//--- display the exact hex values of the DM header
	if (xAIDef != NULL)
	{
		float fStdColor[3] = { .7, .7, .7 };
		float fLowColor[3] = { .3, .3, .3 };
		float fHPColor[3] = { .4, 1.0f, .2 };
		float fArmorColor[3] = { .9, .9, .9 };
		float fRedColor[3] = { 1.0, .1, .1 };
		float fPoisonColor[3] = { 0.1, 0.8, 0.1 };
		float* fPtrColor = fStdColor;
		float fFinalColor[3] = {1.0, 0, 1.0};
		int iStepCodon = (iFntHexSize*3 - 2*4);
		char ainame[32]; memset(ainame, 0, 32);
		iStepText = iFntSize-10;
		for (i = 0; i < iNbAI; i++)
		{
			cSep = '-';
			xAIDef = &(xSkullExe.xAI[i]);
			xpAIDef = (unsigned char*) xAIDef;
			y -= iStepText;

			x = 75;
			setTextProperties (iFntHexSize, .7, 1.0f, 1.0f);
			fontDrawString (x - (iStepCodon*2), y, "%02d", i);

			fFinalColor[0] = 0.7f;
			fFinalColor[1] = 1.0f;
			fFinalColor[2] = 1.0f;
			if (iCursorAIIndex == i)
			{
				fFinalColor[0] = fFinalColor[0]*fsinv;
				fFinalColor[1] = fFinalColor[1]*fsinv;
				fFinalColor[2] = fFinalColor[2]*fsinv;
			}
			setTextProperties (iFntHexSize, fFinalColor[0], fFinalColor[1], fFinalColor[2]);
			strncpy(ainame, txt_skull_ai_groupnames[i], 18);
			fontDrawString (x - (iStepCodon*1), y, "%16s", ainame);
			
			x = 275;
			
			for (B = 0; B < iSizeOfAIDef; B++)
			{

				fPtrColor = fStdColor;

				if (B == 2)	// Armor
				{
					fPtrColor = fArmorColor;
					if (xpAIDef[B] == 0xFF)
						fPtrColor = fRedColor;
				}


				if (B == 4 || B == 5)	// HP
					fPtrColor = fHPColor;

				if (B == 6)	// Attack Strength
				{
					fPtrColor = fRedColor;
				}
				if (B == 7)	// Poison
				{
					fPtrColor = fPoisonColor;
				}


				if (B == (iSizeOfAIDef-1))
					cSep = ' ';

				if (B == 8)	// ??
				{
					fPtrColor = fArmorColor;
					if (xpAIDef[B] == 0xFF)
						fPtrColor = fRedColor;
				}
				if (B == 24)	// ??
				{
					fPtrColor = fArmorColor;
					if (xpAIDef[B] == 0xFF)
						fPtrColor = fRedColor;
				}
				if (B == 29)	// ??
				{
					fPtrColor = fArmorColor;
					if (xpAIDef[B] == 0xFF)
						fPtrColor = fRedColor;
				}

				
				if (xpAIDef[B] == 0)
					fPtrColor = fLowColor;

				fFinalColor[0] = fPtrColor[0];
				fFinalColor[1] = fPtrColor[1];
				fFinalColor[2] = fPtrColor[2];

				if (iCursorAIIndex == i && iCursorAIAttribute == B)
				{
					fValueColor[0] = fFinalColor[0];
					fValueColor[1] = fFinalColor[1];
					fValueColor[2] = fFinalColor[2];

					fFinalColor[0] = fFinalColor[0]*fsinv;
					fFinalColor[1] = fFinalColor[1]*fsinv;
					fFinalColor[2] = fFinalColor[2]*fsinv;
					iCurrentValue = xpAIDef[B];
				}


				setTextProperties (iFntHexSize, fFinalColor[0], fFinalColor[1], fFinalColor[2]);
				fontDrawString (x+((iStepCodon)+2)*B, y, "%1X", (xpAIDef[B] & 0xF0)>>4);
				setTextProperties (iFntHexSize, fFinalColor[0], fFinalColor[1], fFinalColor[2]);
				fontDrawString (x+((iStepCodon)+2)*B+(iFntHexSize-2), y, "%1X", (xpAIDef[B] & 0x0F));
				setTextProperties (iFntHexSize, fStdColor[0], fStdColor[1], fStdColor[2]);
				fontDrawString (x+((iStepCodon)+2)*B+(iFntHexSize-2)*2, y, "%c", cSep);
			}
		}
	}
	

	//--- Display info about selected value
	y = winH - 75;
	setTextProperties (iFntSize, .9, .7, .7);
	y -= iStepText;
	x = 700;
	fontDrawString (x, y, "ATTRIBUTE %02d: %s", iCursorAIAttribute, txt_dm2_ai_attributes[iCursorAIAttribute]);

	if (iCursorAIAttribute == 2 && iCurrentValue == 0xFF)
		strcpy(sHelpObjectText, "(DON'T TAKE DAMAGE)");
	else if (iCursorAIAttribute == 8 && iCurrentValue == 0xFF)
		strcpy(sHelpObjectText, "(CAN'T BE HIT)");
	else if (iCursorAIAttribute == 29 && iCurrentValue == 0xFF)
		strcpy(sHelpObjectText, "(CAN'T BE MOVED)");

	y -= (iStepText*1.5);
	setTextProperties (iFntSize, fValueColor[0], fValueColor[1], fValueColor[2]);
	fontDrawString (x, y, "VALUE: %02X %s", iCurrentValue, sHelpObjectText);

	//--- Flag infos
	y -= (iStepText*2.5);
	if (iCursorAIAttribute == 14)
	{
		int iFlagActive = 0;
		char sFlagActive = '.';
		int iLocalStepText = iStepText*1.5;
		x = 1300;
		setTextProperties (iFntSmaller, .9, .9, 0);
		if (iCurrentValue & 0x80) sFlagActive = 'X'; else sFlagActive = '.';
		fontDrawString (x, y, "%c x80: POISON CLOUD", sFlagActive);
		y -= (iLocalStepText);
		setTextProperties (iFntSmaller, .9, .9, 0);
		if (iCurrentValue & 0x40) sFlagActive = 'X'; else sFlagActive = '.';
		fontDrawString (x, y, "%c x40: LIGHTNING", sFlagActive);
		y -= (iLocalStepText);
		setTextProperties (iFntSmaller, .9, .9, 0);
		if (iCurrentValue & 0x20) sFlagActive = 'X'; else sFlagActive = '.';
		fontDrawString (x, y, "%c x20: DISPELL", sFlagActive);
		y -= (iLocalStepText);
		setTextProperties (iFntSmaller, .9, .9, 0);
		if (iCurrentValue & 0x10) sFlagActive = 'X'; else sFlagActive = '.';
		fontDrawString (x, y, "%c x10: FIREBALL", sFlagActive);
		y -= (iLocalStepText);
		setTextProperties (iFntSmaller, .9, .9, 0);
		if (iCurrentValue & 0x08) sFlagActive = 'X'; else sFlagActive = '.';
		fontDrawString (x, y, "%c x08: SHOOT", sFlagActive);
		y -= (iLocalStepText);
		setTextProperties (iFntSmaller, .9, .9, 0);
		if (iCurrentValue & 0x04) sFlagActive = 'X'; else sFlagActive = '.';
		fontDrawString (x, y, "%c x04: STEAL", sFlagActive);
		y -= (iLocalStepText);
		setTextProperties (iFntSmaller, .9, .9, 0);
		if (iCurrentValue & 0x02) sFlagActive = 'X'; else sFlagActive = '.';
		fontDrawString (x, y, "%c x02: PUSH BACK", sFlagActive);
		y -= (iLocalStepText);
		setTextProperties (iFntSmaller, .9, .9, 0);
		if (iCurrentValue & 0x01) sFlagActive = 'X'; else sFlagActive = '.';
		fontDrawString (x, y, "%c x01: MELEE", sFlagActive);
		y -= (iLocalStepText);
	}

}


void
printDungeonSpecificationsInfo ()
{
	int B = 0;
	int iFntSize = 24;
	int iFntSizeBigTitle = 32;
	int iFntHexSize = 32;
	char sHelpObjectText[256];

	int iMapStartOffset = 0;
	int iMapEndOffset = 0;

	int x = 75;//winW-370;
	int y = winH - iFntSizeBigTitle*4;
	int iRowText = 0;
	int iStepText = iFntSize+2;
	char cSep = '-';

	int i = 0;
	int g = 0;

	unsigned char* xHexDungeonHeader = NULL;

	dm_dungeon_header* xDungeonHeader = NULL;
	xDungeonHeader = getDungeon();

	xHexDungeonHeader = (unsigned char*) (xDungeonHeader);

	memset(sHelpObjectText, 0, 256);

	setTextProperties (iFntSizeBigTitle, .5, 1, .8); 
	outputTextLineAt (200, winH-40, "DUNGEON INFORMATION");

	//--- Hex raw of header
	for (B = 0; B < 12; B++)
	{
		if (B == 11)
			cSep = ' ';
		setTextProperties (iFntHexSize, .7, .7, .7);
		fontDrawString (x+((iFntHexSize*3)+2)*B, y+(15*2), "%02X%c", xHexDungeonHeader[B], cSep);
	}

	y -= iStepText;

	B = 0;	// random seed
	setTextProperties (iFntHexSize, 0.0, 1.0, 1.0);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", (xHexDungeonHeader[B]));
	B = 1;	// random seed
	setTextProperties (iFntHexSize, 0.0, 1.0, 1.0);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", (xHexDungeonHeader[B]));

	B = 2;	// map data size
	setTextProperties (iFntHexSize, 1.0, 1.0, 1.0);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", (xHexDungeonHeader[B]));
	B = 3;	// map data size
	setTextProperties (iFntHexSize, 1.0, 1.0, 1.0);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", (xHexDungeonHeader[B]));

	B = 4;	// nb maps
	setTextProperties (iFntHexSize, 1.0, 0.2, 0.2);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", (xHexDungeonHeader[B]));

	B = 6;	// text data size
	setTextProperties (iFntHexSize, .5, .7, .9);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", xHexDungeonHeader[B]);
	B = 7;	// text data size
	setTextProperties (iFntHexSize, .5, .7, .9);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", xHexDungeonHeader[B]);

	B = 10;	// item list data size
	setTextProperties (iFntHexSize, 1.0, 1.0, 0.5);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", xHexDungeonHeader[B]);
	B = 11;	// item list data size
	setTextProperties (iFntHexSize, 1.0, 1.0, 0.5);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", xHexDungeonHeader[B]);


	y -= iStepText;
	setTextProperties (iFntSize, 0.0, 1.0, 1.0);
	y -= iStepText;
	fontDrawString (x, y, "RANDOM SEED: x%04x (%02d) [%c%c]", xDungeonHeader->randomGraphicsSeed, xDungeonHeader->randomGraphicsSeed, 
		xHexDungeonHeader[0], xHexDungeonHeader[1]);

	setTextProperties (iFntSize, 1.0, 1.0, 1.0);
	y -= iStepText;
	fontDrawString (x, y, "MAP DATA SIZE: x%04X (%05d)", xDungeonHeader->mapDataSize, xDungeonHeader->mapDataSize);

	setTextProperties (iFntSize, 1.0, 0.2, 0.2);
	y -= iStepText;
	fontDrawString (x, y, "NB. MAPS: %02d (x%01X) (MAX = 255?)", xDungeonHeader->nLevels, xDungeonHeader->nLevels);

	setTextProperties (iFntSize, .9, .7, .7);
	y -= iStepText;
	fontDrawString (x, y, "UNUSED: x%01X (%02d)", xDungeonHeader->null, xDungeonHeader->null);
	
	setTextProperties (iFntSize, .5, .7, .9);
	y -= iStepText;
	fontDrawString (x, y, "TEXT DATA SIZE: x%02X (%04d)", xDungeonHeader->textsDataSize, xDungeonHeader->textsDataSize);

	setTextProperties (iFntSize, .9, .7, .7);
	y -= iStepText;
	fontDrawString (x, y, "START X: %02d", xDungeonHeader->x_start);

	setTextProperties (iFntSize, .9, .7, .7);
	y -= iStepText;
	fontDrawString (x, y, "START Y: %02d", xDungeonHeader->y_start);

	setTextProperties (iFntSize, .9, .7, .7);
	y -= iStepText;
	fontDrawString (x, y, "START F: %02d", xDungeonHeader->f_start);

	setTextProperties (iFntSize, .9, .7, .7);
	y -= iStepText;
	fontDrawString (x, y, "START M: %02d (IGNORED)", xDungeonHeader->l_start);

	setTextProperties (iFntSize, 1.0, 1.0, 0.5);
	y -= iStepText;
	fontDrawString (x, y, "ITEM LIST SIZE: x%02X (%02d)", xDungeonHeader->itemListSize, xDungeonHeader->itemListSize);

	y -= iStepText;
	y -= iStepText;

	//--- Next part: number of items per category
	for (i = 0; i < 16; i++)
	{
		unsigned char* xHexObjectNb = 0;
		xHexObjectNb = (unsigned char*) &(xDungeonHeader->nObjects[i]);

		g = i%8;

		B = 0;
		setTextProperties (iFntHexSize, text_cat_colors[i][0], text_cat_colors[i][1], text_cat_colors[i][2]);
		fontDrawString (x+((iFntHexSize*2)+2)*(g*2+B), y,			"%02X", (xHexObjectNb[B]));
		B = 1;
		setTextProperties (iFntHexSize, text_cat_colors[i][0], text_cat_colors[i][1], text_cat_colors[i][2]);
		fontDrawString (x+((iFntHexSize*2)+2)*(g*2+B), y,			"%02X", (xHexObjectNb[B]));

		if (i == 7)
			y -= iStepText;
	}

	y -= iStepText;
	//--- Nb of items per category
	for (i = 0; i < 16; i++)
	{
		setTextProperties (iFntSize, text_cat_colors[i][0], text_cat_colors[i][1], text_cat_colors[i][2]);
		fontDrawString (x, y, "#NB. %16s : %04d (%04X) (%02d %%)", txt_ObjectCategory_Display[i], xDungeonHeader->nObjects[i], xDungeonHeader->nObjects[i], xDungeonHeader->nObjects[i]*100/1022);
		y -= iStepText;
	}

/*
	unsigned short nObjects[16];
	*/

}
void
printGeneralHelpInfo ()
{
	int iLocalFntSize = 15;
	int y = winH-iStdFntSize;
	int iStepText = iLocalFntSize+2;

	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "DMBUILDER GENERAL HELP");
	y -= iStepText;
	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "----------------------\n");

	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F1 : General Help (this screen)\n");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F2 : Level Properties\n");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F3 : List of Items\n");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F4 : List of Creatures\n");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F5 : List of Actuators\n");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F6 : Main Header Dungeon Properties\n");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F7 : DM2 AI Table Values\n");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F8 : DM2 AI Table Values\n");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F9 : Invoke load dungeon screen");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F10 : Export text to file");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F11 : Import text to file");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "F12 : Invoke save dungeon screen");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "ESC : Back to main map editor screen");
	y -= iStepText;	setTextProperties (iStdFntSize, 1, 1, 1);
	fontDrawString (22, y, "How to quit DMBuilder ? close the window.\n");
}

void
printLevelSpecificationsInfo ()
{
	static char* txtLSpec[] = { "GRAPHISMS", "SPECIFICATIONS" };
	//level_p level = (level_p) &getLevels()[getEditCursor (cursor_L)];
	level_p level = (level_p) getLevel(getEditCursor (cursor_L));
	unsigned char* xHexLevelHeader = NULL;
	sk_level_header* skLevelHeader = NULL;
	int B = 0;
	int iFntSize = 24;
	int iFntHexSize = 32;
	char sHelpObjectText[256];
	char sMusicName[256];

	int iMapStartOffset = 0;
	int iMapEndOffset = 0;

	int x = 75;//winW-370;
	int y = winH - 520;
	int iRowText = 0;
	int iStepText = iFntSize+2;
	char cSep = '-';

	int iMusicID = 0;

	memset(sHelpObjectText, 0, 256);
	memset(sMusicName, 0, 256);

	//--- display the exact hex values of the DM header
	xHexLevelHeader = (unsigned char*) &(level->header);
	skLevelHeader = (sk_level_header*) &(level->header);

	iMapStartOffset = level->startoffset;
	iMapEndOffset = level->endoffset;

	iMusicID = level->iMusicID;

	setTextProperties (iFntHexSize, 1.0, 1.0, 1.0);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y+(15*5),"MAP HEADER 16 BYTES -- HEXA BREAKDOWN ");

	for (B = 0; B < 16; B++)
	{
		if (B == 15)
			cSep = ' ';
		setTextProperties (iFntHexSize, .7, .7, .7);
		fontDrawString (x+((iFntHexSize*3)+2)*B, y+(15*2), "%02X%c", xHexLevelHeader[B], cSep);
	}

	y -= iStepText;

	B = 0;	// map data offset
	setTextProperties (iFntHexSize, 1.0, 1.0, 1.0);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", (level->header.mapOffset & 0xFF00)>>8);
	B = 1;	// map data offset
	setTextProperties (iFntHexSize, 1.0, 1.0, 1.0);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", (level->header.mapOffset & 0x00FF));

	B = 6;	// x offset
	setTextProperties (iFntHexSize, .5, .9, .5);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", level->header.xOffset);
	B = 7;	// y offset
	setTextProperties (iFntHexSize, .5, .5, .9);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%02X", level->header.yOffset);

	B = 8;
	setTextProperties (iFntHexSize, .1, 0.9, 1.0);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%1X", (xHexLevelHeader[B]&0xF0)>>4);	//	part of x ?
	setTextProperties (iFntHexSize, 1.0, 0.5, 0.0);
	fontDrawString (x+((iFntHexSize*3)+2)*B+iFntHexSize, y,	"%1X", xHexLevelHeader[B]&0x0F);		// level stack altitude

	B = 9;
	setTextProperties (iFntHexSize, .1, 1.0, 0.6);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%1X", (xHexLevelHeader[B]&0xF0)>>4);	//	part of y ?
	setTextProperties (iFntHexSize, .1, 0.9, 1.0);
	fontDrawString (x+((iFntHexSize*3)+2)*B+iFntHexSize, y,	"%1X", xHexLevelHeader[B]&0x0F);		// part of x and y ?


	B = 10;
	setTextProperties (iFntHexSize, .6, .2, .2);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%1X", (xHexLevelHeader[B]&0xF0)>>4);	// random walls
	setTextProperties (iFntHexSize, .8, .4, .4);
	fontDrawString (x+((iFntHexSize*3)+2)*B+iFntHexSize, y,	"%1X", xHexLevelHeader[B]&0x0F);		// nb walls

	B = 11;
	setTextProperties (iFntHexSize, .2, .6, .2);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%1X", (xHexLevelHeader[B]&0xF0)>>4);	// random floor
	setTextProperties (iFntHexSize, .4, .8, .4);
	fontDrawString (x+((iFntHexSize*3)+2)*B+iFntHexSize, y,	"%1X", xHexLevelHeader[B]&0x0F);		// nb floors
	
	B = 12;
	setTextProperties (iFntHexSize, .9, .1, .1);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%1X", (xHexLevelHeader[B]&0xF0)>>4);	// nb monsters
	setTextProperties (iFntHexSize, .9, .8, .1);
	fontDrawString (x+((iFntHexSize*3)+2)*B+iFntHexSize, y,	"%1X", xHexLevelHeader[B]&0x0F);		// nb door ornates
	
	B = 13;
	setTextProperties (iFntHexSize, .8, .5, 1.0);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%1X", (xHexLevelHeader[B]&0xF0)>>4);	// depth
	B = 14;
	setTextProperties (iFntHexSize, .7, .3, .8);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%1X", (xHexLevelHeader[B]&0xF0)>>4);	// tileset
	B = 15;
	setTextProperties (iFntHexSize, .9, .5, .1);
	fontDrawString (x+((iFntHexSize*3)+2)*B, y,			"%1X", (xHexLevelHeader[B]&0xF0)>>4);	// door 2
	setTextProperties (iFntHexSize, .5, .9, .1);
	fontDrawString (x+((iFntHexSize*3)+2)*B+iFntHexSize, y,	"%1X", xHexLevelHeader[B]&0x0F);		// door 1


	setTextProperties (iFntSize, .9, .7, .7);
	y -= iStepText;
	fontDrawString (x, y, "EDITING: %s", txtLSpec[level_spec]);

	setTextProperties (iFntSize, .8, .9, .9);
	y -= iStepText;
	fontDrawString (x, y, "MAP ID: %d", getEditCursor (cursor_L));
	if (getEditCursor (cursor_LevelSpec) == 0)
		setTextProperties (iFntSize, .9, 1, .9);
	else	setTextProperties (iFntSize, .7, .7, .7);
	y -= iStepText; setTextProperties (iFntSize, .8, .5, 1.0);
	fontDrawString (x, y, "DIFFICULTY: %d", level->header.depth);

	if (getEditCursor (cursor_LevelSpec) == 1)
		setTextProperties (iFntSize, .9, 1, .9);
	else	setTextProperties (iFntSize, .5, .9, .5);
	y -= iStepText;	
	fontDrawString (x, y, "X OFFSET: %02d (%02X)", level->header.xOffset, level->header.xOffset);

	if (getEditCursor (cursor_LevelSpec) == 2)
		setTextProperties (iFntSize, .9, 1, .9);
	else	setTextProperties (iFntSize, .5, .5, .9);
	y -= iStepText;
	fontDrawString (x, y, "Y OFFSET: %02d (%02X)", level->header.yOffset, level->header.yOffset);

	y -= iStepText; setTextProperties (iFntSize, 1.0, 1.0, 1.0);
	fontDrawString (x, y, "MAP DATA OFFSET: %04X [ %04X-%04X ]", level->header.mapOffset, iMapStartOffset, iMapEndOffset);

	y -= iStepText; setTextProperties (iFntSize, 1.0, 0.5, 0.0);
	fontDrawString (x, y, "Z ALTITUDE: %d", level->header.level);

	y -= iStepText; setTextProperties (iFntSize, .1, 0.9, 1.0);
	fontDrawString (x, y, "DIMENSION X (+1): %02d (%02X)", level->header.xDim+1, level->header.xDim);
	y -= iStepText; setTextProperties (iFntSize, .1, 1.0, 0.6);
	fontDrawString (x, y, "DIMENSION Y (+1): %02d (%02X)", level->header.yDim+1, level->header.yDim);


	y -= iStepText; setTextProperties (iFntSize, .8, .4, .4);
	fontDrawString (x, y, "NB WALLS: %d", level->header.nWalls);
	y -= iStepText; setTextProperties (iFntSize, .6, .2, .2);
	fontDrawString (x, y, "RAND WALLS: %d", level->header.rWalls);
	y -= iStepText; setTextProperties (iFntSize, .4, .8, .4);
	fontDrawString (x, y, "NB FLOORS: %d", level->header.nFloors);
	y -= iStepText; setTextProperties (iFntSize, .2, .6, .2);
	fontDrawString (x, y, "RAND FLOORS: %d", level->header.rFloors);
	y -= iStepText; setTextProperties (iFntSize, .9, .8, .1);
	fontDrawString (x, y, "NB DOOR ORNATES: %d", level->header.nOrnates);
	y -= iStepText;	setTextProperties (iFntSize, .9, .1, .1);
	fontDrawString (x, y, "NB MONSTERS: %d", level->header.nMonsters);
	y -= iStepText;	setTextProperties (iFntSize, .7, .3, .8);
	getTilesetName(sHelpObjectText, skLevelHeader->tileset);
	fontDrawString (x, y, "TILESET: %d (%s)", skLevelHeader->tileset, sHelpObjectText);

	y -= iStepText;		setTextProperties (iFntSize, .5, .9, .1);
	getDoorName(sHelpObjectText, level->header.door1);
	fontDrawString (x, y, "DOOR 1: %d (%s)", level->header.door1, sHelpObjectText);
	y -= iStepText;		setTextProperties (iFntSize, .9, .5, .1);
	getDoorName(sHelpObjectText, level->header.door2);
	fontDrawString (x, y, "DOOR 2: %d (%s)", level->header.door2, sHelpObjectText);

	if (SKULLKEEP)
	{
		if (iMusicID < 64)
			strcpy(sMusicName, txt_dm2_musics[iMusicID]);
		y -= iStepText;		setTextProperties (iFntSize, .6, 0.9, 1.0);
		fontDrawString (x, y, "MUSIC : %d [%02X] %s", iMusicID, iMusicID, sMusicName);
	}


}
/*
typedef struct 
{
	unsigned short mapOffset;
	char unused1[4];
*/




//------------------------------------------------------------------------------

void
text_frame_simple_actuator (reference_p reference, int x, int y)
{
	simple_actuator_p xSimpleActuator = (simple_actuator_p) getItem (reference);
	short sSAct = *((short*) xSimpleActuator);
	int iLocalFntSize = 15;
	//x = winW-370;
	//y = winH/2;
	x = iInfoX;
	y = (winH - iInfoYNeg) - (y*__STD_STACK_SIZE__/2);

	iLocalFntSize = (int)(((double)iInfoFntSize) * 0.75);

	//if (reference->category == category_Weapon)
	{
		setTextProperties (iInfoFntSize, .9, .9, .8);
		fontDrawString (x, y, "SIMPLE ACTUATOR");
		setTextProperties (iInfoFntSize, 1, .5, .4);
		fontDrawString (x + 15*iInfoFntSize, y, "-HEX: %04X", sSAct);

		setTextProperties (iLocalFntSize, 1, .5, .4);
		y -= iLocalFntSize;
		fontDrawString (x, y, "ACTIVATED: %02d", xSimpleActuator->Activated);
		setTextProperties (iLocalFntSize, 1, .5, .4);
		y -= iLocalFntSize;
		fontDrawString (x, y, "MODE:      %02d", xSimpleActuator->Mode);
		setTextProperties (iLocalFntSize, 1, .5, .4);
		y -= iLocalFntSize;
		fontDrawString (x, y, "GFX:       %02d (%02x)", xSimpleActuator->Graphism, xSimpleActuator->Graphism);
		setTextProperties (iLocalFntSize, 1, .5, .4);
		y -= iLocalFntSize;
		fontDrawString (x, y, "EXTENDED:  %02d (%s)", xSimpleActuator->ExtendedUsage, txt_dm2_sact_extuse[xSimpleActuator->ExtendedUsage]);
	}
	/*
		unsigned short	Activated:1;	// And for Text Visibility
	unsigned short 	Mode:2;			// 0 = text / 1 = active decoration / 2 = complex
	unsigned short	Graphism:8;	//2^7 = 128 (range: 0-7F)
	unsigned short	ExtendedUsage:5;
	*/
}


void
text_frame_weapon (reference_p reference, int x, int y)
{
	weapon_p weapon = (weapon_p) getItem (reference);
	//x = winW-370;
	//y = winH/2;
	x = iInfoX;
	y = (winH - iInfoYNeg) - (y*__STD_STACK_SIZE__/2);

	if (reference->category == category_Weapon)
	{
		setTextProperties (iInfoFntSize, .8, .9, .9);
		fontDrawString (x, y, "WEAPON");
		setTextProperties (iInfoFntSize, 1, .5, .4);
		y -= iInfoFntSize;
		fontDrawString (x, y, "TYPE: %02d (x%02X)", weapon->type, weapon->type);
		setTextProperties (iInfoFntSize, 1, .5, .4);
		y -= iInfoFntSize;
		fontDrawString (x, y, "KEEP IT: %02d", weapon->keep);
		setTextProperties (iInfoFntSize, 1, .5, .4);
		y -= iInfoFntSize;
		fontDrawString (x, y, "CHARGES: %02d", weapon->charges);
	}
}

void
text_frame_clothing (reference_p reference, int x, int y)
{
	clothing_p clothing = (clothing_p) getItem (reference);
	x = iInfoX;
	y = (winH - iInfoYNeg) - (y*__STD_STACK_SIZE__/2);

	if (reference->category == category_Clothing)
	{
		setTextProperties (iInfoFntSize, .8, .5, .1);
		fontDrawString (x, y, "CLOTHING");
		setTextProperties (iInfoFntSize, .6, .6, .4);
		y -= iInfoFntSize;
		fontDrawString (x, y, "TYPE: %02d (x%02X)", clothing->type, clothing->type);
		setTextProperties (iInfoFntSize, .6, .6, .4);
		y -= iInfoFntSize;
		fontDrawString (x, y, "KEEP IT: %02d", clothing->keep);
	}
}

void
text_frame_potion (reference_p reference, int x, int y)
{
	potion_p potion = (potion_p) getItem (reference);
	x = iInfoX;
	y = (winH - iInfoYNeg) - (y*__STD_STACK_SIZE__/2);

	if (reference->category == category_Potion)
	{
		setTextProperties (iInfoFntSize, .1, .7, 1);
		fontDrawString (x, y, "POTION");
		setTextProperties (iInfoFntSize, .1, 1, .8);
		y -= iInfoFntSize;
		fontDrawString (x, y, "KEEP IT : %02d", potion->keep);
		setTextProperties (iInfoFntSize, .1, 1, .8);
		y -= iInfoFntSize;
		fontDrawString (x, y, "POWER: %02d", potion->power);
	}
}

void
text_frame_container (reference_p reference, int x, int y)
{
	chest_p chest = (chest_p) getItem (reference);
	x = winW-370; 
	y = (winH - iInfoYNeg) - (y*__STD_STACK_SIZE__/2);

	if (reference->category == category_Chest)
	{
		int type = 0;

		type = objectContainerGetType((short*) chest);

		setTextProperties (iInfoFntSize, 1.0, 1.0, .1);
		fontDrawString (x, y, "CONTAINER or MAP");
		y -= iInfoFntSize;

		setTextProperties (iInfoFntSize, 0.75, .7, .0);
		fontDrawString (x, y, "TYPE: %02d", type);
		y -= iInfoFntSize;

		setTextProperties (iInfoFntSize, 0.75, .7, .0);
		fontDrawString (x, y, "Unknown 2: %04X", (unsigned short) chest->unk2);
		y -= iInfoFntSize;

		setTextProperties (iInfoFntSize, 0.75, .7, .0);
		fontDrawString (x, y, "POSSESSION: %04X", (unsigned short) chest->chested);

	}

}

void
text_frame_misc (reference_p reference, int x, int y)
{
	misc_p misc = (misc_p) getItem (reference);
	x = iInfoX;
	y = (winH - iInfoYNeg) - (y*__STD_STACK_SIZE__/2);

	if (reference->category == category_Miscs)
	{
		setTextProperties (iInfoFntSize, .1, 1, .1);
		fontDrawString (x, y, "MISCELLANEOUS (%03d | x%02x)", misc->type, misc->type);
		setTextProperties (iInfoFntSize, .1, .7, 1);
		y -= iInfoFntSize;
		fontDrawString (x, y, "KEEP IT : %02d", misc->keep);
		setTextProperties (iInfoFntSize, .1, .7, 1);
		y -= iInfoFntSize;
		fontDrawString (x, y, "CAPACITY: %d", misc->capacity);
	}
}

void
text_frame_scroll (reference_p reference, int x, int y)
{
	scroll2_p scroll = (scroll2_p) getItem (reference);
	int iUseGDATText = 0;
	x = iInfoX;
	y = (winH - iInfoYNeg) - (y*__STD_STACK_SIZE__/2);

	if (reference->category == category_Scroll)
	{
		if (scroll->reftxt > 0)
			iUseGDATText = 1;

		setTextProperties (iInfoFntSize, 0.8, 1.0, 1.0);
		fontDrawString (x, y, "SCROLL (%03d | x%02x)", scroll->type, scroll->type);
		setTextProperties (iInfoFntSize, .2, .8, .8);
		y -= iInfoFntSize;
		fontDrawString (x, y, "USE GDAT TXT : %02d (mode = M)", iUseGDATText);
		setTextProperties (iInfoFntSize, .2, .8, .8);
		if (iUseGDATText == 0)
			setTextProperties (iInfoFntSize, .1, .4, .4);
		y -= iInfoFntSize;
		fontDrawString (x, y, "TEXT REF     : %02d", scroll->reftxt);
	}
}

int yreftextinfo = 24;

void
text_frame_text (reference_p reference, int x, int y)
{
	text_p text = (text_p) getItem (reference);
	int iUseGDATText = 0;
	x = iInfoX;
	y = (winH - iInfoYNeg) - (y*__STD_STACK_SIZE__/2) + 4;

	if (reference->category == category_Text)
	{
		//if (scroll->reftxt > 0)
		//	iUseGDATText = 1;

		setTextProperties (iInfoFntSize, 0.8, 1.0, 1.0);
		fontDrawString (x, y, "TEXT (OFFSET %03d | x%02x)", text->offset, text->offset);
		setTextProperties (iInfoFntSize, .2, .8, .8);
		y -= iInfoFntSize;
		fontDrawString (x, y, "VISIBLE    : %0d", text->silent);
		setTextProperties (iInfoFntSize, .2, .8, .8);
		//if (iUseGDATText == 0)
		//	setTextProperties (iInfoFntSize, .1, .4, .4);
		y -= iInfoFntSize;
		fontDrawString (x, y, "FLAGS 2/3  : %d %d", text->flag2, text->flag3);
		y -= iInfoFntSize;
		yreftextinfo = y;
	}
}


char *
converttexttobuffer (char* s)
{
	static char buffer[300];
	int i, k = 0;
	int max = (strlen(s)>=299)?299:strlen(s);
	for (i = 0; i < max; i++)
	{
		if (s[i] == '{' || s[i] == '}')
			buffer[k++] = ' ';
		else if (s[i] == 0x7f && s[i+1] == 'c')
		{
			buffer[k++] = 't';
			buffer[k++] = 'h';
			buffer[k++] = 'e';
			buffer[k++] = ' ';
			i++;
		}
		else if (s[i] == 0x7f && s[i+1] == 'd')
		{
			buffer[k++] = 'y';
			buffer[k++] = 'o';
			buffer[k++] = 'u';
			buffer[k++] = ' ';
			i++;
		}
		else
			buffer[k++] = s[i] + 'A' - 'a';

	}
	buffer[k] = '\0';
	return buffer;
}

void
printSelectedText (reference_p refp)
{
	text_p text;
	int x = iInfoX;	// 100
	int y = yreftextinfo;
	setTextProperties (14, 0.8, 1.0, 1.0);
	if (SKULLKEEP == 0 && refp->category == category_Text)
	{	
		text = (text_p) getItem (refp);
		fontDrawString (x, y, "TEXT: \"%s\"", converttexttobuffer (getText(text->offset)));
	}
	else if (SKULLKEEP == 0 && refp->category == category_Scroll)
	{	
		scroll_p scroll = (scroll_p) getItem (refp);
		fontDrawString (x, y, "SCROLL: \"%s\"", converttexttobuffer (getText(scroll->offset)));
	}
}

void
displaySelectedTextList (unsigned int select)
{
	int t = 0;
	float textsize = 18; // was 11 before
	int size = (getTextsNumber()>=48)?48:getTextsNumber();
	for (t = -24; t < 24; t++)
	{
		setTextProperties (textsize, .7, .8, .9);
		if (t == 0)
			setTextProperties (textsize, 1, .9, .7);
		if (select + t >= 0 && select + t < getTextsNumber())
		fontDrawString (32, winH - (64 + (textsize-1)*(t+24)), "(%s)", converttexttobuffer (getText(select + t)));
	}
}


void
printMonsterHitPoints (reference_p refp)
{
	int i;
	monster_p monster = (monster_p) getItem (refp);
	
	setTextProperties (11, .4, 1, .2);
	fontDrawString (winW/3+120, winH-(64*9), "LEVEL : %d", getMonsterLevel (monster));
	for (i = 0; i < 4; i++)
	{
		if (i <= ((int) monster->number))
			setTextProperties (10, 1, .8, .1);
		else
			setTextProperties (10, .2, .4, .4);
		fontDrawString (winW/3+120, winH-((74+i*2)*8), "%d: (%d)", i, monster->health[i]);
	}
}

void
text_frame_monster (reference_p reference, int x, int y)
{
	int i;
	monster_p monster = (monster_p) getItem (reference);
	//x = winW-370; y = winH/2;
	x = iInfoX;
	//y = winH - iInfoYNeg;
	y = (winH - iInfoYNeg) - (y*__STD_STACK_SIZE__/2);

	if (reference->category == category_Monster)
	{
		char sMonsterName[256]; memset(sMonsterName, 0, 256);
		sprintf(sMonsterName, "???");
		if (!SKULLKEEP && monster->type <= 26)
			sprintf(sMonsterName, "%s", txt_monsters[monster->type]);
		else if (SKULLKEEP && monster->type <= 0x58)
			sprintf(sMonsterName, "%s", txt_monsters_sk[monster->type]);
		setTextProperties (iInfoFntSize, 1, .2, .2);
		fontDrawString (x, y, "CREATURE TYPE: %02x (%02d) \"%s\"", monster->type, monster->type, sMonsterName);
		y -= iInfoFntSize;

		setTextProperties (iInfoFntSize, .1, 1, .1);
		fontDrawString (x, y, "LEVEL : %d // #NB : %d", getMonsterLevel (monster), monster->number+1);
		// Old display : 1 HP per row
		/*
		for (i = 0; i < 4; i++)
		{
			if (i <= ((int) monster->number))
				setTextProperties (iInfoFntSize, 1, .8, .1);
			else
				setTextProperties (iInfoFntSize, .2, .4, .4);
			y -= iInfoFntSize;
			fontDrawString (x, y, "%d: (%d)", i, monster->health[i]);
		}
		*/
		y -= iInfoFntSize;
		setTextProperties (iInfoFntSize, .1, 1, .1);
		fontDrawString (x, y, "HP: ");
		x += (iInfoFntSize*4);
		for (i = 0; i < 4; i++)
		{
			if (i <= ((int) monster->number))
				setTextProperties (iInfoFntSize, 1, .8, .1);
			else
				setTextProperties (iInfoFntSize, .2, .4, .4);
			fontDrawString (x, y, "%03d", monster->health[i]);
			x += (iInfoFntSize*5);
		}
	}
}

void
text_frame_teleport (reference_p reference, int x, int y)
{
	static char* scope[] =
	{"OBJECT","CREATURE","PARTY or OBJECT","EVERYTHING"};
	teleport_p teleport = (teleport_p) getItem (reference);
	
	//x = winW-370; y = winH/2;
	x = iInfoX;
	y = (winH - iInfoYNeg) - (y*__STD_STACK_SIZE__/2);

	setTextProperties (iInfoFntSize, .1, .7, .9);
	fontDrawString (x, y, "TELEPORT SCOPE: (%s)", scope[teleport->scope]);
	setTextProperties (iInfoFntSize, .1, .7, .9);
	y -= iInfoFntSize;
	fontDrawString (x, y, "DESTINATION: [%02d](%02d,%02d)", teleport->ldest, teleport->xdest, teleport->ydest);

	setTextProperties (iInfoFntSize, .1, .7, .9);
	y -= iInfoFntSize;
	fontDrawString (x, y, "UNUSED: %d . %d", teleport->unused4, teleport->unused8);
}


void
text_frame_monster_generator (reference_p reference)
{
	short* item = getItem (reference);
	int x = winW-370;
	int y = winH/2;
	a_2_monster_generator_p effect = (a_2_monster_generator_p) (item + 1);
	a_3_monster_generator_p generator = (a_3_monster_generator_p) (item + 2);
	setTextProperties (10, 1, .7, .5);
	fontDrawString (x, y-(5+2)*10, "Generation: %s", txt_generators[effect->generation]);
	setTextProperties (10, 1, .7, .5);
	fontDrawString (x, y-(6+2)*10, "Toughness: %02x / Pause?: %02x",
		generator->thoughness, generator->pause);
}
/*
void
text_frame_shooter (reference_p reference)
{
	short* item = getItem (reference);
	int x = winW-370;
	int y = winH/2;
	a_3_shooter_p shooter = (a_3_shooter_p) (item + 2);

	setTextProperties (10, 1, 1, .5);
	fontDrawString (x, y-(5+2)*10, "Power: %02d / Range: %02d",
		shooter->power,	shooter->range);
}
*/

char*
get_secured_actuator_name(int wall, int type)
{
	static char text[64];
	int max_act_walls = 0;
	int max_act_floors = 0;

	sprintf(text, "unknown");

	max_act_walls = 128;
	max_act_floors = 128;

	type = (type + 128) % 128;

	if (wall && type <= max_act_walls && txt_act_walls != NULL && txt_act_walls[type] != NULL)
	{
		sprintf(text, txt_act_walls[type]);
	}
	else if (type <= max_act_floors && txt_act_floors != NULL && txt_act_floors[type] != NULL)
	{
		sprintf(text, txt_act_floors[type]);
	}
	return text;
}


void
helper_activator_item(int iAbsoluteValue, int* iItemDB, int* iItemIndex)
{
	if (iAbsoluteValue >= 0 && iAbsoluteValue < 0x0080)	// Weapon
	{
		*iItemDB = category_Weapon;
		*iItemIndex = iAbsoluteValue;
	}
	else if (iAbsoluteValue >= 0x0080 && iAbsoluteValue < 0x0100)	// Clothing
	{
		*iItemDB = category_Clothing;
		*iItemIndex = iAbsoluteValue - 0x0080;
	}
	else if (iAbsoluteValue >= 0x0100 && iAbsoluteValue < 0x0180)	// Miscs
	{
		*iItemDB = category_Miscs;
		*iItemIndex = iAbsoluteValue - 0x0100;
	}
	else if (iAbsoluteValue >= 0x0180 && iAbsoluteValue < 0x01B0)	// Potion
	{
		*iItemDB = category_Potion;
		*iItemIndex = iAbsoluteValue - 0x0180;
	}
	else if (iAbsoluteValue >= 0x01B0 && iAbsoluteValue < 0x01E0)	// Monster
	{
		*iItemDB = category_Monster;
		*iItemIndex = iAbsoluteValue - 0x01B0;
	}
	else if (iAbsoluteValue >= 0x01E0 && iAbsoluteValue < 0x01FC)	// Chest
	{
		*iItemDB = category_Chest;
		*iItemIndex = iAbsoluteValue - 0x01E0;
	}
	else if (iAbsoluteValue >= 0x01FC && iAbsoluteValue < 0x01FF)	// Scroll
	{
		*iItemDB = category_Scroll;
		*iItemIndex = iAbsoluteValue - 0x01FC;
	}

}


void
text_frame_actuator (reference_p reference, int wall, int x, int y)
{
	char actuatorname[64];
	static char* cell[] = { "FLOOR", "WALL"};
	short* item = getItem (reference);
	actuator_p act = (actuator_p) item;
	actuator_effect_p effect = (actuator_effect_p) (item + 1);
	actuator_target_p target = (actuator_target_p) (item + 2);
	a_3_shooter_p shooter = (a_3_shooter_p) (item + 2);
	char type = (act->type == 0x7F)?-1:act->type;
//	short gen[] = {effect->delay, effect->once, effect->inactive, effect->cycle, effect->sound};
//	static char* txt_gen_effect[] = {"delay", "once", "inactive", "cycle", "sound"};
//	short gen[] = {effect->delay, effect->once, effect->inactive, effect->sound};
//	static char* txt_gen_effect[] = {"delay", "once", "inactive", "sound"};
	int i = 0;
	int iselftarget = 0;
	
	int iStartPosX = x;
	int iStartPosY = y;
	char sSelfTarget[16];

	int iActuatorX = 0;
	int iActuatorY = 0;

	char sOperatesFacing[16];

	int iActivatorItemDB = 0;
	int iActivatorItemIndex = 0;

	memset(sSelfTarget, 0, 16);
	memset(sOperatesFacing, 0, 16);

	//x = winW-370;
	//y = winH/2;
//	x = 64*20;
//	y = winH-64;
	x = iInfoX;
	y = (winH - iInfoYNeg) - (y*__STD_STACK_SIZE__/2);

	iActuatorX = getEditCursor (cursor_X);
	iActuatorY = getEditCursor (cursor_Y);
	if (iActuatorX == target->xdest && iActuatorY == target->ydest)
	{
		iselftarget = 1;
		strcpy(sSelfTarget, "(SELF TARGET)");
	}

	strcpy(sOperatesFacing, "NONE");
	if (type == 3)
	{
		switch(act->value)
		{
			case 0:
				strcpy(sOperatesFacing, "NO FACING"); break;
			case 1:
				strcpy(sOperatesFacing, "NORTH"); break;
			case 2:
				strcpy(sOperatesFacing, "EAST"); break;
			case 3:
				strcpy(sOperatesFacing, "SOUTH"); break;
			case 4:
				strcpy(sOperatesFacing, "WEST"); break;
			default:
				sprintf(sOperatesFacing, "UNKNOWN INVALID?"); break;
		}
	}

	//--- For DM2 item activator, it is better to know which category/item is required along the absolute value.
	helper_activator_item(act->value, &iActivatorItemDB, &iActivatorItemIndex);


	/// IDENTITY PART
	setTextProperties (iInfoFntSize, 1, 1, .65);
	sprintf(actuatorname, get_secured_actuator_name(wall, type));
	//fontDrawString (x, y,"%s Actuator: (%d) %s", cell[wall], type, wall?txt_act_walls[type]:txt_act_floors[type]);
	fontDrawString (x, y,"%s ACTUATOR:", cell[wall]);
	setTextProperties (iInfoFntSize, 1, 1, .65);
	y -= iInfoFntSize;
	fontDrawString (x, y,"TYPE : %02X (%02d): %s", type, type, actuatorname);
	setTextProperties (iInfoFntSize, .8, .8, .4);
	y -= iInfoFntSize;
	fontDrawString (x, y,"VALUE: %02X (%02d) [DM2]: <%01X|%02X>", act->value, act->value, iActivatorItemDB, iActivatorItemIndex);
	y -= iInfoFntSize;

	if (act->type == actuator_wall_trigger) {
		setTextProperties (iInfoFntSize, .8, .8, .4);
		fontDrawString (x, y,"START  BITFIELD: %02X", act->value%16);
		y -= iInfoFntSize;
		setTextProperties (iInfoFntSize, .8, .8, .4);
		fontDrawString (x, y,"TARGET BITFIELD: %02X", act->value>>4);
		y -= iInfoFntSize;
	}


	setTextProperties (iInfoFntSize, .8, .8, .4);
	fontDrawString (x, y, "OPERATES FACING: %s", sOperatesFacing);
	y -= iInfoFntSize;

	/// EFFECT PART

	setTextProperties (iInfoFntSize, .4, .6, .8);
	fontDrawString (x, y, "EFFECT BITS: %d.%d", effect->bit1, effect->bit2);
	y -= iInfoFntSize;


	setTextProperties (iInfoFntSize, .4, .8, .4);
	fontDrawString (x, y, "OPERATES ONCE: %d", effect->once);
	y -= iInfoFntSize;
	setTextProperties (iInfoFntSize, .4, .8, .4);
	fontDrawString (x, y, "EFFECT: %02X (%02d) - %s", effect->effect, effect->effect, txt_actuator_effect[effect->effect]);
	y -= iInfoFntSize;
	setTextProperties (iInfoFntSize, .4, .8, .4);
	fontDrawString (x, y, "SOUND: %d", effect->sound);
	y -= iInfoFntSize;
	setTextProperties (iInfoFntSize, .4, .8, .4);
	fontDrawString (x, y, "DELAY (TICKS): %d", effect->delay);
	y -= iInfoFntSize;
	setTextProperties (iInfoFntSize, .4, .8, .4);
	// in case of push button, the "inactive" bit is actually meaning "rotate gfx"
	if (type == 1) // push button
		fontDrawString (x, y, "ROTATE ACT/GFX: %d", effect->inactive);
	else
		fontDrawString (x, y, "IS INACTIVE: %d", effect->inactive);
	y -= iInfoFntSize;
	setTextProperties (iInfoFntSize, .4, .8, .4);
	fontDrawString (x, y, "GFX ID: %d", effect->graphism);
	y -= iInfoFntSize;

	setTextProperties (iInfoFntSize, .4, .6, .8);
	fontDrawString (x, y, "TARGET X: %d %s", target->xdest, sSelfTarget);
	y -= iInfoFntSize;
	setTextProperties (iInfoFntSize, .4, .6, .8);
	fontDrawString (x, y, "TARGET Y: %d %s", target->ydest, sSelfTarget);
	y -= iInfoFntSize;
	setTextProperties (iInfoFntSize, .4, .6, .8);
	fontDrawString (x, y, "TARGET FACING: %d", target->facing);
	y -= iInfoFntSize;
	setTextProperties (iInfoFntSize, .4, .6, .8);
	fontDrawString (x, y, "TARGET MAP: X", 0);
	y -= iInfoFntSize;

/*
	unsigned short bit1:1;
	unsigned short bit2:1;
	unsigned short bit3:1;
	unsigned short bit4:1;
	unsigned short facing:2;
	unsigned short xdest:5;
	unsigned short ydest:5;
*/
	if (wall)
	{
		char sExtraInfo[256]; memset(sExtraInfo, 0, 256);
		switch (act->type)
		{
			case actuator_wall_spell_shooter:
			case actuator_wall_weapon_shooter:
			case actuator_wall_double_spell_shooter:
			case actuator_wall_general_shooter:
			case actuator_wall_double_general_shooter:
			{
				// Direct display
//				text_frame_shooter (reference);
				setTextProperties (iInfoFntSize, .8, .8, .4);
				y -= iInfoFntSize;
				fontDrawString (x, y, "POWER: %02d", shooter->power);
				setTextProperties (iInfoFntSize, .8, .8, .4);
				y -= iInfoFntSize;
				fontDrawString (x, y, "RANGE: %02d", shooter->range);
				break;
				/// power measurement
				// Fireball 10 => ~230 dmg
				// Fireball 1 => ~ 36 dmg
			}
			default:
			{
				setTextProperties (iInfoFntSize, .8, .8, .4);
				y -= iInfoFntSize;
				fontDrawString (x, y, "TARGET FACING: %d", target->facing);
				setTextProperties (iInfoFntSize, .8, .8, .4);
				y -= iInfoFntSize;
				if (act->type == 0x26 && act->value <= 0x58) // creature attractor
					sprintf(sExtraInfo, "(%s)", txt_monsters_sk[act->value]);
				fontDrawString (x, y, "TRIGGER VALUE: %d %s", act->value, sExtraInfo);
				break;
			}
		}
	}
	else
	{ // not wall
		switch (act->type)
		{
			case actuator_floor_monster_generator:
				text_frame_monster_generator (reference);
				break;
			default:
			{
				setTextProperties (iInfoFntSize, 1, 1, .5);
				y -= iInfoFntSize;
				fontDrawString (x, y, "TARGET FACING: %d", target->facing);
				setTextProperties (iInfoFntSize, 1, 1, .5);
				y -= iInfoFntSize;
				fontDrawString (x, y, "TRIGGER VALUE: %d", act->value);
				break;
			}	
		}
	}
	
	y -= iInfoFntSize;
	setTextProperties (iInfoFntSize, .4, .6, .8);
	fontDrawString (x, y, "LAST BITS: %d.%d.%d.%d", target->bit1, target->bit2, target->bit3, target->bit4);
	
}

//------------------------------------------------------------------------------

void
printMainMapHelpInfo ()
{
	int basex = iTileInfo_OffsetX+40;
	int basey = winH-iTileInfo_OffsetY-620;
	int helptfsize = 13;

	int x = basex;
	int y = basey;
	int ystep = helptfsize+2;

	setTextProperties (helptfsize, 1, 1, 1);
	fontDrawString (x, y, "ONLINE HELP");
	y -= ystep;

	if (! isEditingTile ())	// navigating through map, no object locked for edit
	{
		y -= ystep;
		setTextProperties (helptfsize, .7, .7, .7);
		fontDrawString (x, y, "LEFT CLICK:  PAINT TILE WITH OPEN FLOOR");
		y -= ystep;
		setTextProperties (helptfsize, .7, .7, .7);
		fontDrawString (x, y, "RIGHT CLICK: PAINT TILE WITH SOLID WALL");
		y -= ystep;
		setTextProperties (helptfsize, .7, .7, .7);
		fontDrawString (x, y, "ENTER: LOCK OBJECT/STACK ON TILE FOR EDIT");

		y -= ystep;
		y -= ystep;
		setTextProperties (helptfsize, .7, .7, .7);
		fontDrawString (x, y, "LEFT - RIGHT ARROW: CYCLE THROUGH ITEM ID TYPE");
		y -= ystep;
		setTextProperties (helptfsize, .7, .7, .7);
		fontDrawString (x, y, "UP - DOWN ARROW: CYCLE THROUGH ITEMS IN STACK");

		y -= ystep;
		setTextProperties (helptfsize, .7, .7, .7);
		fontDrawString (x, y, "PAGE UP - PAGE DOWN: NAVIGATE THROUGH MAPS");
	}

//--- If editing door, bring this help
	if (isEditingTile ())
	{
		reference_p refp;
		switch ((refp = getCurrentItemReference())->category)
		{
			case category_Door:
				y -= ystep;
				setTextProperties (helptfsize, .85, .85, .85);
				fontDrawString (x, y, "DOOR EDITING:");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'B': SWITCH BUTTON");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'D': DESTROYABLE BY SPELLS");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'S': BASHABLE BY WEAPONS");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'O': OPENING (VERTICAL OR HORIZONTAL)");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "' ': DOOR TYPE (1 OR 2)");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "LEFT - RIGHT ARROW: CYCLE THROUGH AVAILABLE DOOR ORNATES");
			break;
			case category_Monster:
				y -= ystep;
				setTextProperties (helptfsize, 1, .25, .25);
				if (SKULLKEEP)
					fontDrawString (x, y, "CREATURE/OBJECT EDITING:");
				else
					fontDrawString (x, y, "CREATURE EDITING:");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'1': DECREASE LEVEL");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'2': INCREASE LEVEL");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'4': DECREASE 10 LEVELS");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'5': INCREASE 10 LEVELS");

				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'0': REROLL HIT POINTS");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'8': NOVICE MONSTER - LEVEL = 1");

				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'M': CYCLE THROUGH NUMBER OF CREATURES (1 - 4)");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "LEFT - RIGHT ARROW: CYCLE THROUGH MONSTER ID TYPE");
			break;
			case category_Weapon:
				y -= ystep;
				setTextProperties (helptfsize, 1, 1, 1);
				fontDrawString (x, y, "WEAPON EDITING:");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'1': DECREASE CHARGE");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'2': INCREASE CHARGE");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'C': CURSED");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'O': POISONED");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'B': BROKEN");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "LEFT - RIGHT ARROW: CYCLE THROUGH WEAPON ID TYPE");
			break;
			case category_Clothing:
				y -= ystep;
				setTextProperties (helptfsize, .8, .4, .2);
				fontDrawString (x, y, "CLOTHING EDITING:");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'C': CURSED");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'B': BROKEN");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "LEFT - RIGHT ARROW: CYCLE THROUGH CLOTHING ID TYPE");
			break;
			case category_Potion:
				y -= ystep;
				setTextProperties (helptfsize, .25, .25, 1);
				fontDrawString (x, y, "POTION EDITING:");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'1': DECREASE POWER BY 1");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'2': INCREASE POWER BY 1");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'4': DECREASE POWER BY 40");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "'5': INCREASE POWER BY 40");
				y -= ystep;
				setTextProperties (helptfsize, .7, .7, .7);
				fontDrawString (x, y, "LEFT - RIGHT ARROW: CYCLE THROUGH POTION ID TYPE");
			break;
/*
							if (key == '1')
								potion->power-=1;
							else if (key == '2')
								potion->power+=1;
							else if (key == '4')
								potion->power-=40;
							else if (key == '5')
								potion->power+=40;
			
*/
		}

		y -= ystep;
		setTextProperties (helptfsize, .7, .8, .9);
		fontDrawString (x, y, "'P': CYCLE THROUGH TILE QUARTER POSITION");
		y -= ystep;
		y -= ystep;
		setTextProperties (helptfsize, .7, .7, .7);
		fontDrawString (x, y, "ENTER: UNLOCK OBJECT/STACK");
	}


}

//------------------------------------------------------------------------------

void
printNewItemStats ()
{
	setTextProperties (14, 1, 1, 1);

	if (!SKULLKEEP)
	fontDrawString (winW/4, winH - 75, "Used %ss: %d (0x%03X)",
		txt_objects[getEditCursor (cursor_NewItem)], getNumber (getEditCursor (cursor_NewItem)),
		getNumber (getEditCursor (cursor_NewItem)));
	if (SKULLKEEP)
	fontDrawString (winW/4, winH - 75, "Used %ss: %d (0x%03X)",
		txt_DM2_objects[getEditCursor (cursor_NewItem)], getNumber (getEditCursor (cursor_NewItem)),
		getNumber (getEditCursor (cursor_NewItem)));
	
}

void
printGeneralMapInfo ()
{
	printCoordinates ();
	printLevelStack ();
	printTileValue ();
	printReferenceValue ();
	printMainMapHelpInfo();
}
