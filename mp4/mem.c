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
    chunk_t* freed_ptr= return_ptr;
    freed_ptr--;
    chunk_t* temp=Rover;
    // obviously the next line is WRONG!!!!  You must fix it.

    if (Coalescing){
          do{
              if((Rover<freed_ptr && Rover->next>freed_ptr) || (Rover<freed_ptr && Rover->next==&Dummy) ){
                 freed_ptr->next = Rover->next;
                 Rover->next = freed_ptr;
                  break;
              }
              Rover = Rover->next;
          }while(Rover!=temp);
          
           
           int isNext   =0;
           int isBefore = 0;
           if(Rover+Rover->size==freed_ptr)           isBefore=1;  //before free_ptr.
           if(freed_ptr+freed_ptr->size==freed_ptr->next) isNext=1; //after free_ptr.
          
               if(isBefore && Rover!=&Dummy){
                      Rover->size += freed_ptr->size;
                      Rover->next = freed_ptr->next;
                      freed_ptr->next = NULL;
                      if(isNext && Rover->next!=&Dummy){
                          Rover->size +=Rover->next->size;
                          Rover->next = Rover->next->next;
                     }
               } 
               else if(isNext&& freed_ptr->next!=&Dummy){
                   freed_ptr->size +=freed_ptr->next->size;
                   temp = freed_ptr->next;
                   freed_ptr->next = freed_ptr->next->next;
                   temp->next = NULL;
                   Rover = Rover->next;

               }
           
               
        //   Rover=Rover->next;
  
      // }while(temp!=Rover);
    }
    
    else{
        freed_ptr->next = Rover->next;
        Rover->next = freed_ptr;
         }
    //}
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
    size                     = nbytes%sizeof(chunk_t)==0?size:size+1;
    int fit_flag             = 0;

    int diff                 = 0; //difference of abs(Size:freeblock - Size: usr need)
    int diff_ori             = 0; //difference of abs(Size:freeblock - Size: usr need)
    int num_pages            = 0;
    int new_bytes            = 0;
    int new_free_block_size  = 0;
    chunk_t* temp            = Rover;
    chunk_t* fit_ptr         = NULL;
    chunk_t* p               = NULL;
    chunk_t* q               = NULL;

    if (SearchPolicy == FIRST_FIT ){
        do{    
            if(Rover->size >= size){
                fit_flag = 1;
                fit_ptr = Rover;
                break;
            }
            Rover = Rover->next;
        } while(Rover != temp);
    }
    else{
         //BEST_FIT
         //General idea.
         //loop every entry in the free list to find the possible smallest (best fit) block.
         //(free_block size - needed size) ----> should be small.
         diff_ori = Rover->size - size;
         do{ 
             diff = Rover->size - size;
             if(diff >=0 && fit_flag == 0 ){
                 fit_flag = 1;
                 diff_ori = diff;  //smaller block is found.
                 fit_ptr = Rover;
             }
             if(diff >=0 && diff < diff_ori && fit_flag == 1){
                 fit_ptr = Rover;
                 if(diff == 0) break;
             }


             Rover = Rover->next;
         }while (temp != Rover);

        if(fit_flag == 1) Rover = fit_ptr;
    }

    if(fit_flag==0 ){  //out of storage.
            //printf("\nout of storage\n");
            num_pages = size/(PAGESIZE/sizeof(chunk_t))==0?0:size/(PAGESIZE/sizeof(chunk_t));
            num_pages = size%PAGESIZE==0?num_pages:num_pages+1;
            new_bytes = num_pages*PAGESIZE;
            new_free_block_size = new_bytes/sizeof(chunk_t);

            temp = morecore(new_bytes);
            temp->size = new_free_block_size;

            p           = temp;  //p is a block to be returned to the usr

            temp       += size;  //jump to next free block
            temp->size  = new_free_block_size-size;
            p->size     = size;
            
           //temp->next = Rover->next;
           // Rover->next = temp;
           // Rover = Rover->next;
           //Mem_free(p+1);
           chunk_t* ori_rover = Rover;
          do{
              if((Rover<p && Rover->next>temp) || (Rover<temp && Rover->next==&Dummy) ){
                 temp->next = Rover->next;
                 Rover->next = temp;
                  break;
              }
              Rover = Rover->next;
          }while(Rover!=ori_rover);
          
          p->next     = NULL;
    }
    else{
        //not out of storage.
        //printf("\navail\n");
            //greater
        p = Rover;
        
        
        if ((Rover->size-size)>0){
            p             += (Rover->size-size);
            p->size        = size;
            Rover->size    = Rover->size - size;
            p->next        = NULL;

        }
        else{
            while(temp->next != Rover){
                temp = temp->next;
            }//get the prev node pointer.
             temp->next = p->next;
             Rover = p->next;
             p->next     = NULL;
        }



    }
    //Not find in Best and first fit.
    /* Morecore */

    // here are possible post-conditions, depending on your design
    q = p+1;  //XXX Merge fit outof storage.
    //
    // assume p is a pointer to memory block that will be given to the user
    // and q is the address given to the user
    // assert(p + 1 == q);
    // the minus one in the next two tests accounts for the header
    assert((p->size - 1)*sizeof(chunk_t) >= nbytes);
    assert((p->size - 1)*sizeof(chunk_t) < nbytes + sizeof(chunk_t));
    assert(p->next == NULL);  // saftey first!
    return q;


    // obviously the next line is WRONG!!!!  You must fix it.
    //return malloc(nbytes);
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
    chunk_t* temp = Rover;
    int count=0;
    int tBlock=0;
    int min=0, max=Rover->size, ave=0;
    int flag = 0;
    do{
        
        count+=temp->size;
        tBlock++;
        
        if(temp->size!=0 && flag !=1 ) {
            min = temp->size;
            flag = 1;
        }
        if(temp->size!=0 && temp->size <=  min ) min = temp->size;
        if(temp->size!=0 && temp->size > max )  max = temp->size;
      
      //  printf("count: %d\n",count);
        temp = temp->next;
    }while(temp != Rover);
    
    //printf("size: %d\n",NumPages*PAGESIZE);
    int diff = count*sizeof(chunk_t) - NumPages * PAGESIZE;
    diff = diff<0?-diff:diff;
    if(diff==0)
        printf("all memory is in the heap -- no leaks are possible\n");
   
    ave = tBlock==1?count/(tBlock):count/(tBlock-1);
    
    printf("#items in linked-list:%d, min:%d, max:%d, avrage:%d, #Pages:%d, #bytes: %d, #SbrkCalls:%d\n\n",
            tBlock,min*(int)sizeof(chunk_t),max*(int)sizeof(chunk_t),ave*(int)sizeof(chunk_t), NumPages, count*(int)sizeof(chunk_t), NumSbrkCalls);
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
        if(p->size==0 && p!=Rover){
            printf("p=%p, size=%d, end=%p, next=%p %s\n",
                    p, p->size, p + p->size, p->next, p->size!=0?"":"<-- dummy");
        }
        else if(p->size==0 && p==Rover){
            printf("p=%p, size=%d, end=%p, next=%p %s\n",
                    p, p->size, p + p->size, p->next, "<-- Rover/dummy");
        }
        else if(p->size!=0 && p==Rover){
            printf("p=%p, size=%d, end=%p, next=%p %s\n",
                    p, p->size, p + p->size, p->next, p!=Rover?"":"<-- Rover");
        }
        else{
            printf("p=%p, size=%d, end=%p, next=%p %s\n",
                    p, p->size, p + p->size, p->next, "");

        }
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
