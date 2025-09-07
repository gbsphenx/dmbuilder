//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Object Reference / Stacks
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <assert.h>
#include <reference.h>
#include <item.h>
#include <text.h>
#include <editor.h>
#include <level.h>
#include <dm.h>
#include <actuator.h>

#include <tile.h>

// 32*32*16*2 = 2^15 = 32768 bytes
static const unsigned int max_ref = 32*32*MAX_LEVELS;

static const short nullReference = REFERENCE_NONE;
short *floors;
dm_reference stack[65];

dm_reference stackNum[8][65];	// Allow 8 different stacks for special operations (copies)

static int stack_size = 0;

/* this reference is used to give the currently selected item in stack */
static reference_p current = (reference_p) &nullReference;



//------------------------------------------------------------------------------
// switch context
short* Ref_GetFloorsPointer()
{
	assert (floors != NULL);
	return floors;
}

void Ref_SetFloorsPointer(short* pFloors)
{
	assert (pFloors != NULL);
	floors = pFloors;
}

//------------------------------------------------------------------------------
void
flushStack ()
{
	size_t i;
	for (i = 0; i < 65; i++)
		((short*) stack)[i] = nullReference;
	stack_size = 0;
}

void
flushStackNum (int stacknum)
{
	size_t i;
	for (i = 0; i < 65; i++)
		((short*) stackNum[stacknum])[i] = nullReference;
	stack_size = 0;
}

//------------------------------------------------------------------------------

void
startGroundReferences ()
{
	size_t i;
	floors = (short*) calloc (max_ref, sizeof (short));
	for (i = 0; i < max_ref; i++)
		floors[i] = REFERENCE_NONE;
	flushStack ();
	
	for (i = 0; i < 8; i++)
		flushStackNum (i);
}

void
startGroundReferencesKeepStacks () // for multi-dungeon work
{
	size_t i;
	floors = (short*) calloc (max_ref, sizeof (short));
	for (i = 0; i < max_ref; i++)
		floors[i] = REFERENCE_NONE;
	flushStack ();
	// no flush on StackNum

}

reference_p
getGroundReference (char x, char y, char map)
{
	if (map < 0)
		return (reference_p) &nullReference;
	return (reference_p) &(floors[(size_t) x+(32*(size_t) y)+(1024*(size_t) map)]);
}

void
setGroundReference (char x, char y, char map, short reference)
{
	if (floors != NULL)
		floors[(size_t) x+(32*(size_t) y)+(1024*(size_t) map)] = reference;
}

//------------------------------------------------------------------------------

void
updateSelectedItem ()
{	/* stack should already the correct stack */
	current = (reference_p) &stack[(unsigned int) (getEditCursor (cursor_Stack))];
	/* make sure that the StackCursor is [0-64]*/
}

reference_p
getCurrentItemReference ()
{
	return current;
}

reference_p
getStackReference (char position)
{
	return (reference_p) &stack[position];
}

short *
getCurrentSelectedItem ()
{
	reference_p selected = (getStackReference (getEditCursor (cursor_Stack)));
	return getItem (selected);
}
				
//------------------------------------------------------------------------------
// STACK OPERATION
//------------------------------------------------------------------------------

int
getStackSize ()
{
	return stack_size;
}

void
importStack (char x, char y, char map)
{
	size_t i = 0;
	reference_p currentref = NULL;
	short **ref = NULL;
	
	currentref = getGroundReference (x, y, map);
	ref = (short **) &currentref;

	flushStack (); /* stack_size should be set to 0 */
	
	while (**ref != REFERENCE_NONE && **ref != REFERENCE_UNUSED && i < 64)
	{
		((short*) stack)[stack_size++] = **ref;
		currentref = getNextItem (currentref);
	}

	updateSelectedItem ();
}


void
importStackNum (int stacknum, char x, char y, char map)
{
	size_t i = 0;
	reference_p currentref = getGroundReference (x, y, map);
	short **ref = (short **) &currentref;
	
	flushStackNum (stacknum); /* stack_size should be set to 0 */
	
	while (**ref != REFERENCE_NONE && **ref != REFERENCE_UNUSED && i < 64)
	{
		((short*) stackNum[stacknum])[stack_size++] = **ref;
		currentref = getNextItem (currentref);
	}

	//updateSelectedItem ();
}

void
exportStack (char x, char y, char map)
{
	size_t i = 0;
	reference_p currentref = (reference_p) &stack[0];
	short **ref = (short **) &currentref;
	short *itemref;

	setGroundReference (x, y, map, **ref);

	/* index i for security, should be removed */
	while (**ref != REFERENCE_NONE && i < 65)
	{
		getTile(x, y, map)->object = 1;
		itemref = getReferenceItem (currentref++);
		*itemref = **ref;
		i++;
	}
}

void
exportStackNum (int stacknum, char x, char y, char map)
{
	size_t i = 0;
	reference_p currentref = (reference_p) &stackNum[stacknum][0];
	short **ref = (short **) &currentref;
	short *itemref;

	setGroundReference (x, y, map, **ref);

	/* index i for security, should be removed */
	while (**ref != REFERENCE_NONE && i < 65)
	{
		getTile(x, y, map)->object = 1;
		itemref = getReferenceItem (currentref++);
		*itemref = **ref;
		i++;
	}
}

void
copyStackNum (int stacknumsource, int stacknumdest)
{
	size_t i = 0;
	size_t idest = 0;
	dm_reference xRefNewItem;
	short* xItem = NULL;

	reference_p currentref = (reference_p) &stackNum[stacknumsource][0];
	short **ref = (short **) &currentref;
	short *itemref;
	reference_p pNewItemRef = (reference_p) &xRefNewItem;

	xRefNewItem.raw = -2;


	flushStackNum(stacknumdest);

	while (**ref != REFERENCE_NONE && i < 65)
	{
		if (currentref->category == category_Door)
		{
			door_p xDoor = NULL;
			xItem = getItem(currentref);
			xDoor = (door_p) xItem;

			xRefNewItem.raw = -2;
			xRefNewItem.raw = placeNewItem (category_Door, xDoor->type);
			if (xRefNewItem.raw != -2)
			{	
				short* xNewItem = getItem(&xRefNewItem);
				door_p xNewDoor = (door_p) xNewItem;
				//push_reference_in_stack_facing (xRefNewItem, x, y, m, facing);
				xNewDoor->type = xDoor->type;
				xNewDoor->ornate = xDoor->ornate;
				xNewDoor->opensUp = xDoor->opensUp;
				xNewDoor->button = xDoor->button;
				xNewDoor->destroyable = xDoor->destroyable;
				xNewDoor->bashable = xDoor->bashable;
				xNewDoor->unk1 = xDoor->unk1;

				pNewItemRef->position = currentref->position;
			}
		}
		else if (currentref->category == category_Text)
		{
			text_p xText = NULL;
			xItem = getItem(currentref);
			xText = (text_p) xItem;

			xRefNewItem.raw = -2;
			xRefNewItem.raw = placeNewItem (category_Text, xText->offset);
			if (xRefNewItem.raw != -2)
			{	
				short* xNewItem = getItem(&xRefNewItem);
				text_p xNewText = (text_p) xNewItem;
				//push_reference_in_stack_facing (xRefNewItem, x, y, m, facing);
				xNewText->silent = xText->silent;
				xNewText->flag2 = xText->flag2;
				xNewText->flag3 = xText->flag3;
				xNewText->offset = xText->offset;

				pNewItemRef->position = currentref->position;
			}
		}
		else if (currentref->category == category_Actuator)
		{
			actuator_p xActuator = NULL;
			actuator_effect_p xActuatorEffect = NULL;
			actuator_target_p xActuatorTarget = NULL;
			xItem = getItem(currentref);
			xActuator = (actuator_p) xItem;
			xActuatorEffect = (actuator_effect_p) xItem+1;
			xActuatorTarget = (actuator_target_p) xItem+2;

			xRefNewItem.raw = -2;
			xRefNewItem.raw = placeNewItem (category_Actuator, xActuator->type);
			if (xRefNewItem.raw != -2)
			{	
				short* xNewItem = getItem(&xRefNewItem);
				actuator_p xNewActuator = (actuator_p) xNewItem;
				actuator_effect_p xNewActuatorEffect = (actuator_effect_p) xNewItem+1;
				actuator_target_p xNewActuatorTarget = (actuator_target_p) xNewItem+2;

				xNewActuator->type = xActuator->type;
				xNewActuator->value = xActuator->value;

				xNewActuatorEffect->bit1 = xActuatorEffect->bit1;
				xNewActuatorEffect->bit2 = xActuatorEffect->bit2;
				xNewActuatorEffect->once = xActuatorEffect->once;
				xNewActuatorEffect->effect = xActuatorEffect->effect;
				xNewActuatorEffect->sound = xActuatorEffect->sound;
				xNewActuatorEffect->delay = xActuatorEffect->delay;
				xNewActuatorEffect->inactive = xActuatorEffect->inactive;
				xNewActuatorEffect->graphism = xActuatorEffect->graphism;

				xNewActuatorTarget->bit1 = xActuatorTarget->bit1;
				xNewActuatorTarget->bit2 = xActuatorTarget->bit2;
				xNewActuatorTarget->bit3 = xActuatorTarget->bit3;
				xNewActuatorTarget->bit4 = xActuatorTarget->bit4;
				xNewActuatorTarget->facing = xActuatorTarget->facing;
				xNewActuatorTarget->xdest = xActuatorTarget->xdest;
				xNewActuatorTarget->ydest = xActuatorTarget->ydest;
						
				pNewItemRef->position = currentref->position;
			}
		}
		else if (currentref->category == category_Monster)
		{
			monster_p xCreature = NULL;
			xItem = getItem(currentref);
			xCreature = (monster_p) xItem;

			xRefNewItem.raw = -2;
			xRefNewItem.raw = placeNewItem (category_Monster, xCreature->type);
			if (xRefNewItem.raw != -2)
			{	
				short* xNewItem = getItem(&xRefNewItem);
				monster_p xNewCreature = (monster_p) xNewItem;
				//push_reference_in_stack_facing (xRefNewItem, x, y, m, facing);
				xNewCreature->type = xCreature->type;
				xNewCreature->health[0] = xCreature->health[0];
				xNewCreature->health[1] = xCreature->health[1];
				xNewCreature->health[2] = xCreature->health[2];
				xNewCreature->health[3] = xCreature->health[3];
				xNewCreature->number = xCreature->number;
				xNewCreature->chested = -2;	// 0xFFFE
				xNewCreature->position = xCreature->position;

				pNewItemRef->position = currentref->position;
			}
		}
		//--- Put itemref into dest stack
		stackNum[stacknumdest][idest++] = *((dm_reference*)&xRefNewItem);

		itemref = getReferenceItem (currentref++);
		*itemref = **ref;
		i++;
	}
}



void
copyStackNumContexts (int iContextSource, int stacknumsource, int iContextDest, int stacknumdest)
{
	size_t i = 0;
	size_t idest = 0;
	dm_reference xRefNewItem;
	short* xItem = NULL;

	reference_p currentref = (reference_p) &stackNum[stacknumsource][0];
	short **ref = (short **) &currentref;
	short *itemref;

	reference_p pNewItemRef = &xRefNewItem;

	xRefNewItem.raw = -2;

	flushStackNum(stacknumdest);

	while (**ref != REFERENCE_NONE && i < 65)
	{
		if (currentref->category == category_Door)
		{
			door_p xDoor = NULL;
			xItem = getItemContext(iContextSource, currentref);
			xDoor = (door_p) xItem;

			xRefNewItem.raw = -2;
			xRefNewItem.raw = placeNewItemContext (iContextDest, category_Door, xDoor->type);
			if (xRefNewItem.raw != -2)
			{	
				short* xNewItem = getItemContext(iContextDest, &xRefNewItem);
				door_p xNewDoor = (door_p) xNewItem;
				//push_reference_in_stack_facing (xRefNewItem, x, y, m, facing);
				xNewDoor->type = xDoor->type;
				xNewDoor->ornate = xDoor->ornate;
				xNewDoor->opensUp = xDoor->opensUp;
				xNewDoor->button = xDoor->button;
				xNewDoor->destroyable = xDoor->destroyable;
				xNewDoor->bashable = xDoor->bashable;
				xNewDoor->unk1 = xDoor->unk1;

				pNewItemRef->position = currentref->position;
			}
		}
		else if (currentref->category == category_Text)
		{
			text_p xText = NULL;
			xItem = getItemContext(iContextSource, currentref);
			xText = (text_p) xItem;

			xRefNewItem.raw = -2;
			xRefNewItem.raw = placeNewItemContext (iContextDest, category_Text, xText->offset);
			if (xRefNewItem.raw != -2)
			{	
				int iNewTextNum = 0;
				short* xNewItem = getItemContext(iContextDest, &xRefNewItem);
				text_p xNewText = (text_p) xNewItem;
				//push_reference_in_stack_facing (xRefNewItem, x, y, m, facing);
				xNewText->silent = xText->silent;
				xNewText->flag2 = xText->flag2;
				xNewText->flag3 = xText->flag3;

				//--- Copy original text, then get the new offset
				iNewTextNum = addTextContext(iContextDest, getTextContext(iContextSource, xText->offset));
				xNewText->offset = iNewTextNum;

				pNewItemRef->position = currentref->position;
			}
		}
		else if (currentref->category == category_Actuator)
		{
			actuator_p xActuator = NULL;
			actuator_effect_p xActuatorEffect = NULL;
			actuator_target_p xActuatorTarget = NULL;
			xItem = getItemContext(iContextSource, currentref);
			xActuator = (actuator_p) xItem;
			xActuatorEffect = (actuator_effect_p) xItem+1;
			xActuatorTarget = (actuator_target_p) xItem+2;

			xRefNewItem.raw = -2;
			xRefNewItem.raw = placeNewItemContext (iContextDest, category_Actuator, xActuator->type);
			if (xRefNewItem.raw != -2)
			{	
				short* xNewItem = getItemContext(iContextDest, &xRefNewItem);
				actuator_p xNewActuator = (actuator_p) xNewItem;
				actuator_effect_p xNewActuatorEffect = (actuator_effect_p) xNewItem+1;
				actuator_target_p xNewActuatorTarget = (actuator_target_p) xNewItem+2;

				xNewActuator->type = xActuator->type;
				xNewActuator->value = xActuator->value;

				xNewActuatorEffect->bit1 = xActuatorEffect->bit1;
				xNewActuatorEffect->bit2 = xActuatorEffect->bit2;
				xNewActuatorEffect->once = xActuatorEffect->once;
				xNewActuatorEffect->effect = xActuatorEffect->effect;
				xNewActuatorEffect->sound = xActuatorEffect->sound;
				xNewActuatorEffect->delay = xActuatorEffect->delay;
				xNewActuatorEffect->inactive = xActuatorEffect->inactive;
				xNewActuatorEffect->graphism = xActuatorEffect->graphism;

				xNewActuatorTarget->bit1 = xActuatorTarget->bit1;
				xNewActuatorTarget->bit2 = xActuatorTarget->bit2;
				xNewActuatorTarget->bit3 = xActuatorTarget->bit3;
				xNewActuatorTarget->bit4 = xActuatorTarget->bit4;
				xNewActuatorTarget->facing = xActuatorTarget->facing;
				xNewActuatorTarget->xdest = xActuatorTarget->xdest;
				xNewActuatorTarget->ydest = xActuatorTarget->ydest;
						
				pNewItemRef->position = currentref->position;
			}
		}
		else if (currentref->category == category_Monster)
		{
			monster_p xCreature = NULL;
			xItem = getItemContext(iContextSource, currentref);
			xCreature = (monster_p) xItem;

			xRefNewItem.raw = -2;
			xRefNewItem.raw = placeNewItemContext (iContextDest, category_Monster, xCreature->type);
			if (xRefNewItem.raw != -2)
			{	
				short* xNewItem = getItemContext(iContextDest, &xRefNewItem);
				monster_p xNewCreature = (monster_p) xNewItem;
				//push_reference_in_stack_facing (xRefNewItem, x, y, m, facing);
				xNewCreature->type = xCreature->type;
				xNewCreature->health[0] = xCreature->health[0];
				xNewCreature->health[1] = xCreature->health[1];
				xNewCreature->health[2] = xCreature->health[2];
				xNewCreature->health[3] = xCreature->health[3];
				xNewCreature->number = xCreature->number;
				xNewCreature->chested = -2;	// 0xFFFE
				xNewCreature->position = xCreature->position;

				pNewItemRef->position = currentref->position;
			}
		}
		//--- Put itemref into dest stack
		stackNum[stacknumdest][idest++] = *((dm_reference*)&xRefNewItem);

		itemref = getReferenceItem (currentref++);
		*itemref = **ref;
		i++;
	}
}








void
removeCategoryFromStack (int category)
{
	size_t i = 0;
	reference_p currentref = (reference_p) REFERENCE_NONE;

	for (i = 0; i < 65; i++)
	{
		currentref = (reference_p) &stack[i];
		if (*((short*)currentref) != REFERENCE_NONE && *((short*)currentref) != REFERENCE_UNUSED && currentref->category == category)
		{
			*((short*)&stack[i]) = REFERENCE_NONE;
		}
	}
}

void
compressStack()
{
	;
}

void
exportCurrentStack ()
{
	exportStack (getEditCursor (cursor_X), getEditCursor (cursor_Y), getEditCursor (cursor_L));
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

void
referenceID (reference_p refp, int n)
{
	refp->id += n;
	exportCurrentStack ();
}

//------------------------------------------------------------------------------


void
pushReferenceCurrentStack (short reference)
{
	assert (stack_size < 64);
	((short*) stack)[stack_size++] = reference;
}

void
push_reference_in_stack (short reference, char x, char y, char l)
{
	reference_p curref = (reference_p) getGroundReference (x, y, l);
	short **ref = (short **) &curref;
	if (**ref == -2)
	{	**ref = reference; return;}
	while (**ref != -2)
		curref = (reference_p) getReferenceItem (curref);
	**ref = reference;
	
}

void
push_reference_in_stack_facing (short reference, char x, char y, char l, char facing)
{
	reference_p curref = (reference_p) getGroundReference (x, y, l);
	reference_p xRefObject = (reference_p) (&reference);
	short **ref = (short **) &curref;

	if (*((short*)xRefObject) != (short)-2)
	{
		xRefObject->position = facing;
	}

	{	
		tile_p tile = getTile (x, y, l);
		tile->object = 1;
	}

	if (**ref == -2)
	{
		**ref = reference;
		return;
	}
	while (**ref != -2)
	{
		curref = (reference_p) getReferenceItem (curref);
	}
	**ref = reference;
	
}


void
turnItemFacing (reference_p reference)
{
	short **ref = (short **) &reference;
	if (**ref != REFERENCE_NONE /*&& stack_cursor >= 0*/)
	{
		reference->position++;
		exportCurrentStack ();
	}
}

int
stackSwitch (size_t a, size_t b)
{
	dm_reference tmpref;
	if (b >= 0 && b < (unsigned int) stack_size) 
	{
		tmpref = stack[b];
		stack[b] = stack[a];
		stack[a] = tmpref;
		exportCurrentStack ();
		return 1;
	}
	else return 0;
}


void
removeStackItem (size_t i)
{
	//assert (i >= 0 && i < 65);
	//assert (i < (unsigned int) stack_size);
	if (i < 0) return;
	if (*((short*) getCurrentItemReference ()) == -2) return;

	deleteItem (&(stack[i]));
	for (; i < (unsigned int) stack_size; i++)
	{
		stack[i] = stack[i+1];
	}
	*((short*) &stack[i]) = -2;
	stack_size --;
	exportCurrentStack ();
	if (stack_size == 0)
	{	
		tile_p tile = getCurrentTile ();
		getCurrentTile()->object = 0;
		if (tile->type == 4 || tile->type == 5)
		{	tile->type = 1;
			tile->function = 0;
		}
	}
	if (getEditCursor (cursor_Stack) >= stack_size)
		setEditCursor (cursor_Stack, (char) (stack_size-1));
}


void
removeStackItemIfCategory (size_t i, int iCategory)
{
	reference_p ref;

	if (i < 0) return;
	if (*((short*) getCurrentItemReference ()) == -2) return;

	ref = getCurrentItemReference ();
	if (ref->category != iCategory)
		return;

	deleteItem (&(stack[i]));
	for (; i < (unsigned int) stack_size; i++)
	{
		stack[i] = stack[i+1];
	}
	*((short*) &stack[i]) = -2;
	stack_size --;
	exportCurrentStack ();
	if (stack_size == 0)
	{	
		tile_p tile = getCurrentTile ();
		getCurrentTile()->object = 0;
		if (tile->type == 4 || tile->type == 5)
		{	tile->type = 1;
			tile->function = 0;
		}
	}
	if (getEditCursor (cursor_Stack) >= stack_size)
		setEditCursor (cursor_Stack, (char) (stack_size-1));
}



short
makeReference (char category, char position, short id)
{
	dm_reference reference;
	assert (category >= 0 && category < 16);
	assert (position >= 0 && position < 4);
	assert (id >= 0 && id < 1024);
	
	reference.category = category;
	reference.position = position;
	reference.id = id;
	
	return *((short*) &reference);
}

