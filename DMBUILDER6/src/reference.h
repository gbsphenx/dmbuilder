//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Object Reference / Stacks
//------------------------------------------------------------------------------

#ifndef REFERENCE_H
#define REFERENCE_H
/*
typedef struct
{
	unsigned short id:10;
	unsigned short category:4;
	unsigned short position:2;	
} dm_reference;
*/
typedef union {
    struct {
        unsigned short id:10;
        unsigned short category:4;
        unsigned short position:2;
    };
    unsigned short raw;
} dm_reference;


typedef dm_reference* reference_p;

#define REFERENCE_NONE		(short) -2
#define REFERENCE_UNUSED	(short) -1

short* Ref_GetFloorsPointer();
void Ref_SetFloorsPointer(short* pFloors);


extern void startGroundReferences ();
extern void startGroundReferencesKeepStacks ();
extern reference_p getGroundReference (char x, char y, char map);
extern void setGroundReference (char x, char y, char map, short reference);
extern reference_p getStackReference (char position);

extern void importStack (char x, char y, char map);
extern void exportStack (char x, char y, char map);
extern void exportCurrentStack ();

extern void importStackNum (int stacknum, char x, char y, char map);
extern void exportStackNum (int stacknum, char x, char y, char map);

extern int getStackSize ();

extern void turnItemFacing (reference_p reference);
extern reference_p getCurrentItemReference ();

extern void pushReferenceCurrentStack (short reference);

extern short makeReference (char category, char position, short id);

extern short* getCurrentSelectedItem ();

extern void referenceID (reference_p refp, int n);

extern int stackSwitch (unsigned int a, unsigned int b);

extern void removeStackItem (unsigned int i);
extern void removeStackItemIfCategory (unsigned int i, int iCategory);

extern void push_reference_in_stack (short reference, char x, char y, char l);
void push_reference_in_stack_facing (short reference, char x, char y, char l, char facing);

#endif