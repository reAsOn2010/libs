#include"myerror.h"

#ifndef _leftheap_h
#define _leftheap_h

struct tree_node;
typedef struct tree_node *heap;
typedef struct tree_node *position;

heap initialize(void);
int find_min(heap *h);
int is_empty(heap h);
heap merge(heap h1, heap h2);
heap merge1(heap h1, heap h2);
void swapchildren(heap h);
position find(int x, heap h);
void lazy_del(position x);

#define insert(x, h) (h = insert1((x),h))
#define delete_min(h) (h = delete_min1(h))

heap insert1(int x, heap h);
heap delete_min1(heap h);

#endif

#ifndef _stack_h
#define _stack_h

struct node;
typedef struct node *ptr_node;
typedef ptr_node stack;

int stack_is_empty(stack s);/**/
stack create_stack(void);/**/
void dispose_stack(stack s);/**/
void make_empty(stack s);/**/
void push(heap x, stack s);/**/
heap top(stack s);/**/
void pop(stack s);/**/

#endif

struct node
{
	heap element;
	ptr_node next;
};


int stack_is_empty(stack s)
{
	return s->next == NULL;
}

stack create_stack(void)
{
	stack s;
	s = malloc(sizeof(struct node));
	if(s == NULL){
		space_fatal_error();
	}
	s->element = 0;
	s->next = NULL;
	make_empty(s);
	return s;
}

void make_empty(stack s)
{
	if(s == NULL){
		error_log("Must use create_stack first");
	}
	else
		while(!stack_is_empty(s))
			pop(s);
}

void push(heap x, stack s)
{
	ptr_node temp_cell;

	temp_cell = malloc(sizeof(struct node));
	if(temp_cell == NULL){
		space_fatal_error();
	}
	else
	{
		temp_cell->element = x;
		temp_cell->next = s->next;
		s->next = temp_cell;
	}
}

heap top(stack s)
{
	if(!stack_is_empty(s))
		return s->next->element;
	error_log("Empty stack");
}

void pop(stack s)
{
	ptr_node FirstCell;

	if(stack_is_empty(s))
		error_log("Empty stack");
	else{
		FirstCell = s->next;
		s->next = s->next->next;
		free(FirstCell);
	}
}

void dispose_stack(stack s)
{
	while(!stack_is_empty(s))
		pop(s);
}
/*------Upper are functions for stack------*/

#define DELETED 0
#define EXIST 1
struct tree_node
{
	int element;/*store the number of the node*/
	heap left;/*point to it's left subtree*/
	heap right;/*point to it's left subtree*/
	int npl;/*store the Null Path Length*/
	int flag;
};

heap initialize(void)/*this function seems to be of no need*/
{
	heap h = NULL;
	return h;
}

/*------Recursive------*/
/*
heap merge1(heap h1, heap h2)
{
	if(h1->left == NULL)
		h1->left = h2;

	else
	{
		 h1->right = merge(h1->right, h2);
		 if(h1->left->npl < h1->right->npl)
			 swapchildren(h1);
		 h1->npl = h1->right->npl + 1;
	}
	return h1;
}
*/
/*------Recursive------*/

heap merge(heap h1, heap h2)/*a function before merge1*//*to simplify the condition*/
{
	if(h1 == NULL)/*the first two if deal with null points*/
		return h2;
	if(h2 == NULL)
		return h1;
	if(h1->element < h2->element)/*comfirm that the first parameter's container is always the smaller one*/
		return merge1(h1, h2);
	else
		return merge1(h2, h1);/*change the position of the two parameters*/
}
/*------Iterative------*/
heap merge1(heap h1, heap h2)
{
	stack s;/*Use a stack to record the right path*/
	position parent = h1;/*because h1->element is always the small one, it's root*/
	s = create_stack();
	push(h1, s);/*push the root into stack*/
	h1 = h1->right;/*update h1*/
	while(h1!=NULL && h2!=NULL)
	{
		if(h1->element < h2->element)
		{
			parent->right = h1;/*if h1->element is smaller, attach it to its parent*/
			push(h1, s);/*push it into stack*/
			parent = parent->right;/*update the parent*/
			h1 = h1->right;/*update h1*/
		}
		else
		{
			parent->right = h2;/*h2 is the same with h1*/
			push(h2, s);
			parent = parent->right;
			h2 = h2->right;
		}
	}
	if(h1 == NULL)/*this if is to deal with the last right path node*/
		parent->right = h2;
	else
		parent->right = h1;
	while(!stack_is_empty(s))/*this while is to update the npl along the right pash and swap children*/
	{
		parent = top(s);/*pop out the right path from bottom up*/
		pop(s);
		parent->npl = parent->right->npl + 1;/*update npl*//*the right most node must have npl == 0*/
		if(parent->left == NULL || parent->left->npl < parent->right->npl)/*decide swaping or not*/
			swapchildren(parent);
	}
	return parent;/*the last one in the stack must be the root of the leftist heap*/
}
/*------Iterative------*/

heap insert1(int x, heap h)
{
	heap single_node;
	single_node = malloc(sizeof(struct tree_node));/* malloc the space for the node*/
	if(single_node == NULL)/*deal with the situation of malloc error*/
		space_fatal_error();
	else/*initialize the node, and merge it to the former heap*/
	{
		single_node->element = x;
		single_node->left = single_node->right = NULL;
		single_node->flag = EXIST;
		single_node->npl = 0;
		h = merge(single_node, h);
	}
	return h;/*return the root of the new heap*/
}

heap delete_min1(heap x)
{
	heap left_heap, right_heap;/*to store the children of the root, because the root will be freed*/
	heap h;
	if(is_empty(x))/*deal with the null pointer*/
	{
		error_log("Heap is empty");
		return x;
	}
	left_heap = x->left;/*store the children*/
	right_heap = x->right;
	
	while(x->flag == DELETED)/*deal with the situation of the root has been deleted*/
	{
		x = merge(left_heap, right_heap);
		left_heap = x->left;/*store the children*/
		right_heap = x->right;
	}
	free(x);
	h = merge(left_heap, right_heap);/*return the new heap's root*/
	left_heap = h->left;/*store the children*/
	right_heap = h->right;
	while(h->flag == DELETED)/*deal with the situation of the root has been deleted*/
	{
		h = merge(left_heap, right_heap);
		left_heap = h->left;/*store the children*/
		right_heap = h->right;
	}
	return h;
}

int is_empty(heap h)/*check whether h is empty*/
{
	if(h == NULL)/*heap is empty means the pointer is null*/
		return 1;
	else
		return 0;
}



int find_min(heap *h)
{
	if((*h) == NULL)/*deal with the null pointer*/
	{
		error_log("Heap is empty");
		return -1;
	}
	else if((*h)->flag == DELETED)/*deal with the situation of the root has been deleted*/
	{
		(*h)->flag = EXIST;
		*h = delete_min1(*h);
		return find_min(h);/*first delete the root, then find the minimun again*/
	}
	else/*deal with the normal case*/
		return (*h)->element;/*simply return the element of the root*/
}

void swapchildren(heap h)/*swap will be used in the merge pocession*/
{
	heap temp;
	temp = h->right;
	h->right = h->left;
	h->left = temp;
}

void lazy_del(position x)/*lazy deletion of the node*//*the paremeter is the pointer of the node!!*/
{
	if(x != NULL)/*simply mark the node as deleted*/
		x->flag = DELETED;
}

position find(int x, heap h)
{
	position temp;
	if(h == NULL)
		return NULL;
	else if(h->element == x)
		return h;
	else if(temp = find(x, h->left))
		return temp;
	else
		return find(x, h->right);
}
