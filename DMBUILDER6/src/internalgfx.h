//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Internal Graphics
//------------------------------------------------------------------------------

#ifndef INTERNAL_GRAPHICS_H
#define INTERNAL_GRAPHICS_H

extern void drawFrame (float size, float r, float g, float b);
extern void drawFrameXY (float sizex, float sizey, float r, float g, float b);
extern void drawFrameLW (float size, float r, float g, float b, float linewidth);

extern void drawBasicTile (int id, float light);
extern void drawSizeSquare (int id, float size, float light);
extern void drawPositionTile (int id, char position, float light);
extern void drawPositionStack (int id, char position, float light);

extern void drawTargetLine (char x, char y, float size, float pencil, float r, float g, float b, char target);

extern void drawItemSpot (char position, int wall, float r, float g, float b, float light);
extern void drawStairsArrow (char direction, float light);
extern void drawDoor (char direction, char button, char opened, char bashed, int type, float light);

extern void drawStartingPosition (char position);

extern void drawMaskedTexture (int id, int mask, char facing, float size, float light);


extern void
drawCapacityBar (float full, int db, float large, float size);

#endif