/*
    Copyright (C) 1999
    For non-commercial use only.

    File	: tga.c
    Date	: 05/05/1999
    Author	: Nate Miller
    Contact	: vandals1@home.com

    Change Log
    **********
    8/07/99 - made the code portable
    7/26/99 - added code to fclose () a file when there is an error
    6/11/99 - added support for 8bit images, changed commenting
*/
#include "image_tga.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include "memory.h"

GLenum texFormat;


