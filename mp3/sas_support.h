/* sas_support.h 
 * Zice Wei
 * zicew
 * ECE 2230 Fall 2018 
 * MP3
 *
 * Propose: sas_support.h for MP3
 *
 * Assumptions: structure definitions and public functions as defined for
 * assignmnet.  
 *
 * Bugs: there is no bugs
 *
 * You may change this file if needed for your design.
 */

#define MAXLINE 180

// prototype function definitions 

// function to compare secondary user records 
int sas_compare(const su_info_t *rec_a, const su_info_t *rec_b);

// functions to create and cleanup a list 
ListPtr sas_create(ListPtr, const char *);
void sas_cleanup(ListPtr);

void sas_print(ListPtr list_ptr, char *);      // print list of records 
void sas_stats(ListPtr, int, ListPtr);
int sas_invalid_ch(int);

void sas_add(ListPtr, int, ListPtr);
void sas_addtail(ListPtr,int);
void sas_lookup(ListPtr , int);
void sas_remove(ListPtr, ListPtr, int);
void sas_move(ListPtr, int, ListPtr);
void sas_change(ListPtr, int, int);
void sas_assign(ListPtr, int, ListPtr, int);
void sas_list_sort(ListPtr, int);
/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
