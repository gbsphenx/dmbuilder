//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Item Data
//------------------------------------------------------------------------------

#ifndef ITEM_H
#define ITEM_H

#include <reference.h>

extern void startItems ();

extern void itemInitObjectNumbersFromDungeon ();
/* set proper items numbers for each category from a loaded dungeon */



extern reference_p getNextItem (reference_p reference);
/* gives reference to next item*/

extern short* getItem (reference_p reference);
/* gives direct access to item data (not reference) */

extern short* getReferenceItem (reference_p reference);
/* gives pointer to referenced item (used for changing reference)*/


extern short placeNewItem (char category, int type);

//extern int getCurrentItemType ();


extern int cycleActivatorType (reference_p refp, int step);
extern int cycleItem (reference_p refp, int step);

extern unsigned int getNumber (char category);

extern void deleteItem (reference_p refp);

#endif