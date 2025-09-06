//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Dungeon Editor - Monsters & Creatures
//------------------------------------------------------------------------------

#ifndef MONSTER_H
#define MONSTER_H

#include <reference.h>
#include <dms.h>

extern void setMonsterLevel (monster_p monster, char step);
extern unsigned int getMonsterLevel (monster_p monster);

extern void setNoviceMonster (monster_p monster);

extern void setChestedMonster (monster_p monster, reference_p ref);
extern short unchestMonster (monster_p monster);

#endif