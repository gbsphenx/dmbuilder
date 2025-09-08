//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Data
//------------------------------------------------------------------------------

#include <data.h>
#include <dm.h>
#include <dms.h>
#include <actuator.h>

extern int SKULLKEEP;
extern int THERONSQUEST;
extern int TELOS;

//------------------------------------------------------------------------------

unsigned int fakeGetType (short* null)
{
	return 0;
}

void fakeSetType (short* null, unsigned int novalue)
{
	;
}

//------------------------------------------------------------------------------

unsigned int
doorGetType (short *door)
{
	return (char) ((door_p) door)->type;
}

void
doorSetType (short *door, unsigned int type)
{
	((door_p) door)->type = type;
}

unsigned int
doorGetOrnate (short *door)
{
	return (char) ((door_p) door)->ornate;
}

void
doorSetOrnate (short *door, unsigned int ornate)
{
	((door_p) door)->ornate = ornate;
}

//------------------------------------------------------------------------------

// ATTENTION: en interne, l'offset est remplacé par le NUMERO du block de texte voulu.
// et pour l'instant, pas de décalage de codon.
/*unsigned short
textGetNumber (short *text)
{
	return (short) ((struct dm_text*) text)->offset;
}

void
textSetNumber (short *text, short number)
{
	((struct dm_text*) text)->offset = number;
}
*/


//------------------------------------------------------------------------------

int
simpleActuatorGetGraphism (short *simpleactuator)
{
	unsigned char igfx = 0;
	simple_actuator_p sact = (simple_actuator_p) simpleactuator;
	igfx = (unsigned char)sact->Graphism;

	return (int) (igfx);
}

//------------------------------------------------------------------------------

unsigned int
actuatorGetType (short *actuator)
{
	return (char) ((actuator_p) (actuator))->type;
}

unsigned int
actuatorGetGraphism (short *actuator)
{
	return (char) ((actuator_effect_p) (actuator+1))->graphism-1;
}

void
actuatorSetGraphism (short *actuator, short type)
{
	((actuator_effect_p) (actuator+1))->graphism = (char) type;
}

short
actuatorGetValue (short *actuator)
{
	return ((actuator_p) (actuator))->value;
}

void
actuatorSetValue (short *actuator, short value)
{
	((actuator_p) (actuator))->value = value;
}


//------------------------------------------------------------------------------

unsigned int
monsterGetType (short *monster)
{
	return ((monster_p) monster)->type;
}

void
monsterSetType (short *monster, unsigned int type)
{
	((monster_p) monster)->type = (unsigned int) type;
}

//------------------------------------------------------------------------------

unsigned int
objectWeaponGetType (short *weapon)
{
	return (unsigned int) ((weapon_p) weapon)->type;
}

void
objectWeaponSetType (short *weapon, unsigned int type)
{
	((weapon_p) weapon)->type = type;
}
//------------------------------------------------------------------------------

unsigned int
objectClothingGetType (short *clothing)
{
	return ((clothing_p) clothing)->type;
}

void
objectClothingSetType (short *clothing, unsigned int type)
{
	((clothing_p) clothing)->type = type;
}
//------------------------------------------------------------------------------

unsigned int
objectPotionGetType (short *potion)
{
	return ((potion_p) potion)->type;
}

void
objectPotionSetType (short *potion, unsigned int type)
{
	((potion_p) potion)->type = type;
}

//------------------------------------------------------------------------------

short
unchest_Chest (chest_p chest)
{
	short chested = chest->chested;
	chest->chested = -2;
	return chested;
}

unsigned int
objectContainerGetType (short *chest)
{
	char type = 0;
	chest_p	xchest;
	if (SKULLKEEP == 0)
		return 0;
	xchest = (chest_p) chest;
	// Magic MAP stored as 0x6002 must give 0x000B
	// 0b011X.0000/0000.001X => 001.011
	type = ((xchest->data1 & 0xE000)>>13) + ((xchest->data1 & 0x000E)>>1)*8; 
//	type = ((chest_p) chest)->type;
	return type;
}

void
objectContainerSetType (short *chest, unsigned int type)
{
	if (SKULLKEEP == 0)
		return;
	((chest_p) chest)->data1 = (unsigned short) ((type & 0x07)<<13) + (((type & 0x00F8)/8)<<1);
}


//------------------------------------------------------------------------------

unsigned int
objectMiscellaneousGetType (short *misc)
{
	return ((misc_p) misc)->type;
}

void
objectMiscellaneousSetType (short *misc, unsigned int type)
{
	((misc_p) misc)->type = type;
}
//------------------------------------------------------------------------------

getObjectType getItemType[16] =
{
	doorGetType, fakeGetType, fakeGetType, fakeGetType,
	monsterGetType, objectWeaponGetType, objectClothingGetType, fakeGetType,
	objectPotionGetType, objectContainerGetType, objectMiscellaneousGetType, fakeGetType,
	fakeGetType, fakeGetType, fakeGetType, fakeGetType
};

setObjectType setItemType[16] =
{
	doorSetType, fakeSetType, fakeSetType, fakeSetType,
	monsterSetType, objectWeaponSetType, objectClothingSetType, fakeSetType,
	objectPotionSetType, objectContainerSetType, objectMiscellaneousSetType, fakeSetType,
	fakeSetType, fakeSetType, fakeSetType, fakeSetType
};