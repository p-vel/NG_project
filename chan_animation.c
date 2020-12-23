#include "animations.h"
#include "algorithms.h"


// int pow_pow(int t) {
//     int temp_res = 1 << t;
//     int res = 1 << temp_res;
//     return res;
// }

/*
* Function implementing Chan's Algorithm
* Arguments
*   points   : the set of points we wants to convexhullise
*   n_points : the number of points in points
*   hull_size: the size of the hull that will be created (passed by ref)
*   hull     : without any surprises... the future hull
*
*/
void chan_animation(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2], bov_window_t* window, double scale, double speed) {
    exactinit();

    GLfloat xp, yp;

    int min_index = 0;
    yp = points[min_index][1];
    xp = points[min_index][0];
    for (int i = 1; i < n_points; i++) {
        if (points[i][1] < yp || points[i][1] == yp && points[i][0] < xp) {
            min_index = i;
            yp = points[i][1];
            xp = points[i][0];
        }
    }

    int converged = 0;
    int m = 2;  // Initial value for size of subsets
    int finalM; // Boolean
    // Allocation of extra arrays for subdivisions
    GLfloat(*partition)[2] = malloc(sizeof(partition[0]) * n_points); // points of the convex hulls
    int* start_indices = malloc(sizeof(int) * n_points);              // stores the starting index for each subset
    int* size_indices = malloc(sizeof(int) * n_points);               // stores the size of each subset (convenience)

    bov_points_t* pointsDraw = bov_points_new(points, n_points, GL_DYNAMIC_DRAW);
    bov_points_set_color(pointsDraw, COLOUR_BLACK);
    bov_points_set_width(pointsDraw, 0.01 * scale);

    bov_points_t* hullDraw = bov_points_new(points, n_points, GL_DYNAMIC_DRAW);
    bov_points_set_color(hullDraw, COLOUR_RED);
    bov_points_set_width(hullDraw, 0.05 * scale);

    int t = 1;
    // Main Loop
    do {
        m =  min(pow_pow(t), n_points);
        finalM = m == n_points ? 1 : 0;
        int nHulls = n_points % m == 0 ? n_points / m : (n_points / m) + 1;

        // Subdivision step
        int temp_hull_size;
        int hull_count = 0;

        for (int i = 0; i < nHulls; i++) {
            // Computes the hull
            graham_scan(&(points[i * m]), min(m, n_points - (i * m)), &temp_hull_size, &(partition[hull_count]));
            start_indices[i] = hull_count;
            size_indices[i] = temp_hull_size;
            hull_count += temp_hull_size;
        }

        bov_points_t** subhullDraw = malloc(sizeof(bov_points_t*) * nHulls);
        for (int i = 0; i < nHulls; i++) {
            subhullDraw[i] = bov_points_new(&(partition[start_indices[i]]), size_indices[i], GL_DYNAMIC_DRAW);
            bov_points_set_color(subhullDraw[i], (GLfloat[4]) {fabs(cos((float)i)),fabs(cos((float)i-M_PI/4.0)),fabs(sin((float)i)),1.0});
            bov_points_set_width(subhullDraw[i], 0.015 * scale);
        }
        double t_prev, t_now;
        /*
            Start by drawing all the points (in black).
        */
        t_prev = bov_window_get_time(window);
        t_now = bov_window_get_time(window);

        while (t_now - t_prev < TRANSITION_TIME * speed) {
            bov_points_draw(window, pointsDraw, 0, n_points);
            for (int i = 0; i < nHulls; i++) {
                bov_points_draw(window, subhullDraw[i], 0, size_indices[i]);
                bov_fast_line_loop_draw(window, subhullDraw[i], 0, size_indices[i]);
            }
            bov_window_update(window);
            t_now = bov_window_get_time(window);
        }

        GLfloat current[2] = { xp, yp };
        int hullOfCurrent = min_index / m;
        int t_idxOfCurrrent = 0;
        *hull_size = 0;
        for (int i = 0; i < m; i++) {
            hull[i][0] = current[0];
            hull[i][1] = current[1];

            bov_points_update(hullDraw, hull, i);

            t_prev = bov_window_get_time(window);
            t_now = bov_window_get_time(window);

            while (t_now - t_prev < TRANSITION_TIME * speed) {
                bov_points_draw(window, pointsDraw, 0, n_points);
                for (int i = 0; i < nHulls; i++) {
                    bov_points_draw(window, subhullDraw[i], 0, size_indices[i]);
                    bov_fast_line_loop_draw(window, subhullDraw[i], 0, size_indices[i]);
                }
                bov_points_draw(window, hullDraw, 0, i);
                bov_fast_line_strip_draw(window, hullDraw, 0, i);
                bov_window_update(window);
                t_now = bov_window_get_time(window);
            }

            (*hull_size)++;
            GLfloat maxTan[2] = { current[0],current[1] };
            int hullOfMaxTan, t_idxOfMaxTan;
            for (int j = 0; j < nHulls; j++) {
                int t_idx;
                if (j == hullOfCurrent) {
                    t_idx = (t_idxOfCurrrent + 1 + (size_indices[j])) % (size_indices[j]);
                }
                else {
                    t_idx = r_tan(&partition[start_indices[j]], size_indices[j], current);
                }
                GLfloat currentTan[2] = { partition[start_indices[j] + t_idx][0], partition[start_indices[j] + t_idx][1] };
                float orient = orient2d(currentTan, current, maxTan);
                if (orient < 0 || (orient == 0 && alignedCase(current, currentTan, maxTan) == 1) || maxTan[0] == current[0] && maxTan[1] == current[1]) {
                    maxTan[0] = currentTan[0];
                    maxTan[1] = currentTan[1];
                    hullOfMaxTan = j;
                    t_idxOfMaxTan = t_idx;
                }
            }
            current[0] = maxTan[0];
            current[1] = maxTan[1];
            hullOfCurrent = hullOfMaxTan;
            t_idxOfCurrrent = t_idxOfMaxTan;
            if (current[0] == xp && current[1] == yp) {
                converged = 1;
                break;
            }
        }

        for (int i = 0; i < nHulls; i++) {
            bov_points_delete(subhullDraw[i]);
        }
        free(subhullDraw);

        t += 1;

        bov_text_t* message = bov_text_new((GLubyte[]) {""}, GL_DYNAMIC_DRAW);
        bov_text_set_color(message, COLOUR_WHITE);
	   	bov_text_set_pos(message, (GLfloat[2]) {1.0, 0.9});
        GLubyte str[20];
        sprintf(str, "m = \%d\nRedividing...\n", m);
        message = bov_text_update(message, str);

        t_prev = bov_window_get_time(window);
        t_now = bov_window_get_time(window);


        while (t_now - t_prev < TRANSITION_TIME) {
            bov_points_draw(window, pointsDraw, 0, n_points);
            bov_text_draw(window, message);
            bov_window_update(window);
            t_now = bov_window_get_time(window);
        }

		bov_text_delete(message);

    } while (converged == 0 && !finalM);
    // Free the memory
    bov_points_delete(pointsDraw);
    bov_points_delete(hullDraw);
    free(partition);
    free(start_indices);
    free(size_indices);
}
