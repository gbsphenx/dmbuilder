//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Monsters & Creatures
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <monster.h>

unsigned char monster_bonus[] = {
	37, 27, 2, 10, 25, 15, 41, 13,
	7, 30, 8, 19, 5, 9, 11, 17,
	5, 2, 15, 8, 35, 23, 25, 45,
	62, 1, 1
};
unsigned char monster_basehp[] = {
	150, 110, 10, 40, 101, 60, 165, 50,
	30, 120, 33, 80, 20, 39, 44, 70,
	20, 6, 60, 33, 144, 70, 100, 180,
	255, 1, 1
};



static unsigned int
giveHitPoints (char type, unsigned int level)
{
	return (monster_basehp[type]*level) + (rand()%monster_bonus[type]);
}

unsigned int
getMonsterLevel (monster_p monster)
{
	/* first monster determinates level */
	unsigned int level = 0;
	int basehp = 0;
	
	if (monster->type < 26)
		basehp = monster_basehp[monster->type];
	if (basehp <= 0)
		basehp = monster->health[0] + 1;
	level = (monster->health[0] + 1) / basehp;
	// WARNING : this is dangerous and can provoke infinite loop.
	//while ((level+1)*monster_basehp[monster->type] < (unsigned int) (monster->health[0] + 1))
	//	level ++;
	

	return level;
}

void
setNoviceMonster (monster_p monster)
{
	int i;
	for (i = 0; i < monster->number+1; i++)
		monster->health[i] = giveHitPoints (monster->type, 1);
}

void
setMonsterLevel (monster_p monster, char step)
{
	int i;
	unsigned int level = getMonsterLevel (monster);
	if (level == 0 && step == -1) step = 0;
	for (i = 0; i < monster->number+1; i++)
		monster->health[i] = giveHitPoints (monster->type, level+step);
}

void
setChestedMonster (monster_p monster, reference_p ref)
{
	short chested = *((short*) ref);
	if (chested == -1 || chested == -2)
		monster->chested = -2;
	monster->chested = chested;
};

short
unchestMonster (monster_p monster)
{
	short chested = monster->chested;
	monster->chested = -2;
	return chested;
}