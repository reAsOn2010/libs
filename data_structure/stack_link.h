#include"myerror.h"

#ifndef _stack_h
#define _stakc_h

struct node;
typedef struct node *ptr_node;
typedef ptr_node stack;

int is_empty(stack s);/**/
stack create_stack(void);/**/
void dispose_stack(stack s);/**/
void make_empty(stack s);/**/
void push(int X, stack s);/**/
int top(stack s);/**/
void pop(stack s);/**/

#endif

struct node
{
	int element;
	ptr_node next;
};


int is_empty(stack s)
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
		while(!is_empty(s))
			pop(s);
}

void push(int X, stack s)
{
	ptr_node temp_cell;

	temp_cell = malloc(sizeof(struct node));
	if(temp_cell == NULL){
		space_fatal_error();
	}
	else
	{
		temp_cell->element = X;
		temp_cell->next = s->next;
		s->next = temp_cell;
	}
}

int top(stack s)
{
	if(!is_empty(s))
		return s->next->element;
	error_log("Empty stack");
}

void pop(stack s)
{
	ptr_node FirstCell;

	if(is_empty(s))
		error_log("Empty stack");
	else{
		FirstCell = s->next;
		s->next = s->next->next;
		free(FirstCell);
	}
}

void dispose_stack(stack s)
{
	while(!is_empty(s))
		pop(s);
}
	
