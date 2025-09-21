//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Display
//------------------------------------------------------------------------------

#include <internalgfx.h>
#include <display.h>
#include <editor.h>
#include <displaytext.h>
#include <level.h>
#include <filename.h>
#include <reference.h>
#include <dm.h>
#include <item.h>
#include <actuator.h>
#include <data.h>
#include <fileop.h>
#include <version.h>
#include <string.h>
#include <loaddungeon.h>

#include <skullexe.h>
#include <math.h>

#include <gui.h>

#include <stdio.h>

#ifdef __MINGW__
	#ifdef BOOL
	#undef BOOL
	#endif
	#ifdef APIENTRY
	#undef APIENTRY
	#endif
	#ifdef CALLBACK
	#undef CALLBACK
	#endif
	#include <GL/freeglut.h>
#else
	#include <GL/glut.h>	// include glut last
#endif

#include <glTexFont.h>

extern int winW;
extern int winH;

int msg_flag = MSG_PRINT_VERSION;

extern tSkullExe xSkullExe;

int tile = __TILE_GL_BASE__;
float scale = __TILE_SCALE__;	// old = 1
float tileScale = (__TILE_GL_BASE__*__TILE_SCALE__);
float stack_object_size = __STD_STACK_SIZE__;

int iCurrentStackCount = 0;

int iFntSizeBigTitle = 32;

int angle = 0;	// for blinking cursor
float globalfsinv = 0;

// attempt to track the position of current gltranslatef
int iGLVirtualX = 0;
int iGLVirtualY = 0;
int iGLVirtualXMemTab[8];
int iGLVirtualYMemTab[8];
int iGLMatrixLevel = 0;


int shadowmap = 0; // if set, the entire map has low light

extern int nItems[16];

extern int SKULLKEEP;
extern int THERONSQUEST;
extern int TELOS;

int gl_x_Tiles = gl_Tiles;
int gl_x_Doors = gl_Doors;
int gl_x_Monsters = gl_Monsters;
int gl_x_Walls = gl_Walls;
int gl_x_Floors = gl_Floors;
int gl_x_Ornates = gl_Ornates;
int gl_x_Containers = gl_Containers;

int gl_x_Weapons = gl_Weapons;
int gl_x_Clothes = gl_Clothes;
int gl_x_Potions = gl_Potions;
int gl_x_Miscs = gl_Miscs;

int gl_x_Portraits = gl_Portraits;

extern int dt_Door_Max;
extern int dt_Monster_Max;
extern int dt_Wall_Max;
extern int dt_Floor_Max;
extern int dt_Ornate_Max;

extern int dt_Weapons_Max;
extern int dt_Clothing_Max;
extern int dt_Misc_Max;
extern int dt_Containers_Max;

//------------------------------------------------------------------------------
//	Image Graphics to display for each Item of the "Activation Item" List
//------------------------------------------------------------------------------

unsigned int gfx_activation_items[] = {
/* 00 - 03 */	gl_Miscs + misc_Compass, gl_Special, gl_Special, gl_Special, 
/* 04 - 07 */	gl_Weapons + weapon_Torch,	gl_Special + item_actuators_TorchAlmostNew, gl_Special + item_actuators_TorchAlmostBurnt, gl_Special + item_actuators_TorchBurntOut,
/* 08 - 0B */	gl_Miscs + misc_Waterskin, gl_Special + item_actuators_WaterSkinEmpty, gl_Miscs + misc_JewelSymal, gl_Special + item_actuators_JewelSymalUsed,
/* 0C - 0F */	gl_Miscs + misc_Illumulet, gl_Special + item_actuators_IllumuletUsed, gl_Weapons + weapon_Flamitt, gl_Special + item_actuators_FlamittUsed,
/* 10 - 13 */	gl_Weapons + weapon_EyeOfTime, gl_Special + item_actuators_EyeOfTimeUsed, gl_Weapons + weapon_StormRing, gl_Special + item_actuators_StormRingUsed,
/* 14 - 17 */	gl_Weapons + weapon_StaffOfClaws, gl_Special + item_actuators_StaffOfClawsUsed, gl_Special + item_actuators_StaffOfClawsYellow, gl_Weapons + weapon_BoltBlade,
/* 18 - 1B */	gl_Special + item_actuators_BoltBladeUsed, gl_Weapons + weapon_Fury, gl_Special + item_actuators_FuryUsed, gl_Weapons + weapon_TheFirestaffA,
/* 1C - 1F */	gl_Weapons + weapon_TheFirestaffB, 0, gl_Special + special_Scroll, gl_Special + special_Scroll,
/* 20 - 23 */	gl_Weapons + weapon_Dagger, gl_Weapons + weapon_Falchion, gl_Weapons + weapon_Sword, gl_Weapons + weapon_Rapier,
/* 24 - 27 */	gl_Weapons + weapon_Sabre, gl_Weapons + weapon_SamuraiSword, gl_Weapons + weapon_Delta, gl_Weapons + weapon_DiamondEdge,
/* 28 - 2B */	gl_Weapons + weapon_VorpalBlade, gl_Weapons + weapon_TheInquisitor, gl_Weapons + weapon_Axe, gl_Weapons + weapon_HardCleave,
/* 2C - 2F */	gl_Weapons + weapon_Mace, gl_Weapons + weapon_MaceOfOrder, gl_Weapons + weapon_Morningstar, gl_Weapons + weapon_Club,
/* 30 - 33 */	gl_Weapons + weapon_StoneClub, gl_Weapons + weapon_Bow, gl_Weapons + weapon_Crossbow, gl_Weapons + weapon_Arrow,
/* 34 - 37 */	gl_Weapons + weapon_Slayer, gl_Weapons + weapon_Sling, gl_Weapons + weapon_Rock, gl_Weapons + weapon_PoisonDart,
/* 38 - 3B */	gl_Weapons + weapon_ThrowingStar, gl_Weapons + weapon_Stick, gl_Weapons + weapon_Staff, gl_Weapons + weapon_Wand,
/* 3C - 3F */	gl_Weapons + weapon_TeoWand, gl_Weapons + weapon_YewStaff, gl_Weapons + weapon_StaffOfManar, gl_Weapons + weapon_SnakeStaff,
/* 40 - 43 */	gl_Weapons + weapon_TheConduit, gl_Weapons + weapon_DragonSpit, gl_Weapons + weapon_SceptreOfLyf, gl_Clothes + clothing_RobeA,
/* 44 - 47 */	gl_Clothes + clothing_FineRobeA, gl_Clothes + clothing_Kirtle, gl_Clothes + clothing_SilkShirt, gl_Clothes + clothing_ElvenDoublet,
/* 48 - 4B */	gl_Clothes + clothing_LeatherJerkin, gl_Clothes + clothing_Tunic, gl_Clothes + clothing_Ghi, gl_Clothes + clothing_MailAketon,
/* 4C - 4F */	gl_Clothes + clothing_MithralAketon, gl_Clothes + clothing_TorsoPlate, gl_Clothes + clothing_PlateOfLyte, gl_Clothes + clothing_PlateOfDarc,
/* 50 - 53 */	gl_Clothes + clothing_Cape, gl_Clothes + clothing_CloakOfNight, gl_Clothes + clothing_BarbarianHide, gl_Clothes + clothing_RobeB,
/* 54 - 57 */	gl_Clothes + clothing_FineRobeB, gl_Clothes + clothing_Tabard, gl_Clothes + clothing_Gunna, gl_Clothes + clothing_ElvenHuke,
/* 58 - 5B */	gl_Clothes + clothing_LeatherPants, gl_Clothes + clothing_BluePants, gl_Clothes + clothing_GhiTrousers, gl_Clothes + clothing_LegMail,
/* 5C - 5F */	gl_Clothes + clothing_MithralMail, gl_Clothes + clothing_LegPlate, gl_Clothes + clothing_PoleynOfLyte, gl_Clothes + clothing_PoleynOfDarc,
/* 60 - 63 */	gl_Clothes + clothing_BezerkerHelm, gl_Clothes + clothing_Helmet, gl_Clothes + clothing_Basinet, gl_Clothes + clothing_CasqueNCoif,
/* 64 - 67 */	gl_Clothes + clothing_Armet, gl_Clothes + clothing_HelmOfLyte, gl_Clothes + clothing_HelmOfDarc, gl_Clothes + clothing_Calista,
/* 68 - 6B */	gl_Clothes + clothing_CrownOfNerra, gl_Clothes + clothing_Buckler, gl_Clothes + clothing_HideShield, gl_Clothes + clothing_SmallShield,
/* 6C - 6F */	gl_Clothes + clothing_WoodenShield, gl_Clothes + clothing_LargeShield, gl_Clothes + clothing_ShieldOfLyte, gl_Clothes + clothing_ShieldOfDarc,
/* 70 - 73 */	gl_Clothes + clothing_Sandals, gl_Clothes + clothing_SuedeBoots, gl_Clothes + clothing_LeatherBoots, gl_Clothes + clothing_Hosen,
/* 74 - 77 */	gl_Clothes + clothing_FootPlate, gl_Clothes + clothing_GreavesOfLyte, gl_Clothes + clothing_GreavesOfDarc, gl_Clothes + clothing_ElvenBoots,
/* 78 - 7B */	gl_Miscs + misc_GemOfAges, gl_Miscs + misc_EkkhardCross, gl_Miscs + misc_Moonstone, gl_Miscs + misc_TheHellion,
/* 7C - 7F */	gl_Miscs + misc_PendantFeral, gl_Miscs + misc_CopperCoin, gl_Miscs + misc_SilverCoin, gl_Miscs + misc_GoldCoin,
/* 80 - 83 */	gl_Miscs + misc_Boulder, gl_Miscs + misc_BlueGem, gl_Miscs + misc_OrangeGem, gl_Miscs + misc_GreenGem,
/* 84 - 87 */	gl_Miscs + misc_MagicalBoxBlue, gl_Miscs + misc_MagicalBoxGreen, gl_Miscs + misc_MirrorOfDawn, gl_Weapons + weapon_HornOfFear,
/* 88 - 8B */	gl_Miscs + misc_Rope, gl_Miscs + misc_RabbitsFoot, gl_Miscs + misc_Corbamite, gl_Miscs + misc_Choker,
/* 8C - 8F */	gl_Clothes + clothing_Dexhelm, gl_Clothes + clothing_Flamebain, gl_Clothes + clothing_PowerTowers, gl_Weapons + weapon_Speedbow,
/* 90 - 93 */	gl_Special + special_Chest, gl_Special + special_Chest, gl_Miscs + misc_Ashes, gl_Miscs + misc_BonesOfHero,
/* 94 - 97 */	gl_Potions, gl_Potions+1, gl_Potions+2, gl_Potions + potion_VenPotion,
/* 98 - 9B */	gl_Potions + potion_SarPotion, gl_Potions + potion_ZoPotion, gl_Potions + potion_RosPotion, gl_Potions + potion_KuPotion,
/* 9C - 9F */	gl_Potions + potion_DanePotion, gl_Potions + potion_NetaPotion, gl_Potions + potion_AntiVenin, gl_Potions + potion_MaPotion,
/* A0 - A3 */	gl_Potions + potion_YaPotion, gl_Potions + potion_EePotion, gl_Potions + potion_ViPotion, gl_Potions + potion_WaterFlask,
/* A4 - A7 */	gl_Potions+16, gl_Potions+17, gl_Potions+18, gl_Potions + potion_FulBomb,
/* A8 - AB */	gl_Miscs + misc_Apple,  gl_Miscs + misc_Corn, gl_Miscs + misc_Bread, gl_Miscs + misc_Cheese,
/* AC - AF */	gl_Miscs + misc_ScreamerSlice, gl_Miscs + misc_WormRound, gl_Miscs + misc_Drumstick, gl_Miscs + misc_DragonSteak,
/* B0 - B3 */	gl_Miscs + misc_IronKey, gl_Miscs + misc_KeyOfB, gl_Miscs + misc_SolidKey, gl_Miscs + misc_SquareKey,
/* B4 - B7 */	gl_Miscs + misc_TourquoiseKey, gl_Miscs + misc_CrossKey, gl_Miscs + misc_OnyxKey, gl_Miscs + misc_SkeletonKey,
/* B8 - BB */	gl_Miscs + misc_GoldKey, gl_Miscs + misc_WingedKey, gl_Miscs + misc_TopazKey, gl_Miscs + misc_SapphireKey,
/* BC - BF */	gl_Miscs + misc_EmeraldKey, gl_Miscs + misc_RubyKey, gl_Miscs + misc_RaKey, gl_Miscs + misc_MasterKey,
/* C0 - C3 */	gl_Miscs + misc_LockPicks, gl_Miscs + misc_Magnifier, gl_Clothes + clothing_BootsOfSpeed, gl_Potions + potion_EmptyFlask,
/* C4 - C7 */	gl_Clothes + clothing_Halter, gl_Miscs + misc_ZokathraSpell, gl_Miscs + misc_Bones, 0,
/* C8 - CF */	0, 0, 0, 0,  0, 0, 0, 0,
/* D0 - D7 */	0, 0, 0, 0,  0, 0, 0, 0,
/* D8 - DF */	0, 0, 0, 0,  0, 0, 0, 0,
/* E0 - E7 */	0, 0, 0, 0,  0, 0, 0, 0,
/* E8 - EF */	0, 0, 0, 0,  0, 0, 0, 0,
/* F0 - F7 */	0, 0, 0, 0,  0, 0, 0, 0,
/* F8 - FF */	0, 0, 0, 0,  0, 0, 0, 0,
//--- Extended table for handling DM2 values
/* 100 - 11F */	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 
/* 120 - 13F */	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 
/* 140 - 15F */	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 
/* 160 - 17F */	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 
/* 180 - 19F */	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 
/* 1A0 - 1BF */	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 
/* 1C0 - 1DF */	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 
/* 1D0 - 1FF */	0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,		0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 
};

//------------------------------------------------------------------------------
//	Objects Graphics Banks
//------------------------------------------------------------------------------

int gfx_objects[] =
{ 
	gl_Doors, 0, 0, 0,
	gl_Monsters, gl_Weapons, gl_Clothes, gl_Special + special_Scroll/*???*/,
	gl_Potions, 0, gl_Miscs, 0,
	0, 0, 0, 0	// TODO and SPELLS????
};

int gfx_dm2_objects[] =
{ 
	gl_StaticSkullkeep + gl_Doors, 0, 0, 0,
	gl_StaticSkullkeep + gl_Monsters, gl_StaticSkullkeep + gl_Weapons, gl_StaticSkullkeep + gl_Clothes, gl_Special + special_Scroll/*???*/,
	gl_StaticSkullkeep + gl_Potions, 0, gl_StaticSkullkeep + gl_Miscs, 0,
	0, 0, 0, 0	// TODO and SPELLS????
};

int* gfx_x_objects = gfx_objects;

//------------------------------------------------------------------------------
//	Item Colors Spot
//------------------------------------------------------------------------------

static float spot_colors[][3] =
{
	{0, 0, 0}, {0, .5, 1}, {.8,.7, .9}, {.5, .5, .5},
	{1, 0, 0}, {1, 1, 1}, {1, .5, 0}, {.5, 1, 1},
	{0, .3, 1}, {1, 1, 0}, {0, 1, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {1, 0, 1}, {.3,.3,.3}
};

//------------------------------------------------------------------------------
//	GL Virtual moves
//------------------------------------------------------------------------------

void
_GL_Translatef(float x, float y, float z)
{
	iGLVirtualX += (x/2.f);
	iGLVirtualY += (y/2.f);
	glTranslatef (x, y, z);
}

void
_GL_LoadIdentity()
{
	iGLVirtualX = (winW/2.f);
	iGLVirtualY = (winH/2.f);
	glLoadIdentity();
}

void
_GL_PushMatrix()
{
	iGLVirtualXMemTab[iGLMatrixLevel] = iGLVirtualX;
	iGLVirtualYMemTab[iGLMatrixLevel] = iGLVirtualY;
	iGLMatrixLevel++;
	glPushMatrix ();
}

void
_GL_PopMatrix()
{
	glPopMatrix();
	iGLMatrixLevel--;
	if (iGLMatrixLevel < 0)
		iGLMatrixLevel = 0;
	iGLVirtualX = iGLVirtualXMemTab[iGLMatrixLevel];
	iGLVirtualY = iGLVirtualYMemTab[iGLMatrixLevel];
}

//------------------------------------------------------------------------------
//	Screen Placements
//------------------------------------------------------------------------------

static void
move (float x, float y)
{
	//iGLVirtualX += x*tileScale;
	//iGLVirtualY -= y*tileScale;
	//glTranslatef (x*tileScale, -y*tileScale, 0);
	_GL_Translatef(x*tileScale, -y*tileScale, 0);
}

static void
moveSize (float x, float y, float size)
{
	//iGLVirtualX += x*size;
	//iGLVirtualY -= y*size;
	//glTranslatef (x*size, -y*size, 0);
	_GL_Translatef(x*size, -y*size, 0);
}

static void
moveStack (char x, char y)
{
	//iGLVirtualX += x*__STD_STACK_SIZE__;
	//iGLVirtualY -= y*__STD_STACK_SIZE__;
	//glTranslatef (x*__STD_STACK_SIZE__, -y*__STD_STACK_SIZE__, 0);
	_GL_Translatef(x*__STD_STACK_SIZE__, -y*__STD_STACK_SIZE__, 0);
}

static void
moveStackFloat (float x, float y)
{
	//iGLVirtualX += x*__STD_STACK_SIZE__;
	//iGLVirtualY -= y*__STD_STACK_SIZE__;
	//glTranslatef (x*__STD_STACK_SIZE__, -y*__STD_STACK_SIZE__, 0);
	_GL_Translatef(x*__STD_STACK_SIZE__, -y*__STD_STACK_SIZE__, 0);
}

//..............................................................................

static void
moveToUpperMap ()
{
	int iStartX = 0;
	int iStartY = 0;
	int iMainScreen = 0;
	glMatrixMode (GL_MODELVIEW);
	_GL_LoadIdentity();
	//glLoadIdentity ();
	//iGLVirtualX = winW;
	//iGLVirtualY = 0;

	//--- Default for Main Map Screen View
	iStartX = -winW+(__DISP_MAP_START_X__*2);
	iStartY = winH-(__DISP_MAP_START_Y__*2);	// display needs x 2?

	iMainScreen = getScreen();
	if (iMainScreen == screen_Map)
	{
		iStartX = -winW+(__DISP_MAP_START_X__*2);
		iStartY = winH-(__DISP_MAP_START_Y__*2);
		scale = __TILE_SCALE__;
		tileScale = (__TILE_GL_BASE__*scale);
	}
	else if (iMainScreen == screen_Level) // display as mini map at top right
	{
		iStartX = -winW+(__DISP_MAP_START_X__*2)+(1100*2);
		iStartY = winH-(__DISP_MAP_START_Y__*0.75f);
		scale = 0.75f;
		tileScale = (__TILE_GL_BASE__*scale);
	}


	//glTranslatef (-winW+100, winH-100, 0);
	//iStartX = -winW+(80+__DISP_MAP_START_X__);
	//iGLVirtualX += iStartX;
	//iGLVirtualY += iStartY;
	//glTranslatef (iStartX, iStartY, 0);
	_GL_Translatef (iStartX, iStartY, 0);
}

static void
moveToUpperScreen ()
{
	int iStartX = 0;
	int iStartY = 0;
	int iMainScreen = 0;
	glMatrixMode (GL_MODELVIEW);
    //glLoadIdentity ();
	//iGLVirtualX = 0;
	//iGLVirtualY = 0;
	_GL_LoadIdentity();

	//--- Default for Main Map Screen View
	iStartX = -winW+(__DISP_MAP_START_X__*2);
	iStartY = winH-(__DISP_MAP_START_Y__*2);	// display needs x 2?

	{
		iStartX = -winW+(__DISP_MAP_START_X__*2);
		iStartY = winH-(__DISP_MAP_START_Y__*2);
		scale = __TILE_SCALE__;
		tileScale = (__TILE_GL_BASE__*scale);
	}
	//iGLVirtualX += iStartX;
	//iGLVirtualY += iStartY;
	//glTranslatef (iStartX, iStartY, 0);
	_GL_Translatef(iStartX, iStartY, 0);
}

static void
moveToUp ()
{
	glMatrixMode (GL_MODELVIEW);
	_GL_LoadIdentity();
    //glLoadIdentity ();
	//iGLVirtualX = 0;
	//iGLVirtualY = 0;
	
	//iGLVirtualX += 0;
	//iGLVirtualY += (winH-__STD_STACK_SIZE__);
	//glTranslatef (0, winH-__STD_STACK_SIZE__, 0);
	_GL_Translatef(0, winH-__STD_STACK_SIZE__, 0);
}


static void
moveToCenterMap ()
{
	moveToUpperMap ();
	//iGLVirtualX += (15.5*tileScale);
	//iGLVirtualY += (-15.5*tileScale);
	//glTranslatef (15.5*tileScale, -15.5*tileScale, 0);
	_GL_Translatef (15.5*tileScale, -15.5*tileScale, 0);
}

static void
moveToStackUpper ()
{
	moveToUpperMap ();
	//iGLVirtualX += (38.5*tileScale);
	//iGLVirtualY += (-3.5*tileScale);
	//glTranslatef (38*tileScale, -3.5*tileScale, 0);
	_GL_Translatef (38*tileScale, -3.5*tileScale, 0);
}

static void
moveToGraphicsList ()
{
	moveToUpperMap ();
	//iGLVirtualX += (0*tileScale);
	//iGLVirtualY += (-32.5*tileScale);
	//glTranslatef (0, -32.5*tileScale, 0);
	_GL_Translatef (.5*tileScale, -33*tileScale, 0);
}

static void
moveToBottom ()
{
	glMatrixMode (GL_MODELVIEW);
	_GL_LoadIdentity();
    //glLoadIdentity ();
	//iGLVirtualX = 0;
	//iGLVirtualY = 0;
	//iGLVirtualX += 0;
	//iGLVirtualY += (-winH+__STD_STACK_SIZE__);
	//glTranslatef (0, -winH+__STD_STACK_SIZE__, 0);
	_GL_Translatef (0, -winH+__STD_STACK_SIZE__, 0);
}

//------------------------------------------------------------------------------
//	Selection Bar for new item to place
//------------------------------------------------------------------------------

static void
displaySelectNewItem ()
{
	static int symbol_item[] = {
		gl_Doors, gl_Gui + tele_BlueHaze, gl_Walls, gl_Floors,
		gl_Monsters + monster_Screamer, gl_Weapons + weapon_Sword,
		gl_Clothes + clothing_PlateOfLyte, gl_Special + special_OpenScroll,
		gl_Potions + potion_EmptyFlask, gl_Special + special_Chest,
		gl_Miscs + misc_Apple };
	unsigned int i;

	{
		symbol_item[0] = gl_x_Doors;
		symbol_item[2] = gl_Walls;		// This gives the "scrambled text" image
		symbol_item[3] = gl_x_Floors;	
		symbol_item[4] = gl_x_Monsters;

		symbol_item[9] = gl_Special + special_Chest;

		if (SKULLKEEP)
		{
			symbol_item[3] = gl_x_Walls + 0x2A;	// The gear image
			symbol_item[9] = gl_StaticSkullkeep + gl_Containers;
		}

	}

	moveToUpperScreen ();
	moveSize (52-16, 19.5, 48);
	for (i = 0; i < 11; i++)
	{
		drawSizeSquare (symbol_item[i], __STD_STACK_SIZE__, 1.0f);
		if (i == (unsigned int) getEditCursor (cursor_NewItem))
		{
			double fsinv = 0;
			double rad = 0;
			if (angle > 360)
				angle = angle%360;
			rad = ((double)angle) / 360 * (2*3.1415f) * 1;
			fsinv = (double)cos(rad);
			fsinv = (fsinv/2) + 0.5f;
			drawFrame (__STD_STACK_SIZE__, 0, .5, 1);
			drawFrameLW (__STD_STACK_SIZE__, 1.*fsinv, 1.*fsinv, -.5*fsinv, 4.f);
		}
		moveSize (1, 0, __STD_STACK_SIZE__);
	}

	printNewItemStats ();

}

//------------------------------------------------------------------------------
//	Specific Activator displays
//------------------------------------------------------------------------------
static void // draw object on tile
drawActivator (reference_p refp, int wall, unsigned char level)
{
	if (!wall)
	{
		actuator_p type = (actuator_p) getItem (refp);
		actuator_effect_p effect = (actuator_effect_p) (getItem (refp)+1);
		if (effect->graphism != 0)
		{
			int ornate_gfx = (getLevels()[level]).floors[effect->graphism-1];
			int gl_offset = gl_Tiles + xtile_FloorsGFX;

			if (SKULLKEEP && !THERONSQUEST)
				gl_offset = gl_StaticSkullkeep + gl_SpecialTiles;
			else if (SKULLKEEP && THERONSQUEST)
				gl_offset = gl_Tiles + xtile_FloorsGFX;
			drawBasicTile (gl_offset + ornate_gfx, 1.0f);
//			drawBasicTile (gl_Tiles + xtile_FloorsGFX + 
//			(getLevels()[level]).floors[effect->graphism-1], 1.0f);
		}
		if (type->type == actuator_floor_monster_generator)
		{	
			glEnable (GL_BLEND);
			drawSizeSquare (gl_x_Monsters + type->value, tile*1.5, 1.0f);
		}
	}
}


static void
drawDestinationFromCurrent (char xdest, char ydest, float r, float g, float b, char target, int edit)
{
	double fsinv = 0;
	double rad = 0;

	_GL_PushMatrix();
	//glPushMatrix ();
	_GL_LoadIdentity();
	//glLoadIdentity ();
	//iGLVirtualX = 0;
	//iGLVirtualY = 0;

	if (angle > 360)
		angle = angle%360;
	rad = ((double)angle) / 360 * (2*3.1415f) * 4;
	fsinv = (double)cos(rad);
	fsinv = (fsinv/2) + 0.5f;
	//printf("angle = %d / rad = %f / fsinv = %f\n", angle, rad, fsinv);

	if (edit != 1)
		fsinv = 1;

	moveToUpperMap ();	
	move (getEditCursor(cursor_X), getEditCursor(cursor_Y));
	drawTargetLine ((char) (xdest - getEditCursor(cursor_X)), (char) (ydest - getEditCursor(cursor_Y)),
	tileScale, 2.0f, r*fsinv, g*fsinv, b*fsinv, target);
	//glPopMatrix();
	_GL_PopMatrix();
	
}

//------------------------------------------------------------------------------
//	Object displays while in stack
//------------------------------------------------------------------------------

typedef void (*displayObject) (reference_p, float);

static void
displayNoObject (reference_p refp, float light)
{;}

static void
displayDoorObject (reference_p refp, float light)
{
	door_p door = (door_p) getItem (refp);
	char lights[] = {door->button, 1, door->bashable, door->destroyable};
	static int glid[] = {gl_Gui + door_Button, gl_Gui + door_Horizontal,
		gl_Weapons + weapon_Sword, gl_Gui + gui_Explosion};
	int second[] = {0, door->opensUp, 0, 0};
	
	unsigned int i = 0;
	
	drawPositionStack (gl_x_Doors +
		((door->type)?getLevels()[getEditCursor(cursor_L)].header.door2:
		getLevels()[getEditCursor(cursor_L)].header.door1),
		0, 1.0f);
	if (door->ornate > 0)
		drawMaskedTexture (
		gl_x_Ornates + getLevels()[getEditCursor(cursor_L)].ornates[door->ornate-1],
		gl_x_Ornates + getLevels()[getEditCursor(cursor_L)].ornates[door->ornate-1] + gl_Masking,
		0, __STD_STACK_SIZE__, 1.0f);

	for (i = 0; i < 4; i++)
	{
		moveStack (1, 0);
		drawPositionStack (glid[i] + second[i], 0, 0.4 + 0.6 * lights[i]);
	}
	moveStack (-4, 0);
}

static void
displayTeleportObject (reference_p refp, float light)
{
	teleport_p teleport = (teleport_p) getItem (refp);
	tile_teleport_p teletile = (tile_teleport_p) getCurrentTile ();
	char lights[] = {teletile->bluehaze, 1, teleport->absolute,
		1, teleport->sound};
	static int glid[] = {gl_Gui + tele_BlueHaze, gl_Gui + gui_Off,
		gl_Miscs + misc_Compass, gl_Gui + rotation_Ahead,
		gl_Gui + gui_Sound};
	int second[] = {0, teletile->open, 0, teleport->rotation, 0};
	unsigned int i = 0;
	drawPositionStack (gl_Gui + tele_BlueHaze, 0, 1.0f);
	for (i = 0; i < 5; i++)
	{
		moveStack (1, 0);
		drawPositionStack (glid[i] + second[i], 0, 0.4 + 0.6 * lights[i]);
	}
	moveStack (-5, 0);
	if (getEditCursor (cursor_L) == teleport->ldest)
	drawDestinationFromCurrent ((char) teleport->xdest, (char) teleport->ydest, .1, .7, .9, 0, 1);
}

static void
displayTextObject (reference_p refp, float light)
{
	int wall = 0;
	int iGfxCreature = 0;
	text_p text = (text_p) getItem (refp);
	simple_actuator_p sact = (simple_actuator_p) getItem (refp);

	wall = ((getTile (getEditCursor(cursor_X), getEditCursor(cursor_Y), getEditCursor(cursor_L)))->type == tile_Wall)?1:0;

	if (SKULLKEEP == 0)
	{
		drawPositionStack (gl_x_Walls, 0, 1.0f);
		moveStack (1, 0);
		drawPositionStack (gl_Gui + text_Mouth, 0, 0.4 + 0.6 * text->silent);
		moveStack (-1, 0);
	}
	else if (SKULLKEEP == 1)
	{
		if (sact->ExtendedUsage == 1 || sact->ExtendedUsage == 19 || sact->ExtendedUsage == 21)
		{
			iGfxCreature = 1;
			drawPositionStack (gl_x_Monsters + sact->Graphism, 0, 1.0f);
		}

		if (iGfxCreature == 0)
		{
			if (wall)
				drawPositionStack (gl_x_Walls + sact->Graphism, 0, 1.0f);
			else
				drawPositionStack (gl_x_Floors + sact->Graphism, 0, 1.0f);
		}
	}
}

static void
displayActivatorObject (reference_p refp, float light)
{
	int iactid = refp->id;
	int editact = 0;
	tCompanionActuator* xact = NULL;
	actuator_p type = (actuator_p) getItem (refp);
	actuator_effect_p effect = (actuator_effect_p) (getItem (refp)+1);
	actuator_target_p target = (actuator_target_p) (getItem (refp)+2);
	float fEffectColor[3] = { .9, .4, .7 };
	int iEffect = effect->effect;
	int inactive = effect->inactive;

	int wall = ((getTile (getEditCursor(cursor_X), getEditCursor(cursor_Y), getEditCursor(cursor_L)))->type == tile_Wall)?1:0;
	
	xact = &xActuatorTable[iactid];

	if (wall)
	{
		if (effect->graphism != 0)
			drawPositionStack (gl_x_Walls + (getLevels ()[getEditCursor(cursor_L)]).walls[effect->graphism-1], 0, 1.0f);
		moveStack (1, 0);
		// display actuator metatype gfx
		drawPositionStack (gl_StaticSkullkeep + gl_WActuators + type->type, 0, (inactive?0.5f:1.0f));
		moveStack (1, 0);
		switch (type->type)
		{
			case actuator_wall_alcove_item:
			case actuator_wall_item:
			case actuator_wall_item_eater:
			case actuator_wall_weapon_shooter:
			case actuator_wall_item_eater_toggler:
			case actuator_wall_item_removal_toggler:
			//case actuator_wall_general_shooter:
			//case actuator_wall_double_general_shooter:
			case actuator_wall_item_exchange:
			case actuator_wall_item_eater_disappear:
			case actuator_dm2_wall_generic_item_toggler:	// DM2 key/item switch lock
			case actuator_dm2_wall_item_generator:
			case actuator_dm2_wall_alcove_item:
			case actuator_dm2_item_capture_from_creature:
				drawPositionStack (gfx_activation_items[type->value], 0, 1.0f);
				break;
			case actuator_wall_spell_shooter:
			case actuator_wall_double_spell_shooter:
				drawPositionStack (gl_Spells + type->value, 0, 1.0f);
				break;
			case actuator_dm2_wall_monster_generator:
				drawPositionStack (gl_x_Monsters + type->value, 0, 1.0f);
				break;
			case actuator_dm2_wall_champion_cell: // 0x7E DM2 champion cell
			case actuator_wall_champion_mirror: // 0x7F actuator_wall_champion_mirror
				drawPositionStack (gl_x_Portraits + type->value, 0, 1.0f);
				break;
		}
		// display actuator metatype gfx
		/*
		{
			int ioldstacksize = stack_object_size;
			
			if (effect->graphism != 0)
			{
				stack_object_size = stack_object_size/2;
				moveStackFloat (-0.75, .25);
				drawPositionStack (gl_StaticSkullkeep + gl_WActuators + type->type, 0, 1.0f);
				moveStackFloat (0.75, -.25);
			}
			else
				drawPositionStack (gl_StaticSkullkeep + gl_WActuators + type->type, 0, 1.0f);

			stack_object_size = ioldstacksize;
		}*/

	}
	else
	{
		if (effect->graphism != 0)
			drawPositionStack (gl_x_Floors + (getLevels ()[getEditCursor(cursor_L)]).floors[effect->graphism-1], 0, 1.0f);
		moveStack (1, 0);
		// display actuator metatype gfx
		drawPositionStack (gl_StaticSkullkeep + gl_FActuators + type->type, 0, (inactive?0.5f:1.0f));
		moveStack (1, 0);
		switch (type->type)
		{
			case actuator_floor_pad_item:
			case actuator_floor_carried_item:
			case actuator_dm2_item_capture_from_creature:
				drawPositionStack (gfx_activation_items[type->value], 0, 1.0f);
				break;
			case actuator_floor_monster_generator:
				drawPositionStack (gl_x_Monsters + type->value, 0, 1.0f);
				break;
		}

	}
	moveStack (1, 0);
	drawPositionStack (gl_Gui + gui_Sound, 0, 0.4 + 0.6*effect->sound);
	moveStack (-1, 0);
	if	(!isEditingTile() ||
			(isEditingTile() &&
				( *((short*) (getStackReference (getEditCursor (cursor_Stack)))) == (*(short*) refp))
			)
		)

	if ( isEditingTile() && *((short*) (getStackReference (getEditCursor (cursor_Stack)))) == (*(short*) refp))
		editact = 1;


	switch (iEffect)
	{
		//-- OPEN/SET
		case 0:
		case 4:
			fEffectColor[0] = .2f;
			fEffectColor[1] = .9f;
			fEffectColor[2] = .2;
			break;
		//-- CLOSE/CLEAR
		case 1:
		case 5:
			fEffectColor[0] = .9f;
			fEffectColor[1] = .2f;
			fEffectColor[2] = .2;
			break;
		//-- TOGGLE
		case 2:
		case 6:
			fEffectColor[0] = .2f;
			fEffectColor[1] = .2f;
			fEffectColor[2] = .9;
			break;
		//-- "NEEDS CONSTANT"
		case 3:
		case 7:
			fEffectColor[0] = .9f;
			fEffectColor[1] = .9f;
			fEffectColor[2] = .2;
			break;

	}

	if (!effect->inactive)
		drawDestinationFromCurrent ((char) target->xdest, (char) target->ydest, fEffectColor[0], fEffectColor[1], fEffectColor[2], 1, editact);
	moveStack (-2, 0);
}

static void
drawChested (reference_p refp, float light)
{
	short i = 0;
	short** currentref = (short **) &refp;
	while (**currentref != -2 && **currentref != -1)
	{
		moveStack (1, 0);
		drawPositionStack (gfx_x_objects[refp->category] + getItemType[refp->category](getItem (refp)), 0, 1.0f * light);
		refp = getNextItem (refp);
		i++;
	}
	moveStack ((char) -i, 0);
}

static void
displayMonsterObject (reference_p refp, float light)
{
	monster_p monster = (monster_p) getItem (refp);
	drawPositionStack (gl_x_Monsters + monster->type, 0, 1.0f * light);	
	if (monster->chested != -2)
		drawChested ((reference_p) &monster->chested, light);
}

static void
displayWeaponObject (reference_p refp, float light)
{
	int i;
	weapon_p weapon = (weapon_p) getItem (refp);
	char lights[3] = {weapon->cursed, weapon->poisoned, weapon->broken};
	int glid[3] = {gl_Gui + gui_Cursed, gl_Gui + gui_Poisoned, gl_Gui + gui_Broken};
	
	if (!SKULLKEEP)
		drawPositionStack (gl_Weapons + weapon->type, 0, 1.0f*light);
	else if (SKULLKEEP)
		drawPositionStack (gl_StaticSkullkeep + gl_Weapons + weapon->type, 0, 1.0f*light);

	for (i = 0; i < 3; i++)
	{
		moveStack (1, 0);
		drawPositionStack (glid[i], 0, (0.4 + 0.6 * lights[i]) * light);
	}
	moveStack ((char)-i, 0);
}

static void		
displayClothingObject (reference_p refp, float light)
{
	int i;
	clothing_p clothing = (clothing_p) getItem (refp);
	char lights[2] = {clothing->cursed, clothing->broken};
	int glid[2] = {gl_Gui + gui_Cursed, gl_Gui + gui_Broken};

	/*
	if (!SKULLKEEP)
		drawPositionStack (gl_Clothes + clothing->type, 0, 1.0f);
	else if (SKULLKEEP)
		drawPositionStack (gl_StaticSkullkeep + gl_Clothes + clothing->type, 0, 1.0f);*/
	drawPositionStack (gl_x_Clothes + clothing->type, 0, 1.0f*light);
	for (i = 0; i < 2; i++)
	{
		moveStack (1, 0);
		drawPositionStack (glid[i], 0, (0.4 + 0.6 * lights[i]) * light);
	}
	moveStack ((char)-i, 0);
}

static void
displayScrollObject (reference_p refp, float light)
{
	drawPositionStack (gl_Special + special_Scroll, 0, 1.0f * light);
}

static int
find_potion_power (int value)
{
	/* LO = 40 / MON = 240 */
	return (char) (value/40);
}

static void
displayPotionObject (reference_p refp, float light)
{
	potion_p potion = (potion_p) getItem (refp);
	drawPositionStack (gl_x_Potions + potion->type, 0, 1.0f * light);
	moveStack (1, 0);
	drawSizeSquare (gl_Powers + find_potion_power ((int) potion->power), __STD_STACK_SIZE__, 1.0f*light);
	moveStack (-1, 0);
}


static void
displayChestObject (reference_p refp, float light)
{
	chest_p chest = (chest_p) getItem (refp);
	if (SKULLKEEP)
	{
		char type = objectContainerGetType((short*)chest);
		drawPositionStack (gl_x_Containers + type, 0, 1.0f * light);
	}
	else
		drawPositionStack (gl_Special + special_Chest, 0, 1.0f * light);
	if (chest->chested != -2)
		drawChested ((reference_p) &chest->chested, light);
}


static void
displayMiscellaneousObject (reference_p refp, float light)
{
	misc_p misc = (misc_p) getItem (refp);

	drawPositionStack (gl_x_Miscs + misc->type, 0, 1.0f * light);
}

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
enum
{
	bank_Tiles = 0,
	bank_Doors = 1,
	bank_Ornates = 2,
	bank_Walls = 3,
	bank_Floors = 4,
	bank_Monsters = 5,
	bank_Weapons = 6,
	bank_Clothes = 7,
	bank_Potions = 8,
	bank_Miscs = 9, 
	bank_Spells = 10,
	bank_Activators = 11,
	bank_Portraits = 12,
	bank_Containers = 13,
};

static int conversion[] =
{
	bank_Doors, 0, 0, 0,
	bank_Monsters, bank_Weapons, bank_Clothes, 0,
	bank_Potions, bank_Containers, bank_Miscs, 0,
	0, 0, bank_Spells /* false */, 0, /* end for item categories */
	bank_Tiles, bank_Ornates, bank_Walls, bank_Floors,
	bank_Spells, bank_Portraits, bank_Activators, 0,
};

static void
displayActivatorGfxBar (int wall, int value, float scale)
{
	int i;
	level_p level = &getLevels()[getEditCursor(cursor_L)];
	int graphisms[][2] = { 
		{ gl_x_Floors, level->header.nFloors+1},
		{ gl_x_Walls, level->header.nWalls+1},
	};
	unsigned char *lists[] = { level->floors, level->walls };

	int half = graphisms[wall][1]/2;
	int max = graphisms[wall][1];

	moveToBottom ();
	
	moveSize (-half, 0, __STD_STACK_SIZE__*scale);
	for (i = 0; i < max; i++)
	{
		drawSizeSquare (graphisms[wall][0] + lists[wall][(i + value - half + max - 1)%max], __STD_STACK_SIZE__*scale, 1.0f);
		moveSize (1, 0, __STD_STACK_SIZE__*scale);
	}	
	moveToBottom ();
	drawFrame (__STD_STACK_SIZE__*scale, .5, 1, .5);


}

static void
displaySelectionBar (int bank, int value, float scale)
{
	int i;
	int graphisms[][2] = {
		{ gl_Tiles, tile_Max},
		{ gl_x_Doors, dt_Door_Max},
		{ gl_x_Ornates, dt_Ornate_Max},
		{ gl_x_Walls, dt_Wall_Max},
		{ gl_x_Floors, dt_Floor_Max},
		{ gl_x_Monsters, dt_Monster_Max},
		{ gl_Weapons, weapon_Max},
		{ gl_Clothes, clothing_Max},
		{ gl_Potions, potion_Max},
		{ gl_Miscs, misc_Max},
		{ gl_Spells, spell_Max},
		{ 0, item_actuators_Max}, // activation items
		{ gl_Portraits, portrait_Max},
		{ gl_x_Containers, dt_Containers_Max},
	};
	int half = graphisms[bank][1]/2;
	int max = graphisms[bank][1];

	// It changes when we are or not in DM2 dungeon
	graphisms[0][0] = gl_x_Tiles;
	graphisms[1][0] = gl_x_Doors;
	graphisms[2][0] = gl_x_Ornates;
	graphisms[3][0] = gl_x_Walls;
	graphisms[4][0] = gl_x_Floors;
	graphisms[5][0] = gl_x_Monsters;

	graphisms[6][0] = gl_x_Weapons;
	graphisms[7][0] = gl_x_Clothes;
	graphisms[8][0] = gl_x_Potions;
	graphisms[9][0] = gl_x_Miscs;
	//graphisms[10][0] = gl_x_Spells;
	graphisms[13][0] = gl_x_Containers;

	// MISSING CHEST/CONTAINER/MAP category ...

	if (SKULLKEEP && !TELOS)
	{
		dt_Wall_Max = 256;
		graphisms[1][1] = dt_Door_Max;
		graphisms[2][1] = dt_Ornate_Max;
		graphisms[5][1] = dt_Monster_Max;

		graphisms[6][1] = dt_Weapons_Max;
		graphisms[7][1] = dt_Clothing_Max;
		//graphisms[8][1] = potion_Max;
		graphisms[9][1] = dt_Misc_Max;

		graphisms[13][1] = dt_Containers_Max;
	}
	else if (TELOS)
	{
		dt_Wall_Max = 256;
		graphisms[1][1] = dt_Door_Max;
		graphisms[2][1] = dt_Ornate_Max;
		graphisms[5][1] = 254;

		graphisms[6][1] = dt_Weapons_Max;
		graphisms[7][1] = dt_Clothing_Max;
		//graphisms[8][1] = potion_Max;
		graphisms[9][1] = dt_Misc_Max;

		graphisms[13][1] = dt_Containers_Max;
	}
	else
	{
		graphisms[1][1] = door_Max;
		graphisms[2][1] = ornate_Max;
		graphisms[5][1] = monster_Max;

		graphisms[6][1] = weapon_Max;
		graphisms[7][1] = clothing_Max;
		//graphisms[8][1] = potion_Max;
		graphisms[9][1] = misc_Max;

		graphisms[13][1] = 0;
	}


	//--- Reset half and max
	half = graphisms[bank][1]/2;
	max = graphisms[bank][1];

	switch (getScreen ())
	{
	//moveToBottom ();
	// for map view, item cycling
		case screen_Map:
		{
			moveToUpperScreen ();
			moveSize (14.25f, 8.5f, __STD_STACK_SIZE__*scale);
			moveSize (-half, 0, __STD_STACK_SIZE__*scale);
			drawFrameXY (100, 100, .9, .9, .7); // does not appear
			for (i = 0; i < max; i++)
			{
				if (bank == bank_Activators)
					drawSizeSquare (gfx_activation_items[(i + value - half + max)%max], __STD_STACK_SIZE__*scale, 1.0f);
				drawSizeSquare (graphisms[bank][0] + (i + value - half + max)%max, __STD_STACK_SIZE__*scale, 1.0f);
				moveSize (1, 0, __STD_STACK_SIZE__*scale);
			}	
			moveToUpperScreen ();
			moveSize (14.25f, 8.5f, __STD_STACK_SIZE__*scale);
			drawFrame (__STD_STACK_SIZE__*scale, .5, 1, .5);
			drawFrameLW (__STD_STACK_SIZE__, 1.*globalfsinv, 1.*globalfsinv, 0.25*globalfsinv, 4.f);
		}
		break;
		case screen_Level:
		{
			int midoffset = 7;
			double fsinv = 0;
			double rad = 0;
			if (angle > 360)
				angle = angle%360;
			rad = ((double)angle) / 360 * (2*3.1415f) * 1;
			fsinv = (double)cos(rad);
			fsinv = (fsinv/2) + 0.5f;
			globalfsinv = fsinv;

			moveToUpperScreen ();
			//moveSize (half/2, 0, __STD_STACK_SIZE__*scale); // move half screen
			//moveSize (-half, 3, __STD_STACK_SIZE__*scale);
			moveSize (7 - half, 3, __STD_STACK_SIZE__*scale);
			for (i = 0; i < max; i++)
			{
				//drawSizeSquare (graphisms[bank][0] + (i + value - half + max)%max, __STD_STACK_SIZE__*scale, 1.0f);
				drawSizeSquare (graphisms[bank][0] + (i + value - half + max)%max, __STD_STACK_SIZE__*scale, 1.0f);
				moveSize (1, 0, __STD_STACK_SIZE__*scale);
			}
			moveToUpperScreen ();
			moveSize (7, 3, __STD_STACK_SIZE__*scale);
			drawFrame (__STD_STACK_SIZE__*scale, .5, 1, .5);
			drawFrameLW (__STD_STACK_SIZE__*scale, 1.*globalfsinv, 1.*globalfsinv, 0.25*globalfsinv, 8.f);
		}
		break;
	}
}

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------

static void
drawPit (tile_pit_p pit, float light)
{
	int sdt = 0;	// special delta tile

	if (SKULLKEEP)
		sdt = -(xtile_DM1_SpecialOrnate_Max - tile_Max +1);

	if (!pit->open)
		drawBasicTile (gl_x_Tiles + sdt + xtile_ClosedPit, light);
	else if (pit->imaginary && pit->invisible)
		drawBasicTile (gl_x_Tiles + sdt + xtile_ImaginaryHiddenPit, light);
	else if (pit->imaginary)
		drawBasicTile (gl_x_Tiles + sdt + xtile_ImaginaryPit, light);
	else if (pit->invisible)
		drawBasicTile (gl_x_Tiles + sdt + xtile_HiddenPit, light);
	else
		drawBasicTile (gl_x_Tiles + tile_Pit, light);
}					

static void
drawStairs (int m, int x, int y, tile_p tile, float light)
{
	int iRotation = 0;
	int iFacingNorthSouth = 0;
	int iFacingWestEast = 0;
	tile_stairs_p stairs = (tile_stairs_p) tile;
	int iTileTypeLeftTop = tile_Wall;	// defaut as wall, because out of bond tile is a wall
	int iTileTypeRightBottom = tile_Wall;

	//drawPositionTile (gl_x_Tiles + tile->type, (char) stairs->facing, light);
	// default rotation 0 is W/E with darkest part on the left, lightest part on the right.
	// si 0 is a native W/E going down from EAST, or W/E going up from WEST
	// FACING = 0 means NORTH/SOUTH, and FACING = 1 means WEST/EAST

	if (stairs->facing == 1)	// north/south
		iFacingNorthSouth = 1;
	else if (stairs->facing == 0)	// west/east
		iFacingWestEast = 1;


	if (iFacingNorthSouth == 1)	// north/south
	{
		if (y-1 >= 0)
			iTileTypeLeftTop = (int)(getTile ((char)x, (char)(y-1), (char)m))->type;
		if (y+1 < 32)
			iTileTypeRightBottom = (int)(getTile ((char)x, (char)(y+1), (char)m))->type;

		if (stairs->leading == 1 && iTileTypeRightBottom == tile_Wall && iTileTypeLeftTop != tile_Wall)
			iRotation = 1;
		else if (stairs->leading == 1 && iTileTypeLeftTop == tile_Wall && iTileTypeRightBottom != tile_Wall)
			iRotation = 3;
		else if (stairs->leading == 0 && iTileTypeLeftTop == tile_Wall && iTileTypeRightBottom != tile_Wall)
			iRotation = 1;
		else if (stairs->leading == 0 && iTileTypeRightBottom == tile_Wall && iTileTypeLeftTop != tile_Wall)
			iRotation = 3;
	}
	else if (iFacingWestEast == 1)	// west/east
	{
		if (x-1 >= 0)
			iTileTypeLeftTop = (int)(getTile ((char)(x-1), (char)y, (char)m))->type;
		if (x+1 < 32)
			iTileTypeRightBottom = (int)(getTile ((char)(x+1), (char)y, (char)m))->type;

		if (stairs->leading == 1 && iTileTypeLeftTop == tile_Wall && iTileTypeRightBottom != tile_Wall)
			iRotation = 2;
		else if (stairs->leading == 1 && iTileTypeRightBottom == tile_Wall && iTileTypeLeftTop != tile_Wall)
			iRotation = 0;
		else if (stairs->leading == 0 && iTileTypeRightBottom == tile_Wall && iTileTypeLeftTop != tile_Wall)
			iRotation = 2;
		else if (stairs->leading == 0 && iTileTypeLeftTop == tile_Wall && iTileTypeRightBottom != tile_Wall)
			iRotation = 0;
	}

	// Neighbors tile have to be tested to know on which direction to show the stairs!

	drawPositionTile ((int)(gl_x_Tiles + tile->type), (char)iRotation, light);
	drawStairsArrow (stairs->leading);	// up = 1 / down = 0
}

static void
drawTrickWalls (tile_trickwall_p wall, float light)
{
	int sdt = 0;	// special delta tile

	if (SKULLKEEP)
		sdt = -(xtile_DM1_SpecialOrnate_Max - tile_Max +1);

	if (wall->open)
		drawBasicTile (gl_x_Tiles + sdt + xtile_TrickWallOpened, light);
	else if (!wall->open && !wall->imaginary)
		drawBasicTile (gl_x_Tiles + sdt + xtile_TrickWallClosed, light);
	else if (wall->imaginary)
		drawBasicTile (gl_x_Tiles + tile_Trickwall, light);

}

static void
drawStack (char x, char y, unsigned char level)
{
	int iStackIndex = 0;
	float fLightScale = 1.f;	// when some selection, light will be low to allow other panels over
	static displayObject objectStats[] =
	{ 
		displayDoorObject, displayTeleportObject, displayTextObject, displayActivatorObject,
		displayMonsterObject, displayWeaponObject, displayClothingObject, displayScrollObject,
		displayPotionObject, displayChestObject, displayMiscellaneousObject, displayNoObject,
		displayNoObject, displayNoObject, displayNoObject, displayNoObject
	};
	reference_p refp = getGroundReference (x, y, level);
	short** currentref = (short **) &refp;
	int wall = ((getTile (x, y, level))->type == tile_Wall)?1:0;

	iCurrentStackCount = 0;
	moveToStackUpper ();

	while (**currentref != -2 && **currentref != -1)
	{
		short *item = getItem(refp);
		if (iStackIndex >= 5 && isSelectingNewItem ())	// so that new panel is readable
			fLightScale = 0.12f;
		//else if (iStackIndex >= 8) // so that online help panel is readable
		//	fLightScale = 0.12f;
		moveStack (-2, 0);
		//printf("Stack = %d / X= %d / Y=%d\n", iStackIndex, iGLVirtualX, iGLVirtualY);
		setTextProperties (12, fLightScale, fLightScale, fLightScale);
		fontDrawString (iGLVirtualX, iGLVirtualY, "%02d", iStackIndex);
		moveStack (1, 0);
		drawPositionStack (gl_Gui + wall, (char) refp->position, fLightScale);
		moveStack (1, 0);
		objectStats[refp->category](refp, fLightScale);

		if (refp->category == category_Weapon)
			text_frame_weapon (refp, 0, iStackIndex, fLightScale);
		else if (refp->category == category_Clothing)
			text_frame_clothing (refp, 0, iStackIndex, fLightScale);
		else if (refp->category == category_Potion)
			text_frame_potion (refp, 0, iStackIndex, fLightScale);
		else if (refp->category == category_Chest)
			text_frame_container (refp, 0, iStackIndex, fLightScale);
		else if (refp->category == category_Miscs)
			text_frame_misc (refp, 0, iStackIndex, fLightScale);
		else if (refp->category == category_Scroll)
			text_frame_scroll (refp, 0, iStackIndex, fLightScale);
		else if (refp->category == category_Text)
			text_frame_text (refp, 0, iStackIndex, fLightScale);
		else if (refp->category == category_Monster)
			text_frame_monster (refp, 0, iStackIndex, fLightScale);

		else if (refp->category == category_Text)
		{
			if (SKULLKEEP == 1)
			{
				text_frame_simple_actuator(refp, 0, iStackIndex, fLightScale);
			}
		}

		moveStack (0, 1);
		refp = getNextItem (refp);
		iStackIndex++;
		iCurrentStackCount++;
	}

	if (isEditingTile ())
	{
		int iEditStackIndex = getEditCursor (cursor_Stack);
		reference_p selected = (getStackReference (getEditCursor (cursor_Stack)));
		short *item = getItem(selected);

		double fsinv = 0;
		double rad = 0;
		if (angle > 360)
			angle = angle%360;
		rad = ((double)angle) / 360 * (2*3.1415f) * 1;
		fsinv = (double)cos(rad);
		fsinv = (fsinv/2) + 0.5f;
		globalfsinv = fsinv;

		moveToStackUpper ();
		moveStack ((char) isSecondFunction(), getEditCursor (cursor_Stack));
		drawFrameLW (__STD_STACK_SIZE__, 1, 1, .1, 3.f);
		drawFrameLW (__STD_STACK_SIZE__, -.25*fsinv, 1.*fsinv, 1.*fsinv, 4.f);
		if (selected->category == category_Actuator)
		{
			if (isSecondFunction())
			{
				if (wall){
				switch (((actuator_p) (item))->type)
				{
					case actuator_wall_champion_mirror /*actuator_wall_champion_mirror*/:
						displaySelectionBar (bank_Portraits, ((actuator_p) (item))->value, 1);
						break;
					case actuator_wall_alcove_item:
					case actuator_wall_item:
					case actuator_wall_item_eater:
					case actuator_wall_item_eater_toggler:
					case actuator_wall_item_removal_toggler:
					case actuator_wall_general_shooter:
					case actuator_wall_double_general_shooter:
					case actuator_wall_item_exchange:
					case actuator_wall_item_eater_disappear:
					case actuator_dm2_item_capture_from_creature:
						displaySelectionBar (bank_Activators, ((actuator_p) (item))->value, 1);
						break;
					case actuator_wall_spell_shooter:
					case actuator_wall_double_spell_shooter:
						displaySelectionBar (bank_Spells, ((actuator_p) (item))->value, 1);
						break;
					}
		
				}
				else
				{
					switch (((actuator_p) (item))->type)
					{
						case actuator_floor_pad_item:
						case actuator_floor_carried_item:
						case actuator_dm2_item_capture_from_creature:
							displaySelectionBar (bank_Activators, ((actuator_p) (item))->value, 1);
							break;
						case actuator_floor_monster_generator:
							displaySelectionBar (bank_Monsters, ((actuator_p) (item))->value, 1);
							break;
					}
				}

			}
			else
				displayActivatorGfxBar (wall, ((actuator_effect_p) (item+1))->graphism, 1);
		}
		else if (selected->category == category_Text)
		{	
			//--- If DM1/CSB/TQ, this is a normal text
			if (SKULLKEEP == 0)
			{
				displaySelectedTextList (((text_p) (item))->offset);
				shadowmap = 1;
			}
			//--- If DM2/SK, it can be a Simple Actuator
			else if (SKULLKEEP == 1)
			{
				text_frame_simple_actuator(selected, 0, 0, fLightScale);
				if (wall)
					displaySelectionBar (bank_Walls,	simpleActuatorGetGraphism(item), 1);
				else
					displaySelectionBar (bank_Floors,	simpleActuatorGetGraphism(item), 1);
			}
		
		}
		else if (selected->category == category_Teleport)
			text_frame_teleport (selected, 0, iEditStackIndex, fLightScale);
		else if (selected->category == category_Weapon)
		{
			text_frame_weapon (selected, 0, iEditStackIndex, fLightScale);
			displaySelectionBar (conversion[selected->category],
			getItemType[selected->category] (item), 1);
		}
		else if (selected->category == category_Clothing)
		{
			text_frame_clothing (selected, 0, iEditStackIndex, fLightScale);
			displaySelectionBar (conversion[selected->category],
			getItemType[selected->category] (item), 1);
		}/*
		else if (selected->category == category_Scroll)
		{
			text_frame_potion (selected, 0, 0);
			displaySelectionBar (conversion[selected->category],
			getItemType[selected->category] (item), 1);
		}*/
		else if (selected->category == category_Potion)
		{
			text_frame_potion (selected, 0, iEditStackIndex, fLightScale);
			displaySelectionBar (conversion[selected->category],
			getItemType[selected->category] (item), 1);
		}
		else if (selected->category == category_Chest)
		{
			text_frame_container (selected, 0, iEditStackIndex, fLightScale);
			displaySelectionBar (conversion[selected->category],
			getItemType[selected->category] (item), 1);
		}
		else if (selected->category == category_Miscs)
		{
			text_frame_misc (selected, 0, iEditStackIndex, fLightScale);
			displaySelectionBar (conversion[selected->category],
			getItemType[selected->category] (item), 1);
		}
		else if (selected->category == category_Monster)
		{	
			text_frame_monster (selected, 0, iEditStackIndex, fLightScale);
			displaySelectionBar (conversion[selected->category],
			getItemType[selected->category] (item), 1);
		}
		else if (selected->category == category_Scroll)
		{	
			displaySelectedTextList ( ((scroll_p) (item))->offset);
			shadowmap = 1;
		}
		else
			displaySelectionBar (conversion[selected->category],
			getItemType[selected->category] (item), 1);
	}
}

static void
drawObject (char x, char y, unsigned char level, int edittext)
{
	static float lights[] = { 1.0, .5}; /* no shadow / shadow */
	reference_p refp = getGroundReference (x, y, level);
	short** currentref = (short **) &refp;
	int wall = ((getTile (x, y, level))->type == tile_Wall)?1:0;

	if (!edittext){ /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
	while (**currentref != -2 && **currentref != -1)
	{

		/* door and teleport spots will be not displayed */
		if (refp->category == category_Door)
		{
			int doortype = 0;
			tile_door_p tile = (tile_door_p) getTile (x, y, level);
			door_p door = (door_p) getItem (refp);
			doortype = ((door->type)?getLevels()[getEditCursor(cursor_L)].header.door2:
									getLevels()[getEditCursor(cursor_L)].header.door1);
			drawDoor (tile->facing, (char) door->button, (char) !tile->closed, (char) ((tile->function == 1) && tile->closed), doortype);
		}
		
		if (refp->category == category_Actuator)
			drawActivator (refp, wall, level);
		if (refp->category == category_Monster)
		{	
			glEnable (GL_BLEND);
			drawSizeSquare (gl_x_Monsters + ((monster_p) getItem (refp))->type, tile*2.0, lights[shadowmap]);
		}
		if (refp->category != category_Door && refp->category != category_Teleport)
			drawItemSpot ((char) (refp->position), wall, spot_colors[refp->category][0],
			spot_colors[refp->category][1],
			spot_colors[refp->category][2], lights[shadowmap]);

		if (getNumber ((char) refp->category) <= refp->id)
		{
			glBlendFunc(GL_DST_COLOR,GL_ZERO);
			drawMaskedTexture (0, 0, 0, tile, .5f);
			//displayColorQuad (0, tile, 1, 0, 0);
			//drawSizeSquare (0, tile*2, 1.0f);
		}
		refp = getNextItem (refp);
	}}
}

static void
drawMapObject (unsigned char level, int edittext)
{
	char i, j;
	moveToUpperMap ();
	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 32; i++)
		{
			drawObject (i, j, level, edittext);
			move (1, 0);
		}
		move (-i, 1);
	}

}

static void
drawMap (unsigned char level, int edittext)
{
	char i, j;

	//--- If in Level Header screen, the map is put at another place, and drawn at a lower scale

	moveToUpperMap ();
	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < 32; i++)
		{
			float light = 1.0f;
			tile_p tile = getTile (i, j, level);

			if (i >= (getLevels()[level]).header.xDim+1 ||
				j >= (getLevels()[level]).header.yDim+1 ||
				shadowmap == 1)
				light = .50;
			if (edittext)
				light = .25;

			if (tile->type == tile_Pit)
				drawPit ((tile_pit_p) tile, light);
			else if (tile->type == tile_Stairs)
			{
				int selectext = 
				((isEditingTile ()) && ((getCurrentItemReference ()->category == category_Text)
				|| (getCurrentItemReference()->category == category_Scroll)))?1:0;
					if (!selectext)
				drawStairs (level, i, j, tile, light);
			}
			else if (tile->type == tile_Trickwall)
				drawTrickWalls ((tile_trickwall_p) tile, light);
			else
				drawBasicTile (gl_x_Tiles + tile->type, light);
			move (1, 0);
		}
		move (-i, 1);
	}
}

static void
drawLevelPropertiesHelpInfo ()
{
	//--- bottom right, to display online help
	moveToUpperScreen ();

	{
		moveSize (56, 32, 48);
		drawFrameXY (1416, 700, .9, .9, .7);
		printMapPropertiesHelpInfo ();
	}
/*
	if (isSelectingNewItem ())
	{
		drawNewObjectSelectionInfo ();
		printNewObjectHelpInfo ();
	}
	*/
}

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------

static void
drawNewObjectSelectionInfo ()
{
	moveToUpperScreen ();
	moveSize (52, 20, 48);
	drawFrameXY (1800, 350, .9, .9, .7);
}


static void
drawMapHelpInfo ()
{
	//--- bottom right, to display online help
	moveToUpperScreen ();

	if (iCurrentStackCount <= 0)
	{
		moveSize (52, 32, 48);
		drawFrameXY (1800, 700, .9, .9, .7);
		printMainMapHelpInfo ();
	}
	else
	{
		moveSize (56, 32, 48);
		drawFrameXY (1416, 700, .9, .9, .7);
		printMainMapHelpInfo ();
	}

	if (isSelectingNewItem ())
	{
		drawNewObjectSelectionInfo ();
		printNewObjectHelpInfo ();
	}
}




static void 
drawMapGraphics (char map)
{
	char i, j;
	level_p level = (level_p) &(getLevels()[map]);
	char maxtable[] = {
		level->header.nMonsters, level->header.nWalls,
		level->header.nFloors, level->header.nOrnates
	};
	//static int glid[] = { gl_Monsters, gl_Walls, gl_Floors, gl_Ornates};
	int glid[] = { gl_x_Monsters, gl_x_Walls, gl_x_Floors, gl_x_Ornates};
	unsigned char* lists[] = {level->monsters, level->walls, level->floors, level->ornates};
	
	static float gfxsize = 96; // 48
	

	moveToGraphicsList ();
	for (j = 0; j < 4; j++)
	{	
		for (i = 0; i < maxtable[j]; i++)
		{
			drawSizeSquare (glid[j] + lists[j][i], gfxsize, 1.0f);
			moveSize (1, 0, gfxsize);
		}
		for (;i < 16; i++)
		{
			drawFrame (gfxsize, .25f, .25f, .25f);
			moveSize (1, 0, gfxsize);
		}
		moveSize (-i, 1, gfxsize);
	}
	drawSizeSquare (gl_x_Doors + level->header.door1, gfxsize, 1.0f);
	moveSize (1, 0, gfxsize);
	drawSizeSquare (gl_x_Doors + level->header.door2, gfxsize, 1.0f);
}

static void
drawMapFrame (int edittext)
{
	moveToCenterMap ();
	if (!edittext)
		drawFrame (tileScale*32.2, .7, .7, .7);
	else
		drawFrame (tileScale*32.2, .2, .2, .2);
}

static void
drawMapCursor (int stat)
{
	static float cursor_colors[][3] =
	{ {.4, 1, .6}, {1, .3, .2}};
	float blink = 1.f;

	double fsinv = 0;
	double rad = 0;
	if (angle > 360)
		angle = angle%360;
	rad = ((double)angle) / 360 * (2*3.1415f) * 1;
	fsinv = (double)cos(rad);
	fsinv = (fsinv/2) + 0.75f;
	if (isSelectingNewItem ())
		blink = fsinv;

	moveToUpperMap ();
	move (getEditCursor (cursor_X), getEditCursor (cursor_Y));
	//if (getEditCursor (cursor_L) == getEditCursor (cursor_TM))
	drawFrame (tileScale, cursor_colors[stat][0]*blink, cursor_colors[stat][1]*blink, cursor_colors[stat][2]*blink);
}

static void
drawTargetEditCursor (int stat)
{
	static float cursor_colors[][3] =
	{ {.2, .9, 1}, {.9, .9, .2}};
	moveToUpperMap ();

	if (getEditCursor (cursor_L) == getEditCursor (cursor_TM))
	drawDestinationFromCurrent (getEditCursor (cursor_TX), getEditCursor (cursor_TY),
		.1, 1, .2, 0, 1);
	move (getEditCursor (cursor_TX), getEditCursor (cursor_TY));
	drawFrame (tileScale, cursor_colors[stat][0], cursor_colors[stat][1], cursor_colors[stat][2]);
}


static void
drawStartingCursor ()
{
	moveToUpperMap ();
	move (getDungeon()->x_start, getDungeon()->y_start);
	drawStartingPosition ((char) getDungeon()->f_start);
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

static void
displayLevelWindow ()
{
	int iAdjustX = 0;
	unsigned char i = 0;
	unsigned char j = 0;
	int iFntSize = 20;
	level_p level = (level_p) &(getLevels()[getEditCursor(cursor_L)]);
	unsigned char maxtable[] = {
		level->header.nMonsters
		, level->header.nWalls
		, level->header.nFloors
		, level->header.nOrnates
	};
	int glid[] = { gl_x_Monsters, gl_x_Walls, gl_x_Floors, gl_x_Ornates};
	unsigned char doors[] = { level->header.door1, level->header.door2};
	unsigned char* lists[] = {level->monsters, level->walls, level->floors, level->ornates, doors};
	unsigned char* cattxt[] = { "CR", "WL", "FL", "DD", "DR" };
	unsigned char randoms[] = { 0, level->header.rWalls, level->header.rFloors, 0};
	static float gfxsize = 128;
	static int conv[] = { 5, 3, 4, 2, 1};	// monsters, walls, floors, door ornates, door
	char gc = getEditCursor(cursor_Graphics);
	char ic = getEditCursor(cursor_Index);
	int iMapID = getEditCursor(cursor_L);

	setTextProperties (iFntSizeBigTitle, .5, 1, .8); 
	outputTextLineAt (100, winH-40, "F2:    MAP #%02d/%02d PROPERTIES", iMapID, getDungeon()->nLevels-1);

	moveToUpperScreen ();
	moveSize (.25f, -.25f, gfxsize);
	for (i = 0; i < 15; i++)
	{
		setTextProperties (iFntSize, .8, .8, .8);
		fontDrawString (iGLVirtualX+iAdjustX, iGLVirtualY, "%1X", i+1);
		moveSize (1, 0, gfxsize);
	}
	moveSize (-i-.25f, +.25f, gfxsize);
	moveSize (0.5f, 0.5f, gfxsize);
	//--- Display MONSTERS / WALLS / FLOORS / WALL ORNATES
	for (j = 0; j < 4; j++)
	{	
		moveSize (-1.5f, 0, gfxsize);
		setTextProperties (iFntSize, 1, 0, 0);
		fontDrawString (iGLVirtualX+1+iAdjustX, iGLVirtualY-1, "%02s", cattxt[j]);
		moveSize (1.5f, 0, gfxsize);

		for (i = 0; i < maxtable[j]; i++)
		{
			int iObjectID = 0;
			iObjectID = lists[j][i];
			drawSizeSquare (glid[j] + iObjectID, gfxsize, ((i<randoms[j])?.65f:1.0f));

			// Display value over
			//--- Shadowed text
			iAdjustX = -iFntSize;
			setTextProperties (iFntSize, 0, 0, 0);
			fontDrawString (iGLVirtualX+1+iAdjustX, iGLVirtualY-1, "%02X", iObjectID);
			setTextProperties (iFntSize, .9, .9, .9);
			fontDrawString (iGLVirtualX+iAdjustX, iGLVirtualY, "%02X", iObjectID);
			iAdjustX = 0;

			moveSize (1, 0, gfxsize);
		}
		for (i = maxtable[j]; i < 15; i++)
		{
			drawFrame (gfxsize, .20, .20, .20);
			moveSize (1, 0, gfxsize);
		}
		moveSize (-i, 1, gfxsize);
	}
	
	// Door txt
	moveSize (-1.5f, 0, gfxsize);
	setTextProperties (iFntSize, 1, 0, 0);
	fontDrawString (iGLVirtualX+1+iAdjustX, iGLVirtualY-1, "%02s", cattxt[4]);
	moveSize (1.5f, 0, gfxsize);


	//--- Display DOORS
	drawSizeSquare (gl_x_Doors + level->header.door1, gfxsize, 1.0f);
	moveSize (1, 0, gfxsize);
	drawSizeSquare (gl_x_Doors + level->header.door2, gfxsize, 1.0f);

	moveToUpperScreen ();
	moveSize (0.5f, 0.5f, gfxsize);
	moveSize (ic, gc, gfxsize);
	if (isEditingGraphics())
	{	
		drawFrame (gfxsize, .9, .25, .3);
		displaySelectionBar (conv[gc], lists[gc][ic], 2.);
	}
	else
		drawFrame (gfxsize, .1, 1, .4);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void
displayCreaturesLists ()
{
	dm_reference ref;
	reference_p refalive;
	unsigned int nbmonsters = getNumber (category_Monster);
	unsigned int id = 0;
	unsigned int j = 0;
	unsigned int maxid = 27;
	int iCounterPerType[256];
	float size = __STD_STACK_SIZE__*0.75f;
	ref.category = category_Monster;
	ref.position = 0;

	setTextProperties (iFntSizeBigTitle, .5, 1, .8); 
	if (SKULLKEEP)
		outputTextLineAt (200, winH-40, "F3:  CREATURES & OBJECTS LIST : #%03d", nbmonsters);
	else
		outputTextLineAt (200, winH-40, "F3:    CREATURES LIST : #%03d", nbmonsters);


	for (id = 0; id < 256; id++)
		iCounterPerType[id] = 0;

	moveToUpperMap ();
	for (j = 0; j < 16; j++)
	{
		for (id = 0; id < 16; id++)
		{
			monster_p monster;
			ref.id = id+j*16;
			if (ref.id <= 1022)
			{
				monster = (monster_p) getItem (&ref);
				refalive = getNextItem (&ref);
				iCounterPerType[monster->type]++;
				if (ref.id < nbmonsters)
				{
					drawSizeSquare (gl_x_Monsters + monster->type, size, ((refalive->raw == 0xFFFF)?0.25f:1.0f));
					if (refalive->raw == 0xFFFF)
						drawSizeSquare (gl_Gui + gui_Poisoned, size/2, .5f);
				}
				else
					drawFrame (size, .10, .10, .10);
			}
			moveSize (0, 1, size);
		}
		moveSize (1, -16, size);
	}
	//--- Display quantity per type
	moveToUpperMap ();
	moveSize (18, 0, size);
	if (SKULLKEEP)
		maxid = 0x58;
	for (id = 0; id < maxid; id++)
	{
		if (id%16 == 0 && id != 0)
		{
			moveSize (3, -16, size);
		}
		drawSizeSquare (gl_x_Monsters + id, size, 1.0f);
		moveSize (1, 0, size);
		if (iCounterPerType[id] == 0)
			setTextProperties (14, .3, .5, .4);
		else
			setTextProperties (14, .5, 1, .8);
		fontDrawString (iGLVirtualX, iGLVirtualY-7, "%02d", iCounterPerType[id]);
		moveSize (-1, 1, size);
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void
displayAIInfos ()
{
	int iExeSizeLoaded = 0;

	setTextProperties (iFntSizeBigTitle, .5, 1, .8); 
	outputTextLineAt (200, winH-40, "AI STATS INFO :");

	//
	//iExeSizeLoaded = xSkullExe.iExeDataSize;
	//outputTextLineAt (200, winH-70, "Size of Skull.exe : %06d", iExeSizeLoaded);
	printAIDetailsInfos();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void
displayItemsLists ()
{
	dm_reference ref;
	unsigned int iNbWeapons = getNumber (category_Weapon);
	unsigned int iNbClothings = getNumber (category_Clothing);
	unsigned int iNbScrolls = getNumber (category_Scroll);
	unsigned int iNbPotions = getNumber (category_Potion);
	unsigned int iNbChests = getNumber (category_Chest);
	unsigned int iNbMiscs = getNumber (category_Miscs);
	unsigned int id = 0;
	unsigned int j = 0;
	unsigned int iNbTotalItems = 0;
	unsigned int iNbMaxCols = 16;
	unsigned int iNbPerCol = 16;
	float fNbPerCol = 16.f;
	int iCounterPerType[128];
	float size = __STD_STACK_SIZE__*0.75f;
	float textcatsize = 12.f;
	ref.category = category_Weapon;
	ref.position = 0;

	iNbTotalItems = iNbWeapons + iNbClothings + iNbScrolls + iNbPotions + iNbChests + iNbMiscs;
	setTextProperties (iFntSizeBigTitle, .5, 1, .8); 
	outputTextLineAt (200, winH-40, "F4:    ITEMS LIST : #%03d", iNbTotalItems);

	if (iNbTotalItems > 512)
	{
		size = __STD_STACK_SIZE__*0.5f;
		textcatsize = 8.f;
		iNbPerCol = 24;
		fNbPerCol = 24.f;
	}
	if (iNbTotalItems >= 1024)
	{
		size = __STD_STACK_SIZE__*0.25f;
		textcatsize = 6.f;
		iNbPerCol = 32;
		fNbPerCol = 32.f;
	}

	for (id = 0; id < 128; id++)
		iCounterPerType[id] = 0;

	moveToUpperMap ();
	moveSize (-1, 0, size);
	setTextProperties (textcatsize, .8, .8, .2); 
	fontDrawString (iGLVirtualX, iGLVirtualY-7, "SCROLLS");
	moveSize (1, 0, size);

	moveSize (0, 1, size);
	iNbMaxCols = (iNbScrolls / iNbPerCol) + 1;
	ref.category = category_Scroll;
	ref.position = 0;
	for (j = 0; j < iNbMaxCols; j++)
	{
		for (id = 0; id < iNbPerCol; id++)
		{
			scroll_p item;
			ref.id = id+j*iNbPerCol;
			if (ref.id <= 1023)
			{
				item = (scroll_p) getItem (&ref);
				//iCounterPerType[item->type]++;
				drawSizeSquare (gl_Special + special_Scroll, size, ((ref.id>=iNbScrolls)?.01f:1.0f));
			}
			moveSize (0, 1, size);
		}
		moveSize (1, -fNbPerCol, size);
	}

	moveSize (0, -1, size);
	setTextProperties (textcatsize, .8, .8, .2); 
	fontDrawString (iGLVirtualX, iGLVirtualY-7, "WEAPONS");
	moveSize (0, 1, size);

	moveSize (1, 0, size);
	iNbMaxCols = (iNbWeapons / iNbPerCol) + 1;
	ref.category = category_Weapon;
	ref.position = 0;
	for (j = 0; j < iNbMaxCols; j++)
	{
		for (id = 0; id < iNbPerCol; id++)
		{
			weapon_p item;
			ref.id = id+j*iNbPerCol;
			if (ref.id <= 1023)
			{
				item = (weapon_p) getItem (&ref);
				iCounterPerType[item->type]++;
				drawSizeSquare (gl_x_Weapons + item->type, size, ((ref.id>=iNbWeapons)?.01f:1.0f));
			}
			moveSize (0, 1, size);
		}
		moveSize (1, -fNbPerCol, size);
	}

	moveSize (0, -1, size);
	setTextProperties (textcatsize, .8, .8, .2); 
	fontDrawString (iGLVirtualX, iGLVirtualY-7, "CLOTHINGS");
	moveSize (0, 1, size);

	moveSize (1, 0, size);
	ref.category = category_Clothing;
	ref.position = 0;
	iNbMaxCols = (iNbClothings / iNbPerCol) + 1;
	for (j = 0; j < iNbMaxCols; j++)
	{
		for (id = 0; id < iNbPerCol; id++)
		{
			clothing_p item;
			ref.id = id+j*iNbPerCol;
			if (ref.id <= 1023)
			{
				item = (clothing_p) getItem (&ref);
				iCounterPerType[item->type]++;
				drawSizeSquare (gl_x_Clothes + item->type, size, ((ref.id>=iNbClothings)?.01f:1.0f));
				moveSize (0, 1, size);
			}
		}
		moveSize (1, -fNbPerCol, size);
	}

	moveSize (0, -1, size);
	setTextProperties (textcatsize, .8, .8, .2); 
	fontDrawString (iGLVirtualX, iGLVirtualY-7, "MISCS.");
	moveSize (0, 1, size);

	moveSize (1, 0, size);
	ref.category = category_Miscs;
	ref.position = 0;
	iNbMaxCols = (iNbMiscs / iNbPerCol) + 1;
	for (j = 0; j < iNbMaxCols; j++)
	{
		for (id = 0; id < iNbPerCol; id++)
		{
			misc_p item;
			ref.id = id+j*iNbPerCol;
			if (ref.id <= 1023)
			{
				item = (misc_p) getItem (&ref);
				iCounterPerType[item->type]++;
				drawSizeSquare (gl_x_Miscs + item->type, size, ((ref.id>=iNbMiscs)?.01f:1.0f));
				moveSize (0, 1, size);
			}
		}
		moveSize (1, -fNbPerCol, size);
	}

	moveSize (0, -1, size);
	setTextProperties (textcatsize, .8, .8, .2); 
	fontDrawString (iGLVirtualX, iGLVirtualY-7, "POTIONS");
	moveSize (0, 1, size);

	moveSize (1, 0, size);
	ref.category = category_Potion;
	ref.position = 0;
	iNbMaxCols = (iNbPotions / iNbPerCol) + 1;
	for (j = 0; j < iNbMaxCols; j++)
	{
		for (id = 0; id < iNbPerCol; id++)
		{
			potion_p item;
			ref.id = id+j*iNbPerCol;
			if (ref.id <= 1023)
			{
				item = (potion_p) getItem (&ref);
				iCounterPerType[item->type]++;
				drawSizeSquare (gl_x_Potions + item->type, size, ((ref.id>=iNbPotions)?.01f:1.0f));
				moveSize (0, 1, size);
			}
		}
		moveSize (1, -fNbPerCol, size);
	}

	moveSize (0, -1, size);
	setTextProperties (textcatsize, .8, .8, .2); 
	if (SKULLKEEP)
		fontDrawString (iGLVirtualX, iGLVirtualY-7, "CONT. & MAPS");
	else
		fontDrawString (iGLVirtualX, iGLVirtualY-7, "CHESTS");
	moveSize (0, 1, size);

	moveSize (1, 0, size);
	ref.category = category_Chest;
	ref.position = 0;
	iNbMaxCols = (iNbChests / iNbPerCol) + 1;
	for (j = 0; j < iNbMaxCols; j++)
	{
		for (id = 0; id < iNbPerCol; id++)
		{
			chest_p item;
			int type = 0;

			ref.id = id+j*iNbPerCol;
			if (ref.id <= 1023)
			{
				item = (chest_p) getItem (&ref);
				if (SKULLKEEP)
					type = objectContainerGetType((short*) item);
				else
					type = 0;

				iCounterPerType[type]++;
				if (SKULLKEEP)
					drawSizeSquare (gl_x_Containers + type, size, ((ref.id>=iNbChests)?.01f:1.0f));
				else
					drawSizeSquare (gl_Special + special_Chest, size, ((ref.id>=iNbChests)?.01f:1.0f));
				moveSize (0, 1, size);
			}
		}
		moveSize (1, -fNbPerCol, size);
	}



	//--- Display quantity per type
	//moveToUpperMap ();
	moveSize (2, 0, size);
	for (id = 0; id < 32; id++)
	{
		if (id%16 == 0 && id != 0)
		{
			moveSize (3, -16, size);
		}
		drawSizeSquare (gl_x_Weapons + id, size, 1.0f);
		moveSize (1, 0, size);
		if (iCounterPerType[id] == 0)
			setTextProperties (14, .3, .5, .4);
		else
			setTextProperties (14, .5, 1, .8);
		fontDrawString (iGLVirtualX, iGLVirtualY-7, "%02d", iCounterPerType[id]);
		moveSize (-1, 1, size);
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


void
displayActuatorsLists ()
{
	dm_reference ref;
	unsigned int iNbActuators = getNumber (category_Actuator);
	unsigned int id = 0;
	unsigned int j = 0;
	unsigned int iNbTotalItems = 0;
	int iCounterPerType[128];
	float size = __STD_STACK_SIZE__*0.75f;
	int iFntSize = 16;
	int iAdjustX = 0;
	int iAdjustY = 0;
	char fwtype = '?';
	int actmap = -1;
	int iStepDisplay = 16;
	int oldsize = size;
	int iNbPerCol = 16;
	int iNbCols = 16;
	tCompanionActuator* xact = NULL;

	ref.category = category_Actuator;
	ref.position = 0;

	iNbTotalItems = iNbActuators;
	setTextProperties (iFntSizeBigTitle, .5, 1, .8); 
	outputTextLineAt (200, winH-40, "ALL ACTUATORS LIST : #%03d ITEMS", iNbActuators);

	for (id = 0; id < 128; id++)
		iCounterPerType[id] = 0;

	moveToUpperMap ();
	if (iNbActuators > 256)
	{
		iStepDisplay = 32;
		iFntSize = 8;
		size = size/2;
		iNbPerCol = 32;
		iNbCols = 32;
	}
	for (j = 0; j < (unsigned int) iNbCols; j++)
	{
		for (id = 0; id < (unsigned int) iNbPerCol; id++)
		{
			actuator_p item = NULL;
			ref.id = id+j*iNbPerCol;
			xact = &xActuatorTable[ref.id];

			fwtype = '?';
			actmap = 0;

			if (ref.id <= 1022)
			{
				item = (actuator_p) getItem (&ref);
				iCounterPerType[item->type]++;
				drawSizeSquare (gl_StaticSkullkeep + gl_WActuators + item->type, size, ((ref.id>iNbActuators)?.01f:1.0f));

				//--- Shadowed text
				iAdjustX = -iFntSize;
				setTextProperties (iFntSize, 0, 0, 0);
				fontDrawString (iGLVirtualX+1+iAdjustX, iGLVirtualY-1, "%02X", item->type);
				// If actuator is unreferenced
				actmap = xact->map;
				if (xact->used == 0)
					setTextProperties (iFntSize, .9, .1, .1);
				else if (xact->used == 1)
				{
					if (xact->wact == 0)	// floor actuator
					{
						setTextProperties (iFntSize, .1, .9, .9);
						fwtype = 'F';
					}
					else	// wall actuator
					{
						setTextProperties (iFntSize, .9, .9, .1);
						fwtype = 'W';
					}
				}
				fontDrawString (iGLVirtualX+iAdjustX, iGLVirtualY, "%02X", item->type);
				//if (xact->used == 1)
				{
					iAdjustY = -iFntSize;
					iAdjustX = -1.5f*iFntSize;
					
					setTextProperties (iFntSize, 0, 0, 0);
					fontDrawString (iGLVirtualX+1+iAdjustX, iGLVirtualY-1+iAdjustY, "%C", fwtype);	
					setTextProperties (iFntSize, .8, .8, .8);
					fontDrawString (iGLVirtualX+iAdjustX, iGLVirtualY+iAdjustY, "%C", fwtype);

					iAdjustY = -iFntSize;
					iAdjustX = 0*iFntSize;
					setTextProperties (iFntSize, 0, 0, 0);
					fontDrawString (iGLVirtualX+1+iAdjustX, iGLVirtualY-1+iAdjustY, "%d", actmap);	
					setTextProperties (iFntSize, .4, .9, .4);
					setTextProperties (iFntSize, .4 + (.6*actmap/16.f), .9 - (.9*actmap/16.f), .4 + (.6*actmap/16.f));
					fontDrawString (iGLVirtualX+iAdjustX, iGLVirtualY+iAdjustY, "%d", actmap);
				
				}
			}
			moveSize (0, 1, size);
		}
		moveSize (1, -iStepDisplay, size);
	}
	size = oldsize;

	//--- Display quantity per type
	moveToUpperMap ();
	moveSize (18, 0, size);
	for (id = 0; id < 80; id++)
	{
		if (id%16 == 0 && id != 0)
		{
			moveSize (3, -16, size);
		}
		drawSizeSquare (gl_StaticSkullkeep + gl_WActuators + id, size, 1.0f);
		moveSize (1, 0, size);
		if (iCounterPerType[id] == 0)
			setTextProperties (10, .4, .4, .4);
		fontDrawString (iGLVirtualX, iGLVirtualY, "%02d", iCounterPerType[id]);
		moveSize (-1, 1, size);
	}
}

//------------------------------------------------------------------------------

//int iTileInfo_OffsetX = 860;
//int iTileInfo_OffsetY = __DISP_MAP_START_Y__;

void
drawDatabasesCapacity (float scale)
{
	int i;
	float large = 16;
	moveToUpperMap ();
	
	//moveSize (winW*2 - 400, 16*16*scale, 1);
	moveSize (890*3, 110, 1);
	//fontDrawString (iTileInfo_OffsetX, winH-iTileInfo_OffsetY, "TILE: (%02X)", *tile);

	for (i = 0; i < 16; i++)
	{
		drawCapacityBar ((float) nItems[i]/1020, i, large/2, scale*1020/4);
		moveSize (large, 0, 1);
	}
}

//------------------------------------------------------------------------------


void
redrawScreen ()
{
	static int bFirstDisplay = 1;
	unsigned char level = 0;

	glClear (GL_COLOR_BUFFER_BIT);
	shadowmap = 0;

	angle = angle + 1;
	//printf("angle = %i\n", angle);

	level = getEditCursor (cursor_L);

	if (SKULLKEEP == 1 && THERONSQUEST == 1)
	{
		int ts = 3; // default stone keep 

		ts = ((sk_level_header*) &getLevels()[level].header)->tileset;

		gl_x_Tiles = gl_SK_TilesSet + 16*ts;
		gl_x_Doors = gl_Doors;
		gl_x_Ornates = gl_Ornates;
		gl_x_Monsters = gl_Monsters;
		gl_x_Walls = gl_Walls;
		gl_x_Floors = gl_Floors;

		gl_x_Weapons = gl_Weapons;
		gl_x_Clothes = gl_Clothes;
		gl_x_Potions = gl_Potions;
		gl_x_Containers = gl_Containers;
		gl_x_Miscs = gl_Miscs;

		gl_x_Portraits = gl_Portraits;

		dt_Door_Max = door_Max;
		dt_Monster_Max = monster_Max;
		dt_Wall_Max = wall_Max;
		dt_Floor_Max = floor_Max;
		dt_Ornate_Max = ornate_Max;

		dt_Weapons_Max = weapon_Max;
		dt_Clothing_Max = clothing_Max;
		dt_Misc_Max = misc_Max;
		dt_Containers_Max = 1;

		gfx_objects[0] = gl_x_Doors;
		gfx_objects[4] = gl_x_Monsters;
		gfx_objects[5] = gl_x_Weapons;
		gfx_objects[6] = gl_x_Clothes;
		gfx_objects[7] = gl_Special + special_Scroll;
		gfx_objects[8] = gl_x_Potions;
		gfx_objects[9] = gl_x_Containers;
		gfx_objects[10] = gl_x_Miscs;
		gfx_x_objects = gfx_objects;

	}

	if (SKULLKEEP == 1 && TELOS == 1)
	{
		int ts = 3; // default stone keep 

		ts = ((sk_level_header*) &getLevels()[level].header)->tileset;
		gl_x_Tiles = gl_SK_TilesSet + 16*ts;

		gl_x_Tiles = gl_SK_TilesSet + 16*ts;
		gl_x_Doors = gl_StaticSkullkeep + gl_Doors;
		gl_x_Ornates = gl_StaticSkullkeep + gl_Ornates;
		gl_x_Monsters = gl_StaticTELOS + gl_Monsters;
		gl_x_Walls = gl_StaticSkullkeep + gl_Walls;
		gl_x_Floors = gl_StaticSkullkeep + gl_Floors;

		gl_x_Weapons = gl_StaticSkullkeep + gl_Weapons;
		gl_x_Clothes = gl_StaticSkullkeep + gl_Clothes;
		gl_x_Potions = gl_StaticSkullkeep + gl_Potions;
		gl_x_Containers = gl_StaticSkullkeep + gl_Containers;
		gl_x_Miscs = gl_StaticSkullkeep + gl_Miscs;

		gl_x_Portraits = gl_StaticSkullkeep + gl_Portraits;


		dt_Door_Max = 16;
		dt_Monster_Max = 254;
		dt_Wall_Max = 135;
		dt_Floor_Max = 81;
		dt_Ornate_Max = 16;

		dt_Weapons_Max = 0x2E;
		dt_Clothing_Max = 0x39;
		dt_Misc_Max = 0x43;
		dt_Containers_Max = 16;

		gfx_objects[0] = gl_x_Doors;
		gfx_objects[4] = gl_x_Monsters;
		gfx_objects[5] = gl_x_Weapons;
		gfx_objects[6] = gl_x_Clothes;
		gfx_objects[7] = gl_Special + special_Scroll;
		gfx_objects[8] = gl_x_Potions;
		gfx_objects[9] = gl_x_Containers;
		gfx_objects[10] = gl_x_Miscs;
		gfx_x_objects = gfx_dm2_objects;
	}
	else if (SKULLKEEP == 1 && THERONSQUEST == 0 && TELOS == 0)
	{
		int ts = 3; // default stone keep 

		ts = ((sk_level_header*) &getLevels()[level].header)->tileset;
		gl_x_Tiles = gl_SK_TilesSet + 16*ts;

		gl_x_Tiles = gl_SK_TilesSet + 16*ts;
		gl_x_Doors = gl_StaticSkullkeep + gl_Doors;
		gl_x_Ornates = gl_StaticSkullkeep + gl_Ornates;
		gl_x_Monsters = gl_StaticSkullkeep + gl_Monsters;
		gl_x_Walls = gl_StaticSkullkeep + gl_Walls;
		gl_x_Floors = gl_StaticSkullkeep + gl_Floors;

		gl_x_Weapons = gl_StaticSkullkeep + gl_Weapons;
		gl_x_Clothes = gl_StaticSkullkeep + gl_Clothes;
		gl_x_Potions = gl_StaticSkullkeep + gl_Potions;
		gl_x_Containers = gl_StaticSkullkeep + gl_Containers;
		gl_x_Miscs = gl_StaticSkullkeep + gl_Miscs;

		gl_x_Portraits = gl_StaticSkullkeep + gl_Portraits;


		dt_Door_Max = 11;
		dt_Monster_Max = 88;
		dt_Wall_Max = 135;
		dt_Floor_Max = 81;
		dt_Ornate_Max = 16;

		dt_Weapons_Max = 0x2E;
		dt_Clothing_Max = 0x39;
		dt_Misc_Max = 0x43;
		dt_Containers_Max = 16;

		gfx_objects[0] = gl_x_Doors;
		gfx_objects[4] = gl_x_Monsters;
		gfx_objects[5] = gl_x_Weapons;
		gfx_objects[6] = gl_x_Clothes;
		gfx_objects[7] = gl_Special + special_Scroll;
		gfx_objects[8] = gl_x_Potions;
		gfx_objects[9] = gl_x_Containers;
		gfx_objects[10] = gl_x_Miscs;
		gfx_x_objects = gfx_dm2_objects;

	}
	else if (SKULLKEEP == 0)
	{
		gl_x_Tiles = gl_Tiles;
		gl_x_Doors = gl_Doors;
		gl_x_Ornates = gl_Ornates;
		gl_x_Monsters = gl_Monsters;
		gl_x_Walls = gl_Walls;
		gl_x_Floors = gl_Floors;

		gl_x_Weapons = gl_Weapons;
		gl_x_Clothes = gl_Clothes;
		gl_x_Potions = gl_Potions;
		gl_x_Containers = gl_Containers;
		gl_x_Miscs = gl_Miscs;

		gl_x_Portraits = gl_Portraits;

		dt_Door_Max = door_Max;
		dt_Monster_Max = monster_Max;
		dt_Wall_Max = wall_Max;
		dt_Floor_Max = floor_Max;
		dt_Ornate_Max = ornate_Max;

		dt_Weapons_Max = weapon_Max;
		dt_Clothing_Max = clothing_Max;
		dt_Misc_Max = misc_Max;
		dt_Containers_Max = 1;
	
		gfx_objects[0] = gl_Doors;
		gfx_objects[4] = gl_Monsters;
		gfx_objects[5] = gl_Weapons;
		gfx_objects[6] = gl_Clothes;
		gfx_objects[7] = gl_Special + special_Scroll;
		gfx_objects[8] = gl_Potions;
		gfx_objects[9] = gl_Containers;
		gfx_objects[10] = gl_Miscs;
		gfx_x_objects = gfx_objects;

	}
	switch (getScreen ())
	{
	case screen_GeneralHelp:
		
		printGeneralHelpInfo ();
		break;
	case screen_Map:
		{
			int selectext = 
				((isEditingTile ()) && ((getCurrentItemReference ()->category == category_Text)
				|| (getCurrentItemReference()->category == category_Scroll)))?1:0;
			
			drawDatabasesCapacity (1);
		
			if (isEditingTarget())
				level = getEditCursor (cursor_TM);
			else
				level = getEditCursor (cursor_L);

			if (SKULLKEEP == 1)
			{
				int ts = 3;
				ts = ((sk_level_header*) &getLevels()[level].header)->tileset;
				gl_x_Tiles = gl_SK_TilesSet + 16*ts;
			}

			drawMap (level, selectext);
			drawMapFrame (selectext);
			drawMapObject (level, selectext);
			if (level == 0 && !selectext)	// in DM the starting map is map zero .. and not saved in dungeon.dat
				//drawStartingCursor ((!selectext));
				drawStartingCursor ();
			drawStack (getEditCursor (cursor_X), getEditCursor (cursor_Y), getEditCursor (cursor_L));
			
			if (!selectext)
				drawMapCursor (isEditingTile());
			
			if (isEditingTarget())
				drawTargetEditCursor (0);

			drawMapGraphics (level);
			printGeneralMapInfo ();
			drawMapHelpInfo ();
			{
				reference_p refp = getStackReference (getEditCursor (cursor_Stack));
				if (refp->category == category_Text || refp->category == category_Scroll)
					printSelectedText (refp);
				else if (refp->category == category_Actuator){
				//	printActivator (refp, (getCurrentTile ()->type == tile_Wall));
					text_frame_actuator (refp, (getCurrentTile ()->type == tile_Wall), 0, 0, 1);
				}
			}
			if (isSelectingNewItem())
				displaySelectNewItem ();
		}
		break;
	case screen_Level:
		drawMap (level, 0);
		printLevelSpecificationsInfo ();
		displayLevelWindow ();	// draw graphics after texts
		drawLevelPropertiesHelpInfo ();
		break;

	case screen_MainHeader:
		
		printDungeonSpecificationsInfo ();
		break;


	case screen_ListsCreatures:
		displayCreaturesLists ();
		break;

	case screen_ListsActuators:
		displayActuatorsLists ();
		break;

	case screen_ListsItems:
		displayItemsLists ();
		break;

	case screen_DM2AI:
		displayAIInfos ();
		break;

	case screen_LoadFile:
		{
			unsigned int nFiles = numberOfFilesToLoad ();
			unsigned int i;
			int fontsize = 14;
			int step = (fontsize + 2);
			setTextProperties (32, .8, 1, .5); 
			outputTextLineAt (200, winH-100, "SELECT A DUNGEON FILE TO LOAD");
			if (nFiles <= 20)
			{
				fontsize = 24; step = (fontsize + 2);
			}
			else if (nFiles < 50)
			{
				fontsize = 19; step = (fontsize + 2);
			}
			else if (nFiles >= 50)
			{
				fontsize = 14; step = (fontsize + 2);
			}

			for (i = 0; i < nFiles; i++)
			{
				if (i == (unsigned) selectFile)
					setTextProperties (fontsize, 1, 1, 0); 
				else
					setTextProperties (fontsize, .7, .8, 1); 
				
				outputTextLineAt (100, winH-150-step*i, getFileName (i));
			}
		}
		break;
	case screen_SaveFile:
		{
			unsigned int nFiles = numberOfFilesToLoad ();
			unsigned int i;
			int fontsize = 14;
			int step = (fontsize + 2);
			setTextProperties (32, .8, 1, .5); 
			outputTextLineAt (200, winH-100, "SELECT A FILE TO SAVE DUNGEON");
			if (nFiles <= 20)
			{
				fontsize = 24; step = (fontsize + 2);
			}
			else if (nFiles < 50)
			{
				fontsize = 19; step = (fontsize + 2);
			}
			else if (nFiles >= 50)
			{
				fontsize = 14; step = (fontsize + 2);
			}
			for (i = 0; i < nFiles; i++)
			{
				if (i == (unsigned) selectFile)
					setTextProperties (fontsize, 1, 1, 0); 
				else
					setTextProperties (fontsize, .7, .8, 1); 
				
				outputTextLineAt (100, winH-150-step*i, getFileName (i));
			}
		}
		break;


	}

	{
		char buffer[300];	
		buffer[0] = '\0';
		if (msg_flag & MSG_PRINT_VERSION)
		{	
			char version[32];
			sprintf (version, "v%1d.%02d", DMB_MAJOR, DMB_MINOR);
#ifdef DMB_REVISION
			sprintf (version, "v%1d.%02d.%c", DMB_MAJOR, DMB_MINOR, DMB_REVISION);
#endif
			sprintf (buffer, "WELCOME TO DUNGEON MASTER BUILDER %s !", version);
			printGeneralMessage (buffer);
		}
		else if (msg_flag & MSG_INVALID_DUNGEON)
		{	
			sprintf (buffer, "Invalid dungeon");
			printGeneralMessage (buffer);
		}
		else if (msg_flag & MSG_DUNGEON_LOCKED) // lock 3
		{	sprintf (buffer, "Dungeon is completely locked");
			printGeneralMessage (buffer);
		}
		else if (msg_flag & MSG_DUNGEON_EDITING_LOCK) // lock 1
		{	sprintf (buffer, "Dungeon cannot be edited");
			printGeneralMessage (buffer);
		}
		else if (msg_flag & MSG_DUNGEON_QUICKVIEW) // lock 2
		{	sprintf (buffer, "Only dungeon layout is displayed");
			printGeneralMessage (buffer);
		}
		else if (msg_flag & MSG_LOAD_FAILURE)
		{	sprintf (buffer, "Failed to load the file (%s)", currentFileName);
			printGeneralMessage (buffer);
		}
		else if (msg_flag & MSG_SAVE_FAILURE)
		{	sprintf (buffer, "Failed to save the file (%s)", currentFileName);
			printGeneralMessage (buffer);
		}
		else if (msg_flag & MSG_LOAD_SUCCESS)
		{	sprintf (buffer, "Dungeon (%s) successfully loaded", currentFileName);
			printGeneralMessage (buffer);
		}
		else if (msg_flag & MSG_LOAD_SG_SUCCESS)
		{	sprintf (buffer, "Savegame (%s) successfully loaded", currentFileName);
			printGeneralMessage (buffer);
		}
		else if (msg_flag & MSG_SAVE_SUCCESS)
		{	sprintf (buffer, "Dungeon (%s) successfully saved", currentFileName);
			printGeneralMessage (buffer);
		}
		else if (msg_flag & MSG_SAVE_SG_SUCCESS)
		{	sprintf (buffer, "Savegame (%s) successfully saved", currentFileName);
			printGeneralMessage (buffer);
		}
	}
	if (!bFirstDisplay)
		msg_flag = 0;	
	else
		bFirstDisplay = 0;
}