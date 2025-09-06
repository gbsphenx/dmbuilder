//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2010 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Skullkeep
//------------------------------------------------------------------------------
//	This code is used for direct read from the Skull.exe program (DM2 engine)
// It has some support functions for hacking data within the exe.
// I've found some of the data myself by looking onto the exe - I've good eyes ;-)
// Then I completed after finding 'SKWin' project from Kentaro.
//------------------------------------------------------------------------------

#ifndef __DM_SKULLEXE__H__
#define __DM_SKULLEXE__H__

//------------------------------------------------------------------------------

#define BOOL	unsigned char

//------------------------------------------------------------------------------

typedef enum
{
	SKULLEXE_VERSION__ENGLISH,			// GDAT2: V5_2a
	SKULLEXE_VERSION__MULTI,			// GDAT2: V5_2c
	SKULLEXE_VERSION__MULTI_PATCHED,	// GDAT2: V5_2c
} tSKExeVersion;

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//



typedef	enum SpellSymbol {
		RUNE_LO = 0x60,	// 60
		RUNE_UM,			// 61
		RUNE_ON,			// 62
		RUNE_EE,			// 63
		RUNE_PAL,			// 64
		RUNE_MON,			// 65

		RUNE_YA = 0x66,	// 66
		RUNE_VI,			// 67
		RUNE_OH,			// 68
		RUNE_FUL,			// 69
		RUNE_DES,			// 6a
		RUNE_ZO,			// 6b

		RUNE_VEN = 0x6C,	// 6c
		RUNE_EW,			// 6d
		RUNE_KATH,			// 6e
		RUNE_IR,			// 6f
		RUNE_BRO,			// 70
		RUNE_GOR,			// 71

		RUNE_KU = 0x72,	// 72
		RUNE_ROS,			// 73
		RUNE_DAIN,			// 74
		RUNE_NETA,			// 75
		RUNE_RA,			// 76
		RUNE_SAR,			// 77
} eSKEXE_SpellRune;

enum
{
	SPELLTYPE_POTION	= 1,
	SPELLTYPE_MISSILE	= 2,
	SPELLTYPE_ENCHANTMENT = 3,
	SPELLTYPE_SUMMON	= 4,
};


#define	SKILL_DM_FIGHTER_GLOBAL				0
#define	SKILL_DM_NINJA_GLOBAL				1
#define	SKILL_DM_PRIEST_GLOBAL				2
#define	SKILL_DM_WIZARD_GLOBAL				3

#define SKILL_DM_FIGHTER_SWING				4
#define SKILL_DM_FIGHTER_THRUST				5
#define SKILL_DM_FIGHTER_BLUNT				6
#define SKILL_DM_FIGHTER_PARRY				7

#define SKILL_DM_NINJA_STEAL				8
#define SKILL_DM_NINJA_FIGHT				9
#define SKILL_DM_NINJA_THROW				10
#define SKILL_DM_NINJA_SHOOT				11

#define SKILL_DM_PRIEST_IDENTIFY			12
#define SKILL_DM_PRIEST_HEAL				13
#define SKILL_DM_PRIEST_INFLUENCE			14
#define SKILL_DM_PRIEST_DEFEND				15

#define SKILL_DM_WIZARD_FIRE				16
#define SKILL_DM_WIZARD_AIR					17
#define SKILL_DM_WIZARD_EARTH				18
#define SKILL_DM_WIZARD_WATER				19

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
#define _SKEXE_MAX_SPELLS	34

typedef struct
{
	char			sRune[4];
	char			iDifficulty;
	char			iSkill;
	
	unsigned short	iSpellType:4;
	unsigned short	iSpellResult:6;
	unsigned short	s8:2;
	unsigned short	iSpecial:2;
	unsigned short	s9:2;
} tSKEXE_Spell_Definition;

typedef struct
{
	char	iPower[4][6];
} tSKEXE_Rune_CostTable;

//------------------------------------------------------------------------------
// What is different between axeman and axeman thief?
// B22, B20 and B14. Other values are known and not about thief stuff
//
//
//
//
// What needs to be found?
// speed, caster, spell casted,
// turn speed, behaviour (thief, not, ...)



//--- DB4 Creatures & Objects AI definitions
//--- There are 62 AI of 38 bytes each
#define _SKEXE_MAX_AI	63

#pragma pack(1)
typedef struct
{
	// byte 00
	unsigned char b0f7:1;	// 1 = Non creature? Static object ?
	unsigned char b0f6:1;	// 1 = Reflector unique ???
	unsigned char b0f5:1;	// 1 = Treated as wall for creatures ??
	unsigned char b0f4:1;	// 1 = Only for ghost (???)
	unsigned char b0f3:1;
	unsigned char bGhost:1;	// 1 = Non material being / Ghost
	unsigned char b0f1:1;
	unsigned char b0f0:1;

	// byte 01
//	unsigned char byte01;
	unsigned char b1f7:1;	// 1 = Spiked wall unique	
	unsigned char b1f6:1;	
	unsigned char b1f5:1;
	unsigned char b1f4:1;
	unsigned char b1f3:1;	// 1 = Dragoth unique (end game ???)
	unsigned char b1f2:1;	// 1 = pit/door ghost + void door => ???
	unsigned char b1f1:1;	// 1 = Minions only
	unsigned char bPassable:1;	// 1 = Can pass throught (minions / ghost / bats)

	unsigned char iArmor;			// 255 = can't take damage
	unsigned char byte03;
	// byte 04 + 05
	unsigned short iHitPoints;		

	// byte 06
	unsigned char iAttackStrength;
	unsigned char iPoison;
	// byte 08
	unsigned char iDefense;			 // 255 = can't be hit
	// or is dexterity? Is that some probability to hit? 255 = can't hit, 0 = always successful hit ?
	// Can't understand.
	
	// byte 09 - flags
	// Rightmost
	unsigned char bSplashAttack:1;	// Can attack several champions anywhere in squad
	unsigned char flag6:1;			// 0, not used.
	unsigned char bFlying:1;		// Flying, OK
	unsigned char flag4:1;			// Minions, Ghost, Bats, Dragoth, Chest and Skeleton ?
	unsigned char flag3:1;			// ? Minions have it
	unsigned char flag2:1;			// ?
	unsigned char bInvisible:1;		// Only Pit Ghost has it
	unsigned char flag0:1;			// Not used, always 0
	// Leftmost


	unsigned char byte10;
	unsigned char byte11;
	unsigned char byte12;
	unsigned char byte13;		// 0, not used (except for unused AI)

	//--- byte 14 & 15 - Spell & Attacks types
	unsigned char bCm08:1;
	unsigned char bCm26:1;		// Push back attack
	unsigned char bCm0A:1;		// Steal attack
	unsigned char bCm0E:1;
	unsigned char bSpellFireball:1;
	unsigned char bSpellAntiGhost:1;
	unsigned char bSpellLightning:1;
	unsigned char bSpellPoisonCloud:1;
	unsigned char bSpellPoisonBolt:1;
	unsigned char bSpellPoisonBlob:1;
	unsigned char bSpellPush:1;
	unsigned char bSpellPull:1;
	unsigned char bSpellOther:4;
//	unsigned char byte14;
//	unsigned char byte15;
	
	unsigned char byte16;
	unsigned char byte17;		// 0, not used
	unsigned char byte18;		// 0/1 : Can switch triggers, doors
	unsigned char byte19;		// 0, not used
	unsigned char byte20;
	unsigned char byte21;		// 2 bits value
	unsigned char byte22;		// could that be about speed?

	unsigned char byte23;
	
	unsigned char byte24:4;		// some protection, what? Magic resistance??? or MORALE, FEAR, CALM RESISTANCE?
	unsigned char iFireResistance:4;	// 0xF = immune
	
	// 25 : poison resistance
	unsigned char iPoisonResistance:4;	// 0xF = immune
	unsigned char bits25:4;				// Flying chest and mummy have this? is that a resistance?

	unsigned char byte26;			// Flags again?, 0 for objects
//	unsigned char byte27;			// Flags; 240 for objects
	unsigned char b27f7:1;
	unsigned char b27f6:1;
	unsigned char b27f5:1;
	unsigned char b27f4:1;
	unsigned char b27f3:1;
	unsigned char b27f2:1;
	unsigned char b27f1:1;
	unsigned char b27f0:1;
	unsigned char byte28;			// 0 for objects, 1 4 9 for all other (creatures)

	unsigned char iWeight; // ~weight, 0xFF = can't be moved

	unsigned char fill[6];	

	// B30	1 = fountain / 34 = altar / 65-66 = tables
	// B31	Used for static objects only
	// B32	correspondance with B30 / B33
	// B33	About putting objects? only tables and cauldrons have it - true
	// B34
	// B35	0 , 1 or 2 : no idea.
} tSKEXE_AI_Definition;
#pragma pack()

enum
{
	AI_ARMOR			= 2,
	AI_HITPOINTS		= 4,
	AI_ATK_STRENGTH		= 6,
	AI_DEFENSE			= 8,
};

//------------------------------------------------------------------------------

enum
{
	SKULLSHEET_AI	= 0,
	SKULLSHEET_RUNECOST,
	SKULLSHEET_SPELL,
};

typedef struct
{
//--- Supposedly private part
	tSKExeVersion		iSKExeVersion;				// Exe mode = { 5_2a EN, 5_2c MULT, 5_2c patch }

	unsigned int iExeDataSize;
	char*	xExeData;

	//--- Static strings (including null)
	char	sSonglistPath[18];
	char	sVersionStringNormal[5];
	char	sVersionStringLoad[5];
	char	sGraphicsName[12];



	unsigned int				iNumberOfAI;
	tSKEXE_AI_Definition*		xAI;
	
	unsigned int				iNumberOfSpells;
	tSKEXE_Spell_Definition*	xSpells;

	tSKEXE_Rune_CostTable*		xRuneCostTable;

//--- public
	BOOL	bOnlyDataStructure;
	BOOL	bValid;

} tSkullExe;


int		SkullExe_Init(tSkullExe* xSkullExe);
int		SkullExe_Init_AI_Spells(tSkullExe* xSkullExe, int iNbAI, int iNbSpells);

int		SkullExe_Load(tSkullExe* xSkullExe, const char* sFilename);
int		SkullExe_Write(tSkullExe* xSkullExe, const char* sFilename);
int		SkullExe_ExportAITable(tSkullExe* xSkullExe, const char* sFilename);

//--- This is more for a test function, i.e. checking DM2 behaviour with empty tables
int		SkullExe_ResetHardTables(tSkullExe* xSkullExe);
int		SkullExe_InitDM1Creatures(tSkullExe* xSkullExe);
int		SkullExe_InitTELOSCreatures(tSkullExe* xSkullExe);
int		SkullExe_InitTORCCreatures(tSkullExe* xSkullExe);

int		SkullExe_UpdateExeTables(tSkullExe* xSkullExe);

int		SkullExe_CopyAIToAI(tSkullExe* xSkullExe, int iSourceAI, int iDestAI);

int		SkullExe_GetAIValueAt(tSkullExe* xSkullExe, int iIndex, int iAttribute);
int		SkullExe_SetAIValueAt(tSkullExe* xSkullExe, int iIndex, int iAttribute, int iValue);

int		SkullExe_Control(tSkullExe* xSkullExe);	// Control that current exe data is considered valid

tSKEXE_AI_Definition*		SkullExe_GetAI(tSkullExe* xSkullExe, int iAIIndex);
tSKEXE_Spell_Definition*	SkullExe_GetSpell(tSkullExe* xSkullExe, int iSpellIndex);

//------------------------------------------------------------------------------

#endif // __DM_SKULLEXE__H__
