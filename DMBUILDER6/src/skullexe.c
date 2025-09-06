//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2010 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Skullkeep
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#include <SkullExe.h>
#include <memory.h>
#include <stdio.h>

//------------------------------------------------------------------------------

#define FALSE	0
#define TRUE	1

//------------------------------------------------------------------------------

char* _DMSKull_Runes[] =
{
	"LO", "UM", "ON", "EE", "PAL", "MON",
	"YA", "VI", "OH", "FUL", "DES", "ZO",
	"VEN", "EW", "KATH", "IR", "BRO", "GOR",
	"KU", "ROS", "DAIN", "NETA", "RA", "SAR"
};

char* _DMSkull_SpellType[] = 
{
	"Unknown",
	"Potion",
	"Missile",
	"Enchantment",
	"Summoning"
};

char* _DMSkull_SkillNames[] =
{
"Global Fighter 0",
"Global Ninja 0",
"Global Priest 0",
"Global Wizard 0",
"Fighter 1: Swing",
"Fighter 2: Thrust",
"Fighter 3: Club",
"Fighter 4: Parry",
"Ninja 1: Steal",
"Ninja 2: Fight",
"Ninja 3: Throw",
"Ninja 4: Shoot",
"Priest 1: Identify",
"Priest 2: Heal",
"Priest 3: Influence",
"Priest 4: Defend",
"Wizard 1: Fire",
"Wizard 2: Air",
"Wizard 3: Earth",
"Wizard 4: Water"
};

char* _DMSkull_SpellNames[] = 
{
	"Long Light",
	"Darkness",
	"Spell Shield",
	"Invisibility",
	"Magical Shield",
	"Torch Light",
	"Aura of Wisdom",
	"Aura of Dexterity",
	"Fire Shield",
	"Aura of Vitality",
	"Aura of Strength",
	"Aura of Speed",
	"Spell Reflector",
	"Magical Marker",
	"Poison Cloud",
	"Lightning",
	"Fireball",
	"Strength Potion",
	"Dispell",
	"Poison Bolt",
	"Zo Spell",
	"Shield Potion",
	"Stamina Potion",
	"Wisdom Potion",
	"Vitality Potion",
	"Health Potion",
	"Anti Venin",
	"Dexterity Potion",
	"Mana Potion",
	"Summon Attack Minion",
	"Summon Guard Minion",
	"Summon U-Haul Minion",
	"Push",
	"Pull"
};

char* _DMSKull_AINames[64] =
{
"??? Amplifier Like",
"Laboratory Table",
"Stone Altar",
"Bush",
"Pillars",
"Stalagmite",
"Boulder",
"Fountain",
"Obelisks",
"Wood Table",
"Magik Cauldron",
"Skull Brazier",
"Trading Table",
"Scout Minion",
"Attack Minion",
"Carry Minion",
"Fetch Minion",
"Guard Minion",
"U-Haul Minion",
"Thorn Demon",
"Obelisk (Passable)",
"Vortex",
"Flame Orb",
"Cavern Bat",
"Glop",
"Rocky",
"Giggler",
"Thicket Thief",
"Tiger Striped Worm",
"Treant",
"Lord Dragoth",
"Dru Tan",
"Cave In",
"Merchants",
"??? Minion Template",
"Tower Bat",
"Archer Guard",
"Magik Reflector",
"Power Crystal",
"Evil Fountain",
"Spiked Wall",
"Spectre",
"Veg Mouth",
"Evil Attack Minions",
"Axeman",
"Stone Table",
"Mummy",
"Void Door",
"Dark Vexirk",
"Evil Guard Minion",
"Skeleton",
"Amplifier",
"Wolf",
"Pit Ghost",
"??? Door Ghost",
"Vexirk King",
"??? Obelisk Passable",
"Axeman Thief",
"Flying Chest",
"Barrel",
"Pedistal",
"Ghost",
"??? Evil Minion",
"??? Non Sense"
};

char DMSkullAI_CurrentNames[64][32];

//------------------------------------------------------------------------------

int	
SkullExe_Init(tSkullExe* xSkullExe)
{
	printf("SkullExe: Init %08x\n", xSkullExe);
	if (MVALID(xSkullExe))
	{
		xSkullExe->xExeData = NULL;
		xSkullExe->bValid = FALSE;
		xSkullExe->iExeDataSize = 0;
		xSkullExe->bOnlyDataStructure = FALSE;
		xSkullExe->xAI = NULL;
		xSkullExe->xRuneCostTable = NULL;
		xSkullExe->xSpells = NULL;
	}
	else
		return FALSE;
	return TRUE;
}

int
SkullExe_Init_AI_Spells(tSkullExe* xSkullExe, int iNbAI, int iNbSpells)
{
	if (MVALID(xSkullExe))
	{
		xSkullExe->xExeData = NULL;
		xSkullExe->bValid = FALSE;
		xSkullExe->iExeDataSize = 0;
		xSkullExe->bOnlyDataStructure = TRUE;

		xSkullExe->iNumberOfAI = iNbAI;
		xSkullExe->iNumberOfSpells = iNbSpells;

		xSkullExe->xAI = (tSKEXE_AI_Definition*) MEMCALLOC (iNbAI, sizeof(tSKEXE_AI_Definition));
		xSkullExe->xRuneCostTable = NULL;
		xSkullExe->xSpells = (tSKEXE_Spell_Definition*) MEMCALLOC (iNbSpells, sizeof(tSKEXE_Spell_Definition));
	}
	else
		return FALSE;
	return TRUE;

}

//------------------------------------------------------------------------------

int
SkullExe_Load(tSkullExe* xSkullExe, const char* sFilename)
{
	FILE* fp = NULL;
	unsigned int iDataSize = 0;
	
	if (!MVALID(xSkullExe))
		return FALSE;

	fp = fopen (sFilename, "rb");

	fseek(fp, 0, SEEK_END);
	iDataSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	xSkullExe->iExeDataSize = iDataSize;
	xSkullExe->xExeData = (char*) MEMCALLOC (iDataSize, sizeof(char));
	fread(xSkullExe->xExeData, iDataSize, 1, fp);

	fclose(fp);

	if (!MVALID(xSkullExe->xExeData))
		return FALSE;

	//--- Locate different structures
	xSkullExe->xAI = (tSKEXE_AI_Definition*) &(xSkullExe->xExeData[0x72778]);
	xSkullExe->xRuneCostTable = (tSKEXE_Rune_CostTable* ) &(xSkullExe->xExeData[0x765F0]);
	xSkullExe->xSpells = (tSKEXE_Spell_Definition*) &(xSkullExe->xExeData[0x76612]);

	xSkullExe->iNumberOfAI = _SKEXE_MAX_AI;
	xSkullExe->iNumberOfSpells = _SKEXE_MAX_SPELLS;

	xSkullExe->bValid = TRUE;
	printf("SkullExe: %08x\n", xSkullExe);
	
	return TRUE;
}


int
SkullExe_Write(tSkullExe* xSkullExe, const char* sFilename)
{
	FILE* fp = NULL;

	if (!MVALID(xSkullExe))
		return FALSE;

	SkullExe_UpdateExeTables(xSkullExe);

	fp = fopen (sFilename, "wb");
	fwrite(xSkullExe->xExeData, xSkullExe->iExeDataSize, 1, fp);
	fclose(fp);
	

	return TRUE;
}

int
SkullExe_ExportAITable(tSkullExe* xSkullExe, const char* sFilename)
{
	FILE* fp = NULL;

	if (!MVALID(xSkullExe))
		return FALSE;

	//SkullExe_UpdateExeTables(xSkullExe);

	fp = fopen (sFilename, "wb");
	fwrite(xSkullExe->xAI, sizeof(tSKEXE_AI_Definition)*xSkullExe->iNumberOfAI, 1, fp);
	fclose(fp);
	

	return TRUE;
}


//------------------------------------------------------------------------------

int
SkullExe_UpdateExeTables(tSkullExe* xSkullExe)
{
	//--- Copy tables back into exe
	if (MVALID(xSkullExe) && MVALID(xSkullExe->xAI))
	{
		;
	}
	else
		return FALSE;

	return TRUE;
}


//------------------------------------------------------------------------------

int
SkullExe_ResetHardTables(tSkullExe* xSkullExe)
{
	unsigned int i = 0;
	unsigned char* xpAIDef = NULL;

	printf("SkullExe: Reset Hard Tables %08x\n", xSkullExe);

	if (!MVALID(xSkullExe) || !MVALID(xSkullExe->xExeData))
	{
		return FALSE;
	}

	if (MVALID(xSkullExe->xAI))
	{
		for (i = 0; i < _SKEXE_MAX_AI; i++)
		{
			tSKEXE_AI_Definition* xAIs;
			xAIs = &(xSkullExe->xAI[i]);
			xpAIDef = (unsigned char*) xAIs;
			if (MVALID(xAIs))
			{
				memset(xAIs, 0, sizeof(tSKEXE_AI_Definition));

				xpAIDef[0] = 0xC0;
				xpAIDef[1] = 0x0A;
				xpAIDef[2] = 0x41;
				xpAIDef[3] = 0x21;

				xAIs->iHitPoints = 10;
				xpAIDef[6] = 0x06;
				xpAIDef[7] = 0x00;

				xpAIDef[8]  = 0x50;
				xpAIDef[9]  = 0x00;
				xpAIDef[10] = 0x02;
				xpAIDef[11] = 0x24;

				xpAIDef[12] = 0x01;
				xpAIDef[13] = 0x00;
				xpAIDef[14] = 0x01;
				xpAIDef[15] = 0x00;

				// 4 zeros

				xpAIDef[20] = 0x34;
				xpAIDef[21] = 0x10;
				xpAIDef[22] = 0x4A;
				xpAIDef[23] = 0x92;

				xpAIDef[24] = 0xD7;
				xpAIDef[25] = 0x06;
				xpAIDef[26] = 0xFC;
				xpAIDef[27] = 0xFF;

				xpAIDef[28] = 0x04;
				xpAIDef[29] = 0x41;
				xpAIDef[30] = 0x00;
				xpAIDef[31] = 0x00;

				xpAIDef[32] = 0x00;
				xpAIDef[33] = 0x00;
				xpAIDef[34] = 0x01;
				xpAIDef[35] = 0x01;

	//			xAIs->byte00 = 69;	// weirdly enough, 64 makes crash .. maybe it is related to "animated" vs non animated objects?
				/*
				xAIs->iArmor = 50;
				xAIs->iHitPoints = 10;
				xAIs->byte23 = 1;
				xAIs->iFireResistance = 1;
				xAIs->byte24 = 1;
				//TODO: check bin flags to make 240
	//			xAIs->byte27 = 240;
				xAIs->b27f0 = 1; // top left most
				xAIs->b27f1 = 1;
				xAIs->b27f2 = 1;
				xAIs->b27f3 = 1;

				xAIs->iWeight = 10;
				*/
			}
		}
	}

	return TRUE;
}

//------------------------------------------------------------------------------

int
SkullExe_CopyAIToAI(tSkullExe* xSkullExe, int iSourceAI, int iDestAI)
{
	unsigned int i = 0;
	unsigned char* xpAIDefSource = NULL;
	unsigned char* xpAIDefTarget = NULL;

	printf("SkullExe: %08x // Copy AI %02X to %02X\n", xSkullExe, iSourceAI, iDestAI);

	if (!MVALID(xSkullExe) || !MVALID(xSkullExe->xExeData))
	{
		return FALSE;
	}

	if (MVALID(xSkullExe->xAI))
	{
		tSKEXE_AI_Definition* xAI_Source;
		tSKEXE_AI_Definition* xAI_Dest;
		xAI_Source = &(xSkullExe->xAI[iSourceAI]);
		xAI_Dest = &(xSkullExe->xAI[iDestAI]);
		xpAIDefSource = (unsigned char*) xAI_Source;
		xpAIDefTarget = (unsigned char*) xAI_Dest;
		if (MVALID(xpAIDefSource) && MVALID(xpAIDefTarget))
		{
			memcpy(xpAIDefTarget, xpAIDefSource, sizeof(tSKEXE_AI_Definition));
		}
	}

	return TRUE;
}



//------------------------------------------------------------------------------

int
SkullExe_GetAIValueAt(tSkullExe* xSkullExe, int iIndex, int iAttribute)
{
	int iValue = 0;
	if (MVALID(xSkullExe) && MVALID(xSkullExe->xAI))
	{
		unsigned char* xpAIDef = NULL;
		tSKEXE_AI_Definition* xAIs;
		xAIs = &(xSkullExe->xAI[iIndex]);
		xpAIDef = (unsigned char*) xAIs;
		iValue = xpAIDef[iAttribute];
	}
	return iValue;
}

int
SkullExe_SetAIValueAt(tSkullExe* xSkullExe, int iIndex, int iAttribute, int iValue)
{
	int iUse2Bytes = 0;

	if (iAttribute == AI_HITPOINTS)
		iUse2Bytes = 1;

	if (MVALID(xSkullExe) && MVALID(xSkullExe->xAI))
	{
		unsigned char* xpAIDef = NULL;
		tSKEXE_AI_Definition* xAIs;
		xAIs = &(xSkullExe->xAI[iIndex]);
		xpAIDef = (unsigned char*) xAIs;

		if (iUse2Bytes == 1)
		{
			xpAIDef[iAttribute] = (iValue%256);
			xpAIDef[iAttribute+1] = (iValue>>8);
		}
		else
			xpAIDef[iAttribute] = iValue;
	}
	return iValue;
}


//------------------------------------------------------------------------------

int
SkullExe_InitDM1Creatures(tSkullExe* xSkullExe)
{
	unsigned int i = 0;
	unsigned char* xpAIDef = NULL;

	printf("SkullExe: Reset Hard Tables %08x\n", xSkullExe);

	if (!MVALID(xSkullExe) || !MVALID(xSkullExe->xExeData))
	{
		return FALSE;
	}

	SkullExe_ResetHardTables(xSkullExe);

	if (MVALID(xSkullExe->xAI))
	{
		tSKEXE_AI_Definition* xAIs;

		//--- Mummy
		xAIs = &(xSkullExe->xAI[10]);
		xpAIDef = (unsigned char*) xAIs;
		if (MVALID(xAIs))
		{
			xAIs->iHitPoints = 30;
		}
		//--- Screamer
		xAIs = &(xSkullExe->xAI[6]);
		xpAIDef = (unsigned char*) xAIs;
		if (MVALID(xAIs))
		{
			xAIs->iHitPoints = 50;
		}
	}

	return TRUE;
}

int
SkullExe_InitTELOSCreatures(tSkullExe* xSkullExe)
{
	unsigned int i = 0;
	unsigned char* xpAIDef = NULL;

	printf("SkullExe: Reset Hard Tables %08x\n", xSkullExe);

	if (!MVALID(xSkullExe) || !MVALID(xSkullExe->xExeData))
	{
		return FALSE;
	}

	//SkullExe_ResetHardTables(xSkullExe);

	if (MVALID(xSkullExe->xAI))
	{
		tSKEXE_AI_Definition* xAIs;

		//--- Mummy
		xAIs = &(xSkullExe->xAI[10]);
		xpAIDef = (unsigned char*) xAIs;
		if (MVALID(xAIs))
		{
			xAIs->iHitPoints = 1;
		}
		//--- Screamer
		xAIs = &(xSkullExe->xAI[6]);
		xpAIDef = (unsigned char*) xAIs;
		if (MVALID(xAIs))
		{
			xAIs->iHitPoints = 1;
		}
	}

	for (i = 0; i < _SKEXE_MAX_AI; i++)
	{
		tSKEXE_AI_Definition* xAIs;
		xAIs = &(xSkullExe->xAI[i]);
		xpAIDef = (unsigned char*) xAIs;
		if (MVALID(xAIs))
		{
			xAIs->iHitPoints = 5;	// not 1 because some creature may just die like this ????
		}
	}


	return TRUE;
}



int
SkullExe_InitTORCCreatures(tSkullExe* xSkullExe)
{
	unsigned int i = 0;
	unsigned char* xpAIDef = NULL;

	printf("SkullExe: Reset Hard Tables %08x\n", xSkullExe);

	if (!MVALID(xSkullExe) || !MVALID(xSkullExe->xExeData))
	{
		return FALSE;
	}

	//SkullExe_ResetHardTables(xSkullExe);

	/// DEBUG
	/*
	for (i = 0; i < _SKEXE_MAX_AI; i++)
	{
		tSKEXE_AI_Definition* xAIs;
		xAIs = &(xSkullExe->xAI[i]);
		xpAIDef = (unsigned char*) xAIs;
		if (MVALID(xAIs))
		{
			xAIs->iHitPoints = 5;	// not 1 because some creature may just die like this ????
		}
	}*/

	///--- Screamer: reuse 58 (Flying Chest) from a copy of Glop
	SkullExe_CopyAIToAI(xSkullExe, 24, 58);	// copy 24 to 58
	SkullExe_SetAIValueAt(xSkullExe, 58, AI_ARMOR, 0x60);
	SkullExe_SetAIValueAt(xSkullExe, 58, AI_DEFENSE, 0x20);
	SkullExe_SetAIValueAt(xSkullExe, 58, AI_ATK_STRENGTH, 0x04);
	SkullExe_SetAIValueAt(xSkullExe, 58, AI_HITPOINTS, 50);

	///--- Rock Monster: reuse 60 (Pedistal) from a copy of Thorn Demon
	SkullExe_CopyAIToAI(xSkullExe, 19, 60);	// copy 19 to 60
	SkullExe_SetAIValueAt(xSkullExe, 60, AI_ARMOR, 0x90);
	SkullExe_SetAIValueAt(xSkullExe, 60, AI_DEFENSE, 0x80);
	SkullExe_SetAIValueAt(xSkullExe, 60, AI_ATK_STRENGTH, 50);
	SkullExe_SetAIValueAt(xSkullExe, 60, AI_HITPOINTS, 600);


	return TRUE;
}


//------------------------------------------------------------------------------

tSKEXE_AI_Definition*
SkullExe_GetAI(tSkullExe* xSkullExe, int iAIIndex)
{
	if (!MVALID(xSkullExe) || !MVALID(xSkullExe->xAI))
		return NULL;

	if (iAIIndex < 0 || iAIIndex >= xSkullExe->iNumberOfAI)
		return NULL;

	return &(xSkullExe->xAI[iAIIndex]);
}


tSKEXE_Spell_Definition*
SkullExe_GetSpell(tSkullExe* xSkullExe, int iSpellIndex)
{
	if (!MVALID(xSkullExe) || !MVALID(xSkullExe->xSpells))
		return NULL;

	if (iSpellIndex < 0 || iSpellIndex >= xSkullExe->iNumberOfSpells)
		return NULL;

	return &(xSkullExe->xSpells[iSpellIndex]);
}

//------------------------------------------------------------------------------
/*
	unsigned char byte00;
	unsigned char byte01;
	unsigned char iArmor;
	unsigned char byte03;
	unsigned short iHealth;
	unsigned char iAttackStrength;
	unsigned char iPoison;

	unsigned char iDefense; // 255 = cannot be destroyed
	// or is dexterity?
	

	unsigned char flag0:1;			// leftmost
	unsigned char bInvisible:1;
	unsigned char flag2:1;
	unsigned char flag3:1;
	unsigned char flag4:1;
	unsigned char bFlying:1;		====> seems not flying. machine???
	unsigned char flag6:1;
	unsigned char bSplashAttack:1;	// rightmost

	unsigned char byte10;
	unsigned char byte11;
	unsigned char byte12;
	unsigned char byte13;
	unsigned char byte14;
	unsigned char byte15;
	unsigned char byte16;
	unsigned char byte17;
	unsigned char byte18;	// can switch button ?
	unsigned char byte19;
	unsigned char byte20;
	unsigned char byte21;
	unsigned char byte22;

	unsigned char byte23;
	
	unsigned char iFireResistance:4;	// 0xF = immune
	unsigned char byte24:4;
	

	unsigned char bits25:4;
	unsigned char iPoisonResistance:4;	// 0xF = immune
	unsigned char byte26;
	unsigned char byte27;
	unsigned char byte28;

	unsigned char iPushResistance; // ~weight

	unsigned char fill[6];
  */


//------------------------------------------------------------------------------



//--- Checks that the current exe file is considered valid and could be opened by the game
int
SkullExe_Control(tSkullExe* xSkullExe)
{
	printf("SkullExe: Control %08x\n", xSkullExe);

	if (!MVALID(xSkullExe) || !MVALID(xSkullExe->xExeData))
		return FALSE;

	printf("SkullExe: Exe Data %08x\n", xSkullExe->xExeData);
	printf("SkullExe: Size of Exe %06d\n", xSkullExe->iExeDataSize);

	//--- Check that size is 523169.
	//523169
	if (xSkullExe->iExeDataSize == 523169)
	{
		printf("SkullExe: Matches PC-DOS v5.2c (EGF MAC_USA81) [1995-07-28] version.\n");
	}
	else
	{
		printf("SkullExe: Unrecognized size. What version is this ?\n");
	}

	//--- Check exe signature is MZ

	//--- Check entry point is 0x0556

	return TRUE;
}
