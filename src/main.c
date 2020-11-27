#include "inputs.h"
#include "graham.h"
#include <time.h>
#include "myScan.h"
#include "jarvis.h"
#define ANIMATION_ON 0
#define SHOW_INFO 0
int main()
{
	exactinit();
	//printf("%f", orient2d((GLfloat[2]) { 0., 0. }, (GLfloat[2]) { 0., 1. }, (GLfloat[2]) { 1., 1. }));
	printf("%d", r_tan((GLfloat[][2]) { { 0., 0. }, { 0., 1. },{ 1., 1. } },
						3, 
						(GLfloat[2]) { -1., 0. }));

#if ANIMATION_ON
	bov_window_t* window = bov_window_new(800, 800, "Hello there Jackie");
	bov_window_set_color(window, (GLfloat[]){0.9f, 0.85f, 0.8f, 1.0f});
#endif
	
	// ##### Jackie w/ heuristic ###### 
	GLsizei nPoints;
	GLfloat(*coord)[2]     = scanFile("../JackieChan.txt", &nPoints);
	GLfloat(*coord_cut)[2] = malloc(sizeof(coord[0]) * nPoints);
	int rem_points;
	// random_points(coord, nPoints);
	float scaling = 500.;
	for (int i = 0; i < nPoints; i++) {
		coord[i][0] = coord[i][0] / scaling - 1.;
		coord[i][1] = coord[i][1] / scaling - 1.;
		coord_cut[i][0] = coord[i][0];
		coord_cut[i][1] = coord[i][1];
	}

	// ##### Circle or random #####
	// const GLsizei nPoints = 28194;
	// GLfloat (*coord)[2] = malloc(sizeof(coord[0])*nPoints);
	//	// give a bit of entropy for the seed of rand()
	//	// or it will always be the same sequence
	// int seed = (int)time(NULL);
	// srand(seed);
	//	// we print the seed so you can get the distribution of points back
	// printf("seed=%d\n", seed);
	// random_points(coord, nPoints);
	// circle_points(coord, nPoints);

	// Allocate hull...	
	GLfloat(*my_hull)[2] = malloc(sizeof(my_hull[0]) * nPoints);
	int my_hull_size;


	clock_t t0 = clock();
  akl_toussaint(coord_cut, nPoints, &rem_points);
  jarvis_march(coord_cut, rem_points, &my_hull_size, my_hull);
  //jarvis_march(coord, nPoints, &my_hull_size, my_hull);
  // graham_scan(coord_cut, rem_points, &my_hull_size, my_hull);
  // graham_scan(coord, nPoints, &my_hull_size, my_hull);
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
	bov_points_t* coordDraw = bov_points_new(coord, nPoints, GL_STATIC_DRAW);
	bov_points_t* coord_cutDraw = bov_points_new(coord_cut, nPoints, GL_STATIC_DRAW);
	bov_points_t* hullDraw = bov_points_new(my_hull, my_hull_size, GL_STATIC_DRAW);
	bov_points_set_width(coordDraw, .001);
	bov_points_set_width(coord_cutDraw, .001);
	bov_points_set_width(hullDraw, .01);
	bov_points_set_color(coordDraw, (GLfloat[4]) { 0.0, 0.0, 0.0, 1.0 });
	bov_points_set_color(coord_cutDraw, (GLfloat[4]) { 0.0, 1.0, 0.0, 1.0 });
	bov_points_set_color(hullDraw, (GLfloat[4]) { 1.0, 0.0, 0.0, 1.0 });
	bov_points_set_outline_color(coordDraw, (GLfloat[4]) { 0.3, 0.12, 0.0, 0.25 });

	while (!bov_window_should_close(window)) {
		bov_points_draw(window, coordDraw, 0, nPoints);
		bov_points_draw(window, coord_cutDraw, 0, rem_points);
		bov_points_draw(window, hullDraw, 0, my_hull_size);
		bov_fast_line_loop_draw(window, hullDraw, 0, my_hull_size);
		bov_window_update(window);
	}

	bov_points_delete(coordDraw);
	bov_points_delete(hullDraw);
	bov_points_delete(coord_cutDraw);
	bov_window_delete(window);
#endif
	free(coord);
	free(coord_cut);
	free(my_hull);
	return EXIT_SUCCESS;
}
