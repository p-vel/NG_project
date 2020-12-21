/* 
* 
* Contains two structures used for Graham Scan (stack) and Chan (subHull)
* 
*/


#ifndef _MYSTRUCT_H_
#define _MYSTRUCT_H_

#include "BOV.h"
#include <stdio.h>
#include <stdlib.h>


// Data structure for stack
struct subHull
{
    int hull_size;  
    GLfloat** hull;
};

struct subHull* newSubHull(GLfloat points[][2], GLsizei n_points);
void delete_sh(struct subHull *pt);

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
