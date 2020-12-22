#include "inputs.h"
#include "algorithms.h"
#include "animations.h"
#include <time.h>

#define ANIMATION_ON 1
#define SHOW_INFO 1
#define HEURISTIC 0

int main(int argc, char* argv[])
{
    int seed = (int)time(NULL);
    srand(seed);
    exactinit();
    // Text mode
    if (argc == 2) {
        // Command line arguments
        int n_points = atoi(argv[1]);

        GLfloat(*coord)[2] = malloc(sizeof(GLfloat[2]) * n_points);
        GLfloat(*hull)[2] = malloc(sizeof(GLfloat[2]) * n_points);
        int hull_size;
        int rem_size;

        clock_t t0, t1;
        double elapsed_time;

        random_points(coord, n_points);
        // circle_points(coord, n_points);

        t0 = clock();
        jarvis_march(coord, n_points, &hull_size, hull);
        t1 = clock();
        elapsed_time = (double)(t1 - t0)/CLOCKS_PER_SEC;
        printf("#################################################\n");
        printf("###              JARVIS'S MARCH               ###\n");
        printf("\n");
        printf("The number of points: %d\n", n_points);
        printf("The convex hull consists of: %d points\n", hull_size);
        printf("The elapsed time is %.6f seconds.\n", elapsed_time);
        printf("#################################################\n");

        t0 = clock();
        akl_toussaint(coord, n_points, &rem_size);
        graham_scan(coord, rem_size, &hull_size, hull);
        t1 = clock();
        elapsed_time = (double)(t1 - t0)/CLOCKS_PER_SEC;
        printf("#################################################\n");
        printf("###              GRAHAM SCAN                  ###\n");
        printf("\n");
        printf("The number of points: %d\n", n_points);
        printf("The convex hull consists of: %d points\n", hull_size);
        printf("The elapsed time is %.6f seconds.\n", elapsed_time);
        printf("#################################################\n");

        t0 = clock();
        akl_toussaint(coord, n_points, &rem_size);
        chan(coord, rem_size, &hull_size, hull);
        t1 = clock();
        elapsed_time = (double)(t1 - t0)/CLOCKS_PER_SEC;
        printf("#################################################\n");
        printf("###              CHAN'S ALGORITHM             ###\n");
        printf("\n");
        printf("The number of points: %d\n", n_points);
        printf("The convex hull consists of: %d points\n", hull_size);
        printf("The elapsed time is %.6f seconds.\n", elapsed_time);
        printf("#################################################\n");

        free(coord);
        free(hull);
    }

    else {

        bov_window_t* window = bov_window_new(800, 800, "Hello there Jackie");
        bov_window_set_color(window, (GLfloat[]){0.9f, 0.85f, 0.8f, 1.0f});

        // ##### Jackie w/ heuristic ######
        int n_points = 100;
        GLfloat(*coord)[2] = malloc(sizeof(GLfloat[2]) * n_points);
        GLfloat(*hull)[2] = malloc(sizeof(GLfloat[2]) * n_points);
        int hull_size;

        random_points(coord, n_points);

        // graham_scan_animation(coord, n_points, &hull_size, hull, window);
        chan_animation(coord, n_points, &hull_size, hull, window);

        bov_points_t* pointsDraw = bov_points_new(coord, n_points, GL_DYNAMIC_DRAW);
        bov_points_t* hullDraw = bov_points_new(hull, hull_size, GL_DYNAMIC_DRAW);
        bov_points_set_color(pointsDraw, (GLfloat[4]) {0.0, 0.0, 0.0, 1.0});
        bov_points_set_width(pointsDraw, 0.01);
        bov_points_set_color(hullDraw, (GLfloat[4]) {1.0, 0.0, 0.0, 1.0});
        bov_points_set_width(hullDraw, 0.01);

        while (!bov_window_should_close(window) && glfwGetKey(window->self,GLFW_KEY_ESCAPE) != GLFW_PRESS) {
            bov_points_draw(window, pointsDraw, 0, n_points);
            bov_points_draw(window, hullDraw, 0, hull_size);
            bov_fast_line_loop_draw(window, hullDraw, 0, hull_size);
            bov_window_update(window);
        }

        free(coord);
        free(hull);
    }
}
