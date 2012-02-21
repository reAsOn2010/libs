/*
preorder(node)
  if node = null then return
  print node.value
  preorder(node.left) 
  preorder(node.right)

inorder(node)
  if node = null then return
  inorder(node.left)
  print node.value
  inorder(node.right)

postorder(node)
  if node = null then return
  postorder(node.left)
  postorder(node.right)
  print node.value;
*/


/*------place following codes in implementing file---------*/
/*------and change "tree" to certain tree------------------*/
#ifndef _tree_traversal_h
#define _tree_traversal_h

void preorder(tree t);
void inorder(tree t);
void postorder(tree t);

#endif
void preorder(tree t)
{
	if(t == NULL);
	else
	{
		printf("%d ",t->element);
		preorder(t->left);
		preorder(t->right);
	}
}

void inorder(tree t)
{
	if(t == NULL);
	else
	{
		inorder(t->left);
		printf("%d ", t->element);
		inorder(t->right);
	}
}

void postorder(tree t)
{
	if(t == NULL);
	else
	{
		postorder(t->left);
		postorder(t->right);
		printf("%d ", t->element);
	}
}
