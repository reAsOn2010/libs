/*#include"stack_array.h"*/
/*#include"stack_link.h"*/
/*#include"avltree.h"*/
/*#include"tree.h"*/
/*#include"heap.h"*/
#include"leftheap.h"

void preorder(heap t)
{
	if(t == NULL);
	else
	{
		printf("%d ",t->element);
		preorder(t->left);
		preorder(t->right);
	}
}

void inorder(heap t)
{
	if(t == NULL);
	else
	{
		inorder(t->left);
		printf("%d ", t->element);
		inorder(t->right);
	}
}

void postorder(heap t)
{
	if(t == NULL);
	else
	{
		postorder(t->left);
		postorder(t->right);
		printf("%d ", t->element);
	}
}

int main()
{
	/*------------tree-----------
	avl_tree t = NULL;
	t = insert(2, t);
	t = insert(1, t);
	t = insert(4, t);
	t = insert(5, t);
	t = insert(9, t);
	t = insert(3, t);
	t = insert(6, t);
	t = insert(7, t);
	preorder(t);
	printf("\n");
	inorder(t);
	printf("\n");
	postorder(t);
	printf("\n");
	---------------------------*/

	/*--------avl_tree-----------
	avl_tree t = NULL;
	t = insert(2, t);
	t = insert(1, t);
	t = insert(4, t);
	t = insert(5, t);
	t = insert(9, t);
	t = insert(3, t);
	t = insert(6, t);
	t = insert(7, t);
	preorder(t);
	printf("\n");
	inorder(t);
	printf("\n");
	postorder(t);
	printf("\n");
	---------------------------*/

	/*-------heap----------------
	heap h;
	int i, size;

	h = initialize(20);
	insert(10, h);
	insert(19, h);
	insert(15, h);
	insert(5, h);
	insert(6, h);
	insert(13, h);
	insert(9, h);
	insert(2, h);
	insert(7, h);
	insert(14, h);
	size = h->size;
	for(i = 1; i <= size; i++)
		printf("%d\n",h->elements[i]);
	for(i = 1; i <= size; i++)
		printf("%d\n",delete_min(h));
	---------------------------*/

	heap h1, h2;
	h1 = initialize();
	insert(3, h1);
	insert(10, h1);
	insert(8, h1);
	insert(21, h1);
	insert(14, h1);
	insert(17, h1);
	insert(23, h1);
	insert(26, h1);
	preorder(h1);
	printf("\n");
	inorder(h1);
	printf("\n");
	postorder(h1);
	printf("\n");
	
	h2 = initialize();
	insert(6, h2);
	insert(12, h2);
	insert(7, h2);
	insert(18, h2);
	insert(24, h2);
	insert(37, h2);
	insert(19, h2);
	insert(33, h2);
	preorder(h2);
	printf("\n");
	inorder(h2);
	printf("\n");
	postorder(h2);
	printf("\n");

	lazy_del(find(6, h2));
	lazy_del(find(7, h2));
	lazy_del(find(18, h2));
	printf("%d\n",find_min(&h2));
	preorder(h2);
	printf("\n");
	inorder(h2);
	printf("\n");
	postorder(h2);
	printf("\n");
	lazy_del(find(12, h2));
	printf("%d\n",find_min(&h2));
	preorder(h2);
	printf("\n");
	inorder(h2);
	printf("\n");
	postorder(h2);
	printf("\n");

	h1 = merge(h1, h2);
	preorder(h1);
	printf("\n");
	inorder(h1);
	printf("\n");
	postorder(h1);
	printf("\n");

	lazy_del(h1);
	preorder(h1);
	printf("\n");
	inorder(h1);
	printf("\n");
	postorder(h1);
	printf("\n");

	delete_min(h1);
	preorder(h1);
	printf("\n");
	inorder(h1);
	printf("\n");
	postorder(h1);
	printf("\n");


	return 0;
}
