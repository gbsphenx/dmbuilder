//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Security Lock
//------------------------------------------------------------------------------

#include <security.h>
#include <item.h>
#include <reference.h>
#include <dms.h>
#include <dm.h>

int current_lock = 0;
int valid = 0;

int
verifyLock ()
{
	dm_reference ref;
	chest_p code;
	unsigned short* vector;
	int lock = 0;

	ref.id = (getNumber (category_Chest))-1;
	ref.position = 0;
	ref.category = category_Chest;
	code = (chest_p) getReferenceItem (&ref);
	vector = (unsigned short*) code;

	if (vector[0] == 0xFFFF)
		if (vector[2] == 0xFFFE)
			lock = 1;
	return lock;
}