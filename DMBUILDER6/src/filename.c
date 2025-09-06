//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Filename
//------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <filename.h>

/* 0 is used for LOAD, 1 for SAVE */

/* these strings have 80 characters */
char dungeon_file[][81] = {
"                                                                               ",
"                                                                               "};

char fileop_txt[][8] = {"load ", "save "};
char fileopsuc_txt[][8] = {" loaded", " saved"};

char buffer[300];

static char exportname[] = "texts.txt";
static char filename[] = "filename.txt";
static char dungeondefaultname[] = "dungeon.dat";
static FILE *namesfile;

//------------------------------------------------------------------------------

char *
getLoadDungeonName ()
{
	return dungeon_file[0];
}

char *
getSaveDungeonName ()
{
	return dungeon_file[1];
}

char *
fileopFailedMessage (int f)
{
	// assert (f == 0 || f == 1);
	strcpy (buffer, "Failed to ");
	strcat (buffer, fileop_txt[f]);
	strcat (buffer, dungeon_file[f]);
	return buffer;
}

char *
fileopSuccessMessage (int f)
{
	// assert (f == 0 || f == 1);
	strcpy (buffer, dungeon_file[f]);
	strcat (buffer, fileopsuc_txt[f]);
	return buffer;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

void
createAndOpen ()
{
	namesfile = fopen (filename, "w+t");
	fprintf (namesfile, "load: %s\nsave: %s\ntext: %s", dungeondefaultname, dungeondefaultname,
		exportname);
	fclose (namesfile);
	namesfile = fopen (filename, "rt");
}


//------------------------------------------------------------------------------

void
readFileNames ()
{
	namesfile = fopen (filename, "rt");
	if (namesfile == NULL)
		createAndOpen ();
	fscanf (namesfile, "load: %s\n", dungeon_file[0]);
	fscanf (namesfile, "save: %s\n", dungeon_file[1]);
	fscanf (namesfile, "text: %s\n", exportname);
	fclose (namesfile);
}

