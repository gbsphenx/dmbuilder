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
#include <math.h>

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

dm_text_champion	edit_champion;
dm_text_plain		edit_plain_text;

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

int
convertABCDToInt(char* sAlpha4)
{
	int val = 0;
	int i = 0;
	for (i = 0; i < 4; i++)
		val += (sAlpha4[i] - 'a')*pow(16,(3-i));
	return val;
}

int
convertABToInt(char* sAlpha2)
{
	int val = 0;
	int i = 0;
	for (i = 0; i < 2; i++)
		val += (sAlpha2[i] - 'a')*pow(16,(1-i));
	return val;
}

int
convertAToInt(char* sAlpha)
{
	int val = 0;
	int i = 0;
	val += sAlpha[0] - 'a';
	return val;
}



void
convertIntToA(char* sAlpha, int value)
{
	sAlpha[0] = value + 'a';
}

void
convertIntToAB(char* sAlpha, int value)
{
	int i = 0;
	for (i = 0; i < 2; i++)
		sAlpha[i] = (int)(value/pow(16,1-i))%16 + 'a';
}

void
convertIntToABCD(char* sAlpha, int value)
{
	int i = 0;
	for (i = 0; i < 4; i++)
		sAlpha[i] = (int)(value/pow(16,3-i))%16 + 'a';
}

void
initTextToChampion (dm_text_champion* sChampionStruct)
{
	size_t i;
	if (sChampionStruct != NULL)
	{	
		strcpy(sChampionStruct->firstname, "BOGUS");
		strcpy(sChampionStruct->lastname, "THE CHAMPION");
		sChampionStruct->gender = 'F';

		sChampionStruct->health = 40;
		sChampionStruct->stamina = 500;
		sChampionStruct->mana = 10;



		for (i = 0; i < 7; i++)
			sChampionStruct->attributes[i] = 30;
		for (i = 0; i < 16; i++)
			sChampionStruct->skills[i] = 2;
	}
}

void
convertChampionToText (unsigned int number, dm_text_champion* sChampionStruct)
{
	char* sOutputText = NULL;
	dm_text_champion* scs = sChampionStruct;

	if (sChampionStruct != NULL)
	{
		sOutputText = TEXTS[number];

		// size of text is
		// fname (max 7) + lname (max ??) + 1 + 3*4 + 7*2 + 16*1 + 6 breaks = size (name) + 48
		if (sOutputText != NULL)
			free (sOutputText);

		sOutputText = calloc ( 48 + 7 + 32, sizeof(char));

		if (sOutputText != NULL)
		{
			int i = 0;
			char fname[8];
			char lname[31];
			char g = 'f';
			char bsk[13];
			char attr[15];
			char skills[17];
			memset (fname, 0, 8);
			memset (lname, 0, 31);
			memset (bsk, 'a', 13); bsk[12] = 0;
			memset (attr, 'a', 15); attr[14] = 0;
			memset (skills, 'a', 17); skills[16] = 0;

			//printf("1) %s}%s}}%c}%s}%s}%s", fname, lname, g, &bsk, &attr, &skills);
			//sprintf(sOutputText, "1) %s}%s}}%c}%s}%s}%s", fname, lname, g, &bsk, &attr, &skills);
			//printf("bsk = %8x / health = %03d / champion %8x\n", bsk, scs->health, sChampionStruct);
			convertIntToABCD(&bsk[0], scs->health);
			convertIntToABCD(&bsk[4], scs->stamina);
			convertIntToABCD(&bsk[8], scs->mana);

			for (i = 0; i < 7; i++)
			{
				fname[i] = scs->firstname[i];
				if (fname[i] >= 'A' && fname[i] <= 'Z')
					fname[i] = fname[i] + ('a' - 'A');
				if (fname[i] == ' ' || fname[i] == '_')
					fname[i] = '{';
			}
			for (i = 0; i < 30; i++)
			{
				lname[i] = scs->lastname[i];
				if (lname[i] >= 'A' && lname[i] <= 'Z')
					lname[i] = lname[i] + ('a' - 'A');
				if (lname[i] == ' ' || lname[i] == '_')
					lname[i] = '{';
			}
			g = scs->gender;
			if (g == 'M') g = 'm';
			else if (g == 'F') g = 'f';

			for (i = 0; i < 7; i++)
				convertIntToAB(&(attr[i*2]), scs->attributes[i]);

			for (i = 0; i < 16; i++)
				convertIntToA(&(skills[i]), scs->skills[i]);

			//printf("%s:%s", scs->firstname, scs->lastname);
			//printf("2) %s}%s}}%c}%s}%s}%s", fname, lname, g, &bsk, &attr, &skills);
			sprintf(sOutputText, "%s}%s}}%c}%s}%s}%s", fname, lname, g, &bsk, &attr, &skills);
			//printf("New text = %s\n", sOutputText);

			TEXTS[number] = sOutputText;
		}

	}
}


void
convertTextToChampion (unsigned int number, dm_text_champion* sChampionStruct)
{
	int iBreaksCount = 0;
	int iSegment = 0;
	int iSegmentSize = 0;
	char* sInputText = NULL;
	size_t i = 0;

	if (TXTTYPE[number] != text_champion)
		return;

	sInputText = TEXTS[number];

	if (sChampionStruct != NULL)
	{
		if (sInputText != NULL)
		{
			size_t len = strlen (sInputText);
			size_t c = 0;
			size_t out = 0;
			char cv = 0;
			char hsm[12];
			char attr[14];
			char skills[16];
			int digit[4];

			memset(sChampionStruct->firstname, 0, sizeof(sChampionStruct->firstname));
			memset(sChampionStruct->lastname, 0, sizeof(sChampionStruct->lastname));

			iBreaksCount = 0;
			iSegment = 0;

			for (c = 0; c < len; c++)
			{
				cv = sInputText[c];
				if (cv == '}')
				{
					iBreaksCount++;
					iSegment++;
					iSegmentSize = 0;
					out = 0;
				}
				else if (iSegment == 0)
				{
					sChampionStruct->firstname[out] = cv + ('A'-'a');
					if (cv == '{')
						sChampionStruct->firstname[out] = cv;
					out++;
				}
				else if (iSegment == 1)
				{
					sChampionStruct->lastname[out] = cv + ('A'-'a');
					if (cv == '{')
						sChampionStruct->lastname[out] = ' ';
					else if (cv == 0x7F)
					{
						if (TEXTS[number][c+1] == 'c')
						{
							sChampionStruct->lastname[out++] = 'T';
							sChampionStruct->lastname[out++] = 'H';
							sChampionStruct->lastname[out++] = 'E';
							sChampionStruct->lastname[out] = ' ';
							c++;
						}
					}
					out++;
				}
				else if (iSegment == 3)
				{
					if (cv == 'f')
						sChampionStruct->gender = 'F';
					else if (cv == 'm')
						sChampionStruct->gender = 'M';
				}
				else if (iSegment == 4)
				{
					hsm[out] = cv;
					out++;
				}
				else if (iSegment == 5)
				{
					attr[out] = cv;
					out++;
				}
				else if (iSegment == 6)
				{
					skills[out] = cv;
					out++;
				}
				else
					iSegmentSize++;
			}
			sChampionStruct->health = convertABCDToInt(&hsm[0]);
			sChampionStruct->stamina = convertABCDToInt(&hsm[4]);
			sChampionStruct->mana = convertABCDToInt(&hsm[8]);

			for (i = 0; i < 7; i ++)
				sChampionStruct->attributes[i] = convertABToInt(&attr[i*2]);
			for (i = 0; i < 16; i ++)
				sChampionStruct->skills[i] = convertAToInt(&skills[i]);
		}
	}
}

void
convertTextToPlain (unsigned int number, dm_text_plain* sPlainText)
{
	int iBreaksCount = 0;
	int iSegment = 0;
	int iSegmentSize = 0;
	char* sInputText = NULL;
	size_t i = 0;
	int out = 0;
	int outrow = 0;

	if (TXTTYPE[number] == text_champion)
		return;

	sInputText = TEXTS[number];

	if (sPlainText != NULL)
	{
		if (sInputText != NULL)
		{
			size_t len = strlen (sInputText);
			size_t c = 0;
			size_t out = 0;
			char cv = 0;

			iBreaksCount = 0;
			iSegment = 0;

			for (outrow = 0; outrow < 9; outrow++)
				memset(sPlainText->textline[outrow], 0, 100);

			outrow = 0;

			for (c = 0; c < len; c++)
			{
				cv = TEXTS[number][c];
				if (cv == '}')
				{
					iBreaksCount++;
					iSegment++;
					iSegmentSize = 0;
					outrow++;
					out = 0;
				}
				else
				{
					if (cv >= 'a' && cv <= 'z')
						sPlainText->textline[outrow][out] = cv + ('A'-'a');
					else if (cv == '{')
						sPlainText->textline[outrow][out] = ' ';
					else if (cv == '|')
						sPlainText->textline[outrow][out] = '.';
					else if (cv == 0x7F)
					{
						if (TEXTS[number][c+1] == 'b')
						{
							sPlainText->textline[outrow][out] = '!';
							c++;
						}
						else if (TEXTS[number][c+1] == 'c')
						{
							sPlainText->textline[outrow][out++] = 'T';
							sPlainText->textline[outrow][out++] = 'H';
							sPlainText->textline[outrow][out++] = 'E';
							sPlainText->textline[outrow][out] = ' ';
							c++;
						}
						else if (TEXTS[number][c+1] == 'd')
						{
							sPlainText->textline[outrow][out++] = 'Y';
							sPlainText->textline[outrow][out++] = 'O';
							sPlainText->textline[outrow][out++] = 'U';
							sPlainText->textline[outrow][out] = ' ';
							c++;
						}
					}
					else
						sPlainText->textline[outrow][out] = cv;
					iSegmentSize++;
					out++;
				}
			}

		}
	}
}

void
resetTypes ()
{
	size_t i;
	for (i = 0; i < totalTexts; i++)
	{
		TXTTYPE[i] = text_undefined;
	}
}

void
findHeros ()
{
	// form of champion text:
	//"BOGUS\012ILLEGAL HERO\012\012M\012AABBAABBAABB\012ABABABABABABAB\012CCCCCCCCCCCCCCCC";
	//"chani}sayyadina{sihaya}}f}aainahnkaadd}edcpdjedbpdjcp}cageaaaaaaaagegc"
	//}}1}12}14}16
	size_t i;
	int iSegment = 0;
	int iSegmentSize = 0;
	int iBreaksCount = 0;
	int iHeroCandidate = 0;
	for (i = 0; i < totalTexts; i++)
	{
		if (TEXTS[i] != NULL)
		{
			size_t len = strlen (TEXTS[i]);
			size_t c = 0;
			char cv = 0;

			iHeroCandidate = 0;
			iBreaksCount = 0;
			iSegment = 0;
			//printf("%d %s\n", i, TEXTS[i]);

			for (c = 0; c < len; c++)
			{
				cv = TEXTS[i][c];
				if (cv == '}')
				{
					//printf("break/size = %d %d\n", iSegment, iSegmentSize);
					if (iSegment == 4 && iSegmentSize == 12)
						iHeroCandidate = 1;
					else if (iSegment == 5 && iSegmentSize != 14)
						iHeroCandidate = 0;
					else if (iSegment == 6 && iSegmentSize != 16)
						iHeroCandidate = 0;
					iBreaksCount++;
					iSegment++;
					iSegmentSize = 0;
				}
				else
					iSegmentSize++;
			}
			//printf("end/size = %d %d\n", iSegment, iSegmentSize);
			// Hero text will have 6 "}" at fixed relative position
			//printf("breaks = %d\n", iBreaksCount);
			if (iBreaksCount == 6 && iHeroCandidate == 1)
				TXTTYPE[i] = text_champion;
			else if (iBreaksCount >= 8)
				TXTTYPE[i] = text_narrative;
			else if (iBreaksCount >= 4)
				TXTTYPE[i] = text_scroll;
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
	resetTypes ();
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
createEmptyTextChampion()
{
	int itxt = addText ("bogus}illegal{hero}}m}aabbaabbaabb}ababababababab}cccccccccccccccc");
	TXTTYPE[itxt] = text_champion;
	return itxt;
}

int
createEmptyText()
{
	return createEmptyTextChampion ();
	//return addText ("edit}new}text");
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

void
controlTextAttributeValue (int subattribute, int deltavalue)
{
	int iTextNum = getTextCursor (cursor_Text);
	int oldvalue = 0;
	int newvalue = 0;

	if (subattribute >= 13 && subattribute <= 28)
	{
		int skill = subattribute - 13;
		edit_champion.skills[skill] += deltavalue;
		if ( edit_champion.skills[skill] < 0)
			edit_champion.skills[skill] = 0;
		else if ( edit_champion.skills[skill] > 15)
			edit_champion.skills[skill] = 15;
	}
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

//------------------------------------------------------------------------------

void
putTextToEditBuffer (unsigned int number)
{
	if (TXTTYPE[number] == text_champion)
	{
		convertTextToChampion (number, &edit_champion);
	}
//edit_champion
//edit_plain_text
}

void
putEditBufferToText (unsigned int number)
{
	if (TXTTYPE[number] == text_champion)
	{
		convertChampionToText (number, &edit_champion);
	}
}


//------------------------------------------------------------------------------
