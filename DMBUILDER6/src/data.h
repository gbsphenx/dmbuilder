//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Data
//------------------------------------------------------------------------------

#ifndef DATA_H
#define DATA_H

#include <dms.h>

typedef unsigned int (*getObjectType) (short*);	// changed char to int to allow more than 127 values (SK extended)
typedef void (*setObjectType) (short*, unsigned int);

extern getObjectType getItemType[16];
extern setObjectType setItemType[16];

extern unsigned int doorGetOrnate (short *door);
extern void doorSetOrnate (short *door, unsigned int ornate);

extern short unchest_Chest (chest_p chest);

unsigned int objectContainerGetType (short *chest);
void objectContainerSetType (short *chest, unsigned int type);

#endif