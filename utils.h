#ifndef _UTILS_H_
#define _UTILS_H_

#include <math.h>
#include "BOV.h"

GLfloat dot(GLfloat x[2], GLfloat y[2]);
GLfloat cross(GLfloat x[2], GLfloat y[2]);
GLfloat norm(GLfloat x[2]);
void swap(int i, int j, GLfloat arr[][2]);

#endif
