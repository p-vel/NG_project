#ifndef _UTILS_H_
#define _UTILS_H_

#include <math.h>
#include "BOV.h"


GLfloat dot(GLfloat x[2], GLfloat y[2]);
GLfloat cross(GLfloat x[2], GLfloat y[2]);
GLfloat norm(GLfloat x[2]);
void swap(int i, int j, GLfloat arr[][2]);

struct stack
{
    int maxsize;    // define max capacity of stack
    int top;
    int* items;
};
int pop(struct stack* pt);
int peek(struct stack* pt);
void push(struct stack* pt, int x);
int isFull(struct stack* pt);
int isEmpty(struct stack* pt);
int size(struct stack* pt);
int nextToTop(struct stack* pt);
struct stack* newStack(int capacity);
void delete_st(struct stack* pt);

#endif
