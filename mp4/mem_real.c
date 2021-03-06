/* mem.c A template
 * My Name is Zice Wei
 * My User ID is zicew
 * Lab4: Dynamic Memory Allocation
 * ECE 2230, Fall 2018
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "mem.h"

// Global variables required in mem.c only
static chunk_t Dummy = {&Dummy, 0};
static chunk_t * Rover = &Dummy;
int NumSbrkCalls = 0;  /*For counting # of sbrk calls*/
int NumPages = 0;      /*For counting # pages*/

// private function prototypes
void mem_validate(void);

/* function to request 1 or more pages from the operating system.
 *
 * new_bytes must be the number of bytes that are being requested from
 *           the OS with the sbrk command.  It must be an integer 
 *           multiple of the PAGESIZE
 *
 * returns a pointer to the new memory location.  If the request for
 * new memory fails this function simply returns NULL, and assumes some
 * calling function will handle the error condition.  Since the error
 * condition is catastrophic, nothing can be done but to terminate 
 * the program.
 */
chunk_t *morecore(int new_bytes) 
{
    char *cp;
    chunk_t *new_p;
    // preconditions
    assert(new_bytes % PAGESIZE == 0 && new_bytes > 0);
    assert(PAGESIZE % sizeof(chunk_t) == 0);
    cp = sbrk(new_bytes);
    if (cp == (char *) -1)  /* no space available */
        return NULL;
    new_p = (chunk_t *) cp;
    // You should add some code to count the number of calls
    // to sbrk, and the number of pages that have been requested
    // Ex: NumSbrkCalls++; NumPages += new_bytes/PAGESIZE;
    NumSbrkCalls++;
    NumPages += new_bytes/PAGESIZE;

    return new_p;
}

/* deallocates the space pointed to by return_ptr; it does nothing if
 * return_ptr is NULL.  
 *
 * This function assumes that the Rover pointer has already been 
 * initialized and points to some memory block in the free list.
 */
void Mem_free(void *return_ptr)
{
    // precondition
    assert(Rover != NULL && Rover->next != NULL);

    // obviously the next line is WRONG!!!!  You must fix it.
    if(return_ptr == NULL){
        return;
    }else{
        if (Coalescing){
            ;
        }else{
            chunk_t* freed_ptr = return_ptr;
            freed_ptr = Rover->next;
            Rover = freed_ptr;
        }
    }
}

/* returns a pointer to space for an object of size nbytes, or NULL if the
 * request cannot be satisfied.  The memory is uninitialized.
 *
 * This function assumes that there is a Rover pointer that points to
 * some item in the free list.  The first time the function is called,
 * Rover is null, and must be initialized with a dummy block whose size
 * is one, but set the size field to zero so this block can never be 
 * removed from the list.  After the first call, the Rover can never be null
 * again.
 */
void *Mem_alloc(const int nbytes)
{
    // precondition
    assert(nbytes > 0);
    assert(Rover != NULL && Rover->next != NULL);


    // Insert your code here to find memory block
    int size                 = nbytes/sizeof(chunk_t)+1;
    int fit_flag             = 0;
    int greater_flag         = 0;
    int exact_flag           = 0;
    int diff                 = 0; //difference of abs(Size:freeblock - Size: usr need)
    int diff_ori             = 0; //difference of abs(Size:freeblock - Size: usr need)
    int real_pagesize        = PAGESIZE/sizeof(chunk_t);
    chunk_t* temp            = Rover;
    chunk_t* best_fit_ptr    = Rover;
    chunk_t* p               = Rover;
    chunk_t* q               = Rover;

    if (SearchPolicy == FIRST_FIT ){
        while(Rover != temp){
            if(Rover->size > size){
                greater_flag = 1;
                break;
            }
            if(Rover->size == size){
                exact_flag = 1;
                break;
            }
            Rover = Rover->next;
        }
    
        if(Rover == temp ){  //out of storage.
            if(size% real_pagesize == 0 && size > real_pagesize ){
                temp = morecore(size);  //multiple of Pagesizes
            }else{
                //#pages+1
                temp = morecore( (size/ real_pagesize + 1)* real_pagesize );                     //temp+
            }
            if(temp == NULL) return NULL;
            
            p           = temp;  //p is a block to be returned to the usr
            p->size     = size;
            
            temp       += size;  //jump to next free block
            temp->next  = rover;
            rover->next = temp;
            p->next     = NULL;
            
        }else{  
            //not out of storage.
            //greater
            p = Rover;
            if (greater_flag){
                p          += Rover->size-size; 
                Rover->size = Rover->size-size; // a tricky way to break a block.
                p->size     = size;
                p->next     = NULL;
            }
            if (exact_flag){
                while(temp->next != rover){
                    temp = temp->next;
                }//get the prev node pointer.
                p           = rover;
                p->size     = size;
                p->next     = NULL;
                Rover       = Rover->next;
                temp->next  = Rover;

            }

       }
        q = p+1;  //XXX Merge fit outof storage. 
        return q;   
    
    }else{ 
       //BEST_FIT
       //General idea.
       //loop every entry in the free list to find the possible smallest (best fit) block.
       //(free_block size - needed size) ----> should be small.
       diff_ori = Rover->size - size;
       while (temp != Rover ){
           diff = Rover->size - size;
           if(diff >=0 ){
               fit_flag = 1;
               diff_ori = diff;  //smaller block is found.
               best_fit_ptr = Rover;
           }
           Rover = Rover->next;
       }



    }
    //Not find in Best and first fit.
    /* Morecore */

    // here are possible post-conditions, depending on your design
    //
    // assume p is a pointer to memory block that will be given to the user
    // and q is the address given to the user
    // assert(p + 1 == q);
    // the minus one in the next two tests accounts for the header
    // assert((p->size - 1)*sizeof(chunk_t) >= nbytes);
    // assert((p->size - 1)*sizeof(chunk_t) < nbytes + sizeof(chunk_t));
    // assert(p->next == NULL);  // saftey first!
    // return q;


    // obviously the next line is WRONG!!!!  You must fix it.
    return malloc(nbytes);
}

/* prints stats about the current free list
 *
 * -- number of items in the linked list including dummy item
 * -- min, max, and average size of each item (in bytes)
 * -- total memory in list (in bytes)
 * -- number of calls to sbrk and number of pages requested
 *
 * A message is printed if all the memory is in the free list
 */
void Mem_stats(void)
{
    printf("the student must implement mem stats\n");
    // One of the stats you must collect is the total number
    // of pages that have been requested using sbrk.
    // Say, you call this NumPages.  You also must count M,
    // the total number of bytes found in the free list 
    // (including all bytes used for headers).  If it is the case
    // that M == NumPages * PAGESiZE then print
    printf("all memory is in the heap -- no leaks are possible\n");
}

/* print table of memory in free list 
 *
 * The print should include the dummy item in the list 
 */
void Mem_print(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    chunk_t *p = Rover;
    chunk_t *start = p;
    do {
        // example format.  Modify for your design
        printf("p=%p, size=%d, end=%p, next=%p %s\n", 
                p, p->size, p + p->size, p->next, p->size!=0?"":"<-- dummy");
        p = p->next;
    } while (p != start);
    mem_validate();
}

/* This is an experimental function to attempt to validate the free
 * list when coalescing is used.  It is not clear that these tests
 * will be appropriate for all designs.  If your design utilizes a different
 * approach, that is fine.  You do not need to use this function and you
 * are not required to write your own validate function.
 */
void mem_validate(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    assert(Rover->size >= 0);
    int wrapped = FALSE;
    int found_dummy = FALSE;
    int found_rover = FALSE;
    chunk_t *p, *largest, *smallest;

    // for validate begin at Dummy
    p = &Dummy;
    do {
        if (p->size == 0) {
            assert(found_dummy == FALSE);
            found_dummy = TRUE;
        } else {
            assert(p->size > 0);
        }
        if (p == Rover) {
            assert(found_rover == FALSE);
            found_rover = TRUE;
        }
        p = p->next;
    } while (p != &Dummy);
    assert(found_dummy == TRUE);
    assert(found_rover == TRUE);

    if (Coalescing) {
        do {
            if (p >= p->next) {
                // this is not good unless at the one wrap
                if (wrapped == TRUE) {
                    printf("validate: List is out of order, already found wrap\n");
                    printf("first largest %p, smallest %p\n", largest, smallest);
                    printf("second largest %p, smallest %p\n", p, p->next);
                    assert(0);   // stop and use gdb
                } else {
                    wrapped = TRUE;
                    largest = p;
                    smallest = p->next;
                }
            } else {
                assert(p + p->size < p->next);
            }
            p = p->next;
        } while (p != &Dummy);
        assert(wrapped == TRUE);
    }
}
/* vi:set ts=8 sts=4 sw=4 et: */

