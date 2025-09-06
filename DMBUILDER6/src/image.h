//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Image
//------------------------------------------------------------------------------

#ifndef IMAGE_H
#define IMAGE_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

//------------------------------------------------------------------------------

// GLenum is defined in GL/gl.h
//typedef unsigned int GLenum;

typedef struct
{
	unsigned width;
	unsigned height;
	unsigned nPlanes;
	unsigned char* data;
	unsigned activePlane;
	GLenum texFormat;
} image_t;

typedef struct
{
	unsigned width;
	unsigned height;
	char superpalette[6];
	unsigned char* data;
} image16c_t;


extern image16c_t* currentEditedImage;


typedef unsigned char Color16[3];

//------------------------------------------------------------------------------

extern image_t*
Image_ScissorCut (unsigned startx, unsigned starty, unsigned endx, unsigned endy,
				  image_t* origin);

extern int Image_Bind (image_t* image, int id);


extern char* Image_GetCurrentEditedPixel (unsigned short x, unsigned short y);

//extern void Image_InitialisePortraits ();
extern image16c_t* Image_GetPortrait (unsigned i);

extern void Image_FillAreaWithColor (image16c_t* image, unsigned x, unsigned y, char color);

#define SMALLER 0
#define TALLER 1
// size will tells how to do the stretch
// black will blacken the side if not streched, otherwise, side-color is used.
extern int Image16_SquaredBind (image16c_t* image, int id,
				int stretch, int black, int size);

void Image_TEST_updateDagger ();

extern int
Image16_SquaredBindCreature (image16c_t* image, int id,
		     int stretch, int black, int size, int preferredColor);


//------------------------------------------------------------------------------

//#include "image_tga.h"

//------------------------------------------------------------------------------

#endif
