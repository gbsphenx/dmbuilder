//------------------------------------------------------------------------------
//  DMBuilder
//  Copyright (c) 2001 - 2025 Sphenx
//  Licensed under the MIT License (see LICENSE file for details)
//------------------------------------------------------------------------------
// Memory Management
//------------------------------------------------------------------------------


#include <memory.h>

#include <stdlib.h>
#include <stdio.h>

static long MemAllocated = 0;
static unsigned int KiloMem = -1;

#define	FALSE	0
#define TRUE	1

int
MVALID (void* xPointer)
{
	if (xPointer == NULL)
		return FALSE;
	return TRUE;
}

void*
MEMCALLOC (unsigned int nbElem, unsigned int sizeOfElem)
{
	void* pointer;
	unsigned int i;
	unsigned int k;
	unsigned int allocatedBytes = nbElem * sizeOfElem;
	MemAllocated += allocatedBytes;
	pointer = (void*) malloc (allocatedBytes + sizeof (unsigned int));
	k = MemAllocated>>14;
/*	if (k != KiloMem)
	  {
	    printf ("MEM (%p): Currently using %d bytes\n", pointer, MemAllocated);
	    KiloMem = k;
	  }
*/	((unsigned int*) pointer)[0] = allocatedBytes;
	for (i = 0; i < allocatedBytes; i++)
		((char*)pointer)[sizeof(unsigned int)+i] = 0;
	return (void*) ((int*)pointer + 1);
//	return (void*) calloc (nbElem, sizeOfElem);
}

void*
MEMALLOC (unsigned int size)
{
	void* pointer;
	unsigned int allocatedBytes = size;
	unsigned int k;
	MemAllocated += allocatedBytes;
	pointer = (void*) malloc (allocatedBytes + sizeof (unsigned int));
	k = MemAllocated>>14;
	if (k != KiloMem)
	  {
	    printf ("MEM (%p): Currently using %d bytes\n", pointer, MemAllocated);
	    KiloMem = k;
	  }
	((unsigned int*) pointer)[0] = allocatedBytes;
	return (void*) ((int*)pointer + 1);
//	return (void*) calloc (nbElem, sizeOfElem);
}

void
MEMFREE (void* pointer)
{
	unsigned int deallocatedBytes;
	unsigned int k;
	pointer = (unsigned int*)pointer - 1;
	deallocatedBytes = ((unsigned int*)pointer)[0];

	MemAllocated -= deallocatedBytes;
	k = MemAllocated>>10;
/*	if (k != KiloMem)
	  {
	    printf ("MEM (%p): Freeing %d bytes\n", pointer, deallocatedBytes);
	    printf ("MEM: Currently using %d bytes\n", MemAllocated);
	    KiloMem = k;
	  }
*/	free (pointer);
	return;
}
