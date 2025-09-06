//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Load Graphics Sets
//------------------------------------------------------------------------------

#include <tga.h>
#include <dm.h>
#include <gui.h>
#include <string.h>
#include <stdlib.h>

#include <memory.h>
#include <image.h>

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
	Graphics_LoadSkullkeepImagesSet(name, gl_StaticSkullkeep + category*0x100, 16, 16);
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

//..............................................................................
//..............................................................................
//..............................................................................
//..............................................................................

}