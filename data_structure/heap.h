#include"myerror.h"

#ifndef _heap_h
#define _heap_h

#define MinHSize 10
#define MinData (-99999)

struct heap_struct;
typedef struct heap_struct *heap;

heap initialize(int max_elements);
void destory(heap h);
void make_empty(heap h);
void insert(int x, heap h);
int delete_min(heap h);
int find_min(heap h);
int is_empty(heap h);
int is_full(heap h);

#endif

struct heap_struct
{
	int capacity;
	int size;
	int *elements;
};

heap initialize(int max_elements)
{
	heap h;

	if(max_elements < MinHSize)
		error_log("Heap size is too small");

	h = malloc(sizeof(struct heap_struct));
	if(h == NULL)
		space_fatal_error();

	h->elements = malloc((max_elements+1)*sizeof(int));

	if(h->elements == NULL)
		space_fatal_error();

	h->capacity = max_elements;
	h->size = 0;
	h->elements[0] = MinData;

	return h;
}

void destory(heap h)
{
	if(h != NULL)
	{
		if(h->elements != NULL)
			free(h->elements);
		free(h);
	}
}

void make_empty(heap h)
{
	while(!is_empty(h))
		delete_min(h);
}

void insert(int x, heap h)
{
	int i;

	if(is_full(h))
	{
		error_log("Heap is full");
		return;
	}

	for(i = ++h->size; h->elements[i/2] > x; i /= 2)
		h->elements[i] = h->elements[i/2];
	h->elements[i] = x;
}

int delete_min(heap h)
{
	int i, child;
	int min_element, last_element;

	if(is_empty(h))
	{
		error_log("Heap is empty");
		return h->elements[0];
	}
	min_element = h->elements[1];
	last_element = h->elements[h->size--];

	for(i = 1; i * 2 <= h->size; i = child)
	{
		child = i * 2;
		if(child != h->size && h->elements[child+1] < h->elements[child])
			child++;

		if(last_element > h->elements[child])
			h->elements[i] = h->elements[child];
		else
			break;
	}
	h->elements[i] = last_element;
	return min_element;
}

int find_min(heap h)
{
	if(h != NULL)
		return h->elements[1];
	else{
		error_log("Heap does not exist");
		return h->elements[0];
	}
}

int is_empty(heap h)
{
	if(h != NULL)
	{
		if(h->size == 0)
			return 1;
		else 
			return 0;
	}
	else{
		error_log("heap doex not exist");
		return MinData;
	}
}

int is_full(heap h)
{
	if(h != NULL)
	{
		if(h->size < h->capacity)
			return 0;
		else
			return 1;
	}
	else{
		error_log("heap doex not exist");
		return MinData;
	}
}
