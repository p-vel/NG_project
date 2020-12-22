
/*
* Header file of the algorithms
*/

#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "BOV.h"
#include "utils.h"
#include "inputs.h"
#include "predicates.h"




// graham.c
void graham_scan(GLfloat points[][2], GLsizei n_points, int *hull_size, GLfloat hull[][2]);
int compare(const void *a, const void *b);
void akl_toussaint(GLfloat points[][2], GLsizei n_points, GLsizei* rem_points);

// jarvis.c
int alignedCase(GLfloat* a, GLfloat* p, GLfloat* b);
void jarvis_march(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2]);

// chan.c
void chan(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2]);
GLsizei r_tan(GLfloat hull[][2], GLsizei hull_size, GLfloat p[2]);
int pow_pow(int t);
#define min(a, b) (((a) < (b)) ? (a) : (b))


#endif
