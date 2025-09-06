//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Load Dungeon
//------------------------------------------------------------------------------

#ifndef LOAD_DUNGEON_H
#define LOAD_DUNGEON_H


typedef struct
{
	unsigned short id;
	unsigned char wact;	// 1 = is wall actuator / 0 = is floor actuator
	unsigned char used;	// 1 = is used and found in the dungeon / 0 = is not found in the dungeon
	unsigned char map;	// in which map this actuator is found
} tCompanionActuator;

//--- Companion table for actuators usage
tCompanionActuator xActuatorTable[1024];

int loadDungeonData (char* dungeonname);
int loadMusicList (char* sFilenameSongListDat);

void updateActivationItems(int iDM2Mode);

#endif