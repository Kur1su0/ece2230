#include<stdio.h>
#include<limits.h>
#include<assert.h>
#include <malloc.h>
#include "bst.h"

//private function.
bst_node_t* find_max_value( bst_node_t* node_ptr, bst_key_t key);
bst_node_t* find_min_value(bst_node_t* node_ptr, bst_key_t key);
bst_node_t* find_parent(bst_key_t key, bst_node_t* rover, bst_t* T);

void destruct_recursive(bst_node_t* node_ptr);
bst_node_t* recursive_insert(bst_t*,bst_node_t*,bst_key_t, data_t, int*);
data_t recursive_remove(bst_t* T, bst_node_t* rover, bst_key_t key, int* isFound);
data_t remove_helper(bst_t* T, bst_node_t* rover, bst_key_t key);

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
    if(T == NULL) return;
    bst_node_t* node_ptr = T->root;
    destruct_recursive(node_ptr);

    T->root = NULL;
    //destruct header info.
    free(T);
    T = NULL;
}
/* Destruct recursively
 * input pointer of a node.
 * no return_val
 */
void destruct_recursive(bst_node_t* node_ptr){
    if(node_ptr!=NULL){
    //Recursively destruct.
    destruct_recursive(node_ptr->left);
    destruct_recursive(node_ptr->right);

    if(node_ptr->data_ptr!=NULL){
        free(node_ptr->data_ptr);
        node_ptr->data_ptr = NULL;
    }
        free(node_ptr);
        node_ptr = NULL;

    }
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
    //Goes right
    if(key > rover->key){
        T->num_recent_key_comparisons+=2;
        rover->right = recursive_insert(T,rover->right,key,data_ptr, isReplace);

    }
    //goes left
    else if(key < rover->key){
        T->num_recent_key_comparisons+=2;
        rover->left = recursive_insert(T, rover->left,key, data_ptr, isReplace);
    //found key.
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
    //clear comparison flag
    T->num_recent_key_comparisons = 0;
    int isReplace = 0;

    T->root = recursive_insert(T, T->root, key, data_ptr, &isReplace);


    T->tree_size = isReplace==1?T->tree_size:T->tree_size+1;
    //int lens = bst_int_path_len(T);
    //printf("lens: %d\n", lens);
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
    if(node_ptr->right==NULL) return node_ptr;
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

return rover;


 }
/* A helper function to REMOVE node.
 *
 * input key, data, and pointer to node.
 * return pointer to node.
 *
 */
data_t remove_helper(bst_t* T, bst_node_t* rover, bst_key_t key){
    //Add new node.
    bst_node_t* prev = NULL;
    bst_node_t* max = NULL;
    data_t back =  NULL;
    if(rover == NULL  ){
        return NULL;
    }
    while(rover!=NULL){
    if(rover==NULL) return NULL;
        if(key < rover->key){
            T->num_recent_key_comparisons+=2;
            rover = rover->left;
        }
    if(rover==NULL) return NULL;

        if(key > rover->key){
            T->num_recent_key_comparisons+=2;
            rover = rover->right;
        }
    if(rover==NULL) return NULL;
        if(key == rover->key){
            break;
        }
    }
    if(key == rover->key){
            T->num_recent_key_comparisons++;
            back = rover->data_ptr;
            //isFound = 1;
        //del node is a leave.
        if(rover->left==NULL && rover->right==NULL){
            //printf("both NULL\n");
            prev = find_parent(rover->key, T->root, T);
            //parent its self
            if(prev == T->root && prev==rover){
                T->root = NULL;
                back = prev->data_ptr;
                free(prev);
                return back;
            }
            else{
                if(prev->left == rover){
                    prev->left = NULL;
                }
                else{
                //right child case.
                    prev->right = NULL;
                }
                free(rover);
                return back;
            }

        }
        else if(rover->left==NULL || rover->right==NULL){
            bst_node_t* del = NULL;
            if(rover->left==NULL){
                del = rover->right;

                rover->data_ptr = del->data_ptr;
                rover->key = del->key;
                rover->right = del->right;
                rover->left = del->left;

                free(del);
                return back;
            }

            else if(rover->right==NULL){
                del = rover->left;

                rover->data_ptr = del->data_ptr;
                rover->key = del->key;
                rover->left = del->left;
                rover->right = del->right;

                free(del);
                return back;
            }
        }else{
            //not empty.
            max = find_max_value(rover->left, key);
            prev = find_parent(max->key,T->root,T);
            //removed node is a parent.
            rover->data_ptr = max->data_ptr;
            rover->key = max->key;
            if(prev == T->root || prev->left==max){
                 prev->left = max->left;
            }else{
                 prev->right = max->left;
            }
            free(max);
            return back;
        }
    }
    return back;
}

/*
 * Remove a node and free memory block.
 * pass a pointer to a root and a key val to be removed.
 *
 * Return pointer to be removed.
 * rerurn NULL is key is not found.
 */
data_t bst_remove (bst_t *T, bst_key_t key){
    T->num_recent_key_comparisons = 0;
    data_t data_ptr = NULL;
    // bst_node_t* temp;
    data_ptr = remove_helper(T, T->root, key);
    if(data_ptr!=NULL){
        T->tree_size--;
    }
    return data_ptr==NULL?NULL:data_ptr;
    //return back;
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
    T->num_recent_key_comparisons = 0;
    bst_node_t* temp = T->root;
    while(temp != NULL){
         //found
        if(temp->key == key){
            T->num_recent_key_comparisons++;
            break;
        }
        if(temp==NULL) break;
        //left
        if(temp->key < key){
            T->num_recent_key_comparisons+=2;
            temp = temp->right;
        }
        if(temp==NULL) break;
        //right
        if(temp->key > key){
           T->num_recent_key_comparisons+=2;
           temp = temp->left;

        }
    }
    return temp==NULL?NULL:temp->data_ptr;
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

/* recursively find path
 * input pointer to node, level number
 * return: total length of path.
 */
int recursive_path(bst_node_t* rover, int level){
     if (rover==NULL){
        return 0;
     }
     //Super nested recursion,
     return level + recursive_path(rover->left, level + 1)
                  + recursive_path(rover->right, level+ 1);


}
/* Purpose: find internal path length of the tree.
 * Input: pointer to a tree.
 *
 * return: length of a tree.
 */
int bst_int_path_len(bst_t * T){
    int lens = 0;
    lens = recursive_path(T->root, lens);
    return lens;
}

// Debug print function.
// input pointer to a tree.
// no return val.
void bst_debug_print_tree(bst_t * T){
    ugly_print(T->root, 0);
    bst_debug_validate(T);
}


//void bst_debug_validate(bst_t *);
/*
 * validate tree.
 * input pointer to a Tree.
 * no return val.
 */

void bst_debug_validate(bst_t *T){
    int size = 0;
    assert(bst_debug_validate_rec(T->root, INT_MIN, INT_MAX, &size) == TRUE);
    assert(size == T->tree_size);
}

/* validate tree helper function.
 * 
 * input: pointer to a node. min val, max val and pointer to a counter.
 * 
 * return TRUE  (1) if passed
 *        FALSE (0) no passed
 */
int bst_debug_validate_rec(bst_node_t *N, int min, int max, int *count){
    if (N == NULL) return TRUE;
    if (N->key <= min || N->key >= max) return FALSE;
    assert(N->data_ptr != NULL);
    *count += 1;

    return bst_debug_validate_rec(N->left, min, N->key, count) &&
        bst_debug_validate_rec(N->right, N->key, max, count);
}
