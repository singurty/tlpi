/* Exercise 7-2 */

/* bmalloc = better malloc, our malloc
 * bfree = better free, our free
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FREES 1000

size_t NEXT_FREE = 0; /* at what index next free block should be added */
void *freeList[MAX_FREES];

void *bmalloc(size_t size)
{
	/* freeLIst empty, gotta sbrk */
	if (NEXT_FREE == 0) {
		void *programBreak = sbrk(size + sizeof(size_t));
		/* add metadata before the data segment giving the block form:
		 * Length of block + Acutal data
		 */
		memcpy(programBreak, &size, sizeof(size_t));
		/* return just the data segment hiding the metadata */
		return programBreak + sizeof(size_t);
	}
	size_t currentFree = NEXT_FREE - 1;
	size_t totalSize = 0;
	for (; totalSize<(size + sizeof(size_t)); currentFree--) {
		totalSize += *(size_t *) freeList[currentFree] + sizeof(size_t);
		printf("totalSize is %zu\n", totalSize);
	}
	NEXT_FREE = currentFree - 1;
	memcpy(freeList[currentFree], &size, sizeof(size_t));
	return freeList[currentFree] + sizeof(size_t);
}

/* finds a block in free list that can serially attach to the head (position=0) or tail (position=1) of another block, forming a chain */
int findChainMate(int freeIndex, int position)
{
	if (position == 0) {
		void *prevPtr = freeList[freeIndex] + sizeof(size_t);
		void *nextPtr;
		for (int i=0; i<NEXT_FREE; i++) {
			nextPtr = freeList[i] + sizeof(size_t) + sizeof(void *);
			if (nextPtr == prevPtr)
				return i;
		}
	}
	if (position == 1) {
		void *nextPtr = freeList[freeIndex] + sizeof(size_t) + sizeof(void *);
		void *prevPtr;
		for (int i=0; i<NEXT_FREE; i++) {
			prevPtr = freeList[i] + sizeof(size_t);
			if (nextPtr == prevPtr)
				return i;
		}
	}
	return -1;
}

void bfree(void *ptr)
{
	ptr = ptr - sizeof(size_t);
	/* save pointer to free block in freeList */
	freeList[NEXT_FREE] = ptr;
	if (NEXT_FREE == 0) {
		++NEXT_FREE;
		return;
	}
	/* store address to previous block and next block in the block itself
	 * giving the block form: Length of block + Pointer to previous block
	 * + Pointer to next block + Remaining block
	 */
	memcpy(ptr + sizeof(size_t), (void *) freeList[NEXT_FREE-1], sizeof(void *));
	/* we don't know what the next block will be yet */
//	memcpy(ptr + sizeof(size_t) + sizeof(void ()), (void *) NULL, sizeof(void *));
	/* update "next pointer" of previous block */
	memcpy(freeList[NEXT_FREE-1] + sizeof(size_t) + sizeof(void *), (void *) ptr, sizeof(void *));
	NEXT_FREE++;
}

int main()
{
	size_t memSize = 1024;
	void *addr = bmalloc(memSize);
	printf("addr is %p\n", addr);
	void *addr2 = bmalloc(memSize);
	printf("addr2 is %p\n", addr2);
	void *addr3 = bmalloc(memSize);
	printf("addr3 is %p\n", addr3);
	void *addr4 = bmalloc(memSize);
	printf("addr4 is %p\n", addr4);
	bfree(addr);
	bfree(addr2);
	bfree(addr3);
	bfree(addr4);
	void *newAddr = bmalloc(2048);
	printf("the new memory of size %zu is at %p\n", *(size_t *) (newAddr - sizeof(size_t)), newAddr);
}
