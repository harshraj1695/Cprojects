
//typedef struct btree {
//    int val;
//    struct btree* left;
//    struct btree* right;
//} btree;


typedef struct btree {
    int val;
    int h;
    struct btree* left;
    struct btree* right;
} btree;
typedef struct {
    btree **arr;
    int capacity;
    int size;
    int front;
    int rear;
} queuetree;

void init(queuetree *q, int cap);
int isfull(queuetree *q);
int isempty(queuetree *q);
void push(queuetree *q, btree *node);
btree* pop(queuetree *q);
int returnsize(queuetree* q);

btree* createnode(int key);
btree* insertval();

void inorder(btree* root);
void postorder(btree* root);
void preeorder(btree* root);

void levelorder(btree* root);