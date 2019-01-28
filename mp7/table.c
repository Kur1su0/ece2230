#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "table.h"

extern int Verbose;
#define MAX(X,Y) X>Y?X:Y

/*  The empty table is created.  The table must be dynamically allocated and
 *  have a total size of table_size.  The maximum number of (K, I) entries
 *  that can be stored in the table is table_size-1.  For open addressing, 
 *  the table is filled with a special empty key distinct from all other 
 *  nonempty keys.  
 *
 *  the probe_type must be one of {LINEAR, DOUBLE, QUAD}
 *
 *  Do not "correct" the table_size or probe decrement if there is a chance
 *  that the combinaion of table size or probe decrement will not cover
 *  all entries in the table.  Instead we will experiment to determine under
 *  what conditions an incorrect choice of table size and probe decrement
 *  results in poor performance.
 */
table_t *table_construct(int table_size, int probe_type){ 
    table_t* T = (table_t*)calloc(1, sizeof(table_t));
    
    T->table_size_M = table_size;
    //LINEAR DOUBLE QUAD
    T->probe_type = probe_type;
    T->num_keys_stored_in_table = 0;
    
    T->oa = (table_entry_t*)calloc(table_size, sizeof(table_entry_t));
    int  i = 0;
    for(i=0; i<table_size; i++){
        T->oa[i].key = 0;
        //T->oa[i].data_ptr == NULL;
    }
    
    return T;
}


/* Sequentially remove each table entry (K, I) and insert into a new
 * empty table with size new_table_size.  Free the memory for the old table
 * and return the pointer to the new table.  The probe type should remain
 * the same.
 *
 * Do not rehash the table during an insert or delete function call.  Instead
 * use drivers to verify under what conditions rehashing is required, and
 * call the rehash function in the driver to show how the performance
 * can be improved.
 */
table_t *table_rehash(table_t * T, int new_table_size){
    table_t* new = table_construct(new_table_size, T->probe_type);
    int size = T->table_size_M;
    int i = 0;
    for(i = 0; i < size; i++){
        if(T->oa[i].key != 0 && T->oa[i].key!=-1 ){
            table_insert(new,T->oa[i].key, T->oa[i].data_ptr);
            T->oa[i].data_ptr = NULL;
        }
    }
    //free(T->oa);
    //free(T);
    table_destruct(T);
    T = NULL;  
    return new;
}

/* returns number of entries in the table */
int table_entries(table_t * T){
    return T->num_keys_stored_in_table;

}

/* returns 1 if table is full and 0 if not full. */
int table_full(table_t * T){
    return T->num_keys_stored_in_table==T->table_size_M-1?1:0;
}

/* returns the number of table entries marked as deleted */
int table_deletekeys(table_t * T){
    int i=0;
    int num_del = 0;
    for(i = 0 ; i < T->table_size_M; i++)
        num_del += T->oa[i].key==-1?1:0;
   
    return num_del;   

}


/* Insert a new table entry (K, I) into the table provided the table is not
 * already full.  
 * Return:
 *      0 if (K, I) is inserted, 
 *      1 if an older (K, I) is already in the table (update with the new I), or 
 *     -1 if the (K, I) pair cannot be inserted.
 */
int table_insert(table_t* T, hashkey_t K, data_t I){
    
    T->num_probes_for_most_recent_call = 1;
    int i,  ProbeDec, place = -1;
    int M = T->table_size_M, flag=0;
    //hashkey_t ProbeKey;
 
    //Hash func and prob
    i = K % M;
    int max_jump = M;
    if(T->probe_type == LINEAR) ProbeDec = 1;
    if(T->probe_type == DOUBLE) ProbeDec = MAX( 1, (K/M)%M );
    if(T->probe_type == QUAD)   ProbeDec = 0;
    int j = 0;
    
    //if(Verbose)
    //    printf("init insert: K:%d at:%d\n",K,i);
    //insert
    while(T->oa[i].key>0){    //FIXME: delete key.
        if( (max_jump <= 0)||( T->oa[i].key == K)) break;
        if(T->oa[i].key == -1 && flag==0) {
            place = i;
            flag = 1;
        }
        T->num_probes_for_most_recent_call++;
        j++;
        if (T->probe_type == QUAD) i-=j;
        else i-=ProbeDec;
        if(i < 0) i+=M;
        
        //max_jump-=T->probe_type==QUAD?j:ProbeDec;
        
        max_jump--;
        
    }

    //alread in.
    if(T->oa[i].key==K){
        //XXX:malloc?
        free(T->oa[i].data_ptr);
        T->oa[i].data_ptr = I;
        return 1;
    }
    //full
    if((T->num_keys_stored_in_table ) == M - 1) {
        return -1;
    }
    
    //no empty slot
            
    if(max_jump <= 0 && place ==-1) {
        return -1;
    }
    
    //if(prob_flag!=0 && ori_pos) return -1;
    //not full.
    i=(place!=-1&&T->oa[i].key!=K)?place:i;
    T->oa[i].key = K;
    T->oa[i].data_ptr = I;
    T->num_keys_stored_in_table++;
   // if(Verbose){
   //     printf("insert: K:%d at:%d\n",K,i);
   //     table_debug_print(T);
   // }
    return 0;
}

/* Delete the table entry (K, I) from the table.  
 * Return:
 *     pointer to I, or
 *     null if (K, I) is not found in the table.  
 *
 * See the note on page 490 in Standish's book about marking table entries for
 * deletions when using open addressing.
 */
data_t table_delete(table_t * T, hashkey_t K){
    T->num_probes_for_most_recent_call = 1;
    int i,  ProbeDec;
    int M = T->table_size_M;
    data_t temp = NULL;
    //hashkey_t ProbeKey;

    //Hash func and prob
    i = K % M;
    //if(i == 1) printf("K:23 at pos:%d %d\n",i,K);
    int max_jump = M;
    if(T->probe_type == LINEAR) ProbeDec = 1;
    if(T->probe_type == DOUBLE) ProbeDec = MAX( 1, (K/M)%M );
    if(T->probe_type == QUAD)   ProbeDec = 0;
    int j = 0;

    //insert
    while(T->oa[i].key!=K){
        if( (max_jump <= 0)) break;
        T->num_probes_for_most_recent_call++;
        j++;
        if (T->probe_type == QUAD) i-=j;
        else i-=ProbeDec;
        if(i < 0) i+=M;
        //max_jump-=T->probe_type==QUAD?j:ProbeDec;
        max_jump--;
    }
    if(K == T->oa[i].key){
        T->num_keys_stored_in_table--;    
        T->oa[i].key = -1;
        temp = T->oa[i].data_ptr;
        T->oa[i].data_ptr = NULL;
    }
    return temp;
    
}

/* Given a key, K, retrieve the pointer to the information, I, from the table,
 * but do not remove (K, I) from the table.  Return NULL if the key is not
 * found.
 */
data_t table_retrieve(table_t * T, hashkey_t K){
    T->num_probes_for_most_recent_call = 1;
    int i,  ProbeDec;
    int M = T->table_size_M;
    //hashkey_t ProbeKey;
 
    //Hash func and prob
    i = K % M;
    int max_jump = M;
    if(T->probe_type == LINEAR) ProbeDec = 1;
    if(T->probe_type == DOUBLE) ProbeDec = MAX( 1, (K/M)%M );
    if(T->probe_type == QUAD)   ProbeDec = 0;
    int j = 0;
    
    //insert
    while(T->oa[i].key!=K && T->oa[i].key!=0){
        if( (max_jump <= 0)) break;
        
        T->num_probes_for_most_recent_call++;
        j++;
        if (T->probe_type == QUAD) i-=j;
        else i-=ProbeDec;
        if(i < 0) i+=M;
        max_jump--;
        //max_jump-=T->probe_type==QUAD?j:ProbeDec;

    }
    
    return T->oa[i].key==K?T->oa[i].data_ptr:NULL;
}

/* Free all information in the table, the table itself, and any additional
 * headers or other supporting data structures.  
 */
void table_destruct(table_t * T){
    int i=0;
    for(i=0; i<T->table_size_M; i++){
        if(T->oa[i].data_ptr!=NULL );
        free(T->oa[i].data_ptr);
    }
    free(T->oa);
    free(T);
    T = NULL;

}

/* The number of probes for the most recent call to table_retrieve,
 * table_insert, or table_delete 
 */
int table_stats(table_t * T){
    return T->num_probes_for_most_recent_call;
}

/* This function is for testing purposes only.  Given an index position into
 * the hash table return the value of the key if data is stored in this 
 * index position.  If the index position does not contain data, then the
 * return value must be zero.  Make the first
 * lines of this function 
 *       assert(0 <= index && index < table_size); 
 */
hashkey_t table_peek(table_t *T, int index){
    assert(0 <= index && index < T->table_size_M); 
    
    return T->oa[index].data_ptr!=NULL?T->oa[index].key:0;
}

/* Print the table position and keys in a easily readable and compact format.
 * Only useful when the table is small.
 */
void table_debug_print(table_t *T){
    int i = 0;
    printf("Table info:\n\n");
    printf("%-4s%-4s\n","i","Key");
    printf("--------\n");
    for(i = 0; i < T->table_size_M ; i++){
        if(T->oa[i].key==-1) 
            printf("%-4d%-4s\n",i,"del");
        else if(T->oa[i].key==0) 
            printf("%-4d%-4s\n",i,"empt");
        else 
            printf("%-4d%-4d\n",i,T->oa[i].key);
    }

}
    
    
    
    

    
    

