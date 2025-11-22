//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Load Dungeon
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include <loaddungeon.h>
#include <item.h>
#include <reference.h>
#include <editor.h>
#include <level.h>
#include <text.h>
#include <actuator.h>

#include <gui.h>	// for gl_xxxx values

//------------------------------------------------------------------------------

unsigned short* OFFSETS;
unsigned short* REFERENCES;
unsigned short* RAWTEXTS;
unsigned short* FREESPACE;

int SKULLKEEP = 0;			// It is a skullkeep map?
int THERONSQUEST = 0;		// It is a Therons Quest map ? (special DM2+TQ tag)
int THERONSQUEST_CD = 0;	// It is a Therons Quest from PC-Engine CD ?
int TELOS = 0;				// It is a TELOS(SK) dungeon ?

extern short* ITEMS[16];
extern int nItems[16]; 
extern size_t itemBytes [16];

extern unsigned int gfx_activation_items[];

static const char* txt_dungeon_types_all [] = {
	"UNKNOWN", "MASTER", "CHAOS", "THERON'S QUEST", "SKULLKEEP", "WINUAE AMIGA DEMO" };

static const char** txt_dungeon_types = &txt_dungeon_types_all[1];

tCompanionActuator xActuatorTable[1024];

int iAmigaSTEndian = 0;

void
updateActivationItems(int iDM2Mode)
{
	int i = 0;
	if (iDM2Mode == 1)
	{
		for (i = 0; i < 0x80; i++)
			gfx_activation_items[0x0000 + i] = gl_StaticSkullkeep + gl_Weapons + i;
		for (i = 0; i < 0x80; i++)
			gfx_activation_items[0x0080 + i] = gl_StaticSkullkeep + gl_Clothes + i;
		for (i = 0; i < 0x80; i++)
			gfx_activation_items[0x0100 + i] = gl_StaticSkullkeep + gl_Miscs + i;
		for (i = 0; i < 0x30; i++)
			gfx_activation_items[0x0180 + i] = gl_StaticSkullkeep + gl_Potions + i;
		for (i = 0; i < 0x30; i++)
			gfx_activation_items[0x01B0 + i] = gl_StaticSkullkeep + gl_Monsters + i;
		for (i = 0; i < 0x1C; i++)
			gfx_activation_items[0x01E0 + i] = gl_StaticSkullkeep + gl_Potions + i;
		for (i = 0; i < 1; i++)
			gfx_activation_items[0x01FC + i] = gl_StaticSkullkeep + gl_Special + special_Scroll + i;
	}
	else if (iDM2Mode == 0)
	{
		; // TODO, revert back to original DM1 activation list
	}

}

void
swapItemData(int idbcat, void* pdata, int nItems)
{
	int z = 0;
	char* xdata = (char*) pdata;
	char tmp = 0;
	int x = 0;
	printf("SWAP: Item Data [%02d] for %d items\n", idbcat, nItems); 
	for (z = 0; z < nItems; z++)
	{
		// invert ref, for all
		{
			tmp = xdata[1];
			xdata[1] = xdata[0];
			xdata[0] = tmp;
		}
		//printf("%02d/%03d) R:%04X\n", idbcat, z, *((unsigned short*) xdata));
		xdata += 2;
		if (idbcat == category_Text)
		{
			tmp = xdata[1];
			xdata[1] = xdata[0];
			xdata[0] = tmp;
			xdata += 2;
		}
		if (idbcat == category_Door || idbcat == category_Monster || (idbcat >= category_Weapon && idbcat <= category_Potion) || idbcat >= category_Miscs)
		{
			// invert data too
			tmp = xdata[1];
			xdata[1] = xdata[0];
			xdata[0] = tmp;
			xdata += 2;
		}
		if (idbcat == category_Teleport)
		{
			for (x = 0; x < 2; x++)
			{
				tmp = xdata[1];
				xdata[1] = xdata[0];
				xdata[0] = tmp;
				xdata += 2;
			}
		}
		if (idbcat == category_Actuator || idbcat == category_Chest)
		{
			for (x = 0; x < 3; x++)
			{
				tmp = xdata[1];
				xdata[1] = xdata[0];
				xdata[0] = tmp;
				xdata += 2;
			}
		}
		if (idbcat == category_Monster)
		{
			xdata += 2;	// special 2 bytes not to be swapped
			for (x = 0; x < 4; x++)
			{
				tmp = xdata[1];
				xdata[1] = xdata[0];
				xdata[0] = tmp;
				xdata += 2;
			}
			xdata += 2;
		}
	}
}

void
swapData(void* pdata, int swapsize, int nItems)
{
	int z = 0;
	char* xdata = (char*) pdata;
	char tmp = 0;
	for (z = 0; z < nItems; z++)
	{
		tmp = xdata[1];
		xdata[1] = xdata[0];
		xdata[0] = tmp;
		xdata += swapsize;
	}
}



static void
loadGeneralInfo (FILE* fp)
{
	// DM Dungeon seeds
	// DM-ST : 00 63
	// DM-PC : 00 63	DM-KID : 00 63
	// CSB-Prison-PC : 08 00
	// CSB-PC : 0D 00
	// DM2 : 00 00
	// DM2 (Demo2) : 00 06

	fread (getDungeon(), 44, 1, fp);
	/*if (getDungeon()->randomGraphicsSeed == 0)
	{
		SKULLKEEP = 1;
		updateActivationItems(SKULLKEEP);
	}
	*/
	if (getDungeon()->randomGraphicsSeed == 0x5154)	// "TQ"
	{
		THERONSQUEST = 1;
		SKULLKEEP = 1;	// ?
		updateActivationItems(SKULLKEEP); // ?

	}
	else if (getDungeon()->randomGraphicsSeed == 0x5853)	// "SX"	= TELOS
	{
		THERONSQUEST = 0;
		SKULLKEEP = 1;
		TELOS = 1;
		updateActivationItems(SKULLKEEP);

	}
	else if (getDungeon()->randomGraphicsSeed == 0x0063)
	{
		printf("Looks like PC-DOS format!\n");
		iAmigaSTEndian = 0;
	}

	else if (getDungeon()->randomGraphicsSeed == 0x6300)
	{
		int idb = 0;
		printf("Looks like AMIGA/ST format!\n");
		iAmigaSTEndian = 1;
		swapData(&(getDungeon()->randomGraphicsSeed), 2, 1);
		swapData(&(getDungeon()->mapDataSize), 2, 1);
		swapData(&(getDungeon()->textsDataSize), 2, 1);
		swapData((short*)&(getDungeon()->textsDataSize)+1, 2, 1); // starting position
		swapData(&(getDungeon()->itemListSize), 2, 1);

		for (idb = 0; idb < 16; idb++)
			swapData(&(getDungeon()->nObjects[idb]), 2, 1);
	}
	else
		SKULLKEEP = 0;

	/// check endianess
	{
		int idb = 0;
		int hmswap = 0;
		
		// check the DB numbers and check if we need to swap
		for (idb = 0; idb < 16; idb++)
		{
			if (getDungeon()->nObjects[idb] > 1024)
				hmswap++;
		}
		if (hmswap > 0)
		{
			printf("Looks like AMIGA/ST format!\n");
			iAmigaSTEndian = 1;
			swapData(&(getDungeon()->randomGraphicsSeed), 2, 1);
			swapData(&(getDungeon()->mapDataSize), 2, 1);
			swapData(&(getDungeon()->textsDataSize), 2, 1);
			swapData((short*)&(getDungeon()->textsDataSize)+1, 2, 1); // starting position
			swapData(&(getDungeon()->itemListSize), 2, 1);

			for (idb = 0; idb < 16; idb++)
				swapData(&(getDungeon()->nObjects[idb]), 2, 1);
		}
	}
}




static void
loadLevelSpecifications (FILE* fp)
{
	unsigned char i = 0;
	unsigned int tssum = 0;
	for (i = 0; i < getDungeon()->nLevels; i++)
	{
		fread (&(getLevels()[i].header), 16, 1, fp);
		if (iAmigaSTEndian)
		{
			short* xLevelHeader = (short*) &(getLevels()[i].header);
			swapData(xLevelHeader, 2, 8);	// 16 bytes to be extended
			swapData(xLevelHeader+3, 2, 1);	// actually bytes 6-7 are not swapped
		}
		tssum += getLevels()[i].header.tileset;
	}
	//-- Check if it would seem to be a DM2 dungeon : if tilesets are not all zeros
	if (tssum > 0)
	{
		THERONSQUEST = 0;
		SKULLKEEP = 1;
		updateActivationItems(SKULLKEEP);
	}
}

//------------------------------------------------------------------------------

unsigned short
calculateOffsets ()
{
	// number of columns (i.e. x)
	size_t i;
	unsigned short count = 0;
	for (i = 0; i < getDungeon()->nLevels; i++)
	{
		//printf("[%02d] xDim = %d\n", i, getLevels()[i].header.xDim+1);
		count += (getLevels()[i].header.xDim+1);
	}
	OFFSETS = (unsigned short*) calloc (count, sizeof (short));
	//printf("maps = %d / cols = %d\n", getDungeon()->nLevels, count);
	return count;
}

static void
organizeStacks ()
{
	unsigned short i, j;
	unsigned char l;
	size_t k = 0;
	
	for (l = 0; l < getDungeon()->nLevels; l++)
		for (i = 0; i < getLevels()[l].header.xDim+1; i++)
			for (j = 0; j < getLevels()[l].header.yDim+1; j++)
			{
				if (getLevels()[l].map[i][j].object == 1)
					setGroundReference ((char) i, (char) j, l, REFERENCES[k++]);
			}
}


//------------------------------------------------------------------------------
static void
loadTilesMap (FILE* fp, size_t level)
{
	unsigned short i, j;
	for (i = 0; i < getLevels()[level].header.xDim+1; i++)
		for (j = 0; j < getLevels()[level].header.yDim+1; j++)
			fread (&(getLevels()[level].map[i][j]), 1, 1, fp);
}


static void
loadLevelsData (FILE* fp, enum DUNGEONTYPE dtype)
{
	size_t i;
	level_p currentmap = NULL;
	for (i = 0; i < getDungeon()->nLevels; i++)
	{
		currentmap = getLevel(i);
		currentmap->startoffset = ftell(fp);
		loadTilesMap (fp, i);
		if (dtype != dungeon_TheronQuest)
		{
			fread (&(getLevels()[i].monsters), getLevels()[i].header.nMonsters, 1, fp);
			fread (&(getLevels()[i].walls), getLevels()[i].header.nWalls, 1, fp);
			fread (&(getLevels()[i].floors), getLevels()[i].header.nFloors, 1, fp);
			fread (&(getLevels()[i].ornates), getLevels()[i].header.nOrnates, 1, fp);
		}
		currentmap->endoffset = ftell(fp);
	}
}

void loadRawData (FILE* fp, unsigned short *data, size_t datasize, size_t nItems)
{
	fread (data, datasize, nItems, fp);
}

void loadLittleRawData (FILE* fp, short *data, size_t nItems)
{
	size_t i;
	short *p = data;
	for (i = 0; i < nItems; i++)
	{
		fread (p, 2, 1, fp);
		*p = (((*p)%256)<<8) + (((*p) & 0xFF00)>>8);

		p++;
	}

}

//------------------------------------------------------------------------------

int
initCompanionActuatorsTable()
{
	int i = 0;
	tCompanionActuator* xact = NULL;
	for (i = 0; i < 1024; i++)
	{
		xact = &xActuatorTable[i];
		xact->id = 0;
		xact->map = -1;
		xact->used = 0;
		xact->wact = 0;
	}
	return 1;
}


int
loadMusicList(char* sFilenameSongListDat)
{
	FILE* fp = NULL;
	int i = 0;
	int iMusicID = 0;
	int iNbMaps = 0;
	level_p xMapLevel = NULL;

	iNbMaps = getDungeon()->nLevels;

	fp = fopen (sFilenameSongListDat, "rb");
	if (fp == NULL) {
		printf("Can't load %s\n", sFilenameSongListDat);
		return 0;
	}
	
	for (i = 0; i < 64; i++)
	{
		fread(&iMusicID, 1, 1, fp);
		if (i < iNbMaps)
		{
			xMapLevel = getLevel (i);
			xMapLevel->iMusicID = iMusicID;
		}
	}

	fclose(fp);
	return 1;
}

//------------------------------------------------------------------------------

int
loadDungeonFromDir (char *dungeonname)
{
	char fulldungeonname[512];
	memset(fulldungeonname, 0, 512);
#ifdef __LINUX__
	sprintf(fulldungeonname, "dungeons/%s", dungeonname);
#else
	sprintf(fulldungeonname, "dungeons\\%s", dungeonname);
#endif // __LINUX__
	return loadDungeonData (fulldungeonname);
}

// These offsets are for
// [0] = start of size/dims for maps
// [1] = start of map data layout
// [2] = start of ground references
// [3] = start of item data (0 to 3)
// [4] = start of item data (4 to 11)	-- always aligned to xB800
// [5] = start of text data
int iTQMemOffset[][6] = { 
						{0x0002A0F1, 0x0002A2D7, 0x0002A831, 0x0002AD0F, 0x0002B800, 0x0002C9D8},	// Ak-Tu-Ba
						{0x0006A333, 0x0006A5E9, 0x0006ABA5, 0x0006B031, 0x0006B800, 0x0006CECC},	// Tower of Drator

						{0x000AA9BC, 0x000AAB7D, 0x000AB035, 0x000AB4C9, 0x000AB800, 0x000ACCC8},	// City of Formicia
						{0x000EA31B, 0x000EA4A8, 0x000EA95C, 0x000EADCE, 0x000EB800, 0x000ED22C},	// Tomb of Sarmon
						{0x0012AB3C, 0x0012ACB5, 0x0012B22F, 0x0012B6DD, 0x0012B800, 0x0012D106},	// Den of Shadodan
						{0x0016A034, 0x0016A220, 0x0016A8EC, 0x0016ADEC, 0x0016B800, 0x0016C884},	// Village of Thieves

						{0x001AA831, 0x001AA9EC, 0x001AB045, 0x001AB4BD, 0x001AB800, 0x001ACC0E}	// Demon's Gate

};

int iTQMapsPerDungeon[] = { 4, 8, 5, 6, 3, 4, 4 };
int iTQTextDataSize[] = { 0x13C, 0xD0, 0xE0, 0xE8, 0xE0, 0xD9, 0xE8};

// I do not find where wall/floor decorations are stored.
// Hence I put them here hard coded
void
FIX_TQDecorations (int iTQDungeon)
{
	level_p level = NULL;
	int m = 0;

	level = (level_p) &(getLevels()[0]);
	level->header.nWalls = 1;
	level->walls[0] = 0x2B; // mirror

	for (m = 0; m < getDungeon()->nLevels; m++)
	{
		level = getLevel(m);
		if (m == 0)
			continue;

		printf("FIXING Floor Decorations for map %d\n", m);

		level->header.nFloors = 1;
		level->floors[0] = 0x01;	// square pad
		if (iTQDungeon == 6 && m == 2)
		{
			level->header.nFloors = 2;
			level->floors[0] = 0x05;	// fire pot
			level->floors[1] = 0x01;	// square pad
		}
	}

	switch (iTQDungeon)
	{
	case 0:	// Ak-Tu-Ba
		{
			level = (level_p) &(getLevels()[1]);
			level->header.nWalls = 14;
			level->walls[1] = 0x2D; // lever down
			level->walls[2] = 0x2C; // lever up
			level->walls[5] = 0x1A; // gold lock
			level->walls[11] = 0x02; // vi altar
			level->walls[3] = 0x01; // alcove
			level->walls[4] = 0x31;	// brick
			level->walls[9] = 0x37; // hidden switch pushed
			level->walls[10] = 0x32; // hidden switch
			level->walls[0] = 0x05; // iron lock
			level->walls[12] = 0x26; // torch holder empty
			level->walls[13] = 0x2E; // torch holder full
			level->walls[6] = 0x36; // brick pushed
			level->walls[7] = 0x35; // red button pushed
			level->walls[8] = 0x2F; // red button


			level = (level_p) &(getLevels()[2]);
			level->header.nWalls = 14;
			level->walls[0] = 0x11; // coin slot
			level->walls[13] = 0x10;	// cyan button
			level->walls[3] = 0x02; // vi altar
			level->walls[9] = 0x29; // spell holes
			level->walls[8] = 0x25; // demon's face
			level->walls[4] = 0x35; // red button pushed
			level->walls[5] = 0x2F; // red button
			level->walls[1] = 0x2D; // lever down
			level->walls[2] = 0x2C; // lever up
			level->walls[11] = 0x36; // brick pushed
			level->walls[12] = 0x31; // brick
			level->walls[10] = 0x07; // small switch
			level->walls[6] = 0x26; // torch holder empty
			level->walls[7] = 0x2E; // torch holder full


			level = (level_p) &(getLevels()[3]);
			level->header.nWalls = 6;
			level->walls[0] = 0x01; // alcove
			level->walls[4] = 0x1E; // ra lock
			level->walls[5] = 0x05;	// iron lock
			level->walls[2] = 0x23; // fountain
			level->walls[1] = 0x02; // vi altar
			level->walls[3] = 0x07; // small switch

			
		}
		break;
	case 1:	// Tower of Drator
		break;
	case 2:	// The City of Formicia
		break;
	case 3:	// The Tomb of Sarmon
		break;
	case 4:	// Den of Shadodan
		break;
	case 5:	// Village of Thieves
		break;
	case 6:	// Demon's Gate
		break;
	}
}

void
FIX_TQActuatorList(int iTQDungeon)
{
	// Ak-Tu-Ba
	// 7F/127 = gold coin
	// B0/176 = iron key
	// B8/184 = gold key
	// BE/190 = ra key
	// In TQ-US:
		// 38 = gold coin
		// 55 = iron key
		// 56 = gold key
		// 57 = ra key

	dm_dungeon_header* xDungeonHeader = NULL;
	int iNbMaps = 0;
	int m = 0;
	int i = 0;
	int j = 0;
	int iTileWall = 0;
	level_p xCurrentMap = NULL;
	dm_level_header* xDMLevelHeader = NULL;
	sk_level_header* xSKLevelHeader = NULL;

	//--- There, search through the entire dungeon for all actuators
	xDungeonHeader = getDungeon();
	iNbMaps = xDungeonHeader->nLevels;

	for (m = 0; m < iNbMaps; m++)
	{
		short xRefItem = -2;
		int iTileType = 0;
		reference_p ref = NULL;
		for (j = 0; j < 32; j++)
		{
			for (i = 0; i < 32; i++)
			{
				iTileWall = 0;
				iTileType = getTile (i,j,m)->type;
				ref = getGroundReference (i, j, m);
				xRefItem = *((short*)ref);
				//printf("REF = %04x / %08x\n", xRefItem, ref);
				while (xRefItem != -2)
				{
					actuator_p actuator = NULL;
					int iItemValue = 0;
					int iListBase = 0;
					int iActivatorID = 0;
					int iItemCategory = 0;
					int iItemIndexInCat = 0;

					//printf("CAT = %d\n", ref->category);

					if (ref->category == category_Actuator)
					{
						short* item = getItem (ref);
						actuator_p act = (actuator_p) item;
						actuator_effect_p effect = (actuator_effect_p) (item + 1);
						actuator_target_p target = (actuator_target_p) (item + 2);
						//printf("ITEM = %x\n", item);

						if (iTileType == tile_Wall)	// convert Wall actuators
							iTileWall = 1;
						else	// floor and other tiles
							iTileWall = 0;

						if (iTileWall == 1 && (act->type == actuator_wall_alcove_item ||
							act->type == actuator_wall_item_eater || act->type == actuator_wall_item || act->type == actuator_wall_item_eater_toggler))
						{
							// translate TQ value ID to standard DM value ID
							if (act->value == 0x32) act->value = 0x6E;	// shield defiant (shield of lyte)
							if (act->value == 0x38) act->value = 0x7F;	// gold coin
							if (act->value == 0x55) act->value = 0xB0;	// iron key
							if (act->value == 0x56) act->value = 0xB8;	// gold key
							if (act->value == 0x57) act->value = 0xBE;	// ra key
						}
						if (iTileWall == 0 && (act->type == actuator_floor_carried_item))
						{
							// translate TQ value ID to standard DM value ID
							if (act->value == 0x32) act->value = 0x6E;	// shield defiant (shield of lyte)
						}
					}
					//printf("GOING TO NEXT. Cat = %d\n", ref->category);
					ref = getNextItem (ref);
					xRefItem = *((short*)ref);
					//printf("NEXT REF = %04x\n", xRefItem);
				}
			}
		}
	}
}



int
loadTheronsQuestDungeonData(char* dungeonname, int iTQDungeon)
{
	FILE* fp = NULL;
	size_t i = 0;

	fp = fopen (dungeonname, "rb");
	if (fp != NULL)
	{
		int m = 0;
		int maps = iTQMapsPerDungeon[iTQDungeon];
		char xdims[16];
		char ydims[16];
		char xbuffer[16];
		unsigned short ibuffer[16];
		int iItemDataPart2StartIndex = 4;	// depending on the dungeon, blocks are not the size same

		THERONSQUEST_CD = iTQDungeon + 1;
		/* flush all references */
		startGroundReferences ();

		/// read dimensions
		getDungeon()->nLevels = maps;
		fseek (fp, iTQMemOffset[iTQDungeon][0], SEEK_SET);	// 0x0002A0F1
		fread(xdims, 1, maps, fp);
		fread(ydims, 1, maps, fp);
		for (m = 0; m < maps; m++)
		{
			flushLevelLayout (m);
			setLevelDims((char)m, (int)xdims[m], (int)ydims[m]);
		}
		fread(xbuffer, 1, maps, fp); // x offset
		for (m = 0; m < maps; m++)
		{
			level_p currentmap = getLevel(m);
			currentmap->header.xOffset = xbuffer[m];
		}
		fread(xbuffer, 1, maps, fp); // y offset
		for (m = 0; m < maps; m++)
		{
			level_p currentmap = getLevel(m);
			currentmap->header.yOffset = xbuffer[m];
		}
		fread(xbuffer, 1, maps, fp); // map ID
		fread(xbuffer, 1, maps, fp); // ?
		fread(xbuffer, 1, maps, fp); // ?
		fread(xbuffer, 1, maps, fp); // Nb creatures per map
		for (m = 0; m < maps; m++)
		{
			level_p currentmap = getLevel(m);
			currentmap->header.nMonsters = xbuffer[m];
		}
		fread(xbuffer, 1, maps, fp); // XP mod per map
		for (m = 0; m < maps; m++)
		{
			level_p currentmap = getLevel(m);
			currentmap->header.depth = xbuffer[m];
		}
		fread(ibuffer, 2, 16, fp); // Nb objects per category/db
		// set objects numbers
		for (i = 0; i < 16; i++)
		{
			getDungeon()->nObjects[i] = ibuffer[i];
		}

		// get the doors types
		for (m = 0; m < maps; m++)
		{
			level_p currentmap = getLevel(m);
			fread(xbuffer, 1, 2, fp);
			currentmap->header.door1 = xbuffer[0];
			currentmap->header.door2 = xbuffer[1];
		}

		// get other data
		fread(ibuffer, 2, maps, fp);
		printf("SOME DATA LIST 1 : ");
		for (m = 0; m < maps; m++)
		{
			printf("%04X ", ibuffer[m]);
		}

		fread(ibuffer, 2, maps, fp);
		printf("\nSOME DATA LIST 2 : ");
		for (m = 0; m < maps; m++)
		{
			printf("%04X ", ibuffer[m]);
		}
		printf("\n");

		// then there are the cumulated number of items per column, but we don't really need to read this

		// Move to map data
		fseek (fp, iTQMemOffset[iTQDungeon][1], SEEK_SET);	// 0x0002A2D7
		loadLevelsData (fp, dungeon_TheronQuest);

		// auto add floor gfx for all maps
		/*
		for (m = 0; m < maps; m++)
		{
			int d = 0;
			level_p currentmap = getLevel(m);
			currentmap->header.nFloors = 8;
			for (d = 0; d < 8; d++)
				currentmap->floors[d] = d;
		}*/

		// ground references
		fseek (fp, iTQMemOffset[iTQDungeon][2], SEEK_SET); // 0x0002A831
		getDungeon()->itemListSize = computeGroundRefNumber();
		printf("LOAD: Ground Refs @ %08x (#%d)\n", ftell(fp), getDungeon()->itemListSize); 
		REFERENCES = (unsigned short*) calloc (getDungeon()->itemListSize, sizeof (short));
		loadRawData (fp, REFERENCES, 2, getDungeon()->itemListSize);

		// doors, teleporters, texts, actuators
		fseek (fp, iTQMemOffset[iTQDungeon][3], SEEK_SET);	// 0x0002AD0F

		iItemDataPart2StartIndex = 3;
		if (iTQDungeon == 0)
			iItemDataPart2StartIndex = 4;
		else if (iTQDungeon == 4)
			iItemDataPart2StartIndex = 1;
		else if (iTQDungeon == 5)
			iItemDataPart2StartIndex = 4;

		for (i = 0; i < iItemDataPart2StartIndex; i++)
		{
			printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[i]); 
			loadRawData (fp, (unsigned short*)ITEMS[i], itemBytes[i], getDungeon()->nObjects[i]);
		}
		printf("LOAD: End of Item Data (part 1) @ %08x\n", ftell(fp)); 
		fseek (fp, iTQMemOffset[iTQDungeon][4], SEEK_SET);	// 0x0002B800

		for (i = iItemDataPart2StartIndex; i < 11; i++)
		{
			printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[i]); 
			loadRawData (fp, (unsigned short*)ITEMS[i], itemBytes[i], getDungeon()->nObjects[i]);
		}
		printf("LOAD: End of Item Data @ %08x\n", ftell(fp)); 

		fseek (fp, iTQMemOffset[iTQDungeon][5], SEEK_SET);		// 0x0002C9D8
		
		getDungeon()->textsDataSize = iTQTextDataSize[iTQDungeon];	// 0x13C
		printf("LOAD: Raw texts @ %08x\n", ftell(fp)); 
		RAWTEXTS = (unsigned short*) calloc (getDungeon()->textsDataSize, sizeof (short));
		loadRawData (fp, RAWTEXTS, 2, getDungeon()->textsDataSize);
		
		fclose (fp);

		printf("INIT OBJECTS NUMBERS\n");
		itemInitObjectNumbersFromDungeon ();
		printf("ORGANIZE STACKS\n");
		organizeStacks ();
		loadTexts (RAWTEXTS);
		printf("UPDATE COLORS\n");
		assumeMonstersForMaps ();
		updatePriorityColors ();

		FIX_TQDecorations (iTQDungeon);
		FIX_TQActuatorList (iTQDungeon);

	}
	return 0;
}

int
loadAmigaDemoWinUAEDungeonData(char* dungeonname)
{
	FILE* fp = NULL;
	size_t i = 0;
	int iDBCategory = 0;

	fp = fopen (dungeonname, "rb");
	if (fp != NULL)
	{
		int maps = 1;

		startGroundReferences ();

		getDungeon()->x_start = 7;
		getDungeon()->y_start = 1;
		getDungeon()->f_start = 2;
		getDungeon()->l_start = 0;

		getDungeon()->nLevels = maps;
		setLevelDims(0, 13, 15);
		getLevel(0)->header.nMonsters = 1;
		getLevel(0)->header.nWalls = 12;
		getLevel(0)->header.nFloors = 2;
		getLevel(0)->header.nOrnates = 3;

		fseek(fp, 0x0A6694, SEEK_SET); // map data
		loadLevelsData (fp, dungeon_AmigaDemoWinUAE);

		// fix
		getLevel(0)->header.nMonsters = 1;
		getLevel(0)->monsters[0] = 0x0C;

		// ground references
		fseek (fp, 0xA7871, SEEK_SET);
		getDungeon()->itemListSize = computeGroundRefNumber();
		printf("LOAD: Ground Refs @ %08x (#%d)\n", ftell(fp), getDungeon()->itemListSize); 
		REFERENCES = (unsigned short*) calloc (getDungeon()->itemListSize, sizeof (short));
		loadRawData (fp, REFERENCES, 2, getDungeon()->itemListSize);
		//swapData(REFERENCES, 2, getDungeon()->itemListSize);  // because Amiga
		//printf("%02x %02x %02x %02x %02x\n", REFERENCES[0], REFERENCES[1], REFERENCES[2], REFERENCES[3], REFERENCES[4]);


		iDBCategory = 10;	// miscs
		fseek (fp, 0xA6A12, SEEK_SET);
		getDungeon()->nObjects[iDBCategory] = 12;
		printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[iDBCategory]); 
		loadRawData (fp, (unsigned short*)ITEMS[iDBCategory], itemBytes[iDBCategory], getDungeon()->nObjects[iDBCategory]);
		swapItemData(iDBCategory, (unsigned short*)ITEMS[iDBCategory], getDungeon()->nObjects[iDBCategory]);

  		iDBCategory = 9;	// chests
		fseek (fp, 0xA6C7A, SEEK_SET);
		getDungeon()->nObjects[iDBCategory] = 0;
		printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[iDBCategory]); 
		loadRawData (fp, (unsigned short*)ITEMS[iDBCategory], itemBytes[iDBCategory], getDungeon()->nObjects[iDBCategory]);
		swapItemData(iDBCategory, (unsigned short*)ITEMS[iDBCategory], getDungeon()->nObjects[iDBCategory]);

  		iDBCategory = 8;	// potions
		fseek (fp, 0xA6C7A, SEEK_SET);
		getDungeon()->nObjects[iDBCategory] = 0;
		printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[iDBCategory]); 
		loadRawData (fp, (unsigned short*)ITEMS[iDBCategory], itemBytes[iDBCategory], getDungeon()->nObjects[iDBCategory]);
		swapItemData(iDBCategory, (unsigned short*)ITEMS[iDBCategory], getDungeon()->nObjects[iDBCategory]);

  		iDBCategory = 7;	// scrolls
		fseek (fp, 0xA6C7A, SEEK_SET);
		getDungeon()->nObjects[iDBCategory] = 1;
		printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[iDBCategory]); 
		loadRawData (fp, (unsigned short*)ITEMS[iDBCategory], itemBytes[iDBCategory], getDungeon()->nObjects[iDBCategory]);
		swapItemData(iDBCategory, (unsigned short*)ITEMS[iDBCategory], getDungeon()->nObjects[iDBCategory]);

		iDBCategory = 6;	// clothings
		fseek (fp, 0xA6C7E, SEEK_SET);
		getDungeon()->nObjects[iDBCategory] = 22;
		printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[iDBCategory]); 
		loadRawData (fp, (unsigned short*)ITEMS[iDBCategory], itemBytes[iDBCategory], getDungeon()->nObjects[iDBCategory]);
		swapItemData(iDBCategory, (unsigned short*)ITEMS[iDBCategory], getDungeon()->nObjects[iDBCategory]);

		iDBCategory = 5;	// weapons
		fseek (fp, 0xA6EB6, SEEK_SET);
		getDungeon()->nObjects[iDBCategory] = 8;
		printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[iDBCategory]); 
		loadRawData (fp, (unsigned short*)ITEMS[iDBCategory], itemBytes[iDBCategory], getDungeon()->nObjects[iDBCategory]);
		swapItemData(iDBCategory, (unsigned short*)ITEMS[iDBCategory], getDungeon()->nObjects[iDBCategory]);
	
	  // creatures
		iDBCategory = 4;
		fseek (fp, 0xA6F56, SEEK_SET);
		//fseek (fp, 0xA7066, SEEK_SET);
		getDungeon()->nObjects[iDBCategory] = 32;
		printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[iDBCategory]); 
		loadRawData (fp, (unsigned short*)ITEMS[iDBCategory], itemBytes[iDBCategory], getDungeon()->nObjects[iDBCategory]);
		swapItemData(iDBCategory, (unsigned short*)ITEMS[iDBCategory], getDungeon()->nObjects[iDBCategory]);

		iDBCategory = 3;
		fseek (fp, 0xA7526, SEEK_SET);
		getDungeon()->nObjects[iDBCategory] = 28;
		printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[iDBCategory]); 
		loadRawData (fp, (unsigned short*)ITEMS[iDBCategory], itemBytes[iDBCategory], getDungeon()->nObjects[iDBCategory]);
		swapItemData(iDBCategory, (unsigned short*)ITEMS[iDBCategory], getDungeon()->nObjects[iDBCategory]);

		iDBCategory = 2;
		fseek (fp, 0xA75F6, SEEK_SET);
		getDungeon()->nObjects[iDBCategory] = 23;	// 13 ?
		printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[iDBCategory]); 
		loadRawData (fp, (unsigned short*)ITEMS[iDBCategory], itemBytes[iDBCategory], getDungeon()->nObjects[iDBCategory]);
		swapItemData(iDBCategory, (unsigned short*)ITEMS[iDBCategory], getDungeon()->nObjects[iDBCategory]);

		iDBCategory = 1;
		fseek (fp, 0xA7652, SEEK_SET);
		getDungeon()->nObjects[iDBCategory] = 5;
		printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[iDBCategory]); 
		loadRawData (fp, (unsigned short*)ITEMS[iDBCategory], itemBytes[iDBCategory], getDungeon()->nObjects[iDBCategory]);
		swapItemData(iDBCategory, (unsigned short*)ITEMS[iDBCategory], getDungeon()->nObjects[iDBCategory]);

		iDBCategory = 0;
		fseek (fp, 0xA7670, SEEK_SET);
		getDungeon()->nObjects[iDBCategory] = 6;
		printf("LOAD: Item Data [%02d] @ %08x | Nb = %04d\n", i, ftell(fp), getDungeon()->nObjects[iDBCategory]); 
		loadRawData (fp, (unsigned short*)ITEMS[iDBCategory], itemBytes[iDBCategory], getDungeon()->nObjects[iDBCategory]);
		swapItemData(iDBCategory, (unsigned short*)ITEMS[iDBCategory], getDungeon()->nObjects[iDBCategory]);

		fclose (fp);

		printf("INIT OBJECTS NUMBERS\n");
		itemInitObjectNumbersFromDungeon ();
		printf("ORGANIZE STACKS\n");
		organizeStacks ();
		//loadTexts (RAWTEXTS);
		printf("UPDATE COLORS\n");
		assumeMonstersForMaps ();
		updatePriorityColors ();

	}
	return 0;
}

int
loadDungeonData (char *dungeonname)
{
	FILE* fp = NULL;
	int iDetectedDungeonType = dungeon_Unknown;
	unsigned checkoff = 1;	// SPX 2021-08, remove csum check, specially to load Kid Dungeon
	int itemnumber = 0;
	unsigned int adjustOffset = 0;	// special after item data (SK DMDC2 special case?)
	char dummy = 0;
	int z = 0;
	int dngnum = 0;
	iAmigaSTEndian = 0;
	
	iDetectedDungeonType = assumeDungeonType (dungeonname);

	printf("LOAD: Starts loading dungeon \"%s\" as : %s type.\n", dungeonname, txt_dungeon_types[iDetectedDungeonType]); 

	if (iDetectedDungeonType == dungeon_TheronQuest)
	{
		dngnum = getTextCursor (cursor_SubText);
		return loadTheronsQuestDungeonData(dungeonname, dngnum);
	}
	else if (iDetectedDungeonType == dungeon_AmigaDemoWinUAE)
		return loadAmigaDemoWinUAEDungeonData(dungeonname);

	fp = fopen (dungeonname, "rb");
	//printf("LOAD: Starts loading dungeon \"%s\".\n", dungeonname); 

	initCompanionActuatorsTable();

	if (fp != NULL)
	{
		unsigned short realcsum;
		unsigned short csum;
		unsigned short mapcolumns;
		size_t i;

		{
			char null;
			char nlevs;
			savecsum = 0;
			shsize = 0;
			fseek (fp, 4, SEEK_SET);
			fread (&nlevs, 1, 1, fp);
			fread (&null, 1, 1, fp);
			if (null == 0 && (nlevs >= 1 && nlevs <= 64)) // dungeon.dat
			{
				fseek (fp, 0, SEEK_SET);
				if (SAVEHEADER != NULL)
					free (SAVEHEADER);
				shsize = 0;
				savecsum = 0;
			}
			else	// should be a savegame.dat
			{
				int searching = 1;
				int tries = 0;
				if (SAVEHEADER != NULL)
					free (SAVEHEADER);
				shsize = 0x23c0;

				// for Amiga CSB
				shsize = 0x2390;

				// for Amiga WinUAE Save State (USS)
				// map x34780

				printf("LOAD: This does not look like a DUNGEON.DAT. Trying to load as a SAVEGAME...\n"); 

				while (searching && (tries < 32))
				{
					fseek (fp, shsize + 4, SEEK_SET);
					fread (&nlevs, 1, 1, fp);
					fread (&null, 1, 1, fp);
					if (null == 0 && (nlevs >= 1 && nlevs <= 64)) // dungeon.dat
						searching = 0;
					else
					{
						shsize += 0x2c0;
						tries += 1;
					}
				}
				
				if (tries == 16)
					return 0; // failed
				
				SAVEHEADER = (unsigned char*) calloc (shsize, 1);
				fseek (fp, 0, SEEK_SET);
				fread (SAVEHEADER, shsize, 1, fp);
				//fseek (fp, 0x23c0, SEEK_SET);
				savecsum = 1;
				checkoff = 1;
			}
		}

		/* flush all references */
	startGroundReferences ();

	printf("LOAD: Headers (Cat ##) @ %08x\n", ftell(fp)); 
	loadGeneralInfo (fp);

	printf("LOAD: Levels @ %08x\n", ftell(fp)); 
	loadLevelSpecifications (fp);

	printf("LOAD: Offsets List @ %08x\n", ftell(fp)); 
	itemInitObjectNumbersFromDungeon ();
	
	mapcolumns = calculateOffsets ();
	loadRawData (fp, OFFSETS, 2, mapcolumns);
	if (iAmigaSTEndian)
		swapData(OFFSETS, 2, mapcolumns);

	printf("LOAD: Ground Refs @ %08x\n", ftell(fp)); 
	REFERENCES = (unsigned short*) calloc (getDungeon()->itemListSize, sizeof (short));
	loadRawData (fp, REFERENCES, 2, getDungeon()->itemListSize);
	if (iAmigaSTEndian)
		swapData(REFERENCES, 2, getDungeon()->itemListSize);
	
	printf("LOAD: Raw texts @ %08x\n", ftell(fp)); 
	RAWTEXTS = (unsigned short*) calloc (getDungeon()->textsDataSize, sizeof (short));
	loadRawData (fp, RAWTEXTS, 2, getDungeon()->textsDataSize);
	if (iAmigaSTEndian)
		swapData(RAWTEXTS, 2, getDungeon()->textsDataSize);

	for (i = 0; i < 16; i++)
	{
		printf("LOAD: Item Data [%02d] @ %08x\n", i, ftell(fp)); 
		loadRawData (fp, (unsigned short*)ITEMS[i], itemBytes[i], getDungeon()->nObjects[i]);
		if (iAmigaSTEndian)
			swapItemData(i, (unsigned short*)ITEMS[i], getDungeon()->nObjects[i]);
	}

	//--- Special adjustment
	adjustOffset = 0;
	if (TELOS)
		adjustOffset = 0;	// 16 only for the TELOS 2021 which has been corrupted ???
	for (i = 0; i < adjustOffset; i++)
		fread (&dummy, 1, 1, fp);

	printf("LOAD: Map level data @ %08x\n", ftell(fp)); 
	loadLevelsData (fp, dungeon_Master);

	printf("LOAD: End checksum @ %08x\n", ftell(fp)); 
	realcsum = checksum (fp);
	fread (&csum, 2, 1, fp);
	organizeStacks ();
	loadTexts (RAWTEXTS);

	free (REFERENCES);
	free (OFFSETS);
	free (RAWTEXTS);

	//mark "keep" bits for all present items.
	itemnumber = getDungeon()->nObjects[5];
	for (i = 0; i < (unsigned int)itemnumber; i++)
	{
		weapon_p weapon = (weapon_p) ((short*)(ITEMS[5] + i*(itemBytes[5]>>1) + 1));
		weapon->keep = 1;
	}
	itemnumber = getDungeon()->nObjects[6];
	for (i = 0; i < (unsigned int)itemnumber; i++)
	{
		clothing_p clothing = (clothing_p) ((short*)(ITEMS[6] + i*(itemBytes[6]>>1) + 1));
		clothing->keep = 1;
	}
	itemnumber = getDungeon()->nObjects[8];
	for (i = 0; i < (unsigned int)itemnumber; i++)
	{
		potion_p potion = (potion_p) ((short*)(ITEMS[8] + i*(itemBytes[8]>>1) + 1));
		potion->keep = 1;
	}
	itemnumber = getDungeon()->nObjects[10];
	for (i = 0; i < (unsigned int)itemnumber; i++)
	{
		misc_p misc = (misc_p) ((short*)(ITEMS[10] + i*(itemBytes[10]>>1) + 1));
		misc->keep = 1;
	}
	
	//assumeMonstersForMaps ();
	updatePriorityColors ();
	fclose (fp);
	setEditCursor (cursor_L, 0);	// Reinit current level to zero after loading a new dungeon.. (avoid being beyond limits)
	if (!checkoff && (csum != realcsum))
		return -1; // wrong checksum
	if (savecsum) // was a savegame
		return 2;
	return 1; // success
	}
	return 0; // failed
}


int
assumeDungeonTypeFromDir (char *dungeonname)
{
	char fulldungeonname[512];
	memset(fulldungeonname, 0, 512);
#ifdef __LINUX__
	sprintf(fulldungeonname, "dungeons/%s", dungeonname);
#else
	sprintf(fulldungeonname, "dungeons\\%s", dungeonname);
#endif // __LINUX__
	return assumeDungeonType (fulldungeonname);
}

int
assumeDungeonType (char* dungeonname)
{
	FILE* fp = NULL;
	int iDetectedDungeonType = dungeon_Unknown;
	int itemnumber = 0;
	unsigned int adjustOffset = 0;	// special after item data (SK DMDC2 special case?)
	char dummy = 0;
	dm_dungeon_header dheader;
	char buffer[128];

	fp = fopen (dungeonname, "rb");

	if (fp != NULL)
	{
		unsigned short realcsum;
		unsigned short csum;
		unsigned short mapcolumns;
		size_t i;

		printf("CHECK: Checking dungeon type for \"%s\".\n", dungeonname); 
		{
			char nlevs;
			fread (&dheader, 44, 1, fp);
			if (dheader.null == 0 && (dheader.nLevels >= 1 && dheader.nLevels <= 64)) // dungeon.dat
			{
				iDetectedDungeonType = dungeon_Master;
				if (dheader.randomGraphicsSeed == 0x000D || dheader.randomGraphicsSeed == 0x0D00 || dheader.randomGraphicsSeed == 0x0008 || dheader.randomGraphicsSeed == 0x0800)
					iDetectedDungeonType = dungeon_Chaos;
				if (dheader.randomGraphicsSeed == 0x0000 || dheader.nLevels > 16)
					iDetectedDungeonType = dungeon_Skullkeep;
				if ((dheader.randomGraphicsSeed == 0x0063 || dheader.randomGraphicsSeed == 0x6300) && dheader.nLevels <= 16)
					iDetectedDungeonType = dungeon_Master;
				// Note: write other cases that would say it is NOT a DM type
			}
			// check for Theron's Quest
			if (iDetectedDungeonType == dungeon_Unknown)
			{
				fseek(fp, 0, SEEK_SET);
				fseek(fp, 0x9B3C2, SEEK_SET);
				memset(buffer, 0, 128);
				fread(buffer, 1, 30, fp);
				if (!strcmp(buffer, "GO AWAY AND RESURRECT THERON"))	// TQ02.iso (version ???)
					iDetectedDungeonType = dungeon_TheronQuest;
				else
				{	
					fseek(fp, 0x1B3C2, SEEK_SET);
					if (!strcmp(buffer, "GO AWAY AND RESURRECT THERON"))	// TQUS19
						iDetectedDungeonType = dungeon_TheronQuest;
					else 
					{
						fseek(fp, 0, SEEK_SET);
						fseek(fp, 0x193AE, SEEK_SET);
						memset(buffer, 0, 128);
						fread(buffer, 1, 10, fp);
						if (!strcmp(buffer, "ARCHMASTER"))
							iDetectedDungeonType = dungeon_TheronQuest;	// TQJP19
					}

					{
						fseek(fp, 0x9D91E, SEEK_SET);
						memset(buffer, 0, 128);
						fread(buffer, 1, 10, fp);
						if (!strcmp(buffer, "ARCHMASTER"))
							iDetectedDungeonType = dungeon_TheronQuest;	// TQJP19 bin or TQJP02 bin
					}

				}
			}
			// check for Amiga DM Demo as WinUAE save state (uncompressed)
			if (iDetectedDungeonType == dungeon_Unknown)
			{
				fseek(fp, 0xA83B5, SEEK_SET);
				memset(buffer, 0, 128);
				fread(buffer, 1, 8, fp);
				if (!strcmp(buffer, "ZOKATHRA"))
					iDetectedDungeonType = dungeon_AmigaDemoWinUAE;
			}
		}
		fclose (fp);
	}

	return iDetectedDungeonType;

}
