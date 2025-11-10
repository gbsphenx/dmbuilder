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
	"UNKNOWN", "MASTER", "CHAOS", "THERON'S QUEST", "SKULLKEEP" };

static const char** txt_dungeon_types = &txt_dungeon_types_all[1];

tCompanionActuator xActuatorTable[1024];

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
	else
		SKULLKEEP = 0;
}




static void
loadLevelSpecifications (FILE* fp)
{
	unsigned char i = 0;
	unsigned int tssum = 0;
	for (i = 0; i < getDungeon()->nLevels; i++)
	{
		fread (&(getLevels()[i].header), 16, 1, fp);
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
// [4] = start of item data (4 to 11)
// [5] = start of text data
int iTQMemOffset[][6] = { {0x0002A0F1, 0x0002A2D7, 0x0002A831, 0x0002AD0F, 0x0002B800, 0x0002C9D8},  // Ak-Tu-Ba
						{0x0006A333, 0x0006A5E9, 0x0006ABA5, 0x0006B031, 0x0006B800, 0x0006CECC},	// Tower of Drator

						{0x0006A333, 0x0006A5E9, 0x0006ABA5, 0x0006B031, 0x000AB800, 0x0006CECC},	// Tower of Drator
						{0x0006A333, 0x0006A5E9, 0x0006ABA5, 0x0006B031, 0x000EB800, 0x0006CECC},	// Tower of Drator
						{0x0006A333, 0x0006A5E9, 0x0006ABA5, 0x0006B031, 0x0012B800, 0x0006CECC},	// Tower of Drator
						{0x0006A333, 0x0006A5E9, 0x0006ABA5, 0x0006B031, 0x0016B800, 0x0006CECC},	// Tower of Drator

						{0x001AA831, 0x001AA9EC, 0x001AB045, 0x001AB4BD, 0x001AB800, 0x001ACC0E}	// Demon's Gate

};

int iTQMapsPerDungeon[] = { 4, 8, 5, 6, 3, 4, 4 };
int iTQTextDataSize[] = { 0x13C, 0xD0, 0xE0, 0xE8, 0xE0, 0xD9, 0xE8};

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
		fseek (fp, iTQMemOffset[iTQDungeon][1], SEEK_SET);	// 0x0002A2D7
		
		loadLevelsData (fp, dungeon_TheronQuest);

		// auto add floor gfx for all maps
		for (m = 0; m < maps; m++)
		{
			int d = 0;
			level_p currentmap = getLevel(m);
			currentmap->header.nFloors = 8;
			for (d = 0; d < 8; d++)
				currentmap->floors[d] = d;
		}

		// ground references
		fseek (fp, iTQMemOffset[iTQDungeon][2], SEEK_SET); // 0x0002A831
		getDungeon()->itemListSize = 183;
		printf("LOAD: Ground Refs @ %08x\n", ftell(fp)); 
		REFERENCES = (unsigned short*) calloc (getDungeon()->itemListSize, sizeof (short));
		loadRawData (fp, REFERENCES, 2, getDungeon()->itemListSize);

		// doors, teleporters, texts, actuators
		fseek (fp, iTQMemOffset[iTQDungeon][3], SEEK_SET);	// 0x0002AD0F

		if (iTQDungeon == 1 || iTQDungeon == 6)
			iItemDataPart2StartIndex = 3;	// 0 to 2, then 3 to 10

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
	
	iDetectedDungeonType = assumeDungeonType (dungeonname);

	printf("LOAD: Starts loading dungeon \"%s\" as : %s type.\n", dungeonname, txt_dungeon_types[iDetectedDungeonType]); 

	if (iDetectedDungeonType == dungeon_TheronQuest)
	{
		dngnum = getTextCursor (cursor_SubText);
		return loadTheronsQuestDungeonData(dungeonname, dngnum);
	}

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

	printf("LOAD: Ground Refs @ %08x\n", ftell(fp)); 
	REFERENCES = (unsigned short*) calloc (getDungeon()->itemListSize, sizeof (short));
	loadRawData (fp, REFERENCES, 2, getDungeon()->itemListSize);
	
	printf("LOAD: Raw texts @ %08x\n", ftell(fp)); 
	RAWTEXTS = (unsigned short*) calloc (getDungeon()->textsDataSize, sizeof (short));
	loadRawData (fp, RAWTEXTS, 2, getDungeon()->textsDataSize);

	for (i = 0; i < 16; i++)
	{
		printf("LOAD: Item Data [%02d] @ %08x\n", i, ftell(fp)); 
		loadRawData (fp, (unsigned short*)ITEMS[i], itemBytes[i], getDungeon()->nObjects[i]);
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
			}
			// check for Theron's Quest
			if (iDetectedDungeonType == dungeon_Unknown)
			{
				fseek(fp, 0, SEEK_SET);
				fseek(fp, 0x9B3C2, SEEK_SET);
				memset(buffer, 0, 128);
				fread(buffer, 1, 30, fp);
				if (!strcmp(buffer, "GO AWAY AND RESURRECT THERON"))
					iDetectedDungeonType = dungeon_TheronQuest;
				else 
				{
					fseek(fp, 0, SEEK_SET);
					fseek(fp, 0x193AF, SEEK_SET);
					memset(buffer, 0, 128);
					fread(buffer, 1, 10, fp);
					if (!strcmp(buffer, "ARCHMASTER"))
						iDetectedDungeonType = dungeon_TheronQuest;
				}

			}
		}
		fclose (fp);
	}

	return iDetectedDungeonType;

}
