#include "inputs.h"
#include "graham.h"
#include <time.h>
#include "myScan.h"
#define ANIMATION_ON 1
#define SHOW_INFO 1
int main()
{
	// give a bit of entropy for the seed of rand()
	// or it will always be the same sequence
	int seed = (int) time(NULL);
	srand(seed);

	// we print the seed so you can get the distribution of points back
	printf("seed=%d\n", seed);
#if ANIMATION_ON
	bov_window_t* window = bov_window_new(800, 800, "Hello there");
	bov_window_set_color(window, (GLfloat[]){0.9f, 0.85f, 0.8f, 1.0f});
#endif

	
	const GLsizei nPoints = 28194;
	GLfloat (*coord)[2] = malloc(sizeof(coord[0])*nPoints);
	GLfloat (*my_hull)[2] = malloc(sizeof(my_hull[0])*nPoints);
	int my_hull_size;
	// random_points(coord, nPoints);
	//circle_points(coord, nPoints);
	coord = scanFile("../JackieChan.txt");
	for (int i = 0; i < nPoints; i++) {
		coord[i][0] = coord[i][0] / 500. - 1.;
		coord[i][1] = coord[i][1] / 500. - 1;
	}

	clock_t t0 = clock();
  graham_scan(coord, nPoints, &my_hull_size, my_hull);
	clock_t t1 = clock();
	double elapsed_time = (double)(t1 - t0)/CLOCKS_PER_SEC;

#if SHOW_INFO
	printf("#################################################\n");
	printf("###              GRAHAM's SCAN                ###\n");
	printf("\n");
	printf("The number of points: %d\n", nPoints);
	printf("The convex hull consists of: %d points\n", my_hull_size);
	printf("The elapsed time is %.6f seconds.\n", elapsed_time);
	printf("#################################################\n");
#endif
    // Initialized points object
#if ANIMATION_ON
	bov_points_t *coordDraw = bov_points_new(coord, nPoints, GL_STATIC_DRAW);
	bov_points_t *hullDraw = bov_points_new(my_hull, my_hull_size, GL_STATIC_DRAW);
	bov_points_set_width(coordDraw, .002);
	bov_points_set_color(coordDraw, (GLfloat[4]) {0.0, 0.0, 0.0, 1.0});
	bov_points_set_color(hullDraw, (GLfloat[4]) {1.0, 0.0, 0.0, 1.0});
	bov_points_set_outline_color(coordDraw, (GLfloat[4]) {0.3, 0.12, 0.0, 0.25});

	while(!bov_window_should_close(window)){
		bov_points_draw(window, coordDraw, 0, nPoints);
		//bov_points_draw(window, hullDraw, 0, my_hull_size);
		//bov_fast_line_loop_draw(window, hullDraw, 0, my_hull_size);
		bov_window_update(window);
	}

	bov_points_delete(coordDraw);
	bov_points_delete(hullDraw);
	bov_window_delete(window);
#endif
	free(coord);
	free(my_hull);
	return EXIT_SUCCESS;
}
