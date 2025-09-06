//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Security Lock
//------------------------------------------------------------------------------

#ifndef SECURITY_H
#define SECURITY_H

enum LOCKS
{
	lock_none = 0,	// full editing,
	lock_editing = 1,	// full viewing, no editing
	lock_objects = 2,	// no editing, only display layouts
	lock_full = 3, // no editing, no display
};

extern int verifyLock (); // returns the lock number (0 = no lock)

#endif