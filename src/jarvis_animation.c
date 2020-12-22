#include "animations.h"

#include "algorithms.h"

static GLfloat xp_j, yp_j;  // Global variables




/*
* Function implementing the Jarvis March
* Arguments
*   points   : the set of points we wants to convexhullise
*   n_points : the number of points in points
*   hull_size: the size of the hull that will be created (passed by ref)
*   hull     : without any surprises... the future hull
*
*/
void jarvis_march_animation(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2], bov_window_t* window)
{
    exactinit();
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
    GLfloat y_min = points[min_index][1];
    for (int i = 1; i < n_points; i++) {
        if (points[i][1] < y_min) {       // Ignoring same x for now
            min_index = i;
            y_min = points[i][1];
        }
    }
    // Setting the global variables xp, yp
    xp_j = points[min_index][0];
    yp_j = points[min_index][1];

    GLfloat last[2] = { xp_j,yp_j };
    GLfloat candidate[2];
    int i = 0;
    /*
        Some drawings
    */
    bov_points_t* hullDraw = bov_points_new(hull, n_points, GL_DYNAMIC_DRAW);
    bov_points_set_color(hullDraw, COLOUR_ORANGE);
    bov_points_set_width(hullDraw, 0.01);
    bov_points_t* candidateDraw = bov_points_new(points, 2, GL_DYNAMIC_DRAW);
    bov_points_set_color(hullDraw, COLOUR_RED);
    bov_points_set_width(hullDraw, 0.01);

    do {
        hull[i][0] = last[0];
        hull[i][1] = last[1];
        bov_points_partial_update(hullDraw, )
        candidate[0] = points[0][0];
        candidate[1] = points[0][1];
        for (int j = 0; j < n_points; j++) {
            float orient = orient2d(hull[i], points[j], candidate);
            if ((candidate[0] == last[0] && candidate[1] == last[1]) || orient > 0 || orient == 0 && alignedCase(hull[i], points[j], candidate) == 1) {
                candidate[0] = points[j][0]; candidate[1] = points[j][1];
            }
        }
        i++;
        last[0] = candidate[0]; last[1] = candidate[1];
    } while (!(candidate[0] == hull[0][0] && candidate[1] == hull[0][1]) && i < n_points);

    *hull_size = i;
}


/*
* Function designed to treat the aligned cases in Jarvis March.
* Returns
*   1 if p is `between' a and b
*   0 if a == b
*  -1 otherwise (hence p is not strictly between a and b)
*/
int alignedCase(GLfloat* a, GLfloat* p, GLfloat* b) {

    if (a[0] < p[0] && p[0] < b[0] || a[1] < p[1] && p[1] < b[1] || a[0] > p[0] && p[0] > b[0] || a[1] > p[1] && p[1] > b[1])
        return 1;
    if (a[0] == b[0] && a[1] == b[1])
        return 0;

    return -1;

}
