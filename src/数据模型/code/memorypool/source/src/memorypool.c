static char rcsid[] = "$Id$";
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "except.h"
#include "memorypool.h"
#define T MemoryPool_T
const Except_T MemoryPool_NewFailed =
	{ "MemoryPool creation failed" };
const Except_T MemoryPool_Failed    =
	{ "MemoryPool allocation failed" };
#define THRESHOLD 10
struct T {
	T prev;
	char *avail;
	char *limit;
};
union align {
#ifdef MAXALIGN
	char pad[MAXALIGN];
#else
	int i;
	long l;
	long *lp;
	void *p;
	void (*fp)(void);
	float f;
	double d;
	long double ld;
#endif
};
union header {
	struct T b;
	union align a;
};
static T freechunks;
static int nfree;
T MemoryPool_new(void) {
	T memory_pool = malloc(sizeof (*memory_pool));
	if (memory_pool == NULL)
		RAISE(MemoryPool_NewFailed);
	memory_pool->prev = NULL;
	memory_pool->limit = memory_pool->avail = NULL;
	return memory_pool;
}
void MemoryPool_dispose(T *ap) {
	assert(ap && *ap);
	MemoryPool_free(*ap);
	free(*ap);
	*ap = NULL;
}
void *MemoryPool_alloc(T memory_pool, long nbytes,
	const char *file, int line) {
	assert(memory_pool);
	assert(nbytes > 0);
	nbytes = ((nbytes + sizeof (union align) - 1)/
		(sizeof (union align)))*(sizeof (union align));
	while (nbytes > memory_pool->limit - memory_pool->avail) {
		T ptr;
		char *limit;
		if ((ptr = freechunks) != NULL) {
			freechunks = freechunks->prev;
			nfree--;
			limit = ptr->limit;
		} else {
			long m = sizeof (union header) + nbytes + 10*1024;
			ptr = malloc(m);
			if (ptr == NULL)
				{
					if (file == NULL)
						RAISE(MemoryPool_Failed);
					else
						Except_raise(&MemoryPool_Failed, file, line);
				}
			limit = (char *)ptr + m;
		}
		*ptr = *memory_pool;
		memory_pool->avail = (char *)((union header *)ptr + 1);
		memory_pool->limit = limit;
		memory_pool->prev  = ptr;
	}
	memory_pool->avail += nbytes;
	return memory_pool->avail - nbytes;
}
void *MemoryPool_calloc(T memory_pool, long count, long nbytes,
	const char *file, int line) {
	void *ptr;
	assert(count > 0);
	ptr = MemoryPool_alloc(memory_pool, count*nbytes, file, line);
	memset(ptr, '\0', count*nbytes);
	return ptr;
}
void MemoryPool_free(T memory_pool) {
	assert(memory_pool);
	while (memory_pool->prev) {
		struct T tmp = *memory_pool->prev;
		if (nfree < THRESHOLD) {
			memory_pool->prev->prev = freechunks;
			freechunks = memory_pool->prev;
			nfree++;
			freechunks->limit = memory_pool->limit;
		} else
			free(memory_pool->prev);
		*memory_pool = tmp;
	}
	assert(memory_pool->limit == NULL);
	assert(memory_pool->avail == NULL);
}