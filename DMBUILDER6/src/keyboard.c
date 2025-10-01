//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Keyboard Management
//------------------------------------------------------------------------------

#include <memory.h>
#include <displaytext.h>
#include <keyboard.h>
#include <editor.h>
#include <filename.h>
#include <loaddungeon.h>
#include <savedungeon.h>
#include <generator.h>
#include <reference.h>
#include <item.h>
#include <tile.h>
#include <level.h>
#include <dm.h>
#include <text.h>
#include <actuator.h>
#include <monster.h>
#include <security.h>
#include <version.h>
#include <data.h>
#include <skullexe.h>

#include <fileop.h>

#include <stdlib.h>
#include <string.h>
#include <flush.h>


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

extern int msg_flag;

#define KEY_RETURN 13
#define KEY_TAB	9
#define KEY_ESCAPE 27
#define KEY_CONTROL -96	// 'a' is 97, C-a is 1

#define ControlKey(key) (KEY_CONTROL + (key))
/*
#define GLUT_KEY_F1			1
#define GLUT_KEY_F2			2
#define GLUT_KEY_F3			3
#define GLUT_KEY_F4			4
#define GLUT_KEY_F5			5
#define GLUT_KEY_F6			6
#define GLUT_KEY_F7			7
#define GLUT_KEY_F8			8
#define GLUT_KEY_F9			9
#define GLUT_KEY_F10			10
#define GLUT_KEY_F11			11
#define GLUT_KEY_F12			12
#define GLUT_KEY_LEFT			100
#define GLUT_KEY_UP			101
#define GLUT_KEY_RIGHT			102
#define GLUT_KEY_DOWN			103
#define GLUT_KEY_PAGE_UP		104
#define GLUT_KEY_PAGE_DOWN		105
#define GLUT_KEY_HOME			106
#define GLUT_KEY_END			107
#define GLUT_KEY_INSERT			108
*/


extern int editing_gfx;
extern int level_spec;

extern tSkullExe xSkullExe;

/* convert table for glut key associated number, dependant from <glut.h> */
static char cgk[128] = {
0, 0, 1, 2, 3, 4, 5, 6, 7, 8,
9, 10, 11, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
12, 13, 14, 15, 16, 17, 18, 19, 20, 0};

typedef void (*MAP_NO_EDIT_FUNC) (int);

dm_reference cutpasteref;


unsigned char monster_max[] =
{1, 4, 4, 4, 2, 1, 4, 4,
4, 1, 4, 1, 4, 1, 4, 2,
4, 4, 4, 4, 1, 1, 4, 1,
1, 1, 1}; 

/* 0 = 2 monsters/  1 = 4 (3?) monsters */
/*unsigned char multi_monsters[][2] =
{
	{,}, {,}, {,}, {,}, {,}, {,}, {,}, {,},
	{,}, {,}, {,}, {,}, {,}, {,}, {,}, {,},
	{,}, {,}, {,}, {,}, {,}, {,}, {,}, {,},
	{,}, {,}, {,}
};*/

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void
fastInit ()
{
	// = -1;
	cutpasteref.category = 0xf;
	cutpasteref.id = 0x3ff;
	cutpasteref.position = 0x3;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static void
callFlushDungeon ()
{
	flushDungeon ();
}

static void
execTestDungeon ()
{
	char sExecPath[256];

	if (SKULLKEEP || TELOS)
	{
		//--- Copy current dungeon as "dungeon.dat" in the test folder
#ifdef __LINUX__
		saveDungeonData (__FN_DIRDM2__"/DATA/DUNGEON.DAT");
		if (SKULLKEEP)
			saveMusicList(__FN_DIRDM2__"/DATA/SONGLIST.DAT");
		printf("SkullExe: %08x\n", xSkullExe);
		if (MVALID(&xSkullExe))
		{
			SkullExe_Write(&xSkullExe, __FN_DIRDM2__"/SKULL.EXE");
			SkullExe_ExportAITable(&xSkullExe, __FN_DIRDM2__"/v1d296c.dat");
		}
#else
		saveDungeonData (__FN_DIRDM2__"\\DATA\\DUNGEON.DAT");
		if (SKULLKEEP)
			saveMusicList(__FN_DIRDM2__"\\DATA\\SONGLIST.DAT");
		printf("SkullExe: %08x\n", xSkullExe);
		if (MVALID(&xSkullExe))
		{
			SkullExe_Write(&xSkullExe, __FN_DIRDM2__"\\SKULL.EXE");
			SkullExe_ExportAITable(&xSkullExe, __FN_DIRDM2__"\\v1d296c.dat");
		}
#endif // __LINUX

#ifdef __LINUX__
		strcpy(sExecPath, "./"__FN_DIRDM2__"/dosbox-dm2.sh");
#else
		strcpy(sExecPath, ".\\"__FN_DIRDM2__"\\dosbox-dm2.bat");
#endif
		//--- Start OS run for DM2 PC
		printf("Start: %s\n", sExecPath);
		system(sExecPath);
	}
	else
	{
		//--- Copy current dungeon as "dungeon.dat" in the test folder
#ifdef __LINUX__
		saveDungeonData (__FN_DIRDM1__"/DATA/DUNGEON.DAT");
		strcpy(sExecPath, "./"__FN_DIRDM1__"/dosbox-dm.sh");
#else
		saveDungeonData (__FN_DIRDM1__"\\DATA\\DUNGEON.DAT");
		strcpy(sExecPath, ".\\"__FN_DIRDM1__"\\dosbox-dm.bat");
#endif
		//--- Start OS run for DM2 PC
		printf("Start: %s\n", sExecPath);
		system(sExecPath);
	}
}



/* obsolete
static void
callLoadDungeon ()
{
	int load;
	readFileNames ();
	
	load = loadDungeonData (currentFileName);
	if (load == -1)
		msg_flag += MSG_INVALID_DUNGEON;
	else if (load == 0)
		msg_flag += MSG_LOAD_FAILURE;
	else if (load == 1)
		msg_flag += MSG_LOAD_SUCCESS;
	else // 0
		msg_flag += MSG_LOAD_SG_SUCCESS;

#ifndef CHECKOFF
	if (verifyLock ())
		msg_flag += MSG_DUNGEON_LOCKED;
	if (msg_flag & MSG_DUNGEON_EDITING_LOCK ||
		msg_flag & MSG_INVALID_DUNGEON)
		flushDungeon ();
#endif
}*/

void
Call_LoadDungeon (char* dungeonname)
{
	int load;
	char fulldungeonname[256];
	currentFileName = dungeonname;
#ifndef __LINUX__
	sprintf(fulldungeonname, "dungeons\\%s", dungeonname);
#else
	sprintf(fulldungeonname, "dungeons/%s", dungeonname);
#endif
	load = loadDungeonData (fulldungeonname);
	reinitTextStrings();
	if (load == -1)
		msg_flag += MSG_INVALID_DUNGEON;
	else if (load == 0)
		msg_flag += MSG_LOAD_FAILURE;
	else if (load == 1)
		msg_flag += MSG_LOAD_SUCCESS;
	else // 0
		msg_flag += MSG_LOAD_SG_SUCCESS;
	if (verifyLock ())
		msg_flag += MSG_DUNGEON_LOCKED;
	if (msg_flag & MSG_DUNGEON_EDITING_LOCK ||
		msg_flag & MSG_INVALID_DUNGEON)
		flushDungeon ();

	if (SKULLKEEP)
	{
#ifndef __LINUX__
		loadMusicList("XDM2PCDOS\\DATA\\SONGLIST.DAT");
		SkullExe_Load(&xSkullExe, "XDM2PCDOS\\SKULL.EXE");
#else
		loadMusicList("XDM2PCDOS/DATA/SONGLIST.DAT");
		SkullExe_Load(&xSkullExe, "XDM2PCDOS/SKULL.EXE");
#endif
	}

}

static void
callSaveDungeon ()
{
	char fulldungeonname[256];
	readFileNames ();
#ifndef __LINUX__
	sprintf(fulldungeonname, "dungeons\\%s", getSaveDungeonName ());
#else
	sprintf(fulldungeonname, "dungeons/%s", getSaveDungeonName ());
#endif
	if (!saveDungeonData (fulldungeonname))
		msg_flag += MSG_SAVE_FAILURE;
	else
		msg_flag += MSG_SAVE_SUCCESS;
#ifndef __LINUX__
	sprintf(fulldungeonname, "dungeons\\%s", "backupsave.dat");
#else
	sprintf(fulldungeonname, "dungeons/%s", "backupsave.dat");
#endif
	saveDungeonData (fulldungeonname);
	if (SKULLKEEP)
#ifndef __LINUX__
		saveMusicList("XDM2PCDOS\\DATA\\SONGLIST.DAT");
#else
		saveMusicList("XDM2PCDOS/DATA/SONGLIST.DAT");
#endif

}



static void
Call_SaveDungeon (char *dungeonname)
{
	int save;
	char fulldungeonname[256];
//	readFileNames ();
	currentFileName = dungeonname;
#ifndef __LINUX__
	sprintf(fulldungeonname, "dungeons\\%s", currentFileName);
#else
	sprintf(fulldungeonname, "dungeons/%s", currentFileName);
#endif
	save = saveDungeonData (fulldungeonname);
	if (save == 0)
		msg_flag += MSG_SAVE_FAILURE;
	else if (save == 1)
		msg_flag += MSG_SAVE_SUCCESS;
	else
		msg_flag += MSG_SAVE_SG_SUCCESS;
#ifndef __LINUX__
	sprintf(fulldungeonname, "dungeons\\%s", "backupsave.dat");
#else
	sprintf(fulldungeonname, "dungeons/%s", "backupsave.dat");
#endif
	saveDungeonData (fulldungeonname);
	if (SKULLKEEP) {
#ifndef __LINUX__
		saveDungeonData ("SKDMCD\\DATA\\DUNGEON.DAT");
		saveMusicList("SKDMCD\\DATA\\SONGLIST.DAT");
#else
		saveDungeonData ("SKDMCD/DATA/DUNGEON.DAT");
		saveMusicList("SKDMCD/DATA/SONGLIST.DAT");
#endif
	}
}





void
callPlaceNewItem (int keepnew)
{
	short ref = placeNewItem (getEditCursor (cursor_NewItem), -1);
	if (ref != -2)
	{	
		pushReferenceCurrentStack (ref);
		if (getEditCursor (cursor_NewItem) == category_Door)
			putDoorTile (getEditCursor (cursor_X), getEditCursor (cursor_Y), getEditCursor (cursor_L));
		getCurrentTile ()->object = 1;
		exportCurrentStack ();
	}
	if (keepnew <= 0)
		setSelectingNewItem (0);
}

static void
callUnchest () // targetedit on & 'u' key pressed.
{
	reference_p refp = getCurrentItemReference ();
	if (refp->category == category_Monster)
	{
		tile_p tile = getTile (getEditCursor (cursor_TX), getEditCursor (cursor_TY), getEditCursor (cursor_L));
		short unchest = unchestMonster ((monster_p) getItem (refp));
		tile->object = 1;
		push_reference_in_stack (unchest, getEditCursor (cursor_TX), getEditCursor (cursor_TY), getEditCursor (cursor_L));
		setEditingTarget (0); 
	}
	else if (refp->category == category_Chest)
	{
		tile_p tile = getTile (getEditCursor (cursor_TX), getEditCursor (cursor_TY), getEditCursor (cursor_L));
		short unchest = unchest_Chest ((chest_p) getItem (refp));
		tile->object = 1;
		push_reference_in_stack (unchest, getEditCursor (cursor_TX), getEditCursor (cursor_TY), getEditCursor (cursor_L));
		setEditingTarget (0); 
	}
}

void
Call_ChangeScreen (int newscreen)
{
	screen = newscreen;
	if (screen == screen_Map)
		updatePriorityColors ();
};

static void
callChangeTarget ()
{
	setEditingTarget (!isEditingTarget());
				if (isEditingTarget()) /* acquire target */
				{
					reference_p refp = getCurrentItemReference ();
					if (refp->category == category_Actuator)
					{
						actuator_target_p target = (actuator_target_p) (getItem (refp) + 2);
						setEditCursor (cursor_TX, (char) target->xdest);
						setEditCursor (cursor_TY, (char) target->ydest);
						setEditCursor (cursor_TM, getEditCursor(cursor_L));
					}
					else if (refp->category == category_Teleport)
					{
						teleport_p teleport = (teleport_p) getItem (refp);
						setEditCursor (cursor_TX, (char) teleport->xdest);
						setEditCursor (cursor_TY, (char) teleport->ydest);
						setEditCursor (cursor_TM, (char) teleport->ldest);
					}
					else
					{
						setEditCursor (cursor_TX, getEditCursor (cursor_X));
						setEditCursor (cursor_TY, getEditCursor (cursor_Y));
						setEditCursor (cursor_TM, getEditCursor (cursor_L));
					}
				}
				else /* new target has been defined */
				{
					reference_p refp = getCurrentItemReference ();
					if (refp->category == category_Actuator)
					{
						actuator_target_p target = (actuator_target_p) (getItem (refp) + 2);
						target->xdest = getEditCursor (cursor_TX);
						target->ydest = getEditCursor (cursor_TY);
					}
					else if (refp->category == category_Teleport)
					{
						teleport_p teleport = (teleport_p) getItem (refp);
						teleport->ydest = getEditCursor (cursor_TY);
						teleport->xdest = getEditCursor (cursor_TX);
						teleport->ldest = getEditCursor (cursor_TM);
					}
					else if (refp->category == category_Monster)
					{
						reference_p toChest = getGroundReference (
							getEditCursor (cursor_TX),
							getEditCursor (cursor_TY),
							getEditCursor (cursor_L));
						monster_p monster = (monster_p) getItem (refp);
						if ((monster->chested != -2)
							&& (monster->chested != -1))	// on n'accepte pas de surchester
							return;
						
						setChestedMonster (monster, toChest);
						setGroundReference (getEditCursor (cursor_TX),
							getEditCursor (cursor_TY),
							getEditCursor (cursor_L),
							-2);
						getTile (getEditCursor (cursor_TX),
							getEditCursor (cursor_TY),
							getEditCursor (cursor_L))->object = 0;
					}
					else if (refp->category == category_Chest)
					{
						reference_p toChest = getGroundReference (
							getEditCursor (cursor_TX),
							getEditCursor (cursor_TY),
							getEditCursor (cursor_L));
						chest_p chest = (chest_p) getItem (refp);
						if ((chest->chested != -2)
							&& (chest->chested != -1))	// on n'accepte pas de surchester
							return;
						chest->chested = *((short*)toChest);
						setGroundReference (getEditCursor (cursor_TX),
							getEditCursor (cursor_TY),
							getEditCursor (cursor_L),
							-2);
						getTile (getEditCursor (cursor_TX),
							getEditCursor (cursor_TY),
							getEditCursor (cursor_L))->object = 0;
					}
				}
				
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void keyboard (unsigned char key, int x, int y)
{
//	printf("key/x/y = %d/%d/%d\n", key, x, y);

	switch (getScreen ())
	{
		case screen_GeneralHelp:
		{
			switch (key)
			{
				case KEY_ESCAPE:
					Call_ChangeScreen (screen_Map);
					break;
			}
		}
		break;
		case screen_DM2AI:
		{
			int iValue = 0;
			iValue = SkullExe_GetAIValueAt(&xSkullExe, getEditCursor(cursor_AI_index), getEditCursor(cursor_AI_attribute));
			switch (key)
			{
				case KEY_ESCAPE:
					Call_ChangeScreen (screen_Map);
					break;
				case '0':
					iValue = 0x00;
					break;
				case '1':
					iValue ^= 0x01;
					break;
				case '2':
					iValue ^= 0x02;
					break;
				case '3':
					iValue ^= 0x04;
					break;
				case '4':
					iValue ^= 0x08;
					break;
				case '5':
					iValue ^= 0x10;
					break;
				case '6':
					iValue ^= 0x20;
					break;
				case '7':
					iValue ^= 0x40;
					break;
				case '8':
					iValue ^= 0x80;
					break;
				case '9':
					iValue = 0xFF;
					break;
			}
			SkullExe_SetAIValueAt(&xSkullExe, getEditCursor(cursor_AI_index), getEditCursor(cursor_AI_attribute), iValue);
		}
		break;
		case screen_Map:
		{
			if (isSelectingNewItem () && (key == KEY_RETURN))
				callPlaceNewItem (0);
			else if (!isEditingTile ())
			{
				switch (key)
				{
			//	case KEY_ESCAPE: break;
				case 'E':if (getLevels()[getEditCursor(cursor_L)].header.xDim>0)getLevels()[getEditCursor(cursor_L)].header.xDim--;break;
				case 'R':if (getLevels()[getEditCursor(cursor_L)].header.xDim<31)getLevels()[getEditCursor(cursor_L)].header.xDim++;break;
				case 'D':if (getLevels()[getEditCursor(cursor_L)].header.yDim>0)getLevels()[getEditCursor(cursor_L)].header.yDim--;break;
				case 'C':if (getLevels()[getEditCursor(cursor_L)].header.yDim<31)getLevels()[getEditCursor(cursor_L)].header.yDim++;break;

				case 'B': getDungeon()->nLevels --;
						if (getDungeon()->nLevels < 1)
							getDungeon()->nLevels = 1;
						break;
				case 'N': getDungeon()->nLevels ++;
						if (getDungeon()->nLevels > MAX_LEVELS)
							getDungeon()->nLevels = MAX_LEVELS;
						break;

				case ControlKey('s'):
					SKULLKEEP = !SKULLKEEP; break;
				case ControlKey('x'):
					{
						reference_p refp = getGroundReference (getEditCursor (cursor_X),
							getEditCursor(cursor_Y), getEditCursor(cursor_L));
						unsigned short *buffer = (unsigned short*) &cutpasteref;
						if (*buffer == 0xFFFF) // le 0xFFFF ne passe pas forcément
						{
							unsigned short **refv = (unsigned short**) &refp;
							if (**refv != 0xFFFE && **refv != 0xFFFF)
							{	// copy tile on buffer
								cutpasteref.category = refp->category;
								cutpasteref.id = refp->id;
								cutpasteref.position = refp->position;
								**refv = 0xFFFE;
								getTile (getEditCursor (cursor_X),
							getEditCursor(cursor_Y), getEditCursor(cursor_L))->object = 0;
								// set tile to nothing
							}
							/*else
							{	// target tile is nothing, then buffer becomes nothing
								*refv = (short*) &cutpasteref;
								**refv = 0xFFFF;
							}*/
						}
					}
					break;
				case ControlKey('v'):
				{
					unsigned short *refv = (unsigned short*) &cutpasteref;
					if (*refv != 0xFFFE && *refv != 0xFFFF)
					{	// paste buffer into target tile
						push_reference_in_stack (*refv, getEditCursor (cursor_X),
							getEditCursor (cursor_Y), getEditCursor (cursor_L));
						*refv = 0xFFFF;
						getTile (getEditCursor (cursor_X),
							getEditCursor(cursor_Y), getEditCursor(cursor_L))->object = 1;
					}
				}
					break;
				case ' ':
					switchTile (); break;
				case 'p':
					if ((getEditCursor (cursor_L) == 0)
						&& (getEditCursor (cursor_X) == getDungeon()->x_start)
							&& (getEditCursor (cursor_Y) == getDungeon()->y_start))
								getDungeon()->f_start ++;
					else getCurrentTile ()->type = tile_Pit;
					break;
				case 'f':
					getCurrentTile ()->type = tile_Floor; break;
				case 's':
					getCurrentTile ()->type = tile_Stairs; break;
				case 'i':
					getCurrentTile ()->type = tile_Trickwall; break;
				case 'S':
				if (getEditCursor (cursor_L) == 0)
				{
					getDungeon()->x_start = getEditCursor (cursor_X);	
					getDungeon()->y_start = getEditCursor (cursor_Y);	
				} break;
				case 'g': /* go target */
					goTarget (getCurrentItemReference ());
					break;
				case KEY_RETURN:
					switchEditingTile (); break;
				case '+':
					setSelectingNewItem (1); break;
				case KEY_ESCAPE:
				case '-':
					setSelectingNewItem (0); break;
				case '1': switchTileFunction (getCurrentTile (), 1); break;
				case '2': switchTileFunction (getCurrentTile (), 2); break;
				case '4': switchTileFunction (getCurrentTile (), 4); break;
				case '8': switchTileFunction (getCurrentTile (), 8); break;
				case '9': getCurrentTile ()->type++; break;
				default:
					break;
				}
			}
			else /* editing object */
			{
				reference_p refp;
				switch ((refp = getCurrentItemReference())->category)
				{
					case category_Door:
					{
						door_p door = (door_p) getItem (refp);
						if (key == 'b')
							door->button += 1;
						else if (key == 'd')
							door->destroyable += 1;
						else if (key == 's')
							door->bashable += 1;
						else if (key == ' ')
							door->type += 1;
						else if (key == 'o')
							door->opensUp += 1; 
					}
					break;

					case category_Teleport:
					{
						teleport_p teleport = (teleport_p) getItem (refp);
						if (key == 'o')
							teleport->sound += 1;
						else if (key == 'a')
							teleport->absolute += 1;
						else if (key == 'r')
							teleport->rotation += 1;
						else if (key == 's')
							teleport->scope += 1;
						else if (key == '2')
							teleport->ldest += 1;
						else if (key == '1')
							teleport->ldest -= 1;
						else if (key == 'h')
						((tile_teleport_p)getCurrentTile ())->bluehaze += 1;
						else if (key == ' ')
						((tile_teleport_p)getCurrentTile ())->open += 1;
					
					}
					break;
		
					case category_Text:	/// needs to be checked either as text or simple actuator (DM2)
					{
						text_p text = (text_p) getItem (refp);
						simple_actuator_p sact = (simple_actuator_p) text;

						if (key == 'm')
							sact->Mode = (sact->Mode + 1)%4;
						else if (key == 'e')
							sact->ExtendedUsage = (sact->ExtendedUsage + 1)%32;
						else if (key == 'E')
							sact->ExtendedUsage = (sact->ExtendedUsage + 31)%32;
						else if (key == 'a')
							sact->Activated = !sact->Activated;

						if (key == 's' && sact->Mode == 0)	// standard text
							text->silent += 1;
						break;
					}

					case category_Actuator:
					{
						actuator_p type = (actuator_p) (getItem(refp));
						actuator_effect_p effect = (actuator_effect_p) (getItem(refp)+1);
						actuator_target_p target = (actuator_target_p) (getItem(refp)+2);
						
						if (key == 'd')
							effect->delay += 1;
						else if (key == 's')
							effect->sound += 1;
						else if (key == 'o')
							effect->once += 1;
						else if (key == 'i')
							effect->inactive += 1;
			//			else if (key == 'c')
			//				effect->cycle += 1;
						else if (key == 'e')
							effect->effect += 1;
						else if (key == 'E')
							effect->effect -= 1;
						else if (key == 'f')
							target->facing += 1;
						else if ((key == 'g') && (type->type == 6)) /* mons gen*/
						{
							((a_2_monster_generator_p) effect)->generation++;
							if (((a_2_monster_generator_p) effect)->generation == 0 ||
								((a_2_monster_generator_p) effect)->generation == 8)
								((a_2_monster_generator_p) effect)->generation++;
						}
						else if ((key == 'G') && (type->type == 6))
						{
							((a_2_monster_generator_p) effect)->generation--;
							if (((a_2_monster_generator_p) effect)->generation == 0 ||
								((a_2_monster_generator_p) effect)->generation == 8)
								((a_2_monster_generator_p) effect)->generation--;
						}
						else if ((key == '7') && (type->type == 6))
							((a_3_monster_generator_p) target)->thoughness-=0x10;
						else if ((key == '8') && (type->type == 6))
							((a_3_monster_generator_p) target)->thoughness+=0x10;
						else if ((key == '4') && (type->type == 6))
							((a_3_monster_generator_p) target)->pause-=0x10;
						else if ((key == '5') && (type->type == 6))
							((a_3_monster_generator_p) target)->pause+=0x10;
						
						else if (type->type == 8 || type->type == 9 || type->type == 10 ||
							type->type == 14 || type->type == 15)
						{
							if ((key == 'l')) /* shooter range */
							((a_3_shooter_p) target)->range++;
							else if ((key == 'L'))
							((a_3_shooter_p) target)->range--;
							else if ((key == 'w')) /* shooter power */
							((a_3_shooter_p) target)->power++;
							else if ((key == 'W'))
							((a_3_shooter_p) target)->power--;
						}
					}
					break;

					case category_Monster:
						{
							monster_p monster = (monster_p) getItem (refp);
							if (key == '1')
								setMonsterLevel (monster, -1);
							else if (key == '2')
								setMonsterLevel (monster, 1);
							else if (key == '4')
								setMonsterLevel (monster, -10);
							else if (key == '5')
								setMonsterLevel (monster, 10);
							else if (key == '0')
								setMonsterLevel (monster, 0);
							else if (key == '8')
								setNoviceMonster (monster);
							else if (key == 'm')
							{
								monster->number += 1;
								if (monster->number >= monster_max[monster->type])
									monster->number = 0;
								if (monster->number == 0)
									monster->position = 0xFF;
								else if (monster->number == 1)
									monster->position = 0x0D;
								else 
									monster->position = 0x6C;

							}
							else if (key == 'u' && isEditingTarget())
								callUnchest ();
						}
						break;
					case category_Weapon:
						{
							weapon_p weapon = (weapon_p) getItem (refp);
							if (key == '1')
								weapon->charges--;
							else if (key == '2')
								weapon->charges++;
							else if (key == 'c')
								weapon->cursed+=1;
							else if (key == 'o')
								weapon->poisoned+=1;
							else if (key == 'b')
								weapon->broken+=1;
						}break;
					case category_Clothing:
						{
							clothing_p clothes = (clothing_p) getItem (refp);
							if (key == 'c')
								clothes->cursed+=1;
							else if (key == 'b')
								clothes->broken+=1;
						} break;
					case category_Potion:
						{
							potion_p potion = (potion_p) getItem (refp);
							if (key == '1')
								potion->power-=1;
							else if (key == '2')
								potion->power+=1;
							else if (key == '4')
								potion->power-=40;
							else if (key == '5')
								potion->power+=40;
						} break;

					case category_Scroll:
					{
						int iUseGDATText = 0;
						int iTextValue = 0;
						scroll2_p scroll = (scroll2_p) getItem (refp);

						if (scroll->reftxt > 0)
							iUseGDATText = 1;

						if (key == 'm') // switch to GDAT or REF text
						{
							if (iUseGDATText == 0)
							{
								iTextValue = scroll->type;
								if (iTextValue == 0)
									iTextValue = 1; // that allows next to cycle the value
								scroll->type = 0;
								scroll->reftxt = iTextValue;
							}
							else
							{
								iTextValue = scroll->reftxt;
								scroll->reftxt = 0;
								scroll->type = iTextValue;
							}
						}
						break;
					}

					case category_Chest:
						{
						if (key == 'u' && isEditingTarget())
								callUnchest ();
						} break;
					case category_Miscs:
						{
							misc_p misc = (misc_p) getItem (refp);
							if (key == '1')
								misc->capacity--;
							else if (key == '2')
								misc->capacity++;
						}break;




				}
				
				switch (key)
				{
			case 't': callChangeTarget (); break;
			case 'p':
				turnItemFacing (getCurrentItemReference ());
				break;
			/*case 'r':
				referenceID (getCurrentItemReference (), -1);
				break;
			case 'R':
				referenceID (getCurrentItemReference (), 1);
				break;*/
			case '2':
				cycleActivatorType (getCurrentItemReference (), 1); break;
			case '1':
				cycleActivatorType (getCurrentItemReference (), -1); break;
			case KEY_RETURN:
				switchEditingTile ();
				if (isEditingTarget ())
					callChangeTarget ();
					break;
			case KEY_TAB:
				switchSecondFunction (); break;
				case '-':	
					removeStackItem (getEditCursor (cursor_Stack));
					if (getStackSize () == 0)
						switchEditingTile ();
					break;
				default: break;
				}
			}
		}
		break;
		case screen_Level:
			{
				switch (key)
				{
					case KEY_ESCAPE:
						Call_ChangeScreen (screen_Map);
						break;
					case KEY_RETURN:
						switchEditingGraphics ();
						break;
					case '+':
						if (!level_spec)
						modifyLevelNumberGraphics (getEditCursor (cursor_Graphics), getEditCursor (cursor_L), 1);
						break;
					case '-':
						if (!level_spec)
						modifyLevelNumberGraphics (getEditCursor (cursor_Graphics), getEditCursor (cursor_L), -1);
						break;
					case KEY_TAB:
						switchModifyLevelSpecs ();
						editing_gfx = 0;
						break;
					case 'r':
						{
							char g = getEditCursor (cursor_Graphics);
							if (g == 1 && getLevels()[getEditCursor(cursor_L)].header.rWalls < 15)
								getLevels()[getEditCursor(cursor_L)].header.rWalls += 1;
							else if (g == 2 && getLevels()[getEditCursor(cursor_L)].header.rFloors < 15)
								getLevels()[getEditCursor(cursor_L)].header.rFloors += 1;
						} break;
					case 'R':
						{
							char g = getEditCursor (cursor_Graphics);
							if (g == 1 && getLevels()[getEditCursor(cursor_L)].header.rWalls > 0)
								getLevels()[getEditCursor(cursor_L)].header.rWalls -= 1;
							else if (g == 2 && getLevels()[getEditCursor(cursor_L)].header.rFloors > 0)
								getLevels()[getEditCursor(cursor_L)].header.rFloors -= 1;
						} break;
					case 't':
						getLevels()[getEditCursor(cursor_L)].header.tileset += 1;
						break;
					case 'T':
						getLevels()[getEditCursor(cursor_L)].header.tileset -= 1;
						break;
					case 'm':
						cycleMapMusic ((getEditCursor (cursor_L)), 1);
						break;
					case 'M':
						cycleMapMusic ((getEditCursor (cursor_L)), -1);
						break;
					case 'S':	// switch mode to DM2 or DM1
						SKULLKEEP = !SKULLKEEP;
						break;
				}

			} break;
		case screen_MainHeader:
			{
				if (key == KEY_ESCAPE)
					Call_ChangeScreen (screen_Map);
			} break;
		case screen_ListsActuators:
			{
				if (key == KEY_ESCAPE)
					Call_ChangeScreen (screen_Map);
			} break;
		case screen_ListsItems:
			{
				if (key == KEY_ESCAPE)
					Call_ChangeScreen (screen_Map);
			} break;
		case screen_ListsCreatures:
			{
				if (key == KEY_ESCAPE)
					Call_ChangeScreen (screen_Map);
			} break;
		case screen_TextEditor:
			{
				if (key == KEY_ESCAPE)
					Call_ChangeScreen (screen_Map);
			} break;
		case screen_LoadFile:
		{
			if (key == KEY_ESCAPE)
					Call_ChangeScreen (screen_Map);
			else if (key == KEY_RETURN)
			{	
				Call_ChangeScreen (screen_Map);
				Call_LoadDungeon (getFileName (selectFile));
			}
		} break;
		case screen_SaveFile:
		{
			if (key == KEY_ESCAPE)
					Call_ChangeScreen (screen_Map);
			else if (key == KEY_RETURN)
			{
				Call_SaveDungeon (getFileName (selectFile));
				Call_ChangeScreen (screen_Map);
			}
		} break;

	}
#ifdef __MINGW__
	glutDisplay();
#else
	glutPostRedisplay ();
#endif
}


void arrow_keys (int a_keys, int x, int y)
{
	//printf("a_keys/x/y = %d/%d/%d\n", a_keys, x, y);

	//-- Test first for Fn keys that would change screen
	switch (a_keys)
	{
		case GLUT_KEY_F1: setScreen (screen_GeneralHelp); break;
		case GLUT_KEY_F2: Call_ChangeScreen (screen_Level); break;
		case GLUT_KEY_F3: setScreen (screen_ListsCreatures); break;
		case GLUT_KEY_F4: setScreen (screen_ListsItems); break;
		//case GLUT_KEY_F5: setScreen (screen_ListsActuators); break;
		case GLUT_KEY_F5: execTestDungeon (); break;
		case GLUT_KEY_F6: setScreen (screen_MainHeader); break;
		case GLUT_KEY_F7:
				DMB_AutoEdit_LoadSkullExe();
				setScreen (screen_DM2AI);
				break;
		/*case GLUT_KEY_F8:
			switchContext ();
			break;*/
		case GLUT_KEY_F9: 
			updateFileNames ();
			setScreen (screen_LoadFile);
			break;
		//case GLUT_KEY_F10: exportText (); break;
		case GLUT_KEY_F10: setScreen (screen_TextEditor); break;
		//case GLUT_KEY_F11: importText (); break;
		case GLUT_KEY_F11: setScreen (screen_ListsActuators); break;
		case GLUT_KEY_F12: 
			updateFileNames ();
			setScreen (screen_SaveFile);
			break;
	}

	switch (getScreen ())
	{


		case screen_Map:
		if (isSelectingNewItem())
		{
			if (a_keys == GLUT_KEY_F8)
				switchContext ();
			else if (a_keys == GLUT_KEY_LEFT)
				setEditCursor (cursor_NewItem, (char) (getEditCursor (cursor_NewItem) - 1));
			else if (a_keys == GLUT_KEY_RIGHT)
				setEditCursor (cursor_NewItem, (char) (getEditCursor (cursor_NewItem) + 1));
		}

		else if (isEditingTarget())
		{
			
			switch (a_keys)
			{
			case GLUT_KEY_RIGHT: setEditCursor (cursor_TX, (char) (getEditCursor (cursor_TX) + 1)); break;
			case GLUT_KEY_LEFT:	setEditCursor (cursor_TX, (char) (getEditCursor (cursor_TX) - 1)); break;
			case GLUT_KEY_DOWN: setEditCursor (cursor_TY, (char) (getEditCursor (cursor_TY) + 1)); break;
			case GLUT_KEY_UP: setEditCursor (cursor_TY, (char) (getEditCursor (cursor_TY) - 1)); break;
			case GLUT_KEY_PAGE_DOWN: setEditCursor (cursor_TM, (char) (getEditCursor (cursor_TM) + 1)); break;
			case GLUT_KEY_PAGE_UP: setEditCursor (cursor_TM, (char) (getEditCursor (cursor_TM) - 1)); break;
			}
		}


		else if (isEditingTile())
		{
			
			
			switch (a_keys)
			{
			case GLUT_KEY_DOWN: setEditCursor (cursor_Stack, (char) (getEditCursor (cursor_Stack) + 1)); break;
			case GLUT_KEY_UP: setEditCursor (cursor_Stack, (char) (getEditCursor (cursor_Stack) - 1)); break;
			case GLUT_KEY_RIGHT:
				cycleItem (getStackReference (getEditCursor (cursor_Stack)), 1);
				break;
			case GLUT_KEY_LEFT:
				cycleItem (getStackReference (getEditCursor (cursor_Stack)), -1);
				break;
			case GLUT_KEY_PAGE_DOWN: stackSwitch (getEditCursor (cursor_Stack), getEditCursor (cursor_Stack)+1);
				setEditCursor (cursor_Stack,(char) (getEditCursor (cursor_Stack) +1));break;
			case GLUT_KEY_PAGE_UP: stackSwitch (getEditCursor (cursor_Stack), getEditCursor (cursor_Stack)-1);
				if (getEditCursor (cursor_Stack) > 0) setEditCursor (cursor_Stack, (char) (getEditCursor (cursor_Stack) -1));break;
			
			}
		}
			
		else{switch (a_keys)
		{
			case GLUT_KEY_F1: setScreen (screen_GeneralHelp); break;
			case GLUT_KEY_F2: Call_ChangeScreen (screen_Level); break;
			case GLUT_KEY_F3: setScreen (screen_ListsCreatures); break;
			case GLUT_KEY_F4: setScreen (screen_ListsItems); break;
			//case GLUT_KEY_F5: setScreen (screen_ListsActuators); break;
			case GLUT_KEY_F6: setScreen (screen_MainHeader); break;
			case GLUT_KEY_F7:
					DMB_AutoEdit_LoadSkullExe();
					setScreen (screen_DM2AI);
					break;
			
		//	case GLUT_KEY_F7: exit (0); break;
			case GLUT_KEY_F8:
				switchContext ();
				break;
			case GLUT_KEY_F9: 
				updateFileNames ();
				setScreen (screen_LoadFile);
				break;
			case GLUT_KEY_F10: exportText (); break;
			case GLUT_KEY_F11: importText (); break;
			case GLUT_KEY_F12: 
				updateFileNames ();
				setScreen (screen_SaveFile);
				break;
				// TODO: should make a CONFIRMATION screen before flush (if the user made changes before!!)
			//case GLUT_KEY_F5: callFlushDungeon ();/*startGeneration ();*/ break;
			//case GLUT_KEY_F5: execTestDungeon ();/*startGeneration ();*/ break;
			//case GLUT_KEY_F6: __TestRandomFloorsTiles (0); break;
			//case GLUT_KEY_F6: DMB_AutoEdit_CreateTestDungeon(); break;

			case GLUT_KEY_RIGHT: setEditCursor (cursor_X, (char) (getEditCursor (cursor_X) + 1)); break;
			case GLUT_KEY_LEFT:	setEditCursor (cursor_X, (char) (getEditCursor (cursor_X) - 1)); break;
			case GLUT_KEY_DOWN: setEditCursor (cursor_Y, (char) (getEditCursor (cursor_Y) + 1)); break;
			case GLUT_KEY_UP: setEditCursor (cursor_Y, (char) (getEditCursor (cursor_Y) - 1)); break;
			case GLUT_KEY_PAGE_DOWN:
				if (getEditCursor (cursor_L) < MAX_LEVELS)
				{
					setEditCursor (cursor_X, (char) ((getEditCursor (cursor_X)) +
						getLevels()[getEditCursor (cursor_L)].header.xOffset));
							
					setEditCursor (cursor_Y, (char) ((getEditCursor (cursor_Y)) +
						getLevels()[getEditCursor (cursor_L)].header.yOffset));
					setEditCursor (cursor_L, (char) (getEditCursor (cursor_L) + 1));
					setEditCursor (cursor_X, (char) ((getEditCursor (cursor_X)) -
						getLevels()[getEditCursor (cursor_L)].header.xOffset));
					setEditCursor (cursor_Y, (char) ((getEditCursor (cursor_Y)) -
						getLevels()[getEditCursor (cursor_L)].header.yOffset));
				}
				break;
			case GLUT_KEY_PAGE_UP:
				if (getEditCursor (cursor_L) > 0)
				{
					setEditCursor (cursor_X, (char) ((getEditCursor (cursor_X)) + 
						getLevels()[getEditCursor (cursor_L)].header.xOffset));
					setEditCursor (cursor_Y, (char) ((getEditCursor (cursor_Y)) + 
						getLevels()[getEditCursor (cursor_L)].header.yOffset));
					setEditCursor (cursor_L, (char) (getEditCursor (cursor_L) - 1));
					setEditCursor (cursor_X, (char) ((getEditCursor (cursor_X)) - 
						getLevels()[getEditCursor (cursor_L)].header.xOffset));
					setEditCursor (cursor_Y, (char) ((getEditCursor (cursor_Y)) -
						getLevels()[getEditCursor (cursor_L)].header.yOffset));
				}
				break;
						
			//case GLUT_KEY_PAGE_DOWN: setEditCursor (cursor_L, (char) (getEditCursor (cursor_L) + 1)); break;
			//case GLUT_KEY_PAGE_UP: setEditCursor (cursor_L, (char) (getEditCursor (cursor_L) - 1)); break;
		}}
		break;

		case screen_Level:
		if (!isEditingGraphics () && !level_spec)
		{
			
			switch (a_keys)
			{
				case GLUT_KEY_F1: Call_ChangeScreen (screen_Map); break;
				case GLUT_KEY_F3: setScreen (screen_ListsCreatures); break;
				//case GLUT_KEY_F4: setScreen (screen_ListsActuators); break;
				case GLUT_KEY_F7: 
					DMB_AutoEdit_LoadSkullExe();
					setScreen (screen_DM2AI);
					break;
				case GLUT_KEY_RIGHT: setEditCursor (cursor_Index, (char) (getEditCursor (cursor_Index) + 1)); break;
				case GLUT_KEY_LEFT:	setEditCursor (cursor_Index, (char) (getEditCursor (cursor_Index) - 1)); break;
				case GLUT_KEY_DOWN: setEditCursor (cursor_Graphics, (char) (getEditCursor (cursor_Graphics) + 1)); break;
				case GLUT_KEY_UP: setEditCursor (cursor_Graphics, (char) (getEditCursor (cursor_Graphics) - 1)); break;
				case GLUT_KEY_PAGE_DOWN: setEditCursor (cursor_L, (char) (getEditCursor (cursor_L) + 1)); break;
				case GLUT_KEY_PAGE_UP: setEditCursor (cursor_L, (char) (getEditCursor (cursor_L) - 1)); break;
			}
			
		}
		else if (level_spec)
		{
			level_p level = (level_p) &getLevels()[getEditCursor (cursor_L)];
			int n = getEditCursor (cursor_LevelSpec);
			switch (a_keys)
			{
			case GLUT_KEY_RIGHT:
				{
					if (n == 0)	level->header.depth += 1;
					else if (n == 1) level->header.xOffset += 1;
					else if (n == 2) level->header.yOffset += 1;
				}
				break;
			case GLUT_KEY_LEFT:
				{
					if (n == 0)	level->header.depth -= 1;
					else if (n == 1) level->header.xOffset -= 1;
					else if (n == 2) level->header.yOffset -= 1;
				}
				break;
			case GLUT_KEY_UP:
				setEditCursor (cursor_LevelSpec, (char) (getEditCursor (cursor_LevelSpec) - 1)); break;
			case GLUT_KEY_DOWN:
				setEditCursor (cursor_LevelSpec, (char) (getEditCursor (cursor_LevelSpec) + 1)); break;
			}
		}				
		else
			switch (a_keys)
			{
			case GLUT_KEY_RIGHT:
				cycleLevelGraphics (getEditCursor (cursor_Graphics), getEditCursor (cursor_Index), 1, getEditCursor (cursor_L)); break;
			case GLUT_KEY_LEFT:
				cycleLevelGraphics (getEditCursor (cursor_Graphics), getEditCursor (cursor_Index), -1, getEditCursor (cursor_L)); break;
			default: break;
			}


		case screen_ListsCreatures:
		case screen_ListsItems:
//		case screen_DM2AI:
		case screen_ListsActuators:
		case screen_MainHeader:
			{
				switch (a_keys)
				{
					case GLUT_KEY_F1: setScreen (screen_Map); break;
					case GLUT_KEY_F2: setScreen (screen_Level); break;
					case GLUT_KEY_F3: setScreen (screen_ListsCreatures); break;
					//case GLUT_KEY_F4: setScreen (screen_ListsActuators); break;
				}
			}
			break;
		case screen_DM2AI:
			{
				switch (a_keys)
				{
					case GLUT_KEY_RIGHT: setEditCursor (cursor_AI_attribute, (char) (getEditCursor (cursor_AI_attribute) + 1)); break;
					case GLUT_KEY_LEFT:	setEditCursor (cursor_AI_attribute, (char) (getEditCursor (cursor_AI_attribute) - 1)); break;
					case GLUT_KEY_DOWN: setEditCursor (cursor_AI_index, (char) (getEditCursor (cursor_AI_index) + 1)); break;
					case GLUT_KEY_UP: setEditCursor (cursor_AI_index, (char) (getEditCursor (cursor_AI_index) - 1)); break;

					case GLUT_KEY_F1: setScreen (screen_Map); break;
					case GLUT_KEY_F2: setScreen (screen_Level); break;
					case GLUT_KEY_F3: setScreen (screen_ListsCreatures); break;
					//case GLUT_KEY_F4: setScreen (screen_ListsActuators); break;
				
				}			
			}
			break;
		case screen_TextEditor:
			{/*
				switch (a_keys)
				{
				case KEY_ESCAPE:
					Call_ChangeScreen (screen_Map); break;
				}*/
			break;
			}
		case screen_LoadFile:
			{
				switch (a_keys)
				{
//				case KEY_ESCAPE:
//					Call_ChangeScreen (screen_Map); break;
				case GLUT_KEY_DOWN: selectFile += 1;
					 break;
				case GLUT_KEY_UP: selectFile -= 1;
					 break;
				}
				if (selectFile < 0) selectFile = 0;
				else if (selectFile > (char) (numberOfFilesToLoad()-1)) selectFile = numberOfFilesToLoad()-1;

			break;
			}
		case screen_SaveFile:
			{
				switch (a_keys)
				{
//				case KEY_ESCAPE:
//					Call_ChangeScreen (screen_Map); break;
				case GLUT_KEY_DOWN: selectFile += 1;
					 break;
				case GLUT_KEY_UP: selectFile -= 1;
					 break;
				}
				if (selectFile < 0) selectFile = 0;
				else if (selectFile > (char) (numberOfFilesToLoad()-1)) selectFile = numberOfFilesToLoad()-1;
			break;
			}


	}
#ifdef __MINGW__
	glutDisplay();
#else
	glutPostRedisplay ();
#endif

}