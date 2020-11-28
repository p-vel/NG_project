#ifndef _SUBHULL_H_
#define _SUBHULL_H_

#include "graham.h"


// Data structure for stack
struct subHull
{
    int hull_size;  
    GLfloat** hull;
};

struct subHull* newSubHull(GLfloat points[][2], GLsizei n_points);
void delete_sh(struct subHull *pt);

#endif
