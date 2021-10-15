#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>


/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

struct memoryList
{
  // doubly-linked list
  struct memoryList *last;
  struct memoryList *next;

  int size;            // How many bytes in this block?
  char alloc;          // 1 if this block is allocated,
                       // 0 if this block is free.
  void *ptr;           // location of block in memory pool.
};

strategies myStrategy = NotSet;    // Current strategy

size_t tailMem;
size_t mySize;
void *myMemory = NULL;

static struct memoryList *head;
static struct memoryList *next;
static struct memoryList *tmp;
static struct memoryList *tail;

int num = 0;

/* initmem must be called prior to mymalloc and myfree.

   initmem may be called more than once in a given exeuction;
   when this occurs, all memory you previously malloc'ed  *must* be freed,
   including any existing bookkeeping data.

   strategy must be one of the following:
		- "best" (best-fit)
		- "worst" (worst-fit)
		- "first" (first-fit)
		- "next" (next-fit)
   sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
*/

void initmem(strategies strategy, size_t sz)
{
	myStrategy = strategy;

	/* all implementations will need an actual block of memory to use */
	mySize = sz;
	tailMem = mySize;

	if (myMemory != NULL) free(myMemory); /* in case this is not the first time initmem2 is called */

	/* TODO: release any other memory you were using for bookkeeping when doing a re-initialization! */


	myMemory = malloc(sz);


	/* TODO: Initialize memory management structure. */
	head = (struct memoryList*) malloc(sizeof (struct memoryList));
	head->last = NULL; 
	head->next = NULL;
	head->size = sz;
	head->alloc = 0;
	head->ptr = myMemory;

}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1 
 */

/*mymalloc(100) and after mymalloc(50):
null <- 500(free) -> null // Initialy there is only one head node (a block) which size is equal to the whole memory pool

null <- 100(alloc) <-> 400(free) -> null // Efter mymalloc(100) 

null <- 100(alloc) <-> 50(alloc) <-> 350(free) -> null // Efter mymalloc(50) 


null <- 100(alloc) <-> 50(alloc) <-> 100(alloc) <-> 250(free) -> null // Efter mymalloc(100) 



*/
void *mymalloc(size_t requested)
{

	tailMem = tailMem - requested;

	assert((int)myStrategy > 0);
	
	switch (myStrategy)
	  {
	  case NotSet: 
	  			return NULL;
	  case First:
	  			if (head -> alloc == 0){
	  				tail = head;
	  				tail -> last = head;
	  				tail -> next = NULL;
	  				tail -> size = tailMem;
	  				tail -> alloc = 0;
	  				tail -> ptr = ++myMemory;

	  				head = (struct memoryList*) malloc(sizeof (struct memoryList));
	  				head -> last = NULL;
	  				head -> next = tail;
	  				head -> size = requested;
	  				head -> alloc = 1;
	  				head -> ptr = myMemory;
	  				printf("Bye1\n");

	  				num = 1;

	  				printf("head size: %d\n", head -> size);
	  				printf("tail size: %d\n", tail -> size);
	  			}
	  			else if (num == 1){
	  				next = (struct memoryList*) malloc(sizeof (struct memoryList));
	  				next -> last = head;
	  				next -> next= tail;
	  				next -> size = requested;
	  				next -> alloc = 1;

	  				head -> next = next;

	  				tail -> last = next;
	  				tail -> size = tailMem;
	  				tail -> ptr = ++myMemory; 
	  				next -> ptr = (tail -> ptr) - 1;

	  				num = 2;
	  				printf("next size: %d\n", next -> size);
	  				printf("tail size: %d\n", tail -> size);
	  			}
	  			else if (num == 2){
	  				tmp = next; // save next

	  				next = (struct memoryList*) malloc(sizeof (struct memoryList));
	  				next -> last = tmp;
	  				next -> next = tail;
	  				next -> size = requested;
	  				next -> alloc= 1;

	  				tmp -> next = next;

	  				tail -> last = next;
	  				tail -> size = tailMem;
	  				tail -> ptr = ++myMemory; 

	  				next -> ptr = (tail -> ptr) - 1;


	  				printf("next size: %d\n", next -> size);
	  				printf("tail size: %d\n", tail -> size);
	  			}
	  			
	  case Best:
	            return NULL;
	  case Worst:
	            return NULL;
	  case Next:
	            return NULL;
	  }
	return NULL;
}


/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void *block)
{
	(struct memoryList) *block -> alloc = 0;
}

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when refered to "memory" here, it is meant that the 
 * memory pool this module manages via initmem/mymalloc/myfree. 
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes()
{
	struct memoryList *tmp = head;
	int counter = 0;

	while(tmp != NULL){
		if (tmp -> alloc == 0){
			counter++;
		}
		tmp = tmp -> next;
	}

	return counter;
}

/* Get the number of bytes allocated */
int mem_allocated()
{
	return mySize - tailMem;
}

/* Number of non-allocated bytes */
int mem_free()
{
	struct memoryList *tmp = head;
	int freeMem = 0;

	while(tmp != NULL){
		if (tmp -> alloc == 0){
			freeMem = freeMem + tmp -> size;
		}
		tmp = tmp -> next;
	}

	return freeMem;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
	return 0;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
	return 0;
}       

char mem_is_alloc(void *ptr)
{
        return 0;
}

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may find them useful.
 */


//Returns a pointer to the memory pool.
void *mem_pool()
{
	return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total()
{
	return mySize;
}


// Get string name for a strategy. 
char *strategy_name(strategies strategy)
{
	switch (strategy)
	{
		case Best:
			return "best";
		case Worst:
			return "worst";
		case First:
			return "first";
		case Next:
			return "next";
		default:
			return "unknown";
	}
}

// Get strategy from name.
strategies strategyFromString(char * strategy)
{
	if (!strcmp(strategy,"best"))
	{
		return Best;
	}
	else if (!strcmp(strategy,"worst"))
	{
		return Worst;
	}
	else if (!strcmp(strategy,"first"))
	{
		return First;
	}
	else if (!strcmp(strategy,"next"))
	{
		return Next;
	}
	else
	{
		return 0;
	}
}


/* 
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory()
{
	struct memoryList *tmp = head;

	while(tmp != NULL){
		printf("%d\n", tmp->size);

		tmp = tmp -> next;
	}
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */ 
void print_memory_status()
{
	printf("%d out of %d bytes allocated.\n",mem_allocated(),mem_total());
	printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",mem_free(),mem_holes(),mem_largest_free());
	printf("Average hole size is %f.\n\n",((float)mem_free())/mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv) {
        strategies strat;
	void *a, *b, *c, *d, *e;
	if(argc > 1)
	  strat = strategyFromString(argv[1]);
	else
	  strat = First;
	
	
	/* A simple example.  
	   Each algorithm should produce a different layout. */
	
	initmem(strat,500);
	
	a = mymalloc(100);
	b = mymalloc(100);
	c = mymalloc(100);
	myfree(b);
	d = mymalloc(50);
	myfree(a);
	e = mymalloc(25);
	
	print_memory();
	print_memory_status();
	
}


// 100(free) -> 100(free) -> 100 -> 50 -> 25 -> 125(free)