//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// File Operation
//------------------------------------------------------------------------------

#include <version.h>

#include <fileop.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <assert.h>

#include <loaddungeon.h>

typedef struct 
{
	char* filename;
	char dungeontype;
} file_t;

char *filenames[64];
static unsigned int nNames = 0;

file_t Files[64];


struct dirent *currentFile;

void
updateFileNamesFromListFile (char *s)
{
	static unsigned int checked = 0;
	FILE* fp;
	char buffer[300];
	unsigned int i = 0;
	if (checked) return;

	fp = fopen (s, "rt");
	do{
		if ((fgets (buffer, 299, fp)) == NULL)
			break;
		if (buffer[0] == ' ') continue;
		filenames[i] = (char*) calloc (strlen (buffer)+1, sizeof (char));
		strcpy (filenames[i], buffer);
		if (buffer[0] == 0) break;
		i++;
		//break;
	} while (1);
	nNames = i;

	fclose (fp);
	checked = 1;
}

void
updateFileNamesFromDirEntries ()
{
	static unsigned int checked = 0;
	if (checked) return;
	{
		char currentDir[128];
		unsigned int i = 0;
		TCHAR *buffer;
		HANDLE hfile;
		WIN32_FIND_DATA fileinfo;


		GetCurrentDirectory (127, currentDir);

		hfile = FindFirstFile ("*.dat", &fileinfo);
		if (hfile == INVALID_HANDLE_VALUE)
			return;
		else
		{	
			do{
			buffer = fileinfo.cFileName;
		//	printf ("%s\n", buffer);
			Files[i].filename = (char*) calloc (strlen (buffer)+1, sizeof (char));
			strcpy (Files[i].filename, buffer);

	//		Files[i].dungeontype = supposeDungeon (buffer);
			i++;
			}
			while (FindNextFile (hfile, &fileinfo));
		}

		nNames = i;
		checked = 1;
	}
}



unsigned int
numberOfFilesToLoad ()
{
	return nNames;
}

char *
getFileName (unsigned int n)
{
	assert (n >= 0 && n < nNames);
	return Files[n].filename;
}

char
getFileDungeonType (unsigned int n)
{
	assert (n >= 0 && n < nNames);
	return Files[n].dungeontype;
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

void
updateFileNames ()
{
	updateFileNamesFromDirEntries ();
//	CheckCustomsFileNames ();
}
