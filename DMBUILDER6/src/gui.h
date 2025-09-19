//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Graphical User Interface
//------------------------------------------------------------------------------

#ifndef GUI_H
#define GUI_H

#include <dm.h>

// Monsters, Weapons, Clothes, Potions, Containers and Miscs must be 0x100 their category id

enum GLGROUP
{
 gl_Doors			=	0x0100,

 gl_Monsters		=	0x0400,
 gl_Weapons			=	0x0500,
 gl_Clothes			=	0x0600,
 gl_Potions			=	0x0700,
 gl_Miscs			=	0x0A00,
 gl_Containers		=	0x0B00,
 gl_FActuators		=	0x0C00,
 gl_WActuators		=	0x0D00,

 gl_SpecialTiles	=	0x0F00,	// specially used for DM2 special floor tiles
 gl_Gui				=	0x1000,

 gl_Spells			=	0x1800,
 gl_Powers			=	0x1900,
 gl_Portraits		=	0x1A00,
 gl_Special			=	0x1B00, // use for actuators
 gl_Tiles			=	0x1C00,
 gl_Ornates			=	0x1D00,	// door ornates
 gl_Floors			=	0x1E00,
 gl_Walls			=	0x1F00,

 
 gl_Masking			=	0x2000,	// for door masks
 gl_SK_TilesSet		=	0x2800,
 gl_StaticSkullkeep =	0x3000, // values 0x0000 - this are repeated only for SK display
 gl_StaticTELOS		=	0x4000, // values 0x0000 - this are repeated only for TELOS display
 
};

enum GLBANKS
{
	selection_Tiles = 0,
	selection_Doors,
	selection_Ornates,
	selection_Walls,
	selection_Floors,
	selection_Monsters,
	selection_Weapons,
	selection_Clothings,
	selection_Potions,
	selection_Miscs,
};

enum GUITYPE /* misc gfx for interface */
{
	pos_Corner	=	0,
	pos_Side	=	1,
	pos_Center	=	2,
	spot_Corner	=	3,
	spot_Side,
	spotm_Corner,	 /* masks */
	spotm_Side,
	door_Button,
	door_Horizontal,
	door_Vertical	=	9,
	tele_BlueHaze,
	rotation_Ahead	=	11,
	rotation_Clockwise,
	rotation_Back,
	rotation_AntiClockwise,
	gui_Sound,
	text_Mouth,
	gui_Cursed	=	17,
	gui_Poisoned,
	gui_Broken,
	gui_Off,
	gui_On,
	gui_Explosion,
	gui_Max		=	23,
};


enum EXTENDEDTILE
{
	xtile_FloorsGFX = tile_Max,

	xtile_SquareGrid = tile_Max, /* 0x08 */
	xtile_SquarePad,
	xtile_PoisonPuddle,
	xtile_RoundGrid,
	xtile_HexagonalPad,
	xtile_FirePot,
	xtile_GreyMark,
	xtile_TinyPad,
	xtile_WaterPuddle, /* 0x10 = 16 */
	xtile_DM1_SpecialOrnate_Max = xtile_WaterPuddle,

	xtile_TrickWallClosed,
	xtile_TrickWallOpened,

	xtile_ImaginaryHiddenPit,
	xtile_HiddenPit,
	xtile_ImaginaryPit,
	xtile_ClosedPit,

	xtile_Max = 23,
};

//// In standardised V9 tiles, special tiles are ordered like this:
// trick wall closed
// trick wall opened
// imaginary hidden pit
// hidden pit
// imaginary pit
// closed pit
// unused 1
// unused 2
// that means to access them for skullkeep, use the special value - (waterpuddle + tile_max)

#endif