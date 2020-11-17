#ifndef _STACK_H_
#define _STACK_H_

// Data structure for stack
struct stack
{
    int maxsize;    // define max capacity of stack
    int top;
    int *items;
};
int pop(struct stack *pt);
int peek(struct stack *pt);
void push(struct stack *pt, int x);
int isFull(struct stack *pt);
int isEmpty(struct stack *pt);
int size(struct stack *pt);
int nextToTop(struct stack *pt);
struct stack* newStack(int capacity);
void delete(struct stack *pt);

#endif
