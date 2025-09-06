//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Image
//------------------------------------------------------------------------------

#include <image.h>
#include <image_tga.h>
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>

#include <memory.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include <math.h>

/*
 * Table of Colors 16 - 256:
 *	18 per color value, and it goes from 0 to E (15 different)
 *  0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
 * 000-018-036-054-072-090-108-126-144-162-180-198-216-234-252-xxx
 *  0       1       2       3       4       5       6       7
 */
static Color16 COL_EntrancePalette[16] = {
	{0, 0, 0},{6, 6, 6},
	{8, 8, 8},{8, 4, 0},
	{12, 10, 8},{0, 12, 0},
	{0, 8, 0},{0, 10, 0}, 
	{8, 6, 4},{14, 0, 0},
	{10, 8, 6},{6, 4, 2},
	{5, 5, 5},{11, 11, 11},
	{6, 2, 0},{14, 14, 14},
};

Color16 COL_StandardPalette[16] = {
	{ 0, 0, 0},	{ 6, 6, 6},	// black / dark gray
	{ 8, 8, 8},	{ 6, 2, 0},	// medium gray / dark brown
	{ 0, 14, 14},	{ 8, 4, 0},	// cyan / brown
	{ 0, 7, 0},	{ 0, 14, 0},	// dark green / light green
	{ 14, 0, 0},	{ 14, 11, 0},	// red / orange
	{ 12, 8, 6},	{ 14, 14, 0},	// pink / yellow
	{ 4, 4, 4},	{ 11, 11, 11},	// very dark gray / very light gray
	{ 0, 0, 14},	{ 14, 14, 14},	// blue / white
};

// The switched colors are 7 and 8 from the normal 16 colors palette.
// Color priority: Red - Orange - Blue - Purple - Brown - Green - Golden - Colorless

static Color16 COL_SwitchBlue[2] = {
	{0, 0, 9},	{0, 0, 7},};

static Color16 COL_SwitchGolden[2] = {
	{10, 9, 0},	{7, 5, 0},};

static Color16 COL_SwitchGreen[2] = {
	{12, 9, 0},	{0, 5, 0},};

static Color16 COL_SwitchOrange[2] = {
	{12, 9, 0},	{12, 7, 0},};

static Color16 COL_SwitchPurple[2] = {
	{9, 0, 9},	{7, 0, 7},};

static Color16 COL_SwitchRed[2] = {
	{7, 0, 0},	{5, 0, 0},};

static Color16 COL_SwitchBrown[2] = {
{5, 3, 0},{7, 5, 0},};

static Color16 COL_SwitchColorless[2] = {
	{14, 11, 0},	{12, 8, 6},};

Color16* COL_SwitchTable[8] = {
	COL_SwitchRed, COL_SwitchOrange, COL_SwitchBlue, COL_SwitchPurple,
	COL_SwitchBrown, COL_SwitchGreen, COL_SwitchGolden, COL_SwitchColorless
};

// 32*29 
image16c_t** ImagePortraits;

//------------------------------------------------------------------------------

unsigned char
Image_GetPixel (image_t* image, unsigned i, unsigned j)
{
	assert (i < image->width && j < image->height);
	return image->data[i*image->nPlanes + j*image->width*image->nPlanes + image->activePlane];
}

void
Image_SetPixel (image_t* image, unsigned i, unsigned j, unsigned char pixel)
{
	assert (i < image->width && j < image->height);
	image->data[i*image->nPlanes + j*image->width*image->nPlanes + image->activePlane] = pixel;
}

void
Image_CopyColorPixel (image_t* image, unsigned i, unsigned j, unsigned char* where)
{
	return;
}

//------------------------------------------------------------------------------

image_t*
Image_ScissorCut (unsigned startx, unsigned starty, unsigned endx, unsigned endy,
				  image_t* origin)
{
	image_t* little;
	unsigned i, j, k;
	little = (image_t*) MEMCALLOC (1, sizeof (image_t));
	little->width = (endx - startx);
	little->height = (endy - starty);
	little->nPlanes = origin->nPlanes;
	little->texFormat = origin->texFormat;
	little->data = (unsigned char*) MEMCALLOC (little->width * little->height * 3, sizeof (unsigned char));
	little->activePlane = 0;
	
	for (k = 0; k < little->nPlanes; k++)
	{
		little->activePlane = k;
		origin->activePlane = k;
		for (i = 0; i < little->width; i++)
		{
			for (j = 0; j < little->height; j++)
				Image_SetPixel (little, i, j, Image_GetPixel (origin, i+startx, j+starty));
		}
	}
	return little;
}

image16c_t*
Image16_ScissorCut (unsigned startx, unsigned starty, unsigned endx, unsigned endy,
		    image16c_t* origin)
{
  image16c_t* little;
  unsigned i, j, k;
  little = (image16c_t*) MEMCALLOC (1, sizeof (image16c_t));
  little->width = (endx - startx);
  little->height = (endy - starty);
  little->data = (unsigned char*) MEMCALLOC (little->width * little->height, sizeof (unsigned char));
  for (i = 0; i < little->width; i++)
    {
      for (j = 0; j < little->height; j++)
	//    Image_SetPixel (little, i, j, Image_GetPixel (origin, i+startx, j+starty));
	little->data[i+j*little->width] = origin->data[i+startx + (j+starty)*origin->width];
    }
  return little;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

int
Image_LoadBMP (FILE* imagefile, int id)
{
	return 0;
}








int
Image_Bind (image_t* image, int id)
{
  glBindTexture (GL_TEXTURE_2D, id);
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexImage2D (GL_TEXTURE_2D, 0, image->texFormat, image->width, image->height, 0, image->texFormat, GL_UNSIGNED_BYTE, image->data);
  //free (imageData);
  return 1;
}

static char COL_DM1_Palette_EN_DOUBLE[16][3] = {
	{ 0, 0, 0},	{ 109, 109, 109},	// black / dark grey
	{ 145, 145, 145},	{ 109, 36, 0},	// grey / dark brown
	{ 0, 255, 255},	{ 148, 74, 0},	// cyan / brown
	{ 0, 128, 0},	{ 0, 255, 0},	// dark green / green
	{ 255, 0, 0},	{ 255, 196, 0},	// red / orange
	{ 218, 145, 109},	{ 255, 255, 0},	// pink / yellow
	{ 83, 83, 83},	{ 196, 196, 196},	// very dark grey / light grey
	{ 0, 0, 255},	{ 255, 255, 255},	// blue / white
};

int
Image16_Bind (image16c_t* image, int id)
{
  image_t* binded = (image_t*) MEMCALLOC (1, sizeof (image_t));
  unsigned i, j, k;
  binded->width = image->width;
  binded->height = image->height;
  binded->nPlanes = 3;
  binded->texFormat = GL_RGB;
  binded->data = (unsigned char*) MEMCALLOC (binded->width * binded->height * 3, sizeof (unsigned char));
  binded->activePlane = 0;
  for (k = 0; k < binded->nPlanes; k++)
    {
      binded->activePlane = k;
      for (i = 0; i < binded->width; i++)
	{
	  for (j = 0; j < binded->height; j++)
	    Image_SetPixel (binded, i, j,
			    COL_DM1_Palette_EN_DOUBLE[image->data[i+j*image->width]][k]);
			    //			    Image_GetPixel (origin, i+startx, j+starty));
	}
    }
  Image_Bind (binded, id);
  MEMFREE (binded->data);
  MEMFREE (binded);
  return 1;
}

int
FindPower2 (int value)
{
	int power = 0;
	while (value)
		{
			power++;
			value = value>>1;
		}
	return power;
}

// a GL texture must be 2^ sized!!

int
Image16_SquaredBindCreature (image16c_t* image, int id,
		     int stretch, int black, int size, int preferredColor)
{
  unsigned nwidth = image->width;
  unsigned nheight = image->height;
	int verticalgap = (image->width - image->height)>>2;
  image_t* binded;
  if (image->width == image->height)
    return Image16_Bind (image, id);
  else
    {
      unsigned i, j, k;
      
      if (image->width > image->height)
	nheight = nwidth;
      else
	nwidth = nheight;
      
      binded = (image_t*) MEMCALLOC (1, sizeof (image_t));
      binded->width = (int) pow (2, FindPower2(nwidth));
      binded->height = binded->width;
      binded->nPlanes = 3;
      binded->texFormat = GL_RGB;
      binded->data = (unsigned char*) MEMCALLOC (binded->width * binded->height * 3, sizeof (unsigned char));
      binded->activePlane = 0;
      for (k = 0; k < binded->nPlanes; k++)
	{
	  binded->activePlane = k;
	  for (i = 0; i < binded->width; i++)
	    {
	      for (j = 0; j < binded->height; j++)
		{
				// for better reading/debugging, calculus is clearly split.
			float xi, xj;
			int index, color;
			unsigned char respixel;
			xi = ((float) i / binded->width) * image->width;
			xj = ((float) j / binded->height) * image->height;
			index = (int) xi + ((int) xj) * image->width;
			color = image->data[index];
			if (color >= 16 && color < 22) color = (image->superpalette[color-16]);
			if (color >= 22)
				printf ("(%d)", color);
			if (color == 9 || color == 10)
				respixel = COL_SwitchTable[preferredColor][color-9][k] * 18;
			else
				respixel = COL_StandardPalette[color][k] * 18;
			Image_SetPixel (binded, i, j, respixel);
		}
	    }
	}
      Image_Bind (binded, id);
      MEMFREE (binded->data);
      MEMFREE (binded);
	} 
	return 1;
}




int
Image16_SquaredBind (image16c_t* image, int id,
		     int stretch, int black, int size)
{
  unsigned nwidth = image->width;
  unsigned nheight = image->height;
	int verticalgap = (image->width - image->height)>>2;
  image_t* binded;
  if (image->width == image->height)
    return Image16_Bind (image, id);
  else
    {
      unsigned i, j, k;
      
      if (image->width > image->height)
	nheight = nwidth;
      else
	nwidth = nheight;
      
      binded = (image_t*) MEMCALLOC (1, sizeof (image_t));
      binded->width = (int) pow (2, FindPower2(nwidth));
      binded->height = binded->width;
      binded->nPlanes = 3;
      binded->texFormat = GL_RGB;
      binded->data = (unsigned char*) MEMCALLOC (binded->width * binded->height * 3, sizeof (unsigned char));
      binded->activePlane = 0;
      for (k = 0; k < binded->nPlanes; k++)
	{
	  binded->activePlane = k;
	  for (i = 0; i < binded->width; i++)
	    {
	      for (j = 0; j < binded->height; j++)
		{
				// for better reading/debugging, calculus is clearly split.
			float xi, xj;
			int index, color;
			unsigned char respixel;
			xi = ((float) i / binded->width) * image->width;
			xj = ((float) j / binded->height) * image->height;
			index = (int) xi + ((int) xj) * image->width;
			color = image->data[index];
			if (color >= 16 && color < 22) color = (image->superpalette[color-16]);
			if (color >= 22)
				printf ("(%d)", color);
			respixel = COL_DM1_Palette_EN_DOUBLE[color][k];
			//respixel = COL_EntrancePalette[color][k] * 18;
			Image_SetPixel (binded, i, j, respixel);
		  //  Image_SetPixel (binded, i, j,
		  //	  COL_DM1_Palette_EN_DOUBLE[image->data[i+(j%28)*binded->width]][k]);
		}
	    }
	}
		//	binded->width = 256;
    //  binded->height = 256;
      Image_Bind (binded, id);
      MEMFREE (binded->data);
      MEMFREE (binded);
	} 
	return 1;
}


//------------------------------------------------------------------------------

extern char TestImage[16][16];
image16c_t* currentEditedImage;


char*
Image_GetCurrentEditedPixel (unsigned short x, unsigned short y)
{
	//return &TestImage[x][y];	
	return &currentEditedImage->data[x+y*currentEditedImage->width];
}


/*void
Image_InitialisePortraits ()
{
	image_t* championsgroup, *champion;
	image16c_t* ch16;
	ImagePortraits = (image16c_t**) MEMCALLOC (24, sizeof (image16c_t*));
	championsgroup = Image_LoadFromTGA ("gfx/dmout026.tga");
	if (championsgroup != NULL)
	{
		unsigned x, y;
		unsigned i, j;
		for (x = 0; x < 8; x ++)
		for (y = 0; y < 3; y ++)
		{
			ch16 = (image16c_t*) MEMCALLOC (1, sizeof (image16c_t));
			ch16->height = 29;
			ch16->width = 32;
			ch16->data = (char*) MEMCALLOC (ch16->height*ch16->width, sizeof (char));
			
			//champion = Image_ScissorCut (x*32, y*29, (x+1)*32, (y+1)*29, championsgroup);
			for (j = 0; j < 29; j ++)
			for (i = 0; i < 32; i ++)
				ch16->data[i+j*32] = 1;
			ImagePortraits[x+y*8] = ch16;
		}
		MEMFREE (championsgroup->data);
		MEMFREE (championsgroup);
	}
	currentEditedImage = ImagePortraits[0];
}*/

image16c_t*
Image_GetPortrait (unsigned i)
{
	assert (i < 24);
	return ImagePortraits[i];
}

//------------------------------------------------------------------------------

image16c_t* imageToFill;
char colorFiller;
char colorToReplace;

void
Image_Recursive_4ConnexFiller (short x, short y)
{
	static char directions[4][2] = {
		{1,0}, {0,-1}, {-1,0}, {0,1}
	};
	if ((x < 0) || (y < 0) || (x >= imageToFill->width) || (y >= imageToFill->height))
		return;
	else
	{
		unsigned i;
		imageToFill->data[x+y*imageToFill->width] = colorFiller;
		for (i = 0; i < 4; i++)
		{
			if (imageToFill->data[(x+directions[i][0])+(y+directions[i][1])*imageToFill->width] == colorToReplace)
				Image_Recursive_4ConnexFiller (x+directions[i][0], y+directions[i][1]);
		}
	}
}

void
Image_FillAreaWithColor (image16c_t* image, unsigned x, unsigned y, char color)
{
	colorFiller = color;
	colorToReplace = image->data[x+y*image->width];
	imageToFill = image;
	if (colorFiller != colorToReplace)
		Image_Recursive_4ConnexFiller (x, y);
}

//------------------------------------------------------------------------------



