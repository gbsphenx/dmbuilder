//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
/*******************************************************************************
**
*	DMB Version History
*
*	0.88.g		2025-09-05		Special revision for publishing code to GitHub
*	0.88.d		2024-12-15		Minor improvements for text info display
*	0.88.c		2023-05-09		Revision of interface for standard 1920x1080 + precisions on actuator infos
*	......		..........		Stuff happened... not logged
*
*	0.87.b		2013-xx-xx		Support for DM2 actuators
*	0.87.a		2013-10-13		(Released on spx site)	Support for DM2 graphics.
*	0.86.kappa	2010-04-1x		Add basic DM2 dungeon edit.
*	......		..........		Stuff happened... not logged
*								specially an experimentation with moveable windows that has been scratched
*
*	0.85.mu		2003-04-1x		Add control from mouse to paint tiles
*	0.85.sigma	2003-03-16		Read/write savegame, only for the dungeon part (not ingame state info)
*	0.85.d		2003-03-16?		
*
*	0.85.c		2002-11-10		Add edits
*	0.85.b		2002-11-06		Edit for map infos / add cut/paste
*	0.85		2002-11-02		Better global support
*	0.84		2002-10-01		Better support, visualisation
*	0.83		2002-11-10		Initial release of DMBuilder, better support for actuators
*
*	0.52		2002-01-13		Fix: visualisation issues
*	0.51		2002-01-12		Fix: missing font.tga from release, and add/remove objects
*	0.50		2002-01-11		Initial release of DMEdit, evolved from DMViewer
* 
*
*******************************************************************************/

#ifndef VERSION_H
#define VERSION_H

#define DMB_YEAR_FIRST		2001
#define DMB_YEAR_CURRENT	2025
#define DMB_CORE			6
#define DMB_MONSTER			"Ruster"
#define DMB_MAJOR			0
#define DMB_MINOR			88
#define DMB_REVISION		'g'

#define PUBLIC
#define CHECKOFF

#ifdef PUBLIC
#undef CHECKOFF
#endif

/*******************************************************************************
*	Version Specific Features
*	This defines allow to prepare and test features for other versions than
*	the current. Hence, when a release is done for version X, all features
*	planned for superior version must be deactivated.
*******************************************************************************/


#ifdef _DEBUG
	#define ALL_FEATURES 1
#else
	#define ALL_FEATURES 0
#endif


/*******************************************************************************
*	0.87.a Features
*******************************************************************************/

#if ( (ALL_FEATURES == 1) || (DMB_MINOR > 87) || (DMB_MINOR == 87 && DMB_REVISION >= 'a') )
#define DMBF_ON_MAP_DOOR_COLORED	1
#endif

/*******************************************************************************
*	0.87.b Features
*******************************************************************************/


//	FEATURES LIST:
//	SK: On wall creature generator, display the target creature!
//	SK: Read/Save SK savegames
//	STD: Implement the COPY stack.
//	STD: Read BIG & LITTLE endian (so that DMB can read megacd skullkeep / and maybe amiga dungeons?)
//	SK: having world view / move maps within the world
//	SK: help MAP LINKS creation
//	STD: Display letters before ornates list below map
//	STD: Let choose between SK or DM (or CSB) when creating a new map
//	STD: Make DMB read dungeons from DUNGEON dir instead of ROOT dir
//	STD: Allow choosing path for save/load
//	STD: Make function to check any dungeon format (DM/CSB/SK) and display an ICON (helm) before name of dungeon.
//	STD: Monster list view: add a top monsters with number of occurences
//	STD: Make list for each category item
//	SK:	Dungeon generator (it is more interesting with DM2 engine), though it may use SK(DM1)GDAT for generation?
//	STD: Add title to screens
//	STD: Add help text for each screen




#endif