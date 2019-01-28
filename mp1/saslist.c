/* saslist.c
 * Zice Wei
 * zicew
 * ECE 2230 Fall 2018
 * MP1
 *
 * See ECE 2230 programming guide for required comments!
 *
 * Purpose: ADT code for sequential list to create list,
 *          destruct list, add record, remove record, l-
 *          ook up record. access record, and get the i-
 *          formation like current lenght and total size
 *          of a list.
 *Assumption: Many assumptions are made, such as trying 
 *            to avoid double freed block when removing
 *            a record.
 *Known bugs: there is no bugs.
 *
 * EVERY function must have comments
 */

// The ONLY member you can access in a secusr_info_t block is
// su_id.  All other members in this structure cannot be used
// in this file.
#include <stdlib.h>
#include "saslist.h"

/* purpose: to sort the secdonary users (by su_id)in ascending order.
 * input  : a saslist_t* pointer which contains the secondary usr information.
 * return : none.
 */
void sort(struct saslist_t* list_ptr){
    //Sorting only happens when the list_ptr is not a NULL.
    if(list_ptr != NULL){
        int i=0 , j=0 , lens=saslist_number_entries(list_ptr);
        //Template container for a secondary user.
        struct secusr_info_t* rec_ptr=NULL;
        //First loop.
        for (i = 0; i < lens-1; i++){
            //second loop.
            for (j = 0; j < lens-i-1; j++){
                //if former secondary su_id is greater than latter su_id.
                if (list_ptr->sec_usrs_ptr[j]->su_id > list_ptr->sec_usrs_ptr[j+1]->su_id){
                       //Swap manipulation.
                       rec_ptr = list_ptr->sec_usrs_ptr[j];
                       list_ptr->sec_usrs_ptr[j] = list_ptr->sec_usrs_ptr[j+1];
                       list_ptr->sec_usrs_ptr[j+1] = rec_ptr;
                       rec_ptr = NULL;
                }
           }
       }
   }
}

/* 
 * purpose: to construct new saslist_t datastructure.
 * input  : total size of secondary users.
 * return : pointer to a newly constructed data saslist_t structure.
 *
 */
struct saslist_t *saslist_construct(int size)
{   
    int i = 0;
    struct saslist_t* list_ptr;
    list_ptr = (struct saslist_t*)calloc(1,sizeof(struct saslist_t));
    //return NULL for non-existed list_ptr.
    if (list_ptr== NULL) return NULL;

    //assigning data.
    list_ptr->sas_list_size = size;
    list_ptr->sec_usrs_count = 0;
    //Alloc DM for each element.
    list_ptr->sec_usrs_ptr = (struct secusr_info_t**)calloc(size,sizeof(struct secusr_info_t*));
    if (list_ptr->sec_usrs_ptr == NULL){
        free(list_ptr->sec_usrs_ptr);
        free(list_ptr);
	return NULL;
    }
    //initialzaing each element.
    for (i=0; i<size; i++){
        list_ptr->sec_usrs_ptr[i] = NULL;
    }
    //send the list_ptr back.
    return list_ptr;
}


/* 
 * purpose: desstruct saslist_t ptr.
 * input  : pointer to a saslist_t ptr to be desctuct.
 * return : no return val.
 *
 */
void saslist_destruct(struct saslist_t *ptr)
{   
 
 
    if (ptr != NULL){
        //Free ineer DM.
        int i=0;
        for (i=0; i<saslist_number_entries(ptr);i++){ 
            if(ptr->sec_usrs_ptr[i]!=NULL){
                free(ptr->sec_usrs_ptr[i]);
                ptr->sec_usrs_ptr[i] = NULL;

            }
        }
        free(ptr->sec_usrs_ptr);
        ptr->sec_usrs_ptr = NULL;
        //Free outter DM.
        free(ptr);
        ptr = NULL;
    }
}

/* 
 * purpose: add sec_usr to the array in asscending order.
 * input  : pointer to a saslist_t struc and a pointer to secusr_info_t struc. 
 * return : 1 : element is added.
 *          0 : element is updated.
 *          -1: full and record is not duplicated or list does not exist.
 *
 */
int saslist_add(struct saslist_t *list_ptr, struct secusr_info_t *rec_ptr)
{
    // 1: add
    // 0: replc
    // -1: full
    //Input validation.
    if (list_ptr==NULL || rec_ptr==NULL) return -1;
    int lookup_val = -2;
    //input the first element if there is no record before.
    if(saslist_number_entries(list_ptr)==0){
        list_ptr->sec_usrs_ptr[0] = rec_ptr;
	    list_ptr->sec_usrs_count++;
	    return 1;
    }
    //reaption validation.
    lookup_val = saslist_lookup(list_ptr, rec_ptr->su_id);
    //list is full and no more space to place new record.
    if (lookup_val == -1 && 
        saslist_number_entries(list_ptr) == saslist_arraysize(list_ptr) ) return -1;
    //updating a current record.
    if (lookup_val != -1){
        free(list_ptr->sec_usrs_ptr[lookup_val]);
        list_ptr->sec_usrs_ptr[lookup_val] = NULL;
	    list_ptr->sec_usrs_ptr[lookup_val] = rec_ptr;
        return 0;
    }

    //add new record.
    list_ptr->sec_usrs_ptr[saslist_number_entries(list_ptr)] = rec_ptr;
    list_ptr->sec_usrs_count++;
    sort(list_ptr);
    return 1;
}

/* 
 * purpose: To look up a record by finding matched su_id
 * input  : su_id to be looked up.
 * return : index: index of a record with identical su_id.
 *          -1   : list is not created or record not found.
 *
 */
int saslist_lookup(struct saslist_t *list_ptr, int id)
{   
    //return -1 if list is not create or not valiable to access.
    if (list_ptr==NULL){
        return -1;
    }
    //looking for match record.
    int i=0, flag = 0;
    for (i=0; i< saslist_number_entries(list_ptr);i++){
        if(list_ptr->sec_usrs_ptr[i]->su_id == id){
	    flag = 1;
	    break;
        }
    }
    return flag==1?i:-1;
}

/* 
 * purpose: Found and send back a spcific accessable record.
 * input  : pointer to the list and record's index 
 * return : pointer to the record : if the record if found and accessable.
 *          NULL                  : not accessable.
 *
 */
struct secusr_info_t *saslist_access(struct saslist_t *list_ptr, int index)
{   if (list_ptr == NULL) return NULL;
    //Boundary Check...
    if(index+1 > list_ptr->sas_list_size || index < 0){
	return NULL;
    }

    //Searching...
    if (list_ptr->sec_usrs_ptr[index]!=NULL){
        return list_ptr->sec_usrs_ptr[index];
    }else{
        return NULL;
    }
}

/* 
 * purpose: remove record from the given index.
 * input  : pointer to a list and giving index. 
 * return : pointer to the record to be removed.
 *        : NULL: if no such record.
 *
 */
struct secusr_info_t *saslist_remove(struct saslist_t *list_ptr, int index)
{
    if (list_ptr == NULL) return NULL;
    struct secusr_info_t *rec=NULL;
    int i=0, curr_size = saslist_number_entries(list_ptr);
    if(saslist_access(list_ptr,index) == NULL){
        rec = NULL;
        return NULL;
    }
	rec = list_ptr->sec_usrs_ptr[index];
        //Remove.
	//1. remove at index
	list_ptr->sec_usrs_ptr[index] = NULL;
	//2. reshap (eliminate the gapa.)
	if (index+1 != saslist_number_entries(list_ptr) ){
        for(i=index; i<saslist_number_entries(list_ptr)-1; i++){
            list_ptr->sec_usrs_ptr[i] = list_ptr->sec_usrs_ptr[i+1];
	    }
    }
	//clean the record at the last position.
	list_ptr->sec_usrs_ptr[curr_size-1] = NULL;
	//decrease size.
    list_ptr->sec_usrs_count--;
    return rec;
}

/* 
 * purpose: get the total size of a list.
 * input  : pointer to a list
 * return : int for total size of a list
 *          0 if list is not created.
 *
 */
int saslist_arraysize(struct saslist_t *list_ptr)
{
    return list_ptr==NULL?0:list_ptr->sas_list_size;
}

/* 
 * purpose: get the current size of a list.
 * input  : pointer to a list
 * return : integer for current size of a list.
 *          0 if list is not created.
 *
 */
int saslist_number_entries(struct saslist_t *list_ptr)
{

    return list_ptr==NULL?0:list_ptr->sec_usrs_count;
}
