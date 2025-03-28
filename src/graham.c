
/*
* Contains the functions needed to make a Graham scan
*/
#define DEBUG_MODE  1
#define ANIMATION_ON  1

#include "algorithms.h"

static GLfloat xp_g, yp_g;  // Global variables

/*
* Function that compares a and b with respect to the global minimum
*
* Returns 1 if:         Returns -1 if:
*   a      b               b      a
*    \    /                 \    /
*     \  /                   \  /
*  {xp_g,yp_g}            {xp_g,yp_g}
*
* Treats also the case when the points are aligned: Returns 1 if a is closer to {xp_g,yp_g} and -1 if converse
*
* Returns 0 if a == b
*/
int compare(const void* a, const void* b)
{
    //exactinit();
    // Appropriate casting
    GLfloat* val1 = *(GLfloat (*)[2])a;
    GLfloat* val2 = *(GLfloat (*)[2])b;

    if (val1[1] == yp_g && val2[1] == yp_g) {
        return val1[0] > val2[0] ? 1 : val1[0] == val2[0] ? 0 : -1;
    }

    GLfloat val = -orient2d((GLfloat[2]) { xp_g, yp_g }, val1, val2);

    if (val == 0) {
        if (xp_g < val2[0] && val2[0] < val1[0] || yp_g < val2[1] && val2[1] < val1[1] || xp_g > val2[0] && val2[0] > val1[0] || yp_g > val2[1] && val2[1] > val1[1])
            return -1;
        if (val1[0] == val2[0] && val1[1] == val2[1])
            return 0;
        return 1;
    }

    return val > 0 ? 1 : -1;
}

/*
* Function implementing the Graham Scan
* Arguments
*   points   : the set of points we wants to convexhullise
*   n_points : the number of points in points
*   hull_size: the size of the hull that will be created (passed by ref)
*   hull     : without any surprises... the future hull
*
*/
void graham_scan(GLfloat ref_points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2])
{

    if (n_points == 1) {
        *hull_size = 1;
        hull[0][0] = ref_points[0][0];
        hull[0][1] = ref_points[0][1];
        return;
    }

    GLfloat(*points)[2] = malloc(sizeof(points[0]) * n_points);
    for (int i = 0; i < n_points; i++) {
        points[i][0] = ref_points[i][0];
        points[i][1] = ref_points[i][1];
    }

    // Find the point with lowest y-coord
    int min_index = 0;
    yp_g = points[min_index][1];
    xp_g = points[min_index][0];
    for (int i = 1; i < n_points; i++) {
        if (points[i][1] < yp_g || points[i][1] == yp_g && points[i][0] < xp_g) {
            min_index = i;
            yp_g = points[i][1];
            xp_g = points[i][0];
        }
    }
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
    for (int i = 0; i < size(my_stack); i++) {
        int index = my_stack->items[i];
        hull[i][0] = points[index][0];
        hull[i][1] = points[index][1];
    }
    *hull_size = size(my_stack);

    delete_st(my_stack);
    free(points);
}


/*
* Function implementing AKL-Toussaint heuristic (i.e. removing all the points outside the quadrilatere defined by the extreme
* points of the set. NB: Done in place
* Arguments
*   points    : the set of points we want to Akl-Toussaintise
*   n_points  : the number of points in points
*   rem_points: the number of points that survived the terrible cut.
*
*/
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

    GLfloat point_x_min[2] = { points[x_min_idx][0],points[x_min_idx][1] };
    GLfloat point_x_max[2] = { points[x_max_idx][0],points[x_max_idx][1] };
    GLfloat point_y_min[2] = { points[y_min_idx][0],points[y_min_idx][1] };
    GLfloat point_y_max[2] = { points[y_max_idx][0],points[y_max_idx][1] };
    *rem_points = 0;
    for (int i = 0; i < n_points; i++) {
        GLfloat NW = orient2d(point_x_min, points[i], point_y_max);
        GLfloat NE = orient2d(point_y_max, points[i], point_x_max);
        GLfloat SE = orient2d(point_x_max, points[i], point_y_min);
        GLfloat SW = orient2d(point_y_min, points[i], point_x_min);

        if (((NW <= 0.0) || (NE <= 0.0) || (SW <= 0.0) || (SE <= 0.0))) {
            points[*rem_points][0] = points[i][0];
            points[*rem_points][1] = points[i][1];
            (*rem_points)++;
        }
    }
}
