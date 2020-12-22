#include "animations.h"

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
int compare_animation(const void* a, const void* b)
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
* Function implementing the Graham Scan with animation
* Arguments
*   points   : the set of points we wants to convexhullise
*   n_points : the number of points in points
*   hull_size: the size of the hull that will be created (passed by ref)
*   hull     : without any surprises... the future hull
*   window   : pointer to a bov window
*/
void graham_scan_animation(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2], bov_window_t* window)
{

    if (n_points == 1) {
        *hull_size = 1;
        hull[0][0] = points[0][0];
        hull[0][1] = points[0][1];
        return;
    }

    double t_prev, t_now;
    /*
        Start by drawing all the points (in black).
    */
    bov_points_t* pointsDraw = bov_points_new(points, n_points, GL_DYNAMIC_DRAW);
    bov_points_set_color(pointsDraw, COLOUR_BLACK);
    bov_points_set_width(pointsDraw, 0.01);

    t_prev = bov_window_get_time(window);
    t_now = bov_window_get_time(window);

    while (t_now - t_prev < TRANSITION_TIME) {
        bov_points_draw(window, pointsDraw, 0, n_points);
        bov_window_update(window);
        t_now = bov_window_get_time(window);
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
    /*
    Draw the points and the anchor point (in blue).
    */
    bov_points_t* anchorDraw = bov_points_new(points, 1, GL_DYNAMIC_DRAW);
    bov_points_set_color(anchorDraw, COLOUR_BLUE);
    bov_points_set_width(anchorDraw, 0.01);

    t_prev = bov_window_get_time(window);
    t_now = bov_window_get_time(window);

    while (t_now - t_prev < TRANSITION_TIME) {
        bov_points_draw(window, pointsDraw, 0, n_points);
        bov_points_draw(window, anchorDraw, 0, 1);
        bov_window_update(window);
        t_now = bov_window_get_time(window);
    }

    // Sorting
    qsort(&points[1], n_points - 1, sizeof(GLfloat[2]), compare_animation);
    // Main loop
    struct stack* my_stack = newStack(n_points);

    GLfloat (*stack_)[2] = malloc(sizeof(GLfloat[2])*n_points);
    /*
    Draw the successive points in the stack and the hull.
    */
    bov_points_t* stackDraw = bov_points_new(stack_, n_points, GL_DYNAMIC_DRAW);
    bov_points_set_color(stackDraw, COLOUR_ORANGE);
    bov_points_set_width(stackDraw, 0.01);

    for (int i = 0; i < n_points; i++) {
        while ((size(my_stack) > 1) && (orient2d(points[nextToTop(my_stack)], points[peek(my_stack)], points[i]) < 0)) {
            pop(my_stack);

            t_prev = bov_window_get_time(window);
            t_now = bov_window_get_time(window);

            while (t_now - t_prev < TRANSITION_TIME) {
                bov_points_draw(window, pointsDraw, 0, n_points);
                bov_points_draw(window, stackDraw, 0, size(my_stack));
                bov_fast_line_strip_draw(window, stackDraw, 0, size(my_stack));
                bov_window_update(window);
                t_now = bov_window_get_time(window);
            }
        }
        push(my_stack, i);
        stack_[size(my_stack)-1][0] = points[i][0];
        stack_[size(my_stack)-1][1] = points[i][1];
        bov_points_update(stackDraw, stack_, n_points);

        t_prev = bov_window_get_time(window);
        t_now = bov_window_get_time(window);

        while (t_now - t_prev < TRANSITION_TIME) {
            bov_points_draw(window, pointsDraw, 0, n_points);
            bov_points_draw(window, anchorDraw, 0, 1);
            bov_points_draw(window, stackDraw, 0, size(my_stack));
            bov_fast_line_strip_draw(window, stackDraw, 0, size(my_stack));
            bov_window_update(window);
            t_now = bov_window_get_time(window);
        }
    }
    // Retrieving the convex hull
    for (int i = 0; i < size(my_stack); i++) {
        int index = my_stack->items[i];
        hull[i][0] = points[index][0];
        hull[i][1] = points[index][1];
    }
    *hull_size = size(my_stack);

    delete_st(my_stack);

    bov_points_delete(pointsDraw);
    bov_points_delete(anchorDraw);
    bov_points_delete(stackDraw);

    free(stack_);
}
