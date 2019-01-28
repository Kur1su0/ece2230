enum balanceoptions {BST, AVL, TWOTHREET};
#define TRUE 1
#define FALSE 0
typedef void *data_t;
typedef int bst_key_t;

typedef struct bst_node_tag {
    data_t data_ptr;
    bst_key_t key;
    struct bst_node_tag *left;
    struct bst_node_tag *right;
} bst_node_t;

typedef struct bst_tag {
    bst_node_t *root;
    int tree_policy;
    // must be a balanceoptions
    int tree_size;
    int num_recent_key_comparisons;
} bst_t;

//prototype for bst.c

data_t bst_access (bst_t *, bst_key_t);
bst_t *bst_construct (int tree_policy);
void bst_destruct (bst_t *);
int bst_insert (bst_t *, bst_key_t, data_t);
data_t bst_remove (bst_t *, bst_key_t);
int bst_size(bst_t *);
int bst_stats (bst_t *);
int bst_int_path_len(bst_t *);
//debug print function.
int bst_debug_validate_rec(bst_node_t*, int, int, int*);
void bst_debug_validate(bst_t *);
void bst_debug_print_tree(bst_t *);


