//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Text Data
//------------------------------------------------------------------------------


#ifndef TEXTS_H
#define TEXTS_H

#include <stdio.h>
#include <stdlib.h>

extern int SKULLKEEP;
extern int THERONSQUEST;
extern int TELOS;

extern char* TEXTS[1024];
extern size_t totalTexts;
extern short adresses[1024];

extern void loadTexts (unsigned short *rawtexts);
extern char* getText (unsigned int number);
extern char* getTextContext (int iContext, unsigned int number);
extern unsigned int getTextsNumber ();

extern size_t encodeTexts (unsigned short *rawtexts);

extern void exportText ();
extern void importText ();

extern int addText(const char* sTextStrings);
extern int addTextContext(int iContext, const char* sTextStrings);

extern void convertToInternTexts ();
extern void convertToMasterTexts ();

extern void startTexts ();

#endif