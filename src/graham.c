
/*
* Contains the functions needed to make a Graham scan
*/
#define DEBUG_MODE  1
#define ANIMATION_ON  1

#include "graham.h"
#include "predicates.c"

GLfloat xp, yp;  // Global variables

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

void akl_toussaint(GLfloat points[][2], GLsizei n_points, GLsizei* rem_points)
{
    exactinit();
    // Setting vars
    int y_min_idx = 0;
    int y_max_idx = 0;
    int x_min_idx = 0;
    int x_max_idx = 0;
    GLfloat y_min = points[0][1];
    GLfloat y_max = points[0][1];
    GLfloat x_min = points[0][0];
    GLfloat x_max = points[0][0];
    // Finding the min. values
    for (int i = 1; i < n_points; i++) {
        if (points[i][0] < x_min) {
            x_min = points[i][0];
            x_min_idx = i;
        }
        if (points[i][0] > x_max) {
            x_max = points[i][0];
            x_max_idx = i;
        }
        if (points[i][1] < y_min) {
            y_min = points[i][1];
            y_min_idx = i;
        }
        if (points[i][1] > y_max) {
            y_max = points[i][1];
            y_max_idx = i;
        }
    }
    // Discard points
    *rem_points = 0;
    for (int i = 0; i < n_points; i++) {
        GLfloat NW = orient2d(points[x_min_idx], points[y_max_idx], points[i]);
        GLfloat NE = orient2d(points[y_max_idx], points[x_max_idx], points[i]);
        GLfloat SE = orient2d(points[x_max_idx], points[y_min_idx], points[i]);
        GLfloat SW = orient2d(points[y_min_idx], points[x_min_idx], points[i]);

        if (((NW >= 0.0) || (NE >= 0.0) || (SE >= 0.0) || (SW >= 0.0))) {
            points[*rem_points][0] = points[i][0];
            points[*rem_points][1] = points[i][1];
            (*rem_points)++;
        }
    }
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
