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

#define TRANSITION_TIME 0.5

#define COLOUR_BLACK (GLfloat[4]) {0.0, 0.0, 0.0, 1.0}
#define COLOUR_RED (GLfloat[4]) {1.0, 0.0, 0.0, 1.0}
#define COLOUR_GREEN (GLfloat[4]) {0.0, 1.0, 0.0, 1.0}
#define COLOUR_BLUE (GLfloat[4]) {0.0, 0.0, 1.0, 1.0}
#define COLOUR_ORANGE (GLfloat[4]) {1.0, 0.5, 0.0, 1.0}
#define COLOUR_MIDNIGHT_BLUE (GLfloat[4]) {52.0/255.0, 73.0/255.0, 94.0/255.0, 1.0}
#define COLOUR_GREEN_STYLE (GLfloat[4]) {46./255., 204./255., 113./255., 1.0}
#define COLOUR_WHITE (GLfloat[4]) {1.0, 1.0, 1.0, 1.0}
#define COLOUR_DARK_RED (GLfloat[4]) {192./255., 57./255., 43./255., 1.0}

void graham_scan_animation(GLfloat points[][2], int n_points, int* hull_size, GLfloat hull[][2], bov_window_t* window, double scale, double speed);
void chan_animation(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2], bov_window_t* window, double scale, double speed);

#endif
