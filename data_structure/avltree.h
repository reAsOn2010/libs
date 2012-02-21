#include"myerror.h"

#ifndef _avltree_h
#define _avltree_h

struct avl_node;
typedef struct avl_node *position;
typedef struct avl_node *avl_tree;

avl_tree make_empty(avl_tree t);/**/
position find(int x, avl_tree t);/**/
position find_min(avl_tree t);/**/
position find_max(avl_tree t);/**/
avl_tree insert(int x, avl_tree t);/**/
avl_tree del(int x, avl_tree t);/**/
int retrieve(position p);/**/
static int tell_height(position p);/**/
position single_rotate_with_left(position k2);/**/
position single_rotate_with_right(position k2);/**/
position double_rotate_with_left(position k3);/**/
position double_rotate_with_right(position k3);/**/

#endif /*_avltree_h*/

struct avl_node
{
	int element;
	avl_tree left;
	avl_tree right;
	int height;
};

avl_tree make_empty(avl_tree t)
{
	if(t != NULL)
	{
		make_empty(t->left);
		make_empty(t->right);
		free(t);

	}
	return NULL;
}

position find(int x, avl_tree t)
{
	if(t == NULL)
		return NULL;
	if(x < t->element)
		return find(x, t->left);
	else if(x > t->element)
		return find(x, t->right);
	else
		return t;
}

position find_min(avl_tree t)
{
	if(t == NULL)
		error_log("tree is empty");
	else if(t != NULL)
		while(t->left != NULL)
			t = t->left;
	return t;
}

position find_max(avl_tree t)
{
	if(t == NULL)
		error_log("tree is empty");
	else if(t != NULL)
		while(t->right != NULL)
			t = t->right;
	return t;
}

static int tell_height(position p)
{
	if(p == NULL)
		return (-1);
	else 
		return p->height;
}

avl_tree insert(int x, avl_tree t)
{
	if(t == NULL)
	{
		t = malloc(sizeof(struct avl_node));
		if(t == NULL)
			space_fatal_error();
		else
		{
			t->element = x;
			t->height = 0;
			t->left = t->right = NULL;
		}
	}
	else if(x < t->element)
	{
		t->left = insert(x, t->left);
		if(tell_height(t->left) - tell_height(t->right) == 2)
			if(x < t->left->element)
				t = single_rotate_with_left(t);
			else
				t = double_rotate_with_left(t);
	}
	else if(x > t->element)
	{
		t->right = insert(x, t->right);
		if(tell_height(t->right) - tell_height(t->left) == 2)
			if(x > t->right->element)
				t = single_rotate_with_right(t);
			else
				t = double_rotate_with_right(t);
	}
	else
		error_log("the element is already in the tree");

	t->height = (tell_height(t->left)>tell_height(t->right)) ? (tell_height(t->left)+1) : (tell_height(t->right)+1);
	return t;
}

avl_tree del(int x,avl_tree t)
{
	position temp_cell;

	if(t == NULL)
		error_log("element not found");
	else if(x < t->element)
		t->left = del(x, t->left);
	else if(x > t->element)
		t->right = del(x, t->right);
	else if(t->left && t->right)
	{
		temp_cell = find_min(t->right);
		t->element = temp_cell->element;
		t->right = del(t->element, t->right);
	}
	else
	{
		temp_cell = t;
		if(t->left == NULL)
			t = t->right;
		else if(t-> right == NULL)
			t = t->left;
		free(temp_cell);
	}
	return t;
}

int retrieve(position p)
{
	if(p == NULL)
		error_log("no this position");
	else
		return p->element;
}

position single_rotate_with_left(position k2)
{
	position k1;

	k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
		
	k2->height = (tell_height(k2->left)>tell_height(k2->right)) ? (tell_height(k2->left)+1) : (tell_height(k2->right)+1);
	k1->height = (tell_height(k1->left)>k2->height) ? (tell_height(k1->left)+1) : (k2->height+1);

	return k1; /* new root*/
	
}

position single_rotate_with_right(position k2)
{
	position k1;

	k1 = k2->right;
	k2->right = k1->left;
	k1->left = k2;
	
	k2->height = (tell_height(k2->left)>tell_height(k2->right)) ? (tell_height(k2->left)+1) : (tell_height(k2->right)+1);
	k1->height = (tell_height(k1->right)>k2->height) ? (tell_height(k1->right)+1) : (k2->height+1);

	return k1; /* new root*/
}

position double_rotate_with_left(position k3)
{
	k3->left = single_rotate_with_right(k3->left);
	return single_rotate_with_left(k3);
}

position double_rotate_with_right(position k3)
{
	k3->right = single_rotate_with_left(k3->right);
	return single_rotate_with_right(k3);
}
