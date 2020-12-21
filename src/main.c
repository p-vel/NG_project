#include "inputs.h"
#include "graham.h"
#include <time.h>
#include "myScan.h"
#include "jarvis.h"
#define ANIMATION_ON 1
#define SHOW_INFO 1
#define HEURISTIC 1
#define RTANFIX 0
int main()
{
	exactinit();

#if RTANFIX
	const GLsizei nPoints = 10;
	GLfloat(*coord)[2] = malloc(sizeof(coord[0]) * nPoints);
	GLfloat(*circle)[2] = malloc(sizeof(circle[0]) * nPoints);

	int seed = (int)time(NULL); 
	// int seed = 1608477948;
	srand(seed);
	printf("seed=%d\n", seed);
	random_points(coord, nPoints);
	for (int i = 0; i < nPoints; i++) {
		if (coord[i][0] * coord[i][0] + coord[i][1] * coord[i][1] > 0.99) {
			coord[i][0] /= 2;
			coord[i][1] /= 2;
		}
	}

	circle_points(circle, nPoints);

	GLfloat(*my_hull)[2] = malloc(sizeof(my_hull[0]) * nPoints);
	int my_hull_size;

	jarvis_march(coord, nPoints, &my_hull_size, my_hull);
	for (int i = 0; i < my_hull_size; i++)
		printf("hull[%d] %5.2f; %5.2f\n", i, my_hull[i][0], my_hull[i][1]);

	// R_TAN
	GLfloat(*tanset)[2] = malloc(sizeof(tanset[0]) * nPoints * 2);
	for (int i = 0; i < nPoints; i++) {
		GLfloat p[2] = { circle[i][0],circle[i][1] };
		int theTan = r_tan(my_hull, my_hull_size, p);
		tanset[2 * i][0] = p[0];
		tanset[2 * i][1] = p[1];
		tanset[2 * i + 1][0] = my_hull[theTan][0];
		tanset[2 * i + 1][1] = my_hull[theTan][1];

	}


	bov_window_t* window = bov_window_new(800, 800, "Hello there Jackie");
	bov_window_set_color(window, (GLfloat[]) { 0.9f, 0.85f, 0.8f, 1.0f });

	bov_points_t* coordDraw = bov_points_new(coord, nPoints, GL_STATIC_DRAW);
	bov_points_t* hullDraw = bov_points_new(my_hull, my_hull_size, GL_STATIC_DRAW);
	bov_points_t* circleDraw = bov_points_new(circle, nPoints, GL_STATIC_DRAW);
	bov_points_t* tanDraw = bov_points_new(tanset, nPoints * 2, GL_STATIC_DRAW);


	bov_points_set_width(coordDraw, .01);
	bov_points_set_width(hullDraw, .01);
	bov_points_set_width(circleDraw, .01);

	bov_points_set_color(coordDraw, (GLfloat[4]) { 0.0, 0.0, 0.0, 1.0 });
	bov_points_set_color(hullDraw, (GLfloat[4]) { 1.0, 0.0, 0.0, 1.0 });
	bov_points_set_color(circleDraw, (GLfloat[4]) { 1.0, 0.0, 0.0, 1.0 });
	bov_points_set_outline_color(coordDraw, (GLfloat[4]) { 0.3, 0.12, 0.0, 0.25 });

	while (!bov_window_should_close(window)) {
		bov_points_draw(window, coordDraw, 0, nPoints);
		bov_points_draw(window, circleDraw, 0, nPoints);
		bov_points_draw(window, hullDraw, 0, my_hull_size);
		bov_fast_line_loop_draw(window, hullDraw, 0, my_hull_size);
		bov_fast_lines_draw(window, tanDraw, 0, nPoints * 2);
		bov_window_update(window);
	}

	bov_points_delete(coordDraw);
	bov_points_delete(hullDraw);
	bov_window_delete(window);
	bov_points_delete(circleDraw);
	free(coord); 
	free(circle);
	free(my_hull);
	free(tanset);
	return EXIT_SUCCESS;


#else


#if ANIMATION_ON
	bov_window_t* window = bov_window_new(800, 800, "Hello there Jackie");
	bov_window_set_color(window, (GLfloat[]){0.9f, 0.85f, 0.8f, 1.0f});
#endif

	// ##### Circle or random #####
	//GLsizei side = 4;
	const GLsizei nPoints = 5000;// side* side;
	GLfloat(*coord)[2] = malloc(sizeof(coord[0]) * nPoints);
	//	// give a bit of entropy for the seed of rand()
	//	// or it will always be the same sequence
	int seed = (int)time(NULL);
	//seed = 1608484433;
	srand(seed);
	//	// we print the seed so you can get the distribution of points back
	printf("seed=%d\n", seed);
	random_points(coord, nPoints);
	// circle_points(coord, nPoints);
	//grid(coord, side);
	
	// ##### Jackie w/ heuristic ###### 
	//GLsizei nPoints;
	//GLfloat(*coord)[2]     = scanFile("../JackieChan.txt", &nPoints);
#if HEURISTIC
	GLfloat(*coord_cut)[2] = malloc(sizeof(coord[0]) * nPoints);
	int rem_points;
#endif
	float scaling = 500.;
	for (int i = 0; i < nPoints; i++) {
		if (coord[i][0] * coord[i][0] + coord[i][1] * coord[i][1] > 0.99) {
			coord[i][0] /= 2;
			coord[i][1] /= 2;
		}
	//	coord[i][0] = coord[i][0] / scaling - 1.;
	//	coord[i][1] = coord[i][1] / scaling - 1.;
#if HEURISTIC
		coord_cut[i][0] = coord[i][0];
		coord_cut[i][1] = coord[i][1];
#endif
	}

	// Allocate hull...	
	GLfloat(*my_hull)[2] = malloc(sizeof(my_hull[0]) * nPoints);
	int my_hull_size;


	clock_t t0 = clock();
#if	HEURISTIC
  akl_toussaint(coord_cut, nPoints, &rem_points);
  chan(coord_cut, rem_points, &my_hull_size, my_hull);
  //jarvis_march(coord_cut, rem_points, &my_hull_size, my_hull);
  //graham_scan(coord_cut, rem_points, &my_hull_size, my_hull);  //ATTENTION J'AI CHANGÉ LES ARGUMENTS DE GRAHAM_SCAN (** AU LIEU DE [][2])
#else
  chan(coord, nPoints, &my_hull_size, my_hull);
  //jarvis_march(coord, nPoints, &my_hull_size, my_hull);
  //graham_scan(coord, nPoints, &my_hull_size, my_hull); //ATTENTION J'AI CHANGÉ LES ARGUMENTS DE GRAHAM_SCAN (** AU LIEU DE [][2])
#endif
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
	bov_points_t* hullDraw = bov_points_new(my_hull, my_hull_size, GL_STATIC_DRAW);
	bov_points_set_width(coordDraw, .01);
	bov_points_set_width(hullDraw, .01);
	bov_points_set_color(coordDraw, (GLfloat[4]) { 0.0, 0.0, 0.0, 1.0 });
	bov_points_set_color(hullDraw, (GLfloat[4]) { 1.0, 0.0, 0.0, 1.0 });
	bov_points_set_outline_color(coordDraw, (GLfloat[4]) { 0.3, 0.12, 0.0, 0.25 });
#if HEURISTIC
	bov_points_t* coord_cutDraw = bov_points_new(coord_cut, nPoints, GL_STATIC_DRAW);
	bov_points_set_width(coord_cutDraw, .01);
	bov_points_set_color(coord_cutDraw, (GLfloat[4]) { 0.0, 1.0, 0.0, 1.0 });
#endif

	while (!bov_window_should_close(window)) {
		bov_points_draw(window, coordDraw, 0, nPoints);
#if HEURISTIC
		bov_points_draw(window, coord_cutDraw, 0, rem_points);
#endif
		bov_points_draw(window, hullDraw, 0, my_hull_size);
		bov_fast_line_loop_draw(window, hullDraw, 0, my_hull_size);
		bov_window_update(window);
	}

	bov_points_delete(coordDraw);
	bov_points_delete(hullDraw);
#if HEURISTIC
	bov_points_delete(coord_cutDraw);
#endif
	bov_window_delete(window);
#endif
	free(coord);
#if HEURISTIC
	free(coord_cut);
#endif
	free(my_hull);
	return EXIT_SUCCESS;

#endif
}
