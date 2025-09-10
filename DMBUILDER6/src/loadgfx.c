//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Load Graphics Sets
//------------------------------------------------------------------------------

#include <dm.h>
#include <gui.h>
#include <memory.h>
#include <image.h>

#include <tga.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern GLenum texFormat; // defined in tga

#ifdef __MINGW__
	#undef GLenum
#endif // __MINGW__

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>

//------------------------------------------------------------------------------

int loadArchive (char* archive, int glbank, unsigned int number);

//------------------------------------------------------------------------------

// code from Nate Miller (tga.c)
extern char *getData (FILE *s, int sz, int iBits);
extern int returnError (FILE *s, int error);
extern int checkSize (int x);

// code inspired from Nate Miller (tga.c)
image_t*
Image_LoadFromTGA (char *name, int vflip)
{
    unsigned char type[4];
    unsigned char info[7];
    unsigned char *imageData = NULL;
    int imageWidth, imageHeight;
    int imageBits, size;
	image_t* image;

    FILE *s;

    if (!(s = fopen(name, "r+bt")))
	{
		printf ("Can't load %s image.\n", name);
		return NULL;
	}

    fread(&type, sizeof (char), 3, s);
    fseek(s, 12, SEEK_SET);
    fread(&info, sizeof (char), 6, s);

    if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
        returnError (s, TGA_BAD_IMAGE_TYPE);

    imageWidth = info[0] + info[1] * 256; 
    imageHeight = info[2] + info[3] * 256;
    imageBits =	info[4]; 

    size = imageWidth * imageHeight; 

    if (imageBits != 32 && imageBits != 24 && imageBits != 8)
        returnError (s, TGA_BAD_BITS);

    imageData = getData(s, size, imageBits);

    if (imageData == NULL)
        returnError (s, TGA_BAD_DATA);

    fclose (s);

	image = (image_t*) MEMCALLOC (1, sizeof (image_t));
	image->height = imageHeight;
	image->width = imageWidth;
	image->data = (unsigned char*) MEMCALLOC (image->height*image->width*3, sizeof (unsigned char));
	image->nPlanes = 3;
	image->activePlane = 0;
	image->texFormat = texFormat;
	// copy loaded data into image structure
	if (vflip == 1)	// default, the tga are vertically reversed! (...)
	{
		unsigned i;
		for (i = 0; i < image->height*image->width*3; i++)
			image->data[i] = imageData[i];
	}
	/*if (vflip == 0)	// tga is saved in the normal direction.
	{
		unsigned i;
		for (i = 0; i < image->height*image->width*3; i++)
			image->data[i] = imageData[i];
	}*/
	free (imageData);
	return image;
}

//------------------------------------------------------------------------------

void
Graphics_LoadSkullkeepImagesSet (char* name, unsigned int gl_offset, int sizeX, int sizeY)
{
	image_t* itemset, *item;
	unsigned maxx, maxy;
	itemset = Image_LoadFromTGA (name, 1);
	if (itemset != NULL)
	{
		maxx = itemset->width/sizeX;
		maxy = itemset->height/sizeY;
		if (itemset != NULL)
		{
			unsigned x, y;
			for (x = 0; x < maxx; x ++)
			for (y = 0; y < maxy; y ++)
			{
				item = Image_ScissorCut (x*sizeX, y*sizeY, (x+1)*sizeX, (y+1)*sizeY, itemset);
				Image_Bind (item, gl_offset + maxx*y + x);
				MEMFREE (item->data);
				MEMFREE (item);
			}
		MEMFREE (itemset->data);
		MEMFREE (itemset);
		}
	}
}



/*void
Graphics_LoadSkullkeepItems (char* name, unsigned category)
{
	image_t* itemset, *item;
	unsigned maxx, maxy;
	itemset = Image_LoadFromTGA (name);
	maxx = itemset->width>>4;
	maxy = itemset->height>>4;
	if (itemset != NULL)
	{
		unsigned x, y;
		for (x = 0; x < maxx; x ++)
		for (y = 0; y < maxy; y ++)
		{
			item = Image_ScissorCut (x*16, y*16, (x+1)*16, (y+1)*16, itemset);
			Image_Bind (item, gl_StaticSkullkeep + category*0x100 + 16*y + x);
			MEMFREE (item->data);
			MEMFREE (item);
		}
	MEMFREE (itemset->data);
	MEMFREE (itemset);
	}
}*/

void
Graphics_LoadSkullkeepItems (char* name, unsigned category)
{
	Graphics_LoadSkullkeepImagesSet (name, gl_StaticSkullkeep + category*0x100, 16, 16);
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------


static void
loadGroup (char *directory, int gl_group, unsigned int number)
{
	unsigned int i;
	char path[] ="                                                                                ";
	char num[] = "       ";

	for (i = 1; i < number; i++)
	{
	if (i < 10)
		gcvt (i, 1, num);
	else 
		gcvt (i, 2, num);
	strcpy (path, directory);
	strcat (path, "\\");
	strcat (path, num);
	strcat (path, ".tga");
	loadTGA (path, gl_group + i-1);
	}
}

void loadGraphics ()
{
	loadTGA ("Fake.tga", 0);
//------------------------------------------------------------------------------
	loadArchive ("OrnateMasks", gl_Ornates + gl_Masking, ornate_Max);
	loadArchive ("Potions", gl_Potions, potion_Max);
	loadArchive ("Doors", gl_Doors, door_Max);
	loadArchive ("Walls", gl_Walls, wall_Max);
	loadArchive ("Floors", gl_Floors, floor_Max);
	loadArchive ("Ornates", gl_Ornates, ornate_Max);
	loadArchive ("Monsters", gl_Monsters, monster_Max);
	loadArchive ("Weapons", gl_Weapons, weapon_Max);
	loadArchive ("Clothes", gl_Clothes, clothing_Max);
	loadArchive ("Miscs", gl_Miscs, misc_Max);
	loadArchive ("Portraits", gl_Portraits, portrait_Max);
	loadArchive ("Spells", gl_Spells, spell_Max);
	loadArchive ("Special", gl_Special, special_Max);
	loadArchive ("Tiles", gl_Tiles, xtile_Max);
	loadArchive ("Gui", gl_Gui, gui_Max);
	loadArchive ("Powers", gl_Powers, power_Max);
#ifndef __LINUX__
//------------------------------------------------------------------------------
	Graphics_LoadSkullkeepImagesSet ("gfx\\SKDoors.tga", gl_StaticSkullkeep + gl_Doors, 64, 64);
	Graphics_LoadSkullkeepImagesSet ("gfx\\SKDoorOrnates.tga", gl_StaticSkullkeep + gl_Ornates, 64, 64);
	Graphics_LoadSkullkeepImagesSet ("gfx\\SKTileset.tga", gl_SK_TilesSet, 16, 16);
	Graphics_LoadSkullkeepImagesSet ("gfx\\SKObjects.tga", gl_StaticSkullkeep + gl_Monsters, 64, 64);
	Graphics_LoadSkullkeepImagesSet ("gfx\\SKWallOrnates.tga", gl_StaticSkullkeep + gl_Walls, 64, 64);
	Graphics_LoadSkullkeepImagesSet ("gfx\\SKFloorOrnates.tga", gl_StaticSkullkeep + gl_Floors, 64, 64);
//	Graphics_LoadSkullkeepImagesSet ("gfx\\SKChampions.tga", gl_StaticSkullkeep + gl_Portraits, 32, 32);
	Graphics_LoadSkullkeepImagesSet ("gfx\\TORCChampions.tga", gl_StaticSkullkeep + gl_Portraits, 32, 32);
	Graphics_LoadSkullkeepImagesSet ("gfx\\SKTileSpecialFloors.tga", gl_StaticSkullkeep + gl_SpecialTiles, 16, 16);
	Graphics_LoadSkullkeepImagesSet ("gfx\\SKPotions.tga", gl_StaticSkullkeep + gl_Potions, 16, 16);
	Graphics_LoadSkullkeepImagesSet ("gfx\\SKContainers.tga", gl_StaticSkullkeep + gl_Containers, 16, 16);

	Graphics_LoadSkullkeepImagesSet ("gfx\\SKWActuators.tga", gl_StaticSkullkeep + gl_WActuators, 64, 64);

	Graphics_LoadSkullkeepImagesSet ("gfx\\TELOSObjects.tga", gl_StaticTELOS + gl_Monsters, 64, 64);

	//Graphics_LoadSkullkeepImagesSet ("gfx\\CSBChampions.tga", gl_StaticSkullkeep + gl_Champions, 32, 32);
//	Graphics_LoadSkullkeepItems ("gfx\\SKWeapons.tga", 5);
	Graphics_LoadSkullkeepItems ("gfx\\TORCWeapons.tga", 5);
//	Graphics_LoadSkullkeepItems ("gfx\\SKClothings.tga", 6);
	Graphics_LoadSkullkeepItems ("gfx\\TORCClothings.tga", 6);
	Graphics_LoadSkullkeepItems ("gfx\\SKPotions.tga", 8);
	Graphics_LoadSkullkeepItems ("gfx\\SKBags.tga", 9);
//	Graphics_LoadSkullkeepItems ("gfx\\SKMiscs.tga", 10);
	Graphics_LoadSkullkeepItems ("gfx\\TORCMiscs.tga", 10);
#else
//------------------------------------------------------------------------------
	Graphics_LoadSkullkeepImagesSet ("GFX/SKDoors.tga", gl_StaticSkullkeep + gl_Doors, 64, 64);
	Graphics_LoadSkullkeepImagesSet ("GFX/SKDoorOrnates.tga", gl_StaticSkullkeep + gl_Ornates, 64, 64);
	Graphics_LoadSkullkeepImagesSet ("GFX/SKTileset.tga", gl_SK_TilesSet, 16, 16);
	Graphics_LoadSkullkeepImagesSet ("GFX/SKObjects.tga", gl_StaticSkullkeep + gl_Monsters, 64, 64);
	Graphics_LoadSkullkeepImagesSet ("GFX/SKWallOrnates.tga", gl_StaticSkullkeep + gl_Walls, 64, 64);
	Graphics_LoadSkullkeepImagesSet ("GFX/SKFloorOrnates.tga", gl_StaticSkullkeep + gl_Floors, 64, 64);
//	Graphics_LoadSkullkeepImagesSet ("GFX/SKChampions.tga", gl_StaticSkullkeep + gl_Portraits, 32, 32);
	Graphics_LoadSkullkeepImagesSet ("GFX/TORCChampions.tga", gl_StaticSkullkeep + gl_Portraits, 32, 32);
	Graphics_LoadSkullkeepImagesSet ("GFX/SKTileSpecialFloors.tga", gl_StaticSkullkeep + gl_SpecialTiles, 16, 16);
	Graphics_LoadSkullkeepImagesSet ("GFX/SKPotions.tga", gl_StaticSkullkeep + gl_Potions, 16, 16);
	Graphics_LoadSkullkeepImagesSet ("GFX/SKContainers.tga", gl_StaticSkullkeep + gl_Containers, 16, 16);

	Graphics_LoadSkullkeepImagesSet ("GFX/SKWActuators.tga", gl_StaticSkullkeep + gl_WActuators, 64, 64);

	Graphics_LoadSkullkeepImagesSet ("GFX/TELOSObjects.tga", gl_StaticTELOS + gl_Monsters, 64, 64);

	//Graphics_LoadSkullkeepImagesSet ("GFX/CSBChampions.tga", gl_StaticSkullkeep + gl_Champions, 32, 32);
//	Graphics_LoadSkullkeepItems ("GFX/SKWeapons.tga", 5);
	Graphics_LoadSkullkeepItems ("GFX/TORCWeapons.tga", 5);
//	Graphics_LoadSkullkeepItems ("GFX/SKClothings.tga", 6);
	Graphics_LoadSkullkeepItems ("GFX/TORCClothings.tga", 6);
	Graphics_LoadSkullkeepItems ("GFX/SKPotions.tga", 8);
	Graphics_LoadSkullkeepItems ("GFX/SKBags.tga", 9);
//	Graphics_LoadSkullkeepItems ("GFX/SKMiscs.tga", 10);
	Graphics_LoadSkullkeepItems ("GFX/TORCMiscs.tga", 10);
#endif // __LINUX__
//..............................................................................
//..............................................................................
//..............................................................................
//..............................................................................

}
//------------------------------------------------------------------------------

int extractTGA (FILE* s, int id)
{
    unsigned char type[4];
    unsigned char info[7];
    unsigned char *imageData = NULL;
    int imageWidth, imageHeight;
    int imageBits, size;
    fread (&type, sizeof (char), 3, s);
    fseek (s, 9, SEEK_CUR);
    fread (&info, sizeof (char), 6, s);
    if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
        returnError (s, TGA_BAD_IMAGE_TYPE);
    imageWidth = info[0] + info[1] * 256; 
    imageHeight = info[2] + info[3] * 256;
    imageBits =	info[4]; 
    size = imageWidth * imageHeight; 
    if (!checkSize (imageWidth) || !checkSize (imageHeight))
        returnError (s, TGA_BAD_DIMENSION);
    if (imageBits != 32 && imageBits != 24 && imageBits != 8)
        returnError (s, TGA_BAD_BITS);
    imageData = getData (s, size, imageBits);
    if (imageData == NULL)
        returnError (s, TGA_BAD_DATA);
    glBindTexture (GL_TEXTURE_2D, id);
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D (GL_TEXTURE_2D, 0, texFormat, imageWidth, imageHeight, 0, texFormat, GL_UNSIGNED_BYTE, imageData);
    free (imageData);
    return 1;
}

//------------------------------------------------------------------------------

int loadArchive (char* archive, int glbank, unsigned int number)
{
	unsigned int i;
	FILE* file;
	char buf[] = "                    ";
#ifdef __LINUX__
	strcpy (buf, "GFX/");
#else
	strcpy (buf, "GFX\\");
#endif
	strcat (buf, archive);
	strcat (buf, ".gfx");
	file = fopen (buf, "r+bt");
	if (file != NULL)
	{	
		for (i = 0; i < number; i++)
			extractTGA (file, glbank + i);
		fclose (file);
	}
	else
		printf ("failed to open %s\n", buf);
    return 1;
}


int
Image_LoadAndBind (char* name, int id)
{
	image_t* image;
	if ((image = Image_LoadFromTGA (name, 1)) == NULL)
		return 0;
	Image_Bind (image, id);
	MEMFREE (image->data);
	MEMFREE (image);
	return 1;
}
