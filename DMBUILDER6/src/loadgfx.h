//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Load Graphics Sets
//------------------------------------------------------------------------------

#ifndef LOAD_GRAPHICS_H
#define LOAD_GRAPHICS_H

//------------------------------------------------------------------------------
#include <image.h>

extern void loadGraphics ();

extern int loadArchive (char* archive, int glbank, unsigned int number);
extern image_t* Image_LoadFromTGA (char *name, int vflip);
extern int Image_LoadAndBind (char* name, int id);

//------------------------------------------------------------------------------

#endif