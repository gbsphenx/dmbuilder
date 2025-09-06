//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Memory Management
//------------------------------------------------------------------------------

#ifndef MEMORY_H
#define MEMORY_H

#include <string.h>	// for NULL
//#include <stdio.h>

void*	MEMCALLOC (unsigned int nbElem, unsigned int sizeOfElem);
void*	MEMALLOC (unsigned int size);
void	MEMFREE (void* pointer);
int		MVALID (void* xPointer);

/*
#define VIEWCALLOC (_P_, _nb_, _size_) \
{\
	printf ("Allocating %s", #_P_);\
	_P_ = MEMCALLOC (_nb_, _size_)\
}
*/
#endif