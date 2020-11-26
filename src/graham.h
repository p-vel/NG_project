
/*
* Contains the functions needed to make a Graham scan
*/

#ifndef _GRAHAM_H_
#define _GRAHAM_H_
#include "BOV.h"
#include <math.h>
#include <stdlib.h>
#include "stack.h"
#include "utils.h"
#include <time.h>
#include "inputs.h"
#include "predicates.h"

//GLfloat* grahamScan();
void graham_scan(GLfloat points[][2], GLsizei n_points, int *hull_size, GLfloat hull[][2]);
int compare(const void *a, const void *b);
void akl_toussaint(GLfloat points[][2], GLsizei n_points, GLsizei* rem_points);

#endif
