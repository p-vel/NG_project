#ifndef _ANIMATIONS_H_
#define _ANIMATIONS_H_
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "BOV.h"
#include "utils.h"
#include "inputs.h"
#include "predicates.h"
#include "algorithms.h"

#define TRANSITION_TIME 1

#define COLOUR_BLACK (GLfloat[4]) {0.0, 0.0, 0.0, 1.0}
#define COLOUR_RED (GLfloat[4]) {1.0, 0.0, 0.0, 1.0}
#define COLOUR_GREEN (GLfloat[4]) {0.0, 1.0, 0.0, 1.0}
#define COLOUR_BLUE (GLfloat[4]) {0.0, 0.0, 1.0, 1.0}
#define COLOUR_ORANGE (GLfloat[4]) {1.0, 0.5, 0.0, 1.0}

void jarvis_march_animation(GLfloat points[][2], int n_points, bov_window_t* window);
void graham_scan_animation(GLfloat points[][2], int n_points, int* hull_size, GLfloat hull[][2], bov_window_t* window);
void chan_animation(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2], bov_window_t* window);

#endif
