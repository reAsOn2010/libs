#include"myerror.h"

#ifndef _stack_h
#define _stack_h

struct stack_record;
typedef struct stack_record *stack;

int is_empty(stack s);/**/
int is_full(stack s);/**/
stack create_stack(int max_elements);/**/
void dispose_stack(stack s);/**/
void make_empty(stack s);/**/
void push(int x, stack s);/**/
int top(stack s);/**/
void pop(stack s);/**/
int top_pop(stack s);/**/

#endif 

#define EmptyTOS (-1)
#define MinStackSize (5)


struct stack_record
{
	int capacity;
	int top_of_stack;
	int *array;
};

stack create_stack(int max_elements)
{
	stack s;
	if(max_elements < MinStackSize)
		error_log("Stack size is too small");

	s = malloc(sizeof(struct stack_record));
	if(s == NULL)
		space_fatal_error();

	s->array = malloc(sizeof(int) * max_elements);
	if(s->array == NULL)
		space_fatal_error();

	s->capacity = max_elements;
	make_empty(s);

	return s;
}

void dispose_stack(stack s)
{
	if(s != NULL){
		free(s->array);
		free(s);
	}
}

int is_empty(stack s)
{
	return s->top_of_stack == EmptyTOS;
}

int is_full(stack s)
{
	return s->top_of_stack == s->capacity - 1;
}

void make_empty(stack s)
{
	s->top_of_stack = EmptyTOS;
}

void push(int x, stack s)
{
	if(is_full(s))
		error_log("Full Stack");
	else
		s->array[++s->top_of_stack] = x;
}

int top(stack s)
{
	if(!is_full(s))
		return s->array[s->top_of_stack];
	error_log("Empty stack");
}

void pop(stack s)
{
	if(is_empty(s))
		error_log("Empty stack");
	else
		s->top_of_stack--;
}

int top_pop(stack s)
{
	if(!is_empty(s))
		return s->array[s->top_of_stack--];
	error_log("Empty stack");
}
