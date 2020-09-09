#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	int nData;
	struct Node *next;
}Node;

typedef struct Stack{
	Node *top;
}Stack;

Stack* InitializeStack();
void Push(Stack* stack, int nData);
int Pop(Stack* stack);

int main(void){
	// My example
	Stack* s = InitializeStack();
	
	Push(s, 1);
	Push(s, 2);
	Push(s, 3);
	
	printf("%d\n", Pop(s)); // 3
	printf("%d\n", Pop(s)); // 2
	printf("%d", Pop(s)); // 1
	
	// free memory
	free(s->top);
	free(s);
	
	return 0;
}

Stack* InitializeStack() {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	stack->top = (Node*)malloc(sizeof(Node));
	stack->top->next = NULL;
	stack->top->nData = 0;
	return stack;
}

void Push(Stack* stack, int nData) {
	Node* temp = (Node*)malloc(sizeof(Node));
	temp->nData = nData;
	temp->next = stack->top->next;
	stack->top->next = temp;
}

int Pop(Stack* stack) {
	if(stack->top->next != NULL) {
		Node* temp = stack->top->next;
		int data = temp->nData;
		stack->top->next = temp->next;
		free(temp);
		return data;
	}
	return -1;
}
