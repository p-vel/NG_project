
/*
* Contains the functions needed to make a Graham scan
*/

#ifndef _JARVIS_H_
#define _JARVIS_H_

#include "BOV.h"
#include <math.h>
#include <stdlib.h>
#include "utils.h"
#include <time.h>
#include "inputs.h"
#include "predicates.h"

void jarvis_march(GLfloat points[][2], GLsizei n_points, int *hull_size, GLfloat hull[][2]);

#endif
