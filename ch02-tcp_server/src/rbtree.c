typedef struct rbnode{
    int data;
    int color; /* 0 for red, 1 for black */
    struct rbnode* left;
    struct rbnode* right;
} RBNode;

typedef struct rbtree{
    RBNode* root;
} RBTree;

RBNode* rbnode_create(int val){
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    node->data = val;
    node->color = 0;
    node->left = NULL;
    node->right = NULL;
}

void rbtree_insert(RBNode* rbnode, int val){
    RBNode* tmp = rbnode_create(val);
    if(rbnode == NULL){
	tmp->color = 1;
	rbnode = tmp;
	return 0;	
    }
    
    rval = rbnode->val;
    if(rval > val){
	if(rbnode->left == NULL){
	}
    }

}
/* insert a node to Red-Black Tree */

// always insert a red node first

// if no node in tree, then insert the node as Root (black)

// if its parent node is Black, its ok.

// if its parent node is Red, then start Rotate. 
