#include"rbtree.h"
#include<stdlib.h>
struct rb_node
{
	position parent;
	int color;
	int key;
	int data;
	position right;
	position left;
};

struct rb_node null = {&null, BLACK, EOF, EOF, &null, &null};

struct rb_node *t_nil = &null;
/*----------------------------------------------------------- 
|     x              y 
|    / \    ==>     / \ 
|   a   y          x   c 
|      / \        / \     
|     b   c      a   b    //Rotate with left 
-----------------------------------------------------------*/  
/*-----------------------------------------------------------
 * Left_Rotate(T,x)   (Introduction to Algorithms) 
 * 1 y = x->right
 * 2 x->right = y->left
 * 3 IF y->left != T_nil
 * 4	THEN y->left->parent = x
 * 5 y->parent = x->parent
 * 6 IF x->parent = T_nil
 * 7	THEN T_root = y
 * 8 ELSE IF x == x->parent->left
 * 9	THEN x->parent->left = y
 *10 ELSE x->parent->right = y
 *11 y->left = x
 *12 x->parent = y
 *---------------------------------------------------------*/
rbtree rb_rotate_left(position x, position root)
{
	if(x == t_nil || x->right == t_nil)
		return root;
	position y = x->right;
	x->right = y->left;/* Turn y's left subtree into x's right subtree*/
	if(y->left != t_nil)
		y->left->parent = x;
	y->parent = x->parent;/*Link x's parent to y*/
	if(x->parent == t_nil)
		root = y;
	else if(x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;/*Put x on y's left*/
	x->parent = y;/*Y becomes x's parent*/

	return root;
}

/*----------------------------------------------------------- 
|     x              y 
|    / \    ==>     / \ 
|   y   c          a   x 
|  / \                / \ 
| a   b              b   c //Rotate with right 
-----------------------------------------------------------*/  
/*-----------------------------------------------------------
 * Right_Rotate(T,x)   (Introduction to Algorithms) 
 * 1 y = x->left
 * 2 x->left = y->right
 * 3 IF y->right != T_nil
 * 4	THEN y->right->parent = x
 * 5 y->parent = x->parent
 * 6 IF x->parent = T_nil
 * 7	THEN T_root = y
 * 8 ELSE IF x == x->parent->right
 * 9	THEN x->parent->right = y
 *10 ELSE x->parent->left = y
 *11 y->right = x
 *12 x->parent = y
 *---------------------------------------------------------*/
rbtree rb_rotate_right(position x, position root)
{
	if(x == t_nil || x->left == t_nil)
		return root;
	position y = x->left;
	x->left = y->right;/* Turn y's left subtree into x's right subtree*/
	if(y->right != t_nil)
		y->right->parent = x;
	y->parent = x->parent;/*Link x's parent to y*/
	if(x->parent == t_nil)
		root = y;
	else if(x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;

	y->right = x;/*Put x on y's right*/
	x->parent = y;/*Y becomes x's parent*/

	return root;
}

rbtree rb_search_fixed(int key, position root, struct rb_node **save)
{
	position node = root, parent = t_nil;

	while(node != t_nil)
	{
		parent = node;
		if(node->key > key)
			node = node->left;
		else if(node-> key < key)
			node = node->right;
		else
			return node;
	}
	if(save)
	{
		*save = parent;
	}
	return t_nil;
}

rbtree rb_search(int key, position root)
{
	return rb_search_fixed(key, root, NULL);
}

rbtree rb_new_node(int key, int data)
{
	position node;
	node = malloc(sizeof(struct rb_node));
	if(node == NULL){
		space_fatal_error();
		return NULL;
	}
	else{
		node->key = key;
		node->data = data;
		return node;
	}
}

rbtree rb_insert(int key, int data, position root)
{
	position node, parent = t_nil;

	if(node = rb_search_fixed(key, root, (&parent)))
		return root;

	node = rb_new_node(key, data);
	node->parent = parent;
	node->left = node->right = t_nil;
	node->color = RED;

	if(parent != t_nil)
	{
		if(parent->key > key)
			parent->left = node;
		else
			parent->right = node;
	}
	else
		root = node;

	return rb_insert_fixup(node, root);
}

/*-----------------------------------------------------------
 * Rb_Insert_Fixup(T,z)   (Introduction to Algorithms) 
 * 1 WHILE z->parent->color = RED
 * 2	DO IF z->parent == z->parent->parent->left
 * 3		THEN y = z->parent->parent->right
 * 4		IF y->color = RED
 * 5			THEN z->parent->color = BLACK							//case 1
 * 6						y->color = BLACK										//case 1
 * 7						z->parent->parent->color = RED			//case 1
 * 8						z = z->parent->parent								//case 1								
 * 9		ELSE IF z == z->parent->right
 *10			THEN z = z->parent												//case 2
 *11						Left_Rotate(T,z)										//case 2
 *12		z->parent->color = BLACK										//case 3
 *13		z->parent->parent->color = RED							//case 3
 *14		Right_Rotate(T, z->parent->parent)					//case 3
 *15	ELSE(let "right" and "left" exchanged)
 *16 T.root->color = BLACK
 *---------------------------------------------------------*/
rbtree rb_insert_fixup(position node, position root)
{
	position uncle;
	
	while(node->parent != t_nil && node->parent->color == RED)
	{
		if(node->parent == node->parent->parent->left)/*when node's parent is left child of it's grandparent*/
		{
			uncle = node->parent->parent->right;
			if(uncle != t_nil && uncle->color == RED)
			{
				uncle->color = BLACK;
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else if(node->parent->right == node)
			{
				node = node->parent;
				root = rb_rotate_left(node, root);
			}
			node->parent->color = BLACK;
			node->parent->parent->color = RED;
			root = rb_rotate_right(node->parent->parent, root);
		}

		else
		{
			uncle = node->parent->parent->left;
			if(uncle != t_nil && uncle->color == RED)
			{
				uncle->color = BLACK;
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else if(node->parent->right == node)
			{
				node = node->parent;
				root = rb_rotate_left(node, root);
			}
			node->parent->color = BLACK;
			node->parent->parent->color = RED;
			root = rb_rotate_left(node->parent->parent, root);
		}
	}
	root->color = BLACK;
	return root;
}

rbtree rb_transplant(position node, position next, position root)
{
	if(node->parent == t_nil)
		root = next;
	else if(node == node->parent->left)
		node->parent->left = next;
	else
		node->parent->right = next;

	next->parent = node->parent;

	return root;
}

rbtree find_min(position root)
{
	if(root == t_nil)
		error_log("tree is empty");
	else if(root != t_nil)
		while(root->left != t_nil)
			root = root->left;
	return root;
}

rbtree rb_del(int key, position root)
{
	position temp, node_next, node;
	int temp_color;
	if(!(node = rb_search(key, root)))
	{
		error_log("key not found");
		return root;
	}

	temp = node;
	temp_color = temp->color;

	if(node->left == t_nil)
	{
		node_next = node->right;
		root = rb_transplant(root, node, node->right);
	}
	else if(node->right == t_nil)
	{
		node_next = node->left;
		root = rb_transplant(root, node, node->left);
	}
	else
	{
		temp = find_min(node->right);
		temp_color = temp->color;
		node_next = temp->right;
		if(temp->parent == node)
			node_next->parent = temp;
		else
		{
			root = rb_transplant(root, temp, temp->right);
			temp->right = node->right;
			temp->right->parent = temp;
		}
		root = rb_transplant(root, node, temp);
		temp->left = node->left;
		temp->left->parent = temp;
		temp->color = node->color;
	}
	if(temp_color == BLACK)
		return rb_del_fixup(node_next, root);
	else
		return root;
}

rbtree rb_del_fixup(position node, position root)
{
	position bro;
	while(node != root && node->color ==BLACK)
	{
		if(node == node->parent->left)
		{
			bro = node->parent->right;
			if(bro->color == RED)
			{
				bro->color = BLACK;
				node->parent->color = RED;
				root = rb_rotate_left(node->parent, root);
				bro = node->parent->right;
			}
			if(bro->left->color == BLACK && bro->right->color == BLACK)
			{
				bro->color = RED;
				node = node->parent;
				continue;
			}
			else if(bro->right->color == BLACK)
			{
				bro->left->color = BLACK;
				bro->color = RED;
				root = rb_rotate_right(bro, root);
				bro = node->parent->right;
			}
			bro->color = node->parent->color;
			node->parent->color = BLACK;
			bro->right->color = BLACK;
			root = rb_rotate_left(node->parent, root);
			node = root;
		}
		else if(node == node->parent->right)
		{
			bro = node->parent->left;
			if(bro->color == RED)
			{
				bro->color = BLACK;
				node->parent->color = RED;
				root = rb_rotate_right(node->parent, root);
				bro = node->parent->left;
			}
			if(bro->right->color == BLACK && bro->left->color == BLACK)
			{
				bro->color = RED;
				node = node->parent;
				continue;
			}
			else if(bro->left->color == BLACK)
			{
				bro->right->color = BLACK;
				bro->color = RED;
				root = rb_rotate_left(bro, root);
				bro = node->parent->left;
			}
			bro->color = node->parent->color;
			node->parent->color = BLACK;
			bro->left->color = BLACK;
			root = rb_rotate_right(node->parent, root);
			node = root;
			
		}
	}
	node->color = BLACK;
	return node;
}

/*
int main()
{
	position t;
	rb_insert(5, 100, t);
	rb_insert(4, 50, t);
	rb_insert(3, 60, t);
	rb_insert(8, 80, t);
	rb_insert(7, 70, t);
	rb_insert(1, 0, t);
	rb_insert(9, 10, t);
	rb_insert(4, 50, t);
*/

int main()  
{  
	int i, count = 100;  
	int key;  
	position root = t_nil, node = t_nil;  
				      
	srand(0);  
	for (i = 1; i < count; ++i)  
	{  
		key = rand() % count;  
			if ((root = rb_insert(key, i, root)))  
      {  
		    printf("[i = %d] insert key %d success!\n", i, key);
			}
			else  
			{  
				printf("[i = %d] insert key %d error!\n", i, key);  
			  exit(-1);  
			}  
															    
			if ((node = rb_search(key, root)))  
			{  
				printf("[i = %d] search key %d success!\n", i, key);  
			}  
			else  
			{  
				printf("[i = %d] search key %d error!\n", i, key);  
				exit(-1);  
			}  
			if (!(i % 10))  
			{  
			  if ((root = rb_del(key, root)))  
			  {  
				  printf("[i = %d] erase key %d success\n", i, key);  
			  }  
			  else  
			  {  
				printf("[i = %d] erase key %d error\n", i, key);  
				}  
			}  
	 }  
	 return 0;  
} 

/*
int main()
{
	rbtree root = t_nil;
	rb_insert(10, 5, root);
	return 0;
}
*/
