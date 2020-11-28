
/*
* Contains the functions needed to make a Graham scan
*/

#ifndef _CHAN_H_
#define _CHAN_H_

#include "graham.h"
#include "jarvis.h"
#include "subHull.h"
#include <math.h>

void chan(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2]);

#endif
