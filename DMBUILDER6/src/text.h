//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Text Data
//------------------------------------------------------------------------------

// a message can have 53 chars per line

#ifndef TEXTS_H
#define TEXTS_H

#include <stdio.h>
#include <stdlib.h>

enum 
{
	text_undefined = 0,
	text_champion = 1,
	text_narrative = 2,
	text_wall = 3,
	text_scroll = 4,
};

typedef struct 
{
	char firstname[10];	// 7 chars
	char lastname[30];	// x chars
	char gender;		// f or m
	int health;
	int stamina;
	int mana;
	short attributes[7];
	int skills[16];
	//
	char txthealth[4];
	char txtstamina[4];
	char txtmana[4];
	//
	char editnumber[8];

} dm_text_champion;

typedef struct
{
	char textline[9][100];
} dm_text_plain;


extern dm_text_champion	edit_champion;
extern dm_text_plain	edit_plain_text;


extern int SKULLKEEP;
extern int THERONSQUEST;
extern int TELOS;

extern char* TEXTS[1024];
extern short TXTTYPE[1024];
extern size_t totalTexts;
extern short adresses[1024];

extern void loadTexts (unsigned short *rawtexts);
extern char* getText (unsigned int number);
extern short getTextType (unsigned int number);
extern char* getTextContext (int iContext, unsigned int number);
extern unsigned int getTextsNumber ();

extern size_t encodeTexts (unsigned short *rawtexts);

extern void exportText ();
extern void importText ();

extern int createEmptyText();
extern int createEmptyTextChampion();
extern int addText(const char* sTextStrings);
extern int addTextContext(int iContext, const char* sTextStrings);

extern void putTextToEditBuffer (unsigned int number);
extern void putEditBufferToText (unsigned int number);

extern void initTextToChampion (dm_text_champion* sChampionStruct);
extern void convertTextToChampion (unsigned int number, dm_text_champion* sChampionStruct);
extern void convertTextToPlain (unsigned int number, dm_text_plain* sPlainText);



extern void convertChampionToText (unsigned int number, dm_text_champion* sChampionStruct);

extern void convertToInternTexts ();
extern void convertToMasterTexts ();

extern void controlTextAttributeValue (int subattribute, int deltavalue);
extern void controlTextChar (int textnumber, int keyvalue);

extern void startTexts ();

#endif