//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Item Data
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <assert.h>

#include <editor.h>
#include <dm.h>
#include <item.h>
#include <data.h>
#include <dms.h>
#include <level.h>
#include <actuator.h>
#include <tile.h>
#include <text.h>


extern int dt_Door_Max;
extern int dt_Monster_Max;
extern int dt_Wall_Max;
extern int dt_Floor_Max;
extern int dt_Ornate_Max;

extern int dt_Weapons_Max;
extern int dt_Clothing_Max;
extern int dt_Misc_Max;
extern int dt_Containers_Max;

/*
 * CATEGORIES:
 *
 * 0	Doors		(4 bytes)
 * 1	Teleports	(6 bytes)
 * 2	Texts		(4 bytes)
 * 3	Activators	(8 bytes)
 * 4	Monsters	(16 bytes)
 * 5	Weapons		(4 bytes)
 * 6	Clothes		(4 bytes)
 * 7	Scrolls		(4 bytes)
 *
 * 8	Potions		(4 bytes)
 * 9	Chests		(8 bytes)
 * 10	Miscellaneous (4 bytes)
 * 11	-
 * 12	-
 * 13	-
 * 14	Missiles	(8 bytes)
 * 15	Clouds		(4 bytes)
 */

size_t itemBytes [16] = {
	4, 6, 4, 8, 
	16, 4, 4, 4,
	4, 8, 4, 0,
	0, 0, 8, 4
};

static size_t itemShorts [16] = {
	2, 3, 2, 4, 
	8, 2, 2, 2,
	2, 4, 2, 0,
	0, 0, 4, 2
};

int nItems[16];	// Number of items per category
int lastItem[16]; // Last item ID attributed for a category
// This allow to use space from previously unused/removed items.
// If last > n, that means at least one item has been removed; then the list has to be scanned again.

static int maxItems[16] = {
	1024, 1024, 1024, 1024,
	1024, 1024, 1024, 1024,
	1024, 1024, 1024, 0,
	0, 0, 60, 50
};

static short nullItem = -2;



int itemDefault[] = {
	0, 0, 0, 0,
	monster_Screamer, weapon_Sword, clothing_MithralAketon, 0,
	potion_EmptyFlask, 0, misc_Apple, 0,
	0, 0, 0, 0
};



// Actuator maxtype is the activation item ID, not the graphism
// (since its different if the actuator is on wall or floor)
//
//static
int maxTypeItems[] = {
	door_Max, 1, 1, 512,
	monster_Max, weapon_Max, clothing_Max, 1,
	potion_Max, 1, misc_Max, 1,
	1, 1, /*MISSILE_MAX*/1, /*CLOUDS_MAX*/1
};
// The 1 here serves to avoid a module 0 which wouldn't work in setItemType

int tlbValidPotions[21] = {
	0, 0, 0, 1, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 1, 1
};
/*
char* txtPotions[21] = {
	"Mon Potion", "Um Potion", "Des Potion", "Ven Potion",
	"Sar Potion", "Zo Potion", "Ros Potion", "Ku Potion",
	"Dane Potion", "Neta Potion", "Anti Venin", "Ma Potion",
	"Ya Potion", "Ee Potion", "Vi Potion", "Water Flask",
	"Kath Bomb", "Pew Bomb", "Ra Bomb", "Ful Bomb",
	"Empty Flask",
};*/


unsigned short* ITEMS[16];
// Principal pointer

//------------------------------------------------------------------------------
//	Item Memory Space Initialisation
//------------------------------------------------------------------------------


static void
itemAllocate ()
{
	int i;
	for (i = 0; i < 16; i++)
		ITEMS[i] = (unsigned short*) calloc (maxItems[i]*itemBytes[i], 1);
}

static void
itemInitObjectNumbers ()
{
	int i;
	for (i = 0; i < 16; i++)
	{
		nItems[i] = 0;
		lastItem[i] = -1;
	}
}

void
itemInitObjectNumbersFromDungeon ()
{
	int i;
	for (i = 0; i < 16; i++)
		nItems[i] = getDungeon()->nObjects[i];
}

void itemFlushData (short* item, size_t bytes)
{
	size_t i;
	*(item) = -1;	// That means this item is not referenced anymore (or doesn't exist anymore)
	for (i = 1; i < (bytes/2); i++)
		*(item + i) = (short) 0x0000;	// Zero init is safer

}

void
startItems ()
{
	int i;
	itemAllocate ();
	itemInitObjectNumbers ();
	for (i = 0; i < 16; i++)
	{
		int j;
		for (j = 0; j < maxItems[i]; j++)
			itemFlushData ((short*)ITEMS[i] + j*itemShorts[i], itemBytes[i]);
	}
}



//------------------------------------------------------------------------------
//	Find an empty place for a new item
//------------------------------------------------------------------------------

static int
itemIsUnused (short* item)
{
	return (*item == (short) -1);
}

static int
searchFreeItemID (char category)
{
	if (nItems[category] < maxItems[category])
	{
		if (lastItem[category]+1 == nItems[category])
		{
			nItems[category] ++;
			lastItem[category] ++;
			return lastItem[category];
		}
		else // lastItem+1 < nItems
		{
			int i;
			short* item = (short*)ITEMS[category];
			for (i = 0; i < maxItems[category]; i++)
			{
				if (itemIsUnused (item))
				{
					nItems[category] ++;
					return i;
				}
				else
					item += itemShorts[category];
			}
		}
	}
	return -1;
}

static int
searchFreeItemIDContext (int iContext, char category)
{
	if (xTblDunSaveContexts[iContext].nItems[category] < maxItems[category])
	{
		if (xTblDunSaveContexts[iContext].lastItem[category]+1 == xTblDunSaveContexts[iContext].nItems[category])
		{
			xTblDunSaveContexts[iContext].nItems[category] ++;
			xTblDunSaveContexts[iContext].lastItem[category] ++;
			return xTblDunSaveContexts[iContext].lastItem[category];
		}
		else // lastItem+1 < nItems
		{
			int i;
			short* item = xTblDunSaveContexts[iContext].xItemsData[category];
			for (i = 0; i < maxItems[category]; i++)
			{
				if (itemIsUnused (item))
				{
					xTblDunSaveContexts[iContext].nItems[category] ++;
					return i;
				}
				else
					item += itemShorts[category];
			}
		}
	}
	return -1;
}

//------------------------------------------------------------------------------
//	Items initialisation
//------------------------------------------------------------------------------

typedef void (*ITEM_INITIALISATION_FUNCTION) (short*, int);

static void
do_nothing (short *item, int type)
{;}

static void
initTeleport (short *item, int type)
{
	teleport_p teleport = (teleport_p) item;
	teleport->ldest = getEditCursor (cursor_L);
	teleport->xdest = getEditCursor (cursor_X);
	teleport->ydest = getEditCursor (cursor_Y);
	teleport->sound = 0;
	teleport->absolute = 0;
	teleport->rotation = 0;
	teleport->scope = 0;
	getCurrentTile()->type = tile_Teleport;
	((tile_teleport_p)getCurrentTile ())->open = 1;
}

static void
initText (short *item, int type)
{
	text_p text = (text_p) item;
	text->offset = 0;
	text->silent = 0;
	text->flag2 = 0;
	text->flag3 = 0;
}

static void
initScroll (short *item, int type)
{
	scroll_p scroll = (scroll_p) item;
	scroll->offset = 0;
}

static void
initActuator (short *item, int type)
{
	actuator_target_p target = (actuator_target_p) item+2;
	actuator_p xActuator = (actuator_p) item;

	target->xdest = getEditCursor (cursor_X);
	target->ydest = getEditCursor (cursor_Y);
	xActuator->type = type;
	xActuator->value = 0;

}

static void
initMonster (short *item, int type)
{
	size_t i;
	monster_p monster = (monster_p) item;
	level_p level = (level_p) &(getLevels()[getEditCursor (cursor_L)]);

	monster->chested = -2;
	for (i = 0; i < 4; i ++)
		monster->health[i] = 0;
	monster->number = 0; // That mean 1 monster on the square
	monster->type = monster_Screamer;
	monster->unk1 = 4; // That looks good :/ but I don't know
	monster->position = (char) 0xFF; //defaut

	//--- Some improvement, make this new monster as the last known ID within monster list
	if (level->header.nMonsters > 0)
		monster->type = level->monsters[level->header.nMonsters-1];
	//--- This is for DM2, so that health is not zero and SAVEGAME will succeed to LOAD
	if (SKULLKEEP) 
	{
		for (i = 0; i < 4; i ++)
			monster->health[i] = 1;
	}
}

static void
initWeapon (short *item, int type)
{
	weapon_p weapon = (weapon_p) item;
	weapon->broken = 0;
	weapon->charges = 0;
	weapon->cursed = 0;
	weapon->poisoned = 0;
	weapon->type = weapon_Sword;
	weapon->unk1 = 0; 
	weapon->keep = 1;
}

static void
initClothes (short *item, int type)
{
	clothing_p clothes = (clothing_p) item;
	clothes->broken = 0;
	clothes->cursed = 0;
	clothes->dump = 0;
	clothes->type = clothing_LeatherJerkin;
	clothes->keep = 1;
	clothes->unk2 = 0;
}

static void
initPotion (short *item, int type)
{
	potion_p potion = (potion_p) item;
	potion->power = 0;
	potion->type = potion_EmptyFlask;
	potion->unk1 = 0;
	potion->keep = 1;
}

static void
initChest (short *item, int type)
{
	chest_p chest = (chest_p) item;
	chest->chested = -2;
	/*
	chest->unk2[0] = 0; // Sometimes f9, ou 1 ou 0
	chest->unk2[1] = 0;*/
//	chest->type = 0;
//	chest->unk1 = 0;
	chest->unk2 = 0;
}

static void
initMisc (short *item, int type)
{
	misc_p misc = (misc_p) item;
	misc->capacity = 0;
	misc->type = misc_Apple;
	misc->unk1 = 0;
	misc->keep = 1;
}

static ITEM_INITIALISATION_FUNCTION initializeItem[] =
{
	do_nothing, initTeleport, initText, initActuator,
	initMonster, initWeapon, initClothes, initScroll,
	initPotion, initChest, initMisc, do_nothing,
	do_nothing, do_nothing, do_nothing, do_nothing
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

static short
newItem (int id, char category, int type)
{
	short *item = (short*)ITEMS[category] + id*itemShorts[category];
	assert (id > -1 && id < maxItems[category]);

//	if (type == -1)	// not anymore, use the init instead
//		type = itemDefault[category];
	*item = -2;
//	itemSetType[category] (item+1, (short) type);
	initializeItem[category] ((short*)(item+1), type);

//	if (category == category_Door)
//		putDoorTile ();
	//optionalSet[category] ();
	return makeReference (category, 0, (short) id);
}

static short
newItemContext (int iContext, int id, char category, int type)
{
	short *item = xTblDunSaveContexts[iContext].xItemsData[category] + id*itemShorts[category];
	assert (id > -1 && id < maxItems[category]);

//	if (type == -1)	// not anymore, use the init instead
//		type = itemDefault[category];
	*item = -2;
//	itemSetType[category] (item+1, (short) type);
	initializeItem[category] ((short*)(item+1), type);

//	if (category == category_Door)
//		putDoorTile ();
	//optionalSet[category] ();
	return makeReference (category, 0, (short) id);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



short
placeNewItem (char category, int type)
{
	int id;
	assert (category > -1 && category < 16);

	id = searchFreeItemID (category);

	if (id != -1)
		return newItem (id, category, type);
	return nullItem;
}


short
placeNewItemContext (int iContext, char category, int type)
{
	int id;
	assert (category > -1 && category < 16);

	id = searchFreeItemIDContext (iContext, category);

	if (id != -1)
		return newItemContext (iContext, id, category, type);
	return nullItem;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
reference_p
getNextItem (reference_p reference)
{
	short **ref = (short **) &reference;
	if (**ref == -2 || **ref == -1)
		return (reference_p) &nullItem;
	return (reference_p) (ITEMS[reference->category] + reference->id*itemShorts[reference->category]);
}


short*
getItem (reference_p reference)
{
	short **ref = (short **) &reference;
	if (**ref == -2 || **ref == -1)
		return &nullItem;
	return ((short*)(ITEMS[reference->category] + reference->id*itemShorts[reference->category]) + 1);
}


short*
getItemContext (int iContext, reference_p reference)
{
	short **ref = (short **) &reference;
	if (**ref == -2 || **ref == -1)
		return &nullItem;
	return ((xTblDunSaveContexts[iContext].xItemsData[reference->category] + reference->id*itemShorts[reference->category]) + 1);
}

short*
getReferenceItem (reference_p reference)
{
	short **ref = (short **) &reference;
	if (**ref == -2 || **ref == -1)
		return &nullItem;
	return ((short*)ITEMS[reference->category] + reference->id*itemShorts[reference->category]);
}

void
deleteItem (reference_p refp)
{
	short *item;
	short **ref = (short**) &refp;
	if ((**ref == -2) || (**ref == -1))
		perror ("What are we deleting ?!");
	item = getReferenceItem (refp);
	*item = -1;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void
updateMaxTypeItems ()
{
	if (SKULLKEEP == 1 && TELOS == 0)
	{
		maxTypeItems[0] = dt_Door_Max;
		maxTypeItems[1] = 1;
		maxTypeItems[2] = 1;
		maxTypeItems[3] = 512;
		maxTypeItems[4] = dt_Monster_Max; //monster_Max;
		maxTypeItems[5] = dt_Weapons_Max;
		maxTypeItems[6] = dt_Clothing_Max;
		maxTypeItems[7] = 1;
		maxTypeItems[8] = potion_Max;
		maxTypeItems[9] = dt_Containers_Max; // containers max
		maxTypeItems[10] = dt_Misc_Max;
		maxTypeItems[11] = 1;
		maxTypeItems[12] = 1;
		maxTypeItems[13] = 1;
		maxTypeItems[14] = 1;
		maxTypeItems[15] = 1;
	}
	else if (TELOS == 1)
	{
		maxTypeItems[0] = 16;
		maxTypeItems[1] = 1;
		maxTypeItems[2] = 1;
		maxTypeItems[3] = 512;
		maxTypeItems[4] = 254; //monster_Max;
		maxTypeItems[5] = 254;	// weapons
		maxTypeItems[6] = 254;	// clothings
		maxTypeItems[7] = 1;
		maxTypeItems[8] = potion_Max;	// potions
		maxTypeItems[9] = dt_Containers_Max; // containers max
		maxTypeItems[10] = 254;	// misc
		maxTypeItems[11] = 1;
		maxTypeItems[12] = 1;
		maxTypeItems[13] = 1;
		maxTypeItems[14] = 1;
		maxTypeItems[15] = 1;
	}
	else if (SKULLKEEP == 0)
	{
		maxTypeItems[0] = door_Max;
		maxTypeItems[1] = 1;
		maxTypeItems[2] = 1;
		maxTypeItems[3] = 512;
		maxTypeItems[4] = monster_Max;
		maxTypeItems[5] = weapon_Max;
		maxTypeItems[6] = clothing_Max;
		maxTypeItems[7] = 1;
		maxTypeItems[8] = potion_Max;
		maxTypeItems[9] = 1;
		maxTypeItems[10] = misc_Max;
		maxTypeItems[11] = 1;
		maxTypeItems[12] = 1;
		maxTypeItems[13] = 1;
		maxTypeItems[14] = 1;
		maxTypeItems[15] = 1;
	}
}


int
cycleRegularItem (reference_p refp, int step)
{
	short* item = NULL;
	int iMaxTypeModulo = 16;
	int iPreviousValue = 0;
	item = getItem (refp);

	updateMaxTypeItems ();

	
	iMaxTypeModulo = maxTypeItems[refp->category];
	iPreviousValue = (int) getItemType[refp->category](item);

	setItemType[refp->category](item, (unsigned char) ((iPreviousValue + step + iMaxTypeModulo)%iMaxTypeModulo));
	return 1;
}

int
cycleLevelDependentItem (reference_p refp, int step)
{
	switch (refp->category)
	{
	case category_Door:
		{
			char ornatemax = getLevels()[getEditCursor(cursor_L)].header.nOrnates;
			door_p door = (door_p) getItem (refp);
			doorSetOrnate ((short*) door, (char) ((door->ornate+ornatemax+1+step)%(ornatemax+1)));
		}
		break;
	case category_Actuator: /* cycle through graphics */
		{
			char max;
			level_p level = &getLevels()[getEditCursor(cursor_L)];
			actuator_effect_p effect = (actuator_effect_p) getItem (refp) + 1;
			if (getCurrentTile ()->type != tile_Wall)
				 max = level->header.nFloors + 1;
			else max = level->header.nWalls + 1;
			effect->graphism = (effect->graphism + max + step)%max;
		}
	}
	return 1;
}

int
cycleText (reference_p refp, int step)
{
	if (refp->category == category_Text)
	{
		text_p text = (text_p) getItem (refp);
		if (getTextsNumber())
			text->offset = (text->offset + step + getTextsNumber())%getTextsNumber();
		else
			text->offset = 0;
	}
	return 1;
}


int
cycleSimpleActuator (reference_p refp, int step)
{
	if (refp->category == category_Text)
	{
		simple_actuator_p sact = (simple_actuator_p) getItem (refp);

		sact->Graphism = (sact->Graphism + step + 256)%256;
	}
	return 1;
}

int
cycleTextOrSimpleActuator (reference_p refp, int step)
{
	if (refp->category == category_Text)
	{
		text_p text = (text_p) getItem (refp);
		simple_actuator_p sact = (simple_actuator_p) text;

		if (sact->Mode == 0)
			cycleText(refp, step);
		else
			cycleSimpleActuator(refp, step);
	}
	return 1;
}

int
cycleScroll (reference_p refp, int step)
{
	int iUseGDATText = 0;
	if (refp->category == category_Scroll)
	{
		scroll_p scroll = (scroll_p) getItem (refp);
		scroll2_p scroll2 = (scroll2_p) getItem (refp);

		if (scroll2->reftxt > 0)
			iUseGDATText = 1;

		if (iUseGDATText == 0) // DM1 mode, offset text
		{
			if (getTextsNumber())
				scroll->offset = (scroll->offset + step + getTextsNumber())%getTextsNumber();
			else
				scroll->offset = 0;
		}
		else if (iUseGDATText == 1)
			scroll2->type = ((scroll2->type + step)%64);
	}
	return 1;
}

int
cycleActivatorType (reference_p refp, int step)
{
	if (refp->category == category_Actuator)
	{
		actuator_p act = (actuator_p) getItem (refp);
		act->type += step;
	/* not yet verification */
		if (!SKULLKEEP)
		{
			if (act->type == 0x7E) 
				act->type = actuator_end_pad;
			else if (act->type == actuator_end_pad+1) 
				act->type = 0x7F;
		}
		else
		{
			if (act->type == actuator_dm2_wall_champion_cell-1) // 0x7D
				act->type = actuator_dm2_last;
			else if (act->type == actuator_dm2_last+1) 
				act->type = actuator_dm2_wall_champion_cell;
		}
	}
	return 1;
}

int
cycleActivatorGeneral (reference_p refp, int step)
{
	int iItemActuatorsMax = item_actuators_Max;
	if (SKULLKEEP)
		iItemActuatorsMax = 512;
	if (!isSecondFunction ())
		cycleLevelDependentItem (refp, step);
	else
	{
		int wall = (getCurrentTile ()->type == tile_Wall)?1:0;
		if (wall)
		{
			actuator_p act = (actuator_p) getItem (refp);
			switch (act->type)
			{
			case actuator_wall_champion_mirror /*actuator_wall_champion_mirror*/:
				act->value = (act->value + portrait_Max + step)%portrait_Max; break;
			case actuator_wall_alcove_item:
			case actuator_wall_item:
			case actuator_wall_item_eater:
			case actuator_wall_item_eater_toggler:
				/*actuator_wall_mouse_toggler = 12,*/
			case actuator_wall_item_removal_toggler:
			case actuator_wall_general_shooter:
			case actuator_wall_double_general_shooter:
			case actuator_wall_item_exchange:
			case actuator_wall_item_eater_disappear:
							act->value = (act->value + iItemActuatorsMax + step)%iItemActuatorsMax; break;
			case actuator_wall_spell_shooter:
			case actuator_wall_double_spell_shooter:
				act->value = (act->value + spell_Max + step)%spell_Max; break;
			default:
				act->value += step; break;
			}
		}
		else
		{
			actuator_p act = (actuator_p) getItem (refp);
			switch (act->type)
			{
				case actuator_floor_pad_item:
				case actuator_floor_carried_item:
					act->value = (act->value + iItemActuatorsMax + step)%iItemActuatorsMax; break;
				case actuator_floor_monster_generator:
					act->value = (act->value + monster_Max + step)%monster_Max; break;
				default:
				act->value += step; break;
			}
		}
	}
	return 1;
}

int
cyclePotion (reference_p refp, int step)
{
	short* item;
	potion_p potion;
	item = getItem (refp);
	potion = (potion_p) item;
	setItemType[refp->category](item, (char) ((getItemType[refp->category](item) + step + 21)%21));
	if (!tlbValidPotions[potion->type])
		cyclePotion (refp, step);
	return 1;
}


int
cycleItem (reference_p refp, int step)
{
	switch (refp->category)
	{
	case category_Monster:
	case category_Weapon:
	case category_Clothing:
	case category_Miscs:
		cycleRegularItem (refp, step);
		break;
	case category_Chest:
		if (SKULLKEEP)
			cycleRegularItem (refp, step);
		break;
	case category_Potion:
		{
			cyclePotion (refp, step);
		}
		break;
	case category_Door:
		cycleLevelDependentItem (refp, step);
		break;
	case category_Actuator:
		cycleActivatorGeneral (refp, step);
		break;
	case category_Text:
		cycleTextOrSimpleActuator (refp, step);
		break;
	case category_Scroll:
		cycleScroll (refp, step);
		break;
	}
	return 1;
}

/*
int
getCurrentItemType ()
{
	short *item;
	struct dm_reference ref;
	
	if (current_item_ref != -2 && current_item_ref != -1)
	{
		ref = *((struct dm_reference*) &current_item_ref);
		item = getItem (current_item_ref);
		return (int) itemType[ref.category](item+1);
	}
	else
		perror ("getCurrentItemType.");
	return -1;
}
*/
//------------------------------------------------------------------------------

unsigned int
getNumber (char category)
{
	return nItems[category];
}
