#include<stdio.h>
#include<limits.h>
#include<assert.h>
#include <malloc.h>
#include "bst.h"

//private function.
bst_node_t* recursive_insert(bst_t*,bst_node_t*,bst_key_t, data_t, int*);
void destruct_recursive(bst_node_t* node_ptr);
bst_node_t* access_recursive(bst_node_t* node_ptr, bst_key_t key, bst_t* T, int* isFound);
bst_node_t* recursive_remove(bst_t* T, bst_node_t* rover, data_t* back, bst_key_t key, int* isFound);
bst_node_t* find_max_value( bst_node_t* node_ptr, bst_key_t key);
/*
 * Construct a bst tree.
 * input: int for tree policy.
 * return pointer of bst_t.
 */
bst_t *bst_construct (int tree_policy){
    bst_t* T = (bst_t*)calloc(1,sizeof(bst_t));
    if(T==NULL) return NULL;

    //initializing.
    T->root = NULL;
    T->tree_policy = tree_policy;
    T->tree_size = 0;
    T->num_recent_key_comparisons = 0;
    return T;

}

/*
 * Destruct a tree
 * pass a pointer to a bst_t tree.
 *
 * no return Val.
 */
void bst_destruct (bst_t *T){
    bst_node_t* node_ptr = T->root;
    destruct_recursive(node_ptr);

    //destruct header info.
    free(T);
    T = NULL;
}
/* Destruct recursively
 * input pointer of a node.
 * no return_val
 */
void destruct_recursive(bst_node_t* node_ptr){
    if(node_ptr==NULL) return;

    destruct_recursive(node_ptr->left);
    destruct_recursive(node_ptr->right);

    free(node_ptr->data_ptr);
    node_ptr->data_ptr = NULL;
    free(node_ptr);
    node_ptr = NULL;
}


/* A recursive way to insert node.
 *
 * input key, data, and pointer to node.
 * return pointer to node.
 *
 */
bst_node_t* recursive_insert(bst_t* T, bst_node_t* rover, bst_key_t key, data_t data_ptr, int* isReplace){
    //Add new node.
    if(rover == NULL  ){
        bst_node_t* node_ptr=(bst_node_t*)calloc(1,sizeof(bst_node_t));
        node_ptr->data_ptr = data_ptr;
        node_ptr->key = key;
        node_ptr->left = NULL;
        node_ptr->right = NULL;
        return node_ptr;
    }

    if(key > rover->key){
        T->num_recent_key_comparisons++;
        rover->right = recursive_insert(T,rover->right,key,data_ptr, isReplace);

    }
    else if(key < rover->key){
        T->num_recent_key_comparisons++;
        rover->left = recursive_insert(T, rover->left,key, data_ptr, isReplace);

    }else if(key == rover->key){
        T->num_recent_key_comparisons++;
        *isReplace = 1;
        if(rover->data_ptr!=NULL);
        data_t temp = rover->data_ptr;
        rover->data_ptr = data_ptr;
        free(temp);
        return rover;
    }
    return rover;

}

/*
 * Insert a node into a bst tree.
 *
 * pass pointer to root pointer, a bst_key_t and a
 * data_t pointer.
 * Return 0 if key is already in the bst(memory block is replaced).
 * Return 1 if key is inserted.
 *
 */
int bst_insert (bst_t *T, bst_key_t key, data_t data_ptr){
    assert(T!=NULL);
    int isReplace = 0;

    T->num_recent_key_comparisons = 0;
    T->root = recursive_insert(T, T->root, key, data_ptr, &isReplace);


    T->tree_size = isReplace==1?T->tree_size:T->tree_size+1;
    return isReplace==1?0:1;

}
/* Find a prev pointer to a pointer with key Max val.
 * input pointer to a Tree, pointer to a node, key, int flag for finding min
 *       or max val,
 * return pointer to a node.
 *     NULL if prev is the pointer passed in.
 *
 */

bst_node_t* find_min_value(bst_node_t* node_ptr, bst_key_t key){
  //  if(node_ptr->left!=NULL){
  //      node_ptr->left = find_min_value(node_ptr->left,key);
//    }
    if(node_ptr==NULL) return NULL;
    while(node_ptr->left != NULL){
        node_ptr = node_ptr->left;
    }

    return node_ptr;
}



/* Find a prev pointer to a pointer with key Min val.
 * input pointer to a Tree, pointer to a node, key, int flag for finding min
 *       or max val,
 * return pointer to a node.
 *     NULL if prev is the pointer passed in.
 *
 */


bst_node_t* find_max_value(bst_node_t* node_ptr, bst_key_t key){
    //find max val for key Left  (in right sub tree).
     //  printf("%d ",node_ptr->key);
    /* if(node_ptr->right!=NULL){
       node_ptr->right = find_max_value(node_ptr->right, key);
      }
    //printf("\n");
    printf("max is%d\n",node_ptr->key);
    return node_ptr;
    */
    if(node_ptr->right==NULL) return NULL;
    while(node_ptr->right != NULL){
        node_ptr = node_ptr->right;
    }
    return node_ptr;
}
/* Find  prev node
 * input node.
 *
 */
 bst_node_t* find_parent(bst_key_t key, bst_node_t* rover, bst_t* T){
       //  printf("rover %d\n",rover->key);
  /*
      if(rover->key == key){
             return T->root;
             //no prev node/ root position.
         }else{
             if ( (rover->left!=NULL && rover->left->key == key)
               || (rover->right!=NULL && rover->right->key == key) ){
                 return rover;
             }
             else{
                     if(rover->key > key){
                         rover->left = find_parent(key, rover->left, T);
                     }else{
                         rover->right = find_parent(key, rover->right, T);

                     }
             }
          }
*/
while(rover!=NULL){
    if(rover->key==key) return T->root;
    else{
         if ( (rover->left!=NULL && rover->left->key == key)
           || (rover->right!=NULL && rover->right->key == key) ){
             return rover;
         }
         else{
             if(rover->key > key){
                 rover = rover->left;
             }else{
                 rover = rover->right;
             }
         }



    }
}

return NULL;


 }
/* A recursive way to REMOVE node.
 *
 * input key, data, and pointer to node.
 * return pointer to node.
 *
 */
bst_node_t* recursive_remove(bst_t* T, bst_node_t* rover, data_t* back, bst_key_t key, int* isFound){
    //Add new node.
    //bst_node_t* prev = NULL;
    //bst_node_t* del = NULL;
    //bst_node_t* max = NULL;
    bst_node_t* min = NULL;
    //bst_node_t* save = NULL;
    //data_t temp_data;

    if(rover == NULL  ){
        return NULL;
    }
    if(key < rover->key){
      T->num_recent_key_comparisons+=2;
        recursive_remove(T, rover->left, back, key, isFound);
    }


    else if(key > rover->key){
        T->num_recent_key_comparisons+=2;
        recursive_remove(T, rover->right, back, key, isFound);
    }

    else{
      T->num_recent_key_comparisons++;
        *back = rover->data_ptr; ;
        *isFound = 1;

        if(rover->left==NULL){
          bst_node_t* temp = rover->right;
          *back = rover->data_ptr;
           free(rover);
          return temp;

        }
        if(rover->right == NULL ){
          bst_node_t* temp = rover->left;
          *back = rover->data_ptr;
           free(rover);
          return temp;


        }
        bst_node_t* max = find_min_value(rover->right, key);
        *back = rover->data_ptr;
        rover->key = max->key;
        rover->data_ptr = max->data_ptr;



        //rover->right = recursive_remove(T, rover->right, back,temp->key, isFound );

    }

    return rover;

}

/*
 * Remove a node and free memory block.
 * pass a pointer to a root and a key val to be removed.
 *
 * Return pointer to be removed.
 * rerurn NULL is key is not found.
 */
data_t bst_remove (bst_t *T, bst_key_t key){

    int isFound = 0;
    bst_node_t* temp;
    data_t back = NULL;
    data_t* _back = NULL;
   // bst_node_t* temp;
    temp = recursive_remove(T, T->root,&back, key, &isFound);
    if(back !=NULL){
        T->tree_size--;
    }
    //_back = temp->data_ptr;
    //free(temp);
    return back!=NULL?back:NULL;
}

/*
 * Return the number of keys in the BST.
 * input pointer to abst tree.
 * Return val the number of keys in the bst.
 *
 *
 */
int bst_size(bst_t * T){


   return T->tree_size;
}
/* Access node Recursively
 * Inoput: pointer to a Tree, pointer to a node and
 *         key val.
 * Return: pointer to a node if key is matched,
 *         or a NULL if key is not find.
 */
bst_node_t* access_recursive(bst_node_t* node_ptr, bst_key_t key, bst_t* T, int* isFound){
    if(node_ptr == NULL) return NULL;

    if(key > node_ptr->key){
        T->num_recent_key_comparisons++;
        node_ptr->left = access_recursive(node_ptr->left, key, T, isFound);

    }
    if(key < node_ptr->key){
        T->num_recent_key_comparisons++;
        node_ptr->right = access_recursive(node_ptr->right, key, T, isFound);


    }

    if(key == node_ptr->key){
        T->num_recent_key_comparisons++;
        if(node_ptr->data_ptr==NULL) return NULL;
        *isFound = 1;
        return node_ptr;

    }
    return node_ptr;
}

/*
 * Find the tree element with the matching key.
 * input pointer of a bst tree.
 * Return a |pointer| to the data block that is stored in this node
 * in the tree. If the key is not found in the tree then
 * return |NULL|.
 *
 *
 *
 */
data_t bst_access (bst_t * T, bst_key_t key){
    int isFound = 0;
    T->root = access_recursive(T->root,key, T, &isFound);
    return isFound==0?NULL:T->root->data_ptr;
}
/* Find the number of key comaprisons for most recent
 * call to |bst_access| , |bst_insert| , or |bst_remove| .
 *
 * input pointer of bst tree.
 * Return number of comparison.
 *
 *
 */
int bst_stats (bst_t * T){

   return T->num_recent_key_comparisons;
}

/* Ugly print the tree.
 * Input: pointer to root of a tree, level of a tree.
 * No return Val.
 */
void ugly_print(bst_node_t *N, int level) {
    if (N == NULL) return ;
        ugly_print(N->right, level+1) ;
        for (int i=0; i<level; i++) printf("     ");//5 spaces.
        printf("%5d\n", N->key);
        ugly_print(N->left, level+1);
}
/* 5 spaces */
/* field width is 5 */


/* Purpose: find internal path length of the tree.
 * Input: pointer to a tree.
 *
 * return: length of a tree.
 */
int bst_int_path_len(bst_t * T){
    return -1;
}

//debug print function.
void bst_debug_print_tree(bst_t * T){
    ugly_print(T->root, 0);
    printf("\n\n");
}


//void bst_debug_validate(bst_t *);

void bst_debug_validate(bst_t *T){
    int size = 0;
    assert(bst_debug_validate_rec(T->root, INT_MIN, INT_MAX, &size) == TRUE);
    assert(size == T->tree_size);
}
int bst_debug_validate_rec(bst_node_t *N, int min, int max, int *count){
    if (N == NULL) return TRUE;
    if (N->key <= min || N->key >= max) return FALSE;
    assert(N->data_ptr != NULL);
    *count += 1;

    return bst_debug_validate_rec(N->left, min, N->key, count) &&
        bst_debug_validate_rec(N->right, N->key, max, count);
}
