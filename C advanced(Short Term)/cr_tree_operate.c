#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct node{
	char op[10];
	int op_pri;
	struct node *lchild;
	struct node *rchild;
};
struct node *root;
struct node *operand;
struct node *operat;
static char formula[100];

int opt_ri(char op);
void TraversalTree_DLR(struct node *t);
void TraversalTree_LDR(struct node *t);
void TraversalTree_LRD(struct node *t);
struct node *ini_node();
struct node *func(struct node *subtrees[]);
struct node *rm_bracket(char *str);
void get_value();
double operate(char *r_polish[],double value[]);
double *cr_heap();
void del_tree(struct node *t);

char *r_polish[100];
static double value[100];
int static r_index;
int static stack_index;
double *heap_top;

int main(){
	printf("Please input the formula:\n");
	gets(formula);		/*---store the formula---*/
	root = rm_bracket(formula);	/*---get the root of the tree---*/
	TraversalTree_LRD(root);	/*---restore the formula in Post-order traversal---*/
	printf("\n");
	get_value();	/*---get the value of variables---*/
	heap_top = cr_heap();	/*---heap opeartions---*/
	printf("ans = %lf\n",operate(r_polish,value));
	free(heap_top);
	del_tree(root);
	return 0;
}

/*---return the Priority of each operator---*/
int opt_pri(char op){
	if(op >= 'a' && op <= 'z')
		return 3;
	else
		switch(op){
		case '+':
		case '-':return 1;
		case '*':
		case '/':return 2;
		default :return 10;
	}
}

/*---tree traversals---*/
void TraversalTree_DLR(struct node *root) { 
	if (root != NULL) { 
		printf("%s ", root -> op); 
		TraversalTree_DLR ( root -> lchild ) ; 
		TraversalTree_DLR ( root -> rchild ) ; 
	} 
} 
void TraversalTree_LDR(struct node *root) { 
	if (root != NULL) { 
		TraversalTree_LDR ( root -> lchild ) ; 
		printf("%s ", root -> op); 
		TraversalTree_LDR ( root -> rchild ) ; 
	}
} 
void TraversalTree_LRD(struct node *root) { 
	if (root != NULL) { 
		TraversalTree_LRD ( root -> lchild ) ; 
		TraversalTree_LRD ( root -> rchild ) ; 
		printf("%s ", root -> op); 
		r_polish[r_index++] = root ->op;
	} 
}

/*---create a new tree node---*/
struct node *ini_node(){
	struct node *temp;
	temp = malloc(sizeof(struct node));
	temp->lchild = NULL;
	temp->rchild = NULL;
	return temp;
}

/*---read a set of pointers to struct node, and create a subtree, return its root---*/
struct node *func(struct node *subtrees[]){
	struct node *root;
	char oldopt = '^';
	struct node *current;
	int i;
	
	root = subtrees[0];
	for(i = 1; subtrees[i] != NULL;){
		if(root->op_pri >= subtrees[i]->op_pri){
			current = subtrees[i++];
			current->lchild = root;
			current->rchild = subtrees[i++];
			root = current;
		}else{
			current = subtrees[i++];
			current->lchild = root->rchild;
			current->rchild = subtrees[i++];
			root->rchild = current;
		}
	}
	return root;
}

/*---remove the backet by Recursive method, and form the whole tree, return its root---*/
struct node *rm_bracket(char *str){
	int i, j, k, count, index;
	struct node *root;
	struct node *subtrees[100];
	char temp[100];
	for(i = 0; i < 100; i++)
		subtrees[i] = NULL;
	for(i = 0, j = 0; str[i] != 0; i++){
		if((str[i] >= 'a' && str[i] <= 'z')||(str[i] >= '0' && str[i] <= '9')){
                	root = ini_node();
			for(k = i; !(str[k]=='+'||str[k]=='-'||str[k]=='*'||str[k]=='/'||str[k]==')'||str[k]==0); k++);
			for(index = 0; index < k - i; index++)
				root->op[index] = str[i + index];
			root->op[index] = '\0';
			root->op_pri = opt_pri(str[i]);
			subtrees[j++] = root;
			i = k - 1;
		}
		else if(str[i] == '+' ||str[i] == '-' || str[i] == '*' || str[i] == '/'){
			root = ini_node();
			root->op[0] = str[i];
			root->op[1] = '\0';
			root->op_pri = opt_pri(str[i]);
			subtrees[j++] = root;
		}else if(str[i] == '('){
			count = 0;
			for(k = i + 1; count != -1; k++){
				if(str[k] == '(')
					count++;
				else if(str[k] == ')')
					count--;
			}
			for(index = 0; index < k - i - 1;  index++)
				temp[index] = str[i + index + 1];
			root = rm_bracket(temp);
			root->op_pri = 3;
			subtrees[j++] = root;
			i = k - 1;
		}else break;
	}
	return func(subtrees);
}

/*---give the value of all the constants and variables---*/
void get_value(){
	int i;
	for(i = 0; i < 100 && r_polish[i] != NULL; i++)
		if(*r_polish[i] >= 'a' && *r_polish[i] <= 'z'){
			printf("%s = ",r_polish[i]);
			scanf("%lf",&value[i]);
		}else if(*r_polish[i] >= '0' && *r_polish[i] <= '9'){
			value[i] = atof(r_polish[i]);
		}else{
			value[i] = -1;
		}
	for(i = 0; i < 100 && r_polish[i] != NULL; i++){
		if(value[i] == -1)
			printf("%c ", *r_polish[i]);
		else
			printf("%lf ", value[i]);
	}
	printf("\n");
}

/*---create a heap---*/
double *cr_heap(){
	double *heap;
	heap = malloc(sizeof(double)*100);
	return heap;
}

/*---heap operation---*/
void push(double value){
	*(heap_top++) = value;
}

/*---heap operation---*/
double pop(){
	return *(--heap_top);
}

/*---operate the formula, return the final answer---*/
double operate(char *r_polish[],double value[]){
	int i;
	for(i = 0; r_polish[i] != NULL; i++){
		if(value[i] != -1)
			push(value[i]);
		else{
			switch(*r_polish[i]){
				case'+':push(pop()+pop());break;
				case'-':push(-(pop()-pop()));break;
				case'*':push(pop()*pop());break;
				case'/':push(1/(pop()/pop()));break;
				default:break;
			}
		
		}
	}
	return *(--heap_top);
}

/*---delete the whole tree---*/
void del_tree(struct node *t){
	if(t != NULL){
		del_tree(t->lchild);
		del_tree(t->rchild);
		free(t);
	}
}
