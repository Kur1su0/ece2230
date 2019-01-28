/* list.c                  <<-- A template to be updated and 
 * Zice Wei           <<-- changed
 * zicew
 * ECE 2230 Fall 2018
 * MP3
 *
 * Propose: Basic manipulation for lab3.
 *
 * Assumptions: This C file contains basic manipution for linked list.
 *              For instance, list_struct, list_destruct, finding list
 *              elements and other operation for accessing the specific
 *              positio in a linked list(list_iter_font, list_iter_back
 *              list_iter_next) 
 *
 * Bugs: Many detail have not been implemented.
 */

#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "datatypes.h"   // defines data_t 
#include "list.h"        // defines public functions for list ADT 

// definitions for private constants used in list.c only 
#define SORTED_LIST   0xCACACAC
#define UNSORTED_LIST 0x3535353

// prototypes for private functions used in list.c only 
void list_debug_validate(list_t *L);

void list_sort(list_t*, int );

list_t* insertion_sort(list_t *);
list_node_t* find_min(list_t* , list_node_t*);
void sel_helper(list_t*, list_node_t*);
void re_selection_sort(list_t*);
void itr_selection_sort(list_t*);

void split(list_t*, list_t*, list_t*);
void concat(list_t*, list_t*, list_t*);
void merge_sort(list_t*);
/* ----- below are the functions  ----- */

/* Allocates a new, empty list 
 *
 * By convention, the list is initially assumed to be sorted.  The field sorted
 * can only take values SORTED_LIST or UNSORTED_LIST
 *
 * The inital empty list must have
 *  1. current_list_size = 0
 *  2. list_sorted_state = SORTED_LIST
 *
 * Use list_destruct to remove and deallocate all elements on a list
 * and the header block.
 * Input: *fcomp pointer.
 * Returns: pointer to the head of the list.
 */
list_t *list_construct(int (*fcomp)(const data_t *, const data_t *))
{
    list_t *L;

    L = (list_t *) malloc(sizeof(list_t));
    L->head = NULL;
    L->tail = NULL;
    L->current_list_size = 0;
    L->list_sorted_state = SORTED_LIST;
    L->comp_proc = fcomp;                      //initializig fcomp.
    // the last line of this function must call validate
    //list_debug_validate(L);
    return L;
}

/* Purpose: return the count of number of elements in the list.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Return: the number of elements stored in the list.  
 */
int list_size(list_t *list_ptr)
{
    assert(NULL != list_ptr);
    assert(list_ptr->current_list_size >= 0); 
    return list_ptr->current_list_size;  //return current list size
}

/* Deallocates the contents of the specified list, releasing associated memory
 * resources for other purposes.
 *
 * Free all elements in the list and the header block.
 */
void list_destruct(list_t *list_ptr)
{
    // the first line must validate the list 
    //list_debug_validate(list_ptr);
    // add your code after this line
    if(list_ptr->current_list_size == 0){
        free(list_ptr);
    }else{
        int count=0;
        while(count < list_ptr->current_list_size){
            list_node_t* rover = list_ptr->tail;
            free(list_ptr->tail->data_ptr);                   //free prev tail's data.
            list_ptr->tail = list_ptr->tail->prev; //move tail forward.
            free(rover);                           //free prev tail.
            rover = NULL; 

            count++;
        }
        list_ptr->head = NULL;
        list_ptr->tail = NULL;
        free(list_ptr);             //free outter alloced memory.
        list_ptr = NULL;            
        
    }
}

/* 
 * Return an Iterator that points to the last list_node_t. If the list is empty
 * then the pointer that is returned is NULL.
 *
 * input: pointer of list_ptr.
 * return: pointer to the tail of the list_ptr.
 */
list_node_t * list_iter_back(list_t *list_ptr)
{
    assert(NULL != list_ptr);
    //list_debug_validate(list_ptr);
    return list_ptr->tail;                     //return tail node.
}
/* Return an Iterator that points to the first element in the list.  If the
 * list is empty the value that is returned in NULL.
 */
list_node_t * list_iter_front(list_t *list_ptr)
{
    assert(NULL != list_ptr);
    //list_debug_validate(list_ptr);
    return list_ptr->head;         //return head node.
}

/* Advance the Iterator to the next item in the list.  
 * If the iterator points to the last item in the list, then 
 * this function returns NULL to indicate there is no next item.
 * It is a catastrophic error to call this function if the
 * iterator, idx_ptr, is null.
 */
list_node_t * list_iter_next(list_node_t * idx_ptr)
{
    assert(idx_ptr != NULL);
    return idx_ptr->next==NULL?NULL:idx_ptr->next;  //return next node.
}

/* Finds an element in a list and returns a pointer to it.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: element against which other elements in the list are compared.
 *           Note: uses the comp_proc function pointer found in the list_t
 *           header block. 
 *
 * The function returns an Iterator pointer to the list_node_t that contains 
 * the first matching element if a match if found.  If a match is not found 
 * the return value is NULL.
 *
 * Note: to get a pointer to the matching data_t memory block pass the return
 *       value from this function to the list_access function.
 */
list_node_t * list_elem_find(list_t *list_ptr, data_t *elem_ptr)
{
    //list_debug_validate(list_ptr);
    int i = 0, flag=-1;
    list_node_t* rover = list_ptr->head;
    for(i=0; i<list_ptr->current_list_size;i++){
        //if(elem_ptr==NULL) return NULL;
        if(list_ptr->comp_proc(rover->data_ptr, elem_ptr)==0){
            flag = 1;                   //find the desired node,
            break;
        }else{
            rover = rover->next;        //head to the next node.
        }
        
    }
    return flag==1?rover:NULL;
}

/* purpose: insertion sort
 * input pointer of list to be sorted
 * return val: pointer of sorted list.
 */

list_t* insertion_sort(list_t* list_ptr){
    list_t* new_list_ptr = NULL;
    new_list_ptr = list_construct((list_ptr)->comp_proc);
    int size = list_size(list_ptr);
    data_t* data_ptr = NULL;
    int i = 0;
    for (i = 0; i< size; i++){
       data_ptr = list_remove(list_ptr, list_iter_front(list_ptr));
       list_insert_sorted(new_list_ptr,data_ptr);
       data_ptr = NULL;
    }
    list_ptr->head = new_list_ptr->head;
    list_ptr->tail = new_list_ptr->tail;
    list_ptr->current_list_size = new_list_ptr->current_list_size;
    free(new_list_ptr);
    new_list_ptr = NULL;
    return list_ptr;
}
/* Find the min val of a list
 * input: list of interest
 * return: node of min val.
 */
 list_node_t* find_min(list_t* list_ptr, list_node_t* rover){
     list_node_t* min = rover;
     while(rover!=NULL){
         if(list_ptr->comp_proc(rover->data_ptr, min->data_ptr)== 1){ //rover id <  min id
             min = rover;
         }
         rover = rover->next;
     }
     return min;

 }
/*Selection sort helper function recursively.
 *input head, tail and list to be sorted.
 *no return val
 */
void sel_helper(list_t* list_ptr, list_node_t* rover){
    data_t* temp;
    list_node_t* min_node;
    if (rover != NULL){
        min_node = find_min(list_ptr, rover);

        //exchange min_node with rover.
        temp = min_node->data_ptr;
        min_node->data_ptr = rover->data_ptr;
        rover->data_ptr = temp;

        //recursive
        sel_helper(list_ptr, rover->next);
    }



}

/* recursive Selection Sort
 * input     unsorted list
 * no return val.
 *
 */
 void re_selection_sort(list_t* list_ptr){
    list_node_t* head = list_ptr->head;
    sel_helper(list_ptr, head);

 }


/* Iter selection sort
 * input: list of interest
 * no return val
 *
 */
void itr_selection_sort(list_t* list_ptr){
    list_node_t* min    = NULL;    
    list_node_t* rover  = list_ptr->head;    
    list_node_t* rover_next  = list_ptr->head;    
    data_t* temp   = NULL;    
    
    while(rover->next!=NULL){
        rover_next = rover;
        min = rover;
        while(rover_next->next!=NULL){
           rover_next = rover_next->next;
           if(list_ptr->comp_proc(rover_next->data_ptr, min->data_ptr)== 1){ 
           //rover_next id <  min id
               min = rover_next;
           }
        }
    
        temp = rover->data_ptr;
        rover->data_ptr = min->data_ptr;
        min->data_ptr = temp;
        rover = rover->next;
    }

}

/* Purpose: split list into two pieces.  
 * input: origin list, left list and right list.
 * no return val.
 */
void split(list_t* list_ptr, list_t *L, list_t *R){
    int L_size = (list_ptr->current_list_size) / 2;
    int R_size = list_ptr->current_list_size - L_size;
    int i = 0;
    //move tail to the correct pos.
    
    for(i=0;i<L_size;i++){
        list_insert(L, list_remove(list_ptr,list_ptr->head), NULL);  
    
    }
    while(list_ptr->head!=NULL){
        list_insert(R, list_remove(list_ptr, list_ptr->head), NULL );    
    }
    L->current_list_size = L_size;
    R->current_list_size = R_size;
}
/* concat for merge sort.
 * input splited list1, splited list2
 */
void concat(list_t* list_ptr, list_t* L, list_t* R){
    list_node_t* rover_r = R->head;
    list_node_t* rover_l = L->head;
    list_node_t* temp = NULL;
    int size = L->current_list_size + R->current_list_size;
    int i = 0;
    while(i < size){
        if(rover_l == NULL && rover_r != NULL){   //1-element on R
            temp = rover_r;
            rover_r = rover_r->next;
            list_insert(list_ptr, list_remove(R, temp), NULL);
        }else if(rover_l !=NULL && rover_r == NULL){ //1-element on L
            temp = rover_l;
            rover_l = rover_l->next;
            list_insert(list_ptr, list_remove(L, temp), NULL);
    
        }else if (rover_l == NULL && rover_r == NULL){
            break;   //current merge operation done.
        }else{
            
            if(list_ptr->comp_proc(rover_l->data_ptr, rover_r->data_ptr)== 1){ 
                temp = rover_l;
                rover_l = rover_l->next;
            list_insert(list_ptr, list_remove(L, temp), NULL);

            }else{
                temp = rover_r;
                rover_r = rover_r->next;
            list_insert(list_ptr, list_remove(R, temp), NULL);
            }
        }

        i++;
    }
    free(L);
    free(R);
}

/*Merge sort
 *input list of interest
 *
 *no return value
 *
 */
 void merge_sort(list_t* list_ptr){
 //1. break into two pieces until none of the list has more than 1 item.
     if(list_ptr->head->next == NULL) return;
         list_t* L = list_construct((list_ptr)->comp_proc);
         list_t* R = list_construct((list_ptr)->comp_proc);
       
         split(list_ptr, L, R);

         //split left
          merge_sort(L);

         //split right
         merge_sort(R);
 //2. merge and sort.
         concat(list_ptr, L, R);
 //3. done.
 
 }


/* list sort:
 * 
 * input: pointer of Queue, int of sort_type
 * no return val.
 *
 */
 void list_sort(list_t* list_ptr, int sort_type){
 
     if(list_ptr==NULL) return;
     if(sort_type == 1){       // insertion sort case.
         list_ptr = insertion_sort(list_ptr);    
     }else if(sort_type == 2){
         re_selection_sort(list_ptr);   
     }else if(sort_type == 3){
         itr_selection_sort(list_ptr);   

     }else{
         merge_sort(list_ptr);

     }
    list_ptr->list_sorted_state  = SORTED_LIST;
    list_debug_validate(list_ptr);

 }
/* Inserts the element into the specified sorted list at the proper position,
 * as defined by the comparison function, comp_proc.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * If you use list_insert_sorted, the list preserves its sorted nature.
 *
 * If you use list_insert, the list will be considered to be unsorted, even
 * if the element has been inserted in the correct position.
 *
 * If the list is not sorted and you call list_insert_sorted, this subroutine
 * must generate a system error and the program should immediately stop.
 *
 * The comparison procedure must accept two arguments (A and B) which are both
 * pointers to elements of type data_t.  The comparison procedure returns an
 * integer code which indicates the precedence relationship between the two
 * elements.  The integer code takes on the following values:
 *    1: A should be closer to the front of the list than B
 *   -1: B should be closer to the front of the list than A
 *    0: A and B are equal in rank
 *
 * Note: if the element to be inserted is equal in rank to an element already
 * in the list, the newly inserted element will be placed after all the
 * elements of equal rank that are already in the list.
 */
void list_insert_sorted(list_t *list_ptr, data_t *elem_ptr)
{
    assert(NULL != list_ptr);
    assert(SORTED_LIST == list_ptr->list_sorted_state);
    
    // insert your code here
    if (list_ptr->current_list_size==0){
        list_insert(list_ptr, elem_ptr,list_iter_front(list_ptr));  //0 size.
    }
    else{
        list_node_t* rover = list_ptr->head;

        while(rover!=NULL){                               //trace down.
            if(list_ptr->comp_proc(rover->data_ptr,elem_ptr)==-1) break;
            rover = rover->next;
        }
        if (rover != NULL){                              //found.
            list_insert(list_ptr, elem_ptr, rover);
        }else{                                           //not fount
                                                         //add to tail.
            list_insert(list_ptr, elem_ptr, NULL);
        }

    }
    list_ptr->list_sorted_state = SORTED_LIST;
    // the last line of this function must be the following 
    //list_debug_validate(list_ptr);
}

/* Inserts the data element into the list in front of the iterator 
 * position.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the memory block to be inserted into list.
 *
 * idx_ptr: pointer to a list_node_t.  The element is to be inserted as a 
 *          member in the list at the position that is immediately in front 
 *          of the position of the provided Iterator pointer.
 *
 *          If the idx_ptr pointer is NULL, the the new memory block is
 *          inserted after the last element in the list.  That is, a null
 *          iterator pointer means make the element the new tail.
 *
 * If idx_ptr is set using 
 *    -- list_iter_front, then the new element becomes the first item in 
 *       the list.
 *    -- list_iter_back, then the new element is inserted before the last item 
 *       in the list.
 *    -- for any other idx_ptr, the new element is insert before the 
 *       Iterator
 *
 * For example, to insert at the tail of the list do
 *      list_insert(mylist, myelem, NULL)
 * to insert at the front of the list do
 *      list_insert(mylist, myelem, list_iter_front(mylist))
 *
 * Note that use of this function results in the list to be marked as unsorted,
 * even if the element has been inserted in the correct position.  That is, on
 * completion of this subroutine the list_ptr->list_sorted_state must be equal 
 * to UNSORTED_LIST.
 */
void list_insert(list_t *list_ptr, data_t *elem_ptr, list_node_t * idx_ptr)
{
    assert(NULL != list_ptr);
    // insert your code after this line
    assert(elem_ptr!=NULL);
    list_node_t* node_ptr;
    node_ptr = (list_node_t* )calloc(1,sizeof(list_node_t));
    assert(node_ptr != NULL);
    node_ptr->data_ptr = elem_ptr;
    node_ptr->prev = NULL;
    node_ptr->next = NULL;
    
    if(list_ptr->current_list_size == 0){         //add to the head
        list_ptr->head = node_ptr;
        list_ptr->tail = node_ptr;
    }
    else if(idx_ptr==NULL){
        list_ptr->tail->next = node_ptr;              //add elem to the tail.  
        node_ptr->prev = list_ptr->tail;  //set prev for elem,
        list_ptr->tail = node_ptr;
    }
    else if(idx_ptr!=NULL){
        if(list_ptr->head == idx_ptr){       //add node to head.
            list_ptr->head->prev = node_ptr;
            node_ptr->next = list_ptr->head;
            list_ptr->head = node_ptr;
        }else{                              //add node to non-head non-tail pos.

            list_node_t* rover = idx_ptr;
            list_node_t* rover_prev = idx_ptr->prev;
            //int flag=-1;
            /*int count=0;
            for(count=0; count<list_ptr->current_list_size; count++ ){
                if(list_ptr->comp_proc(rover->data_ptr,idx_ptr->data_ptr)!=0 ){
                    rover = rover->next;
                }else{
                    //flag = 1;
                    rover_prev = rover->prev;
            */   
                    rover_prev->next = node_ptr;           //update front block
                    rover->prev = node_ptr;              //update current block
                    node_ptr->next = rover;                //update elem->next
                    node_ptr->prev = rover_prev;           //update elem->prev
                    rover = NULL;
                    rover_prev = NULL;
           // }
            }
       // }
    } //FIXME DRIVER.C failed to insert 333.
    list_ptr->current_list_size++;
    // the last two lines of this function must be the following 
    if (list_ptr->list_sorted_state == SORTED_LIST) 
	    list_ptr->list_sorted_state = UNSORTED_LIST;
    //list_debug_validate(list_ptr);
}

/* Removes the element from the specified list that is found at the 
 * iterator pointer.  A pointer to the data element is returned.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * idx_ptr: pointer to position of the element to be accessed.  This pointer
 *          must be obtained from list_elem_find, list_iter_front, or
 *          list_iter_next, or list_iter_back.  
 *
 *          If the idx_ptr is null, then assume that the first item
 *          at the head is to be removed.
 *
 * If the list is empty, then the function should return NULL.  Note: if
 *    the list is empty, then the iterator should be a NULL pointer.
 *
 * Note to remove the element at the front of the list use either
 *     list_remove(mylist, list_iter_front(mylist))
 *  or
 *     list_remove(mylist, NULL);
 *
 * Note: a significant danger with this function is that once
 * an element is removed from the list, the idx_ptr is dangling.  That
 * is, the idx_ptr now points to memory that is no longer valid.
 * You should not call list_iter_next on an iterator after there
 * has been a call to list_remove with the same iterator.
 *
 * When you remove the list_node_t in this function, you should null the next
 * and prev pointers before you free the memory block to avoid accidental use
 * of these now invalid pointers. 
 */
data_t * list_remove(list_t *list_ptr, list_node_t * idx_ptr)
{
    assert(NULL != list_ptr);
    if (0 == list_ptr->current_list_size)
	assert(idx_ptr == NULL);
    list_node_t* rover = NULL;
    list_node_t* rover_prev = NULL;
    list_node_t* rover_next = NULL;
    data_t* data_ptr = NULL;
    // insert your code after this line
    if(list_ptr->current_list_size == 1/* && (          //1-element situation.
       (list_ptr->comp_proc(list_ptr->head->data_ptr,idx_ptr->data_ptr)==0)||
       (list_ptr->comp_proc(list_ptr->tail->data_ptr,idx_ptr->data_ptr)==0) )*/
       ){
        rover = list_ptr->head;
        data_ptr = rover->data_ptr;
        list_ptr->tail = NULL;
        list_ptr->head = NULL;
    } 
    else if(idx_ptr == NULL || idx_ptr == list_ptr->head ){  //remove head.
        rover = list_ptr->head;
        data_ptr = rover->data_ptr;
        list_ptr->head = list_ptr->head->next;
        list_ptr->head->prev = NULL;
    }
    else if(idx_ptr == list_ptr->tail ){                 //remove tail.
        rover = list_ptr->tail;
        data_ptr = rover->data_ptr;
        list_ptr->tail = list_ptr->tail->prev;
        list_ptr->tail->next = NULL;


    }else {                                            //remove middle node.
        //if(list_elem_find(list_ptr,idx_ptr->data_ptr)==NULL) return NULL;       
        //int i=0;
        rover = list_ptr->head;
        //while(i< list_ptr->current_list_size){
         //   if(list_ptr->comp_proc(rover->data_ptr,idx_ptr->data_ptr)==0) break;
        //    rover = rover->next;
        //}
        rover = idx_ptr;
        data_ptr = rover->data_ptr;
        rover_next = rover->next;
        rover_prev = rover->prev;
        rover_prev->next = rover_next;
        rover_next->prev = rover_prev;
     }
        
        rover->data_ptr = NULL;
        free(rover);                                    //free rover
        rover = NULL;                                     
        list_ptr->current_list_size--;


    // the last line should verify the list is valid after the remove 
    //list_debug_validate(list_ptr);
    return data_ptr;  // fix the return value
}

/* Return a pointer to an element stored in the list, at the Iterator position
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from list_construct.
 *
 * idx_ptr: pointer to position of the element to be accessed.  This pointer
 *          must be obtained from list_elem_find, list_iter_front, 
 *          list_iter_back, or list_iter_next.  
 *
 * return value: pointer to the data_t element found in the list at the 
 * iterator position. A value NULL is returned if 
 *     a:  the idx_ptr is NULL.
 *     b:  the list is empty 
 */
data_t * list_access(list_t *list_ptr, list_node_t * idx_ptr)
{
    assert(NULL != list_ptr);
    if (idx_ptr == NULL)
	    return NULL;
    // debugging function to verify that the structure of the list is valid 
    //list_debug_validate(list_ptr);
    if(list_size(list_ptr)==0) return NULL;
    list_node_t* node_ptr = NULL;
    node_ptr = list_elem_find(list_ptr, idx_ptr->data_ptr);
    if (node_ptr != NULL){   // you have to fix the return value
        return node_ptr->data_ptr;
    }else{
        return NULL;
    }
}

/* This function verifies that the pointers for the two-way linked list are
 * valid. It checks if the list size equals the number of items in the list.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 *
 * No output is produced if the two-way linked list is correct.  
 * The program terminates and prints a line beginning with "Assertion
 * failed:" if an error is detected.
 *
 * The checks are not exhaustive. An error may still exist in the list even 
 * if these checks pass.
 *
 * If the linked list is sorted it also checks that the elements in the list
 * appear in the proper order.
 *
 */
void list_debug_validate(list_t *L)
{
    assert(NULL != L); 
    assert(SORTED_LIST == L->list_sorted_state || UNSORTED_LIST == L->list_sorted_state);
    if (0 == L->current_list_size) assert(NULL == L->head && L->tail == NULL);
    if (NULL == L->tail) 
	assert(NULL == L->head && 0 == L->current_list_size);
    else
	assert(NULL == L->tail->next);
    if (NULL == L->head) 
	assert(NULL == L->tail && 0 == L->current_list_size);
    else
	assert(NULL == L->head->prev);
    if (NULL != L->tail && L->tail == L->head) assert(1 == L->current_list_size);
    if (1 == L->current_list_size) {
        assert(L->head == L->tail && NULL != L->tail);
        assert(NULL != L->tail->data_ptr);
        assert(NULL == L->tail->next && NULL == L->head->prev);
    }
    if (1 < L->current_list_size) {
        assert(L->head != L->tail && NULL != L->tail && NULL != L->head);
        list_node_t *R = L->head;
        int tally = 0;
        while (NULL != R) {
            if (NULL != R->next) assert(R->next->prev == R);
            else assert(R == L->tail);
            assert(NULL != R->data_ptr);
            ++tally;
            R = R->next;
        }
        assert(tally == L->current_list_size); //assert the size.
    }
    if (NULL != L->head && SORTED_LIST == L->list_sorted_state) {
        list_node_t *R = L->head;
        while (NULL != R->next) {
            assert(-1 != L->comp_proc(R->data_ptr, R->next->data_ptr));
            R = R->next;
        }
    }
    
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */

