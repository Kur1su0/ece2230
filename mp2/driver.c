/* driver.c

   Writing and debugging MP2 is challenging, and it is particularly hard if you
   try and write all the code before you do any testing. It is much easier to
   break the problem into small pieces and test each piece. 

   For MP2, one approach is to write list_insert first, since it might be the
   easiest algorithm and it depends only on list_construct.

   For testing, we use a simple main function in this file.  This file is for 
   testing only, and you will NOT submit it as it is for your personal
   testing during development.

   Below is a simple example that allocates a few structures and inserts 
   them into a list.

   With this file you can compile and run list.c on its own using 
       gcc -Wall -g driver.c list.c sas_support.c -o driver

   The compling command is also found in the provided makefile.  Do
       make driver

   Do this testing before you make ANY changes to sas_support.c
   The linker needs to find the comp_proc function and it is found in the 
   template sas_support.c file (with no changes) 

*/

#include <stdlib.h>
#include <stdio.h>
#include "datatypes.h"
#include "list.h"
#include "sas_support.h"

# define INSERT(id, pos) \
    datap = (data_t *) calloc(1, sizeof(data_t));\
    datap->su_id = id;\
    list_insert(l_ptr, datap, pos);\
    datap = NULL;\
    
# define SORT(id)\
    datap = (data_t *) calloc(1, sizeof(data_t));\
    datap->su_id = id;\
    list_insert_sorted(l_ptr, datap);\
    datap = NULL;\

# define RE(id)\
    datap = (data_t *) calloc(1, sizeof(data_t));\
    datap->su_id = id;\
    if(list_elem_find(l_ptr,datap)!=NULL){\
    free(list_remove(l_ptr, list_elem_find(l_ptr, datap) ) );\
    printf("removed %d\n",id);}\
    else\
       printf("did not remove %d\n",id);\
// prototype for list debug print 
void list_debug_print(list_t *list_ptr);

int main(void)
{
    data_t *datap = NULL;
    list_t *l_ptr = NULL;
    l_ptr = list_construct(sas_compare);

    // create item to test list_insert
   
    //INSERT(555, list_iter_back(l_ptr));
    SORT(555);
    // test list_access with one itme in list
    datap = list_access(l_ptr, list_iter_front(l_ptr));

    if (datap != NULL) {
        printf("first test should find 555 and found (%d)\n\n", datap->su_id);
    } else {
        printf("ERROR: list access is broken?\n");
        exit(1);
    }
    datap = NULL;
    // add a second item to tail of the list

    //INSERT(444, NULL);
    SORT(444);
   
    

    
    
    // insert a new item in the middle 
   
    //INSERT(333, list_iter_back(l_ptr));
    SORT(333);
    //list_insert_sorted(l_ptr, datap);
    datap = NULL;

    // Test use of Iterator to find all three and print 
    IteratorPtr my_index;
    my_index = list_iter_front(l_ptr);
 
    datap = list_access(l_ptr, my_index); 
    printf("Second test\nPosition 0 should find 555 and found (%d)\n", datap->su_id);
    my_index = list_iter_next(my_index);
    datap = list_access(l_ptr, my_index);
    printf("Position 1 should find 333 found (%d)\n", datap->su_id);
    my_index = list_iter_next(my_index);
    datap = list_access(l_ptr, my_index);
    printf("Position 2 should find 444 and found (%d)\n", datap->su_id);

    // Next try to use list_debug_print 
    printf("\nTest of list debug print function\n\n");
    list_debug_print(l_ptr);
    RE(444);
    SORT(777);
    RE(888);
    SORT(123);
    RE(777);
    RE(555);
    list_debug_print(l_ptr);
    // you should repeat the above tests with differents insertion orders

    // next you may want to work on list_insert_sorted, so replace the above
    // list_insert commands with list_insert_sorted
 
    // after the insertion functions work, you can try the find command
 
    // Uncomment this section to test list_elem_find 

    // Uncomment this section when you are ready to test list_elem_find 
    /*********************************************************************

       data_t template;
       template.su_id = 777;
       my_index = list_elem_find(l_ptr, &template);
       data_t *foundp = list_access(l_ptr, my_index);
       printf("\nTest of list elem find\n");
       if (my_index != NULL) {
           if (foundp != NULL) {
               printf("looked for %d and found %d\n",
                       template.su_id, foundp->su_id);
               if (template.su_id != foundp->su_id) {
                   printf("ERROR: returned pointer to wrong record!\n");
               }
           } else {
               printf("ERROR: list_access returned NULL pointer !\n");
           }
       } else {
           printf("ERROR list_elem_find did not return index for %d\n", template.su_id);
       }
    */

    list_destruct(l_ptr);
    return 0;
}

/*

   Next you will want to write your own list_debug_print function to print a
   list. Then you can do "before and after" testing. That is, print the list
   before a change, and print the list after the change to verify that the
   change worked.

   Something like the following

*/
void list_debug_print(list_t *list_ptr) {
    list_node_t *L;
    int count;

    printf("List size: %d\n", list_ptr->current_list_size);
    L = list_ptr->head;
    count = 0;
    while(L != NULL) {
        printf("Pos: %d contains SU: %d\n", count, L->data_ptr->su_id);
        L = L->next;
        count++;
    }
    if (count != list_ptr->current_list_size)
        printf("Hey!! the list size is wrong! count is %d\n", count);
}

