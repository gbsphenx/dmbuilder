//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Filename
//------------------------------------------------------------------------------


#ifndef FILE_NAME_H
#define FILE_NAME_H

extern void readFileNames ();

extern char* getLoadDungeonName ();
extern char* getSaveDungeonName ();

extern char* fileopFailedMessage (int f);
extern char* fileopSuccessMessage (int f);
#endif
