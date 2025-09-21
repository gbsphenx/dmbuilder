//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Display Text
//------------------------------------------------------------------------------

#ifndef DISPLAY_TEXT_H
#define DISPLAY_TEXT_H

#include <reference.h>

extern void loadTextFont ();

extern void printGeneralHelpInfo ();
extern void printGeneralMapInfo ();
extern void printMainMapHelpInfo ();
extern void printMapPropertiesHelpInfo ();
extern void printNewObjectHelpInfo ();

extern void printGeneralMessage (char *message);

extern void printSelectedText (reference_p refp);

extern void printNewItemStats ();

//extern void printActivator (reference_p refp, int wall);

extern void displaySelectedTextList (unsigned int select);

void text_frame_container (reference_p reference, int x, int y, float l);
void text_frame_teleport (reference_p reference, int x, int y, float l);
void text_frame_monster (reference_p reference, int x, int y, float l);
void text_frame_weapon (reference_p reference, int x, int y, float l);
void text_frame_clothing (reference_p reference, int x, int y, float l);
void text_frame_potion (reference_p reference, int x, int y, float l);
void text_frame_misc (reference_p reference, int x, int y, float l);
void text_frame_scroll (reference_p reference, int x, int y, float l);
void text_frame_actuator (reference_p reference, int wall, int x, int y, float l);
void text_frame_simple_actuator (reference_p reference, int x, int y, float l);
void text_frame_text (reference_p reference, int x, int y, float l);

extern void printDungeonSpecificationsInfo ();
extern void printLevelSpecificationsInfo ();
extern void printAIDetailsInfos();

extern void reinitTextStrings();


extern void beginTextOutput ();
extern void setTextProperties (int size, double r, double g, double b);
extern void setWarningText ();
extern void outputTextLineAt (int x, int y, char* string, ...);
extern void outputTextLine (char* string, ...);
extern void outputTextString (char* string, ...);

#endif



