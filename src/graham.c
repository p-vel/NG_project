
/*
* Contains the functions needed to make a Graham scan
*/
#define DEBUG_MODE  1
#define ANIMATION_ON  1

#include "graham.h"
#include "predicates.c"

GLfloat xp, yp;  // Global variables

/*
int orientation(GLfloat p[2], GLfloat q[2], GLfloat r[2])
{
    GLfloat val = -orient2d(p, q, r);
    return val == 0 ? 0 : val > 0 ? 1 : 2;
    if (val == 0.0) {
        return 0;
    }
    else if (val > 0.0) {
        return 1;
    }
    else {
        return 2;
    }
}*/

int compare(const void* a, const void* b)
{
    exactinit();
    // Appropriate casting
    GLfloat(*val1)[2] = (GLfloat(*)[2])a;
    GLfloat(*val2)[2] = (GLfloat(*)[2])b;
    // Get the values
    GLfloat P[2] = { xp, yp };
    //
    GLfloat val = -orient2d(P, *val1, *val2);
    return val == 0 ? 0 : val > 0 ? 1 : -1;
}

void graham_scan(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2])
{
    exactinit();
    // Find the point with lowest y-coord
    int min_index = 0;
    GLfloat y_min = points[min_index][1];
    for (int i = 1; i < n_points; i++) {
        if (points[i][1] < y_min) {       // Ignoring same x for now
            min_index = i;
            y_min = points[i][1];
        }
    }
    // Setting the global variables xp, yp
    xp = points[min_index][0];
    yp = points[min_index][1];
    // Put this point at the beginning of the array
    swap(0, min_index, points);
    // Sorting
    qsort(&points[1], n_points - 1, sizeof(GLfloat[2]), compare);
    // Main loop
    struct stack* my_stack = newStack(n_points);
    for (int i = 0; i < n_points; i++) {
        while ((size(my_stack) > 1) && (orient2d(points[nextToTop(my_stack)], points[peek(my_stack)], points[i]) <= 0)) {
            pop(my_stack);
        }
        push(my_stack, i);
    }
    // Retrieving the convex hull
    int count = 0;
    while (!isEmpty(my_stack)) {
        int index = peek(my_stack);
        pop(my_stack);
        GLfloat x_hull = points[index][0];
        GLfloat y_hull = points[index][1];
        hull[count][0] = x_hull;
        hull[count][1] = y_hull;
        count++;
    }
    *hull_size = count;
    delete(my_stack);
}
