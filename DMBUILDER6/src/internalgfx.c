//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Internal Graphics
//------------------------------------------------------------------------------

#include <GL/glut.h>
#include <internalgfx.h>
#include <gui.h>
#include <version.h>

#include <display.h>	// for define

extern int tile;
extern float scale;
extern float stack_object_size;

extern int dt_Door_Max;
extern int SKULLKEEP;

static float colors[][3] =
{
	{.7, .7, .7}, {0, .5, 1}, {.8,.7, .9}, {.5, .5, .5},
	{1, 0, 0}, {1, 1, 1}, {1, .5, 0}, {.5, 1, 1},
	{0, .3, 1}, {1, 1, 0}, {0, 1, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {1, 0, 1}, {.4,.5,.6}
};

//------------------------------------------------------------------------------

void
fakeTexture ()
{
	glBindTexture (GL_TEXTURE_2D, 0);
}

void
drawFrame (float size, float r, float g, float b)
{
	glTranslatef (-size/2, size/2, 0);
	glLineWidth(2.0f);
	fakeTexture ();
	glColor4f (r, g, b, 1.f);
	glBegin(GL_LINES);									
		glVertex2d(0,0);								
		glVertex2d(size, 0);								
		glVertex2d(size, 0);								
		glVertex2d(size, -size);								
		glVertex2d(size, -size);								
		glVertex2d(0, -size);								
		glVertex2d(0, -size);								
		glVertex2d(0, 0);								
	glEnd();		
	glTranslatef (size/2, -size/2, 0);
}

//------------------------------------------------------------------------------

void
displayColorQuad (int id, float size, float r, float g, float b)
{
	float half = (float) size/2;
	
	glBindTexture (GL_TEXTURE_2D, id);
	glColor4f (r, g, b, .5f);
	glBegin (GL_QUADS);
		glTexCoord2f (0,1);
		glVertex2f (-half, -half);
		glTexCoord2f (1,1);
		glVertex2f (half, -half);
		glTexCoord2f (1,0);
		glVertex2f (half, half);
		glTexCoord2f (0,0);
		glVertex2f (-half, half);	
	glEnd ();
}
//------------------------------------------------------------------------------



void
drawTargetLine (char x, char y, float size, float pencil, float r, float g, float b, char target)
{
	/* target doit définir si on a une flèche en bout, ou un rond */
	target = 0;
	
	glEnable(GL_BLEND);

	glLineWidth(pencil);
	fakeTexture ();
	glColor4f (r, g, b, 1.f);
	if (x != 0 || y != 0)
	{	glBegin(GL_LINES);									
		glVertex2d(0,0);								
		glVertex2d(size*x, -size*y);
		glEnd();
	}
	else
	{/*	glBegin(GL_POINTS);
		glVertex2d(0,0);*/
		displayColorQuad (0, 4, r, g, b);
	}
	glDisable (GL_BLEND);
}

float _pal_std_door[3] = {.55f, 0.2f, 0.0f };
float _pal_dm1_doors[][3] = 
{	{ .25f, 0.25f, 0.25f },
	{ .55f, 0.2f, 0.0f },
	{ .75f, 0.75f, 0.77f },
	{ .95f, 1.0f, 0.0f } };

float _pal_dm2_doors[][3] = 
{	{ .25f, 0.25f, 0.25f },
	{ .55f, 0.2f, 0.0f },
	{ .75f, 0.75f, 0.77f },
	{ .35f, 0.35f, 0.40f },

	{ .40f, 0.60f, 0.40f },
	{ .25f, 0.40f, 0.25f },
	{ .55f, 0.2f, 0.0f },
	{ 0.8f, 0.70f, 0.60f },

	{ .45f, 0.15f, 0.0f },
	{ .30f, 0.50f, 0.30f },
	{ .35f, 0.45f, 0.35f },

 };




void
drawDoor (char direction, char button, char opened, char bashed, int type)
{
	float rotatey = 0.5;
	int usecolors = 0;
	int angle = direction*90;
	glTranslatef (0, -rotatey, 0);
	glRotatef (angle, 0, 0, 1);
	glTranslatef (0, tile*scale/2, 0);
	glLineWidth(2.0f*__TILE_SCALE__);
	glBindTexture (GL_TEXTURE_2D, 0);

#if DMBF_ON_MAP_DOOR_COLORED == 1
	usecolors = 1;
#endif

	if (!opened) // paint door
	{
		float door_rgb[3];
		
		if (!usecolors)
		{
			door_rgb[0] = _pal_std_door[0];
			door_rgb[1] = _pal_std_door[1];
			door_rgb[2] = _pal_std_door[2];
		}
		else if (usecolors)
		{
			if (type >= dt_Door_Max)
				type = 0;
			if (!SKULLKEEP)
			{
				door_rgb[0] = _pal_dm1_doors[type][0];
				door_rgb[1] = _pal_dm1_doors[type][1];
				door_rgb[2] = _pal_dm1_doors[type][2];
			}
			else if (SKULLKEEP)
			{
				door_rgb[0] = _pal_dm2_doors[type][0];
				door_rgb[1] = _pal_dm2_doors[type][1];
				door_rgb[2] = _pal_dm2_doors[type][2];
			}
		}

		//glColor4f (.7f, .3f, 0.0f,  1.f);
		glColor4f (door_rgb[0], door_rgb[1], door_rgb[2],  1.f);
		glBegin(GL_LINES);									
		glVertex2d(0,0);								
		glVertex2d(0,-tile*scale);								
		glEnd();
	}
	
	glColor4f (.0f, .0f, 0.0f,  1.f);
	glLineWidth(4.0f*__TILE_SCALE__);
	glBegin(GL_LINES);									
	glVertex2d(0,0);								
	glVertex2d(0,-tile*scale/6);
	glVertex2d(0,-tile*scale*5/6);								
	glVertex2d(0,-tile*scale);
	glEnd();

	if (button) // paint buttons
	{
		//glColor4f (1.0f, .0f, 0.0f,  1.f);
		glColor4f (1.0f, .0f, .0f,  1.f);
		glLineWidth(2.0f*__TILE_SCALE__);
		glBegin(GL_LINES);									
		glVertex2d(tile*scale/8,0);								
		glVertex2d(tile*scale/8,-tile*scale/6);
		glVertex2d(-tile*scale/8,-tile*scale*5/6);								
		glVertex2d(-tile*scale/8,-tile*scale);
		glEnd();
	}

	glTranslatef (0, -tile*scale/2, 0);
	glRotatef (-angle, 0, 0, 1);
	glTranslatef (0, rotatey, 0);
}

static void
drawGenericArrow (char position, float pencil, float r, float g, float b)
{
	float fBaseV = 8;
	float fDoubleV = 16;

	fBaseV *= __TILE_SCALE__;
	fDoubleV *= __TILE_SCALE__;

	glEnable(GL_BLEND);
	glLineWidth(pencil);
	fakeTexture ();
	glColor4f (r, g, b, 1.f);
	glRotatef (-90*position, 0, 0, 1);
	glTranslatef (0, fBaseV, 0);
	glBegin(GL_LINES);									
		glVertex2d(0,0);								
		glVertex2d(0, -fDoubleV);								
		glVertex2d(0,0);								
		glVertex2d(-fBaseV, -fBaseV);								
		glVertex2d(0,0);								
		glVertex2d(fBaseV, -fBaseV);								
	glEnd();		
	glTranslatef (0, -fBaseV, 0);
	glRotatef (90*position, 0, 0, 1);
	glDisable (GL_BLEND);
	
}

void
drawStairsArrow (char direction) // up = 1 / down = 0
{
	static float colors[][3] = { {1,.2,.2}, {.3,1,.2}};
	static char pos[] = {2, 0};
	drawGenericArrow (pos[direction], 1.5*__TILE_SCALE__, colors[direction][0], colors[direction][1], colors[direction][1]);
}

void
drawStartingPosition (char position)
{
	drawGenericArrow (position, 1.5*__TILE_SCALE__, .7, .5, .1);
	drawFrame (tile*scale, .95, .6, .2);
}
//------------------------------------------------------------------------------

void
drawMaskedTexture (int id, int mask, char facing, float size, float light) 
{
	glEnable(GL_BLEND);
	glRotatef (-90*facing, 0 , 0, 1);
	glBlendFunc(GL_DST_COLOR,GL_ZERO);
	displayColorQuad (mask, size, light, light, light);
	glBlendFunc(GL_ONE, GL_ONE);
	displayColorQuad (id, size, light, light, light);
	glRotatef (90*facing, 0 , 0, 1);
	glDisable(GL_BLEND);
}	

void
drawItemSpot (char position, int wall, float r, float g, float b, float light)
{
	glRotatef (-90*position, 0, 0, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR,GL_ZERO);
	displayColorQuad (gl_Gui + spotm_Corner + wall, tile*scale, 1, 1, 1);
	glBlendFunc(GL_ONE, GL_ONE);
	displayColorQuad (gl_Gui + spot_Corner + wall, tile*scale, r*light, g*light, b*light);
	glDisable(GL_BLEND);
	glRotatef (90*position, 0, 0, 1);
}


//------------------------------------------------------------------------------

void
drawBasicTile (int id, float light)
{
	displayColorQuad (id, tile*scale, light, light, light);
}

void
drawSizeSquare (int id, float size, float light)
{
	glEnable(GL_BLEND);glBlendFunc(GL_ONE, GL_ONE);
	displayColorQuad (id, size, light, light, light);
}

void
drawPositionTile (int id, char position, float light)
{
	glRotatef (-90*position, 0, 0, 1);
	displayColorQuad (id, tile*scale, light, light, light);
	glRotatef (90*position, 0, 0, 1);
}

void
drawPositionStack (int id, char position, float light)
{
	glRotatef (-90*position, 0, 0, 1);
	displayColorQuad (id, stack_object_size, light, light, light);
	glRotatef (90*position, 0, 0, 1);
}


void
drawCapacityBar (float full, int db, float large, float size)
{
	glColor4f (colors[db][0], colors[db][1], colors[db][2],  1.f);
	glLineWidth(large);
	glBegin(GL_LINES);									
	glVertex2d(0,0);								
	glVertex2d(0,size*full);
	glEnd();
	
	//glColor4f (1.0f, 1.f, 1.f, 1.f);
	glColor4f (0.25f, 0.25f, 0.25f, 1.0f);	// lines around
	glLineWidth(1.0f);
	glBegin(GL_LINES);									
	glVertex2d(-large,0);
	glVertex2d(large,0);
	glVertex2d(large,0);
	glVertex2d(large,size);
	glVertex2d(large,size);
	glVertex2d(-large,size);
	glVertex2d(-large,size);
	glVertex2d(-large,0);
	glEnd();
}
