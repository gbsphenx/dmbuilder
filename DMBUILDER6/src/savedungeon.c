//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Save Dungeon
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <savedungeon.h>
#include <editor.h>
#include <level.h>
#include <reference.h>
#include <text.h>

unsigned short* OFFSETS;

unsigned short* REFERENCES;
unsigned short* RAWTEXTS;

unsigned short* FREESPACE;

extern short* ITEMS[16];
extern int nItems[16]; 
extern size_t itemBytes [16];


//------------------------------------------------------------------------------

static void
saveGeneralInfo (FILE* fp)
{
	fwrite (getDungeon(), 44, 1, fp);
}

static void
saveLevelSpecifications (FILE* fp)
{
	unsigned char i;
	for (i = 0; i < getDungeon()->nLevels; i++)
		fwrite (&(getLevels()[i].header), 16, 1, fp);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
unsigned short
calculateOffsetsNumber ()
{
	// number of map columns (on x).
	size_t i;
	unsigned short count = 0;
	for (i = 0; i < getDungeon()->nLevels; i++)
	{
		count += (getLevels()[i].header.xDim+1);
	}
	OFFSETS = (unsigned short*) calloc (count, sizeof(short));
	return count;
}

void
objectsOffsetsUpdate (size_t maxOffsetsSize)
{
	char i, j, l;
	size_t z = 0;
	unsigned short total = 0;

	for (l = 0; l < getDungeon()->nLevels; l++)
	{
		if (z >= maxOffsetsSize) break;
		for (i = 0; i < (char) getLevels()[l].header.xDim+1; i++)
		{	
			OFFSETS[z] = total;
			if (z >= maxOffsetsSize) break;
			for (j = 0; j < (char) getLevels()[l].header.yDim+1; j++)
			{
				if (getLevels()[l].map[i][j].object)
					total ++;
			}
			z++;
		}
	}
}
//------------------------------------------------------------------------------

void
calculateReferences ()
{
	char i, j, l;
	size_t z = 0;
	unsigned short total = 0;

	for (l = 0; l < getDungeon()->nLevels; l++)
	{
		for (i = 0; i < (char) getLevels()[l].header.xDim+1; i++)
		{	
			for (j = 0; j < (char) getLevels()[l].header.yDim+1; j++)
			{
				short ref = *((short*) getGroundReference (i, j, l));
				if (ref != -2)
					REFERENCES[z++] = ref;
			}
		}
	}
	getDungeon()->itemListSize = z;
}

//------------------------------------------------------------------------------
static unsigned short
levelLength (size_t n)
{
	unsigned short length;
	level_p level = &getLevels()[n];

	length = ((level->header.xDim+1) * (level->header.yDim+1))
		+ level->header.nMonsters + level->header.nWalls
		+ level->header.nOrnates + level->header.nFloors;
	return length;
}

unsigned short
mapsLength ()
{
	size_t i = 0;
	unsigned short offset = 0;
	unsigned short length;
	
	getLevels()[0].header.mapOffset = 0;
	for (i = 1; i < getDungeon()->nLevels; i++)
	{
		length = levelLength (i-1);
		offset += length;
		getLevels()[i].header.mapOffset = offset;
	}
	offset += levelLength (getDungeon()->nLevels-1);
	return offset; // retourne la taille totale du layout
}
//------------------------------------------------------------------------------
void
saveTilesMap (FILE* fp, size_t level)
{
	char i, j;
	for (i = 0; i < getLevels()[level].header.xDim+1; i++)
		for (j = 0; j < getLevels()[level].header.yDim+1; j++)
			fwrite (&getLevels()[level].map[i][j], 1, 1, fp);
}

void
saveLevelsData (FILE* fp)
{
	size_t i;
	for (i = 0; i < getDungeon()->nLevels; i++)
	{
		saveTilesMap (fp, i);
		fwrite (&(getLevels()[i].monsters), getLevels()[i].header.nMonsters, 1, fp);
		fwrite (&(getLevels()[i].walls), getLevels()[i].header.nWalls, 1, fp);
		fwrite (&(getLevels()[i].floors), getLevels()[i].header.nFloors, 1, fp);
		fwrite (&(getLevels()[i].ornates), getLevels()[i].header.nOrnates, 1, fp);
	}
}
//------------------------------------------------------------------------------
short*
makeFreeItems (size_t number)
{
	size_t i;
	short *items = (short*) calloc (number, sizeof (short));
	for (i = 0; i < number; i++)
		items[i] = (short) 0xFFFF;
	return items;
}
//------------------------------------------------------------------------------

void saveRawData (FILE* fp, short *data, size_t datasize, size_t nItems)
{
	fwrite (data, datasize, nItems, fp);
}
//------------------------------------------------------------------------------
int
saveDungeonData (char *dungeonname)
{
	unsigned int nFreeItems = 256;
	FILE* fp;
	
	fp = fopen (dungeonname, "w+b");

	if (fp != NULL)
	{
		unsigned short item_offsets;
		unsigned short csum;
		size_t i;
		size_t coded_size;

		getDungeon()->mapDataSize = mapsLength ();

		RAWTEXTS = (short*) calloc (65535, sizeof (short));
		REFERENCES = (short*) calloc (65535, sizeof (short));
	

	
		if (SAVEHEADER != NULL)
		{
			fwrite (SAVEHEADER, shsize, 1, fp);
			// don't free it!! it will prevent user to save several times the savegame
			//free (SAVEHEADER);
			//SAVEHEADER = NULL;
		}
		if (!savecsum)
			nFreeItems = 0;


		coded_size = encodeTexts (RAWTEXTS);
		getDungeon()->textsDataSize = coded_size;

		calculateReferences ();

		for(i = 0; i < 16; i++)
			getDungeon()->nObjects[i] = (unsigned short) nItems[i];

		getDungeon()->itemListSize += nFreeItems;

		saveGeneralInfo (fp);
		saveLevelSpecifications (fp);

		item_offsets = calculateOffsetsNumber ();
		objectsOffsetsUpdate (item_offsets);
		
		saveRawData (fp, (short*) OFFSETS, 2, item_offsets);
		saveRawData (fp, REFERENCES, 2, getDungeon()->itemListSize - nFreeItems);
	
		if (savecsum)
		{
			FREESPACE = makeFreeItems (nFreeItems);
			saveRawData (fp, (short*) FREESPACE, 2, nFreeItems);
		}
		saveRawData (fp, RAWTEXTS, 2, coded_size);
	
		convertToMasterTexts ();
		for (i = 0; i < 16; i++)
			saveRawData (fp, ITEMS[i], itemBytes[i], getDungeon()->nObjects[i]); 

		convertToInternTexts ();
		saveLevelsData (fp);
		
		free (REFERENCES);
		free (OFFSETS);
		free (RAWTEXTS);
		if (savecsum)
			free (FREESPACE);

		//csum = checksum (fp);
		csum = (unsigned short) checksumS (fp, shsize);
		fwrite (&csum, 2, 1, fp);

		fclose (fp);
		if (savecsum) // was a savegame
			return 2;
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
int
saveMusicList (char *sFilenameSongListDat)
{
	FILE* fp = NULL;
	int i = 0;
	int iMusicID = 0;
	int iNbMaps = 0;
	level_p xMapLevel = NULL;
	
	iNbMaps = getDungeon()->nLevels;

	fp = fopen (sFilenameSongListDat, "w+b");

	if (fp != NULL)
	{
		for (i = 0; i < 64; i++)
		{
			iMusicID = 0xFF;
			if (i < iNbMaps)
			{
				xMapLevel = getLevel (i);
				iMusicID = xMapLevel->iMusicID;
			}

			fwrite(&iMusicID, 1, 1, fp);
		}
	}
	fclose (fp);
	return 0;
}
