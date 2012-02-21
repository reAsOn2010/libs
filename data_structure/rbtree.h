#include"myerror.h"

#ifndef _rbtree_h
#define _rbtree_h
#define RED 0
#define BLACK 1

struct rb_node;
typedef struct rb_node *position;
typedef struct rb_node *rbtree;
rbtree rb_rotate_left(position x, position root);
rbtree rb_rotate_right(position x, position root);
rbtree rb_search_fixed(int key, position root, struct rb_node **save);
rbtree rb_search(int key, position root);
rbtree rb_insert(int key, int data, position root);
rbtree rb_insert_fixup(position node, position root);
rbtree find_min(position root);
rbtree rb_del(int key, position root);
rbtree rb_del_fixup(position node, position root);

#endif

