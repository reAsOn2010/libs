#include"myerror.h"

#ifndef _tree_h
#define _tree_h

struct tree_node;/*declare the struct */
typedef struct tree_node *position;/*typedef the struct for simplifiction*/
typedef struct tree_node *search_tree;

search_tree make_empty(search_tree t);/*make the tree empty*/
position find(double x, search_tree t);/*find the element, return the pointer*/
position find_min(search_tree t);/*find the smallest element*/
position find_max(search_tree t);/*find the largest element*/
search_tree insert(double x, search_tree t);/*insert an element into the tree, return the new tree's root*/
search_tree del(double x, search_tree t);/*find and delete the element*/
double retrieve(position p);/*give the pointer, return the element*/

#endif /*_tree_h*/

struct tree_node
{
	double element;/*store the containing element*/
	search_tree left;/*point to leftchild*/
	search_tree right;/*point to rightchild*/
};

search_tree make_empty(search_tree t)/*recursively delete all the node*/
{
	if(t != NULL)
	{
		make_empty(t->left);/*delete left child*/
		make_empty(t->right);/*delete right child*/
		free(t);/*free the root*/
	}
	return NULL;
}

position find(double x, search_tree t)
{
	if(t == NULL)
		return NULL;
	if(x < t->element)/*if x is smaller*/
		return find(x, t->left);/*search the left subtree*/
	else if(x > t->element)/*if x is larger*/
		return find(x, t->right);/*search the right subtree*/
	else
		return t;
}

position find_min(search_tree t)
{
	if(t == NULL)
		error_log("tree is empty");
	else if(t != NULL)/*find the leftmost element*/
		while(t->left != NULL)/*the leftmost element is must be the smallest one in the tree*/
			t = t->left;
	return t;
}

position find_max(search_tree t)
{
	if(t == NULL)
		error_log("tree is empty");
	else if(t != NULL)/*find the rightmost element*/
		while(t->right != NULL)/*the rightmost element is must be the largest one in the tree*/
			t = t->right;
	return t;
}

search_tree insert(double x, search_tree t)
{
	if(t == NULL)/*if the tree do not exist*/
	{
		t = malloc(sizeof(struct tree_node));/*create a new node and let it be the root*/
		if(t == NULL)/*malloc failure*/
			space_fatal_error();
		else
		{
			t->element = x;/*store the element*/
			t->left = t->right = NULL;/*initialize the left and right child*/
		}
	}
	else if(x <= t->element)/*if the new element is smaller than root*/
		t->left = insert(x, t->left);/*insert it to the left subtree*/
	else if(x > t->element)/*if the new element is bigger than root*/
		t->right = insert(x, t->right);/*insert it to the right subtree*/
/*	else
		error_log("the number is already in the tree");
*/
	return t;
}

search_tree del(double x, search_tree t)
{
	position temp_cell;

	if(t == NULL)
		error_log("element not found");
	else if(x < t->element)/*if x is smaller than the root*/
		t->left = del(x, t->left);/*find the element in the left subtree and delete it*/
	else if(x > t->element)/*if x is larger than the root*/
		t->right = del(x, t->right);/*find the element in the right subtree and delete it*/
	else if(t->left && t->right)/*the operations for deleting the inner node*/
	{
		temp_cell = find_min(t->right);
		t->element = temp_cell->element;
		t->right = del(t->element, t->right);
	}
	else/*delete the node with only one child or no*/
	{
		temp_cell = t;
		if(t->left == NULL)
			t = t->right;
		else if(t-> right == NULL)
			t = t->left;
		free(temp_cell);
	}
	return t;/*return the new tree*/
}

double retrieve(position p)
{
	if(p == NULL)
		error_log("no this position");
	else
		return p->element;/*return the element of the exact position*/
}
