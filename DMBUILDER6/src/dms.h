//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Master Data Structures
//------------------------------------------------------------------------------

#ifndef DUNGEON_STRUCTURES_H
#define DUNGEON_STRUCTURES_H

/* NOTE:
 * All structures are stored with Little End order,
 * knowing that this order is used for the Intel processor.
 * Then you generally have to invert every 2-bytes data in the dungeon.dat to get back the real value.
 */




/* structure for a level specification 
 * size = 16 bytes
 */
typedef struct 
{
	unsigned short mapOffset;
	char unused1[4];
	unsigned char xOffset; // apparently up to 255
	unsigned char yOffset;
	unsigned short level:4; // position of the level in the level stack. 0 is top.
	unsigned short unused2:2;
	unsigned short xDim:5; // values 0x00 to 0x1F give dimensions 1 to 32.
	unsigned short yDim:5;
	unsigned char nWalls:4;
	unsigned char rWalls:4;
	unsigned char nFloors:4;	
	unsigned char rFloors:4;	
	unsigned char nOrnates:4;	
	unsigned char nMonsters:4;
	unsigned char unknown1:4;		
	unsigned char depth:4; // determinates the default level of generated monsters.	
	unsigned char empty;
	unsigned char door1:4;
	unsigned char door2:4;
} dm_level_header;

/* structure for a level specification 
 * size = 16 bytes
 */
typedef struct 
{
	unsigned short mapOffset;
	char unused1[4];
	unsigned char xOffset; // apparently up to 255
	unsigned char yOffset;
	unsigned short level:4; // position of the level in the level stack. 0 is top.
	unsigned short unused2:2;
	unsigned short xDim:5; // values 0x00 to 0x1F give dimensions 1 to 32.
	unsigned short yDim:5;
	unsigned char nWalls:4;
	unsigned char rWalls:4;
	unsigned char nFloors:4;	
	unsigned char rFloors:4;	
	unsigned char nOrnates:4;	
	unsigned char nMonsters:4;
	unsigned char unknown1:4;		
	unsigned char depth:4; // determinates the default level of generated monsters.	
	unsigned char unknown2:4;
	unsigned char tileset:4;
	unsigned char door1:4;
	unsigned char door2:4;
} sk_level_header;


//------------------------------------------------------------------------------
/* definitions of all placable objects. 
 * the 2 first bytes are always for the next reference object of the linked list.
 */


typedef struct
{
	unsigned short type:1;
	unsigned short ornate:4;
//	unsigned short unk2:1;
	unsigned short opensUp:1;
	unsigned short button:1;
	unsigned short destroyable:1;
	unsigned short bashable:1;
	unsigned short unk1:7;

} dm_door;

typedef dm_door* door_p;

typedef struct 
{
	unsigned short xdest:5;
	unsigned short ydest:5;
	unsigned short rotation:2;
	unsigned short absolute:1;
	unsigned short scope:2;
	unsigned short sound:1;

	unsigned short unused8:8;
	unsigned short ldest:6;
	unsigned short unused4:2;
} dm_teleport; // sk compatible generic teleport

typedef struct 
{
	unsigned short xdest:5;
	unsigned short ydest:5;
	unsigned short rotation:2;
	unsigned short absolute:1;
	unsigned short scope:2;
	unsigned short sound:1;

	unsigned short unused8:8;
	unsigned short ldest:4;
	unsigned short unused4:4;
} dm1_teleport;

typedef dm_teleport* teleport_p;

typedef struct 
{
	unsigned short visible:1;
	unsigned short flag2:1;
	unsigned short flag3:1;
	unsigned short offset:13;
} dm_text;

typedef dm_text* text_p;


typedef struct 
{
	unsigned short	Activated:1;	// And for Text Visibility
	unsigned short 	Mode:2;			// 0 = text / 1 = active decoration / 2 = complex
	unsigned short	Graphism:8;	//2^7 = 128 (range: 0-7F)
	unsigned short	ExtendedUsage:5;
} dm2_simple_actuator;

typedef dm2_simple_actuator* simple_actuator_p;

typedef struct 
{
	short chested;
	unsigned char type;		// unsigned allow more than 127
	unsigned char position;
	unsigned short health[4];
	unsigned short unused1:5;
	unsigned short number:2;
	unsigned short unused2:1;
	unsigned short unk1:8;	// DM2: .... DD..	D = direction (then 0x04 = facing north, 0x07 = facing west)
} dm_monster;

typedef dm_monster* monster_p;

typedef struct 
{
	unsigned int type:7;	// 6?
	unsigned int keep:1;
	unsigned int cursed:1;
	unsigned int poisoned:1;
	unsigned int charges:4;
	unsigned int broken:1;
	unsigned int unk1:1;
} dm_weapon; 

typedef dm_weapon* weapon_p;

typedef struct 
{
	unsigned int type : 7;
	unsigned int keep : 1;
	unsigned int cursed : 1;
	unsigned int dump : 5;
	unsigned int broken : 1;
	unsigned int unk2 : 1;
} dm_clothing;

typedef dm_clothing* clothing_p;

typedef struct 
{
	unsigned int power: 8;
	unsigned int type : 5;
	unsigned int unk1 : 2;
	unsigned int keep : 1;
} dm_potion;

typedef dm_potion* potion_p;

typedef struct
{
	unsigned int offset:8;	// 8 or 16 ?..
} dm_scroll;

typedef dm_scroll* scroll_p;


typedef struct
{
	unsigned int reftxt:10;
	unsigned int type:6;
} sk_scroll;


typedef sk_scroll* scroll2_p;

typedef struct 
{
	short chested;
//	unsigned short type:13;
//	unsigned short unk1:3;
//	unsigned short type;

	unsigned short data1;

//	unsigned short data1:5;
//	unsigned short type:6;
//	unsigned short data2:5;

	unsigned short unk2;
} dm_chest;

typedef dm_chest* chest_p;

typedef struct 
{
	unsigned int type:7;
	unsigned int keep:1;
	unsigned int unk1:6;
	unsigned int capacity:2;
} dm_misc;

typedef dm_misc* misc_p;

struct dm_missile
{
	char unk1;
	char spell;
	char power;
	char unk2;
	char zero;
	char e;
};

struct dm_cloud
{
	char power;
	char spell;
};

//------------------------------------------------------------------------------


#endif