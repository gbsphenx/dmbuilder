//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// File Operation
//------------------------------------------------------------------------------

#ifndef FILE_OP_H
#define FILE_OP_H

typedef struct 
{
	char* filename;
	char dungeontype;
} file_t;

extern void updateFileNames ();
extern unsigned numberOfFilesToLoad ();
extern char *getFileName (unsigned n);
extern char getFileDungeonType (unsigned n);

#endif