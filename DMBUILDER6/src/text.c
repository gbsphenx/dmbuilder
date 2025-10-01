//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Text Data
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <text.h>
#include <editor.h>
#include <dms.h>
#include <dm.h>
#include <item.h>

/* max size allowed for coded text is 65535 hence nearly 196605 characters.
	supposing nearly 20 characters per lines. then about 9830 possible lines
 */

static char* txtDump[] = {"null{text!!"};
char* TEXTS[1024];
short TXTTYPE[1024];
size_t totalTexts = 0;
short adresses[1024];



struct dm_codon
{
	unsigned short letter3:5;
	unsigned short letter2:5;
	unsigned short letter1:5;
	unsigned short unused:1;
};

typedef struct dm_codon *codon_p;

char const Space = 26;
char const EndOfLine = 28;
char const Extension = 30;
char const EndOfBlock = 31;

//------------------------------------------------------------------------------

void
startTexts ()
{
	size_t i;
	for (i = 0; i < 1024; i++)
	{
		TEXTS[i] = *txtDump;
		TXTTYPE[i] = text_undefined;
	}
	totalTexts = 0;
}

//------------------------------------------------------------------------------
//	Functions for converting text object field
//------------------------------------------------------------------------------

unsigned short
findTextNumber (unsigned short offset)
{
	size_t i = 0;
	for (i = 0; i < 2048; i++)
		if (offset == adresses[i]) // offset must always refers to the BEGINNING of a text
			return i;
	return 0;
}

// Must replace offsets with text number
void
convertToInternTexts ()
{
	size_t i;
	if (SKULLKEEP == 0)
	{
		for (i = 0; i < getDungeon()->nObjects[category_Text]; i++)
		{
			text_p text;
			dm_reference ref;

			ref.category = category_Text;
			ref.id = i;
			text = (text_p) getItem (&ref);
			text->offset = findTextNumber (text->offset);
		}
	}
}

void
convertToMasterTexts ()
{
	size_t i;
	if (SKULLKEEP == 0)
	{
		for (i = 0; i < getDungeon()->nObjects[category_Text]; i++)
		{
			text_p text;
			dm_reference ref;
			ref.category = category_Text;
			ref.id = i;
			text = (text_p) getItem (&ref);
			text->offset = adresses[text->offset];
		}
	}
}

//------------------------------------------------------------------------------
//	Scan texts for heros
//------------------------------------------------------------------------------

void
findHeros ()
{
	// form of champion text:
	//"BOGUS\012ILLEGAL HERO\012\012M\012AABBAABBAABB\012ABABABABABABAB\012CCCCCCCCCCCCCCCC";
	//"chani}sayyadina{sihaya}}f}aainahnkaadd}edcpdjedbpdjcp}cageaaaaaaaagegc"
	size_t i;
	int iBreaksCount = 0;
	for (i = 0; i < totalTexts; i++)
	{
		if (TEXTS[i] != NULL)
		{
			size_t len = strlen (TEXTS[i]);
			size_t c = 0;
			char cv = 0;

			iBreaksCount = 0;
			printf("%d %s\n", i, TEXTS[i]);

			for (c = 0; c < len; c++)
			{
				cv = TEXTS[i][c];
				if (cv == '}')
					iBreaksCount++;
			}
			// Hero text will have 6 "}" at fixed relative position
			printf("breaks = %d\n", iBreaksCount);
			if (iBreaksCount == 6)
				TXTTYPE[i] = text_champion;
		}
		//TXTTYPE[i] = text_undefined;
	}
}


//------------------------------------------------------------------------------
//	Operations on a 3-letters codon
//------------------------------------------------------------------------------

char
getLetter (codon_p codon, size_t letter)
{
	switch (letter)
	{
		case 0: return (char) codon->letter1; break;
		case 1: return (char) codon->letter2; break;
		case 2: return (char) codon->letter3; break;
	}
	return EndOfBlock;
}

void
setLetter (codon_p codon, size_t letter, char value)
{
	switch (letter)
	{	
		case 0: codon->letter1 = value; break;
		case 1: codon->letter2 = value; break;
		case 2: codon->letter3 = value; break;
	}
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

void
extractLines (unsigned short* rawtexts)
{
	static char buffer[255];
	codon_p codon;
	size_t read = 0; // Number of read codons
	size_t text_i = 0; // Index of current row to be written
	unsigned short *textp = (unsigned short*) rawtexts; // Codon pointer

	while (read < getDungeon()->textsDataSize)
	{
		int end = 0;
		size_t i = 0; // Index within the buffer
		adresses[text_i] = read; // Address table of encoded texts!
		while (!end)
		{
			size_t k;
			codon = (codon_p) textp++;
			for (k = 0; k < 3; k++)
			{
				char letter = getLetter (codon, k);
				if (letter != EndOfBlock)
					buffer[i++] = letter + 'a';
				else
				{
					buffer[i++] = 0;
					end = 1;
				}
			}
			read++;
		}
		TEXTS[text_i] = (char*) calloc (strlen (buffer)+1, sizeof (char));
		strcpy (TEXTS[text_i++], buffer);
	}
	totalTexts = text_i;
}

size_t
encodeTexts (unsigned short *rawtexts)
{
//	FILE* debug;
	
	char *buffer;
	size_t i;
	size_t wrote = 0;
	codon_p codon = (codon_p) calloc (1, sizeof (struct dm_codon));

//	debug = fopen ("dump.txt", "w+b");
	for (i = 0; i < totalTexts; i++)
	{
		size_t k = 0;
		
		adresses[i] = wrote;
		
		buffer = TEXTS[i];
		while (buffer[k] != 0)
		{
			setLetter (codon, k%3, (char) (buffer[k] - 'a'));
			k++;
		//	fprintf (debug, "[%c%c%c]\n", codon->letter1+'a', codon->letter2+'a', codon->letter3+'a');
			if (k%3 == 0)
			{
				rawtexts[wrote++] = *((short*) codon);
		//		fprintf (debug, "[%c%c%c]\n\n", codon->letter1+'a', codon->letter2+'a', codon->letter3+'a');
				//fwrite (codon, 2, 1, debug);
				codon->letter1 = 31;
				codon->letter2 = 0;
				codon->letter3 = 0;
			}
		}
		setLetter (codon, k%3, 31);
		rawtexts[wrote++] = *((short*) codon);
	}
//	fclose (debug);
	return wrote;
}


//------------------------------------------------------------------------------
//	Functions called for having texts in a dungeon
//------------------------------------------------------------------------------


void
loadTexts (unsigned short *rawtexts)
{
	extractLines (rawtexts);
	findHeros ();
	if (SKULLKEEP == 0)
		convertToInternTexts ();
}


void
exportText ()
{
	size_t i;
	FILE *fp;

	fp = fopen ("export.txt", "w+t");

	for (i = 0; i < totalTexts; i++)
		fprintf (fp, "%04d %s\n", i, TEXTS[i]);

	fclose (fp);
}


void
importText ()
{
	size_t i;
	FILE *fp;
	char buffer[300];
	char textbuffer[300];

	fp = fopen ("export.txt", "rt");

	totalTexts = 0;
	while (fgets (buffer, 299, fp) != NULL)
	{
		sscanf (buffer, "%04d %s", &i, textbuffer);
		if (TEXTS[i] != NULL)
			free (TEXTS[i]);
		TEXTS[i] = (char*) calloc (strlen (textbuffer)+1, sizeof (char));
		strcpy (TEXTS[i], textbuffer);
		totalTexts++;
	}
	fclose (fp);
}

int
addText(const char* sTextStrings)
{
	int iTextNum = -1;
	if (totalTexts < 1024)
	{
		TEXTS[totalTexts] = (char*) calloc (strlen (sTextStrings)+1, sizeof (char));
		strcpy (TEXTS[totalTexts], sTextStrings);
		iTextNum = totalTexts;
		totalTexts++;
	}
	return iTextNum;
}

int 
addTextContext(int iContext, const char* sTextStrings)
{
	int iTextNum = -1;
	if (xTblDunSaveContexts[iContext].nTotalTexts < 1024)
	{
		xTblDunSaveContexts[iContext].xTextStrings[xTblDunSaveContexts[iContext].nTotalTexts] = (char*) calloc (strlen (sTextStrings)+1, sizeof (char));
		strcpy (xTblDunSaveContexts[iContext].xTextStrings[xTblDunSaveContexts[iContext].nTotalTexts], sTextStrings);
		iTextNum = xTblDunSaveContexts[iContext].nTotalTexts;
		xTblDunSaveContexts[iContext].nTotalTexts = xTblDunSaveContexts[iContext].nTotalTexts+1;

		if (iGlbCurrentContext == iContext)
		{
			totalTexts = xTblDunSaveContexts[iContext].nTotalTexts;
			TEXTS[iTextNum] = (char*) calloc (strlen (sTextStrings)+1, sizeof (char));
			strcpy (TEXTS[iTextNum], sTextStrings);
		}
	}
	return iTextNum;
}


//------------------------------------------------------------------------------

char*
getText (unsigned int number)
{
	return TEXTS[number];
}

short
getTextType (unsigned int number)
{
	return TXTTYPE[number];
}

char*
getTextContext (int iContext, unsigned int number)
{
	return xTblDunSaveContexts[iContext].xTextStrings[number];
}

unsigned int
getTextsNumber ()
{
	return totalTexts;
}