#include "inputs.h"
#include "graham.h"
#include <time.h>


int main()
{
	exactinit();
	double a[2] = { 2.,2. };
	double b[2] = { 12.,12. };
	double c[2] = { 24.,24. };

	double val = orient2d(a, b, c);
	printf("main_orientation=%f\n", val);
	// give a bit of entropy for the seed of rand()
	// or it will always be the same sequence
	int seed = (int) time(NULL);
	srand(seed);

	// we print the seed so you can get the distribution of points back
	printf("seed=%d\n", seed);

	bov_window_t* window = bov_window_new(800, 800, "Hello there");
	bov_window_set_color(window, (GLfloat[]){0.9f, 0.85f, 0.8f, 1.0f});

	const GLsizei nPoints = 10;
	GLfloat (*coord)[2] = malloc(sizeof(coord[0])*nPoints);
	GLfloat (*my_hull)[2] = malloc(sizeof(my_hull[0])*nPoints);
	int my_hull_size;
	random_points(coord, nPoints);

  graham_scan(coord, nPoints, &my_hull_size, my_hull);
    // Initialized points object
	bov_points_t *coordDraw = bov_points_new(coord, nPoints, GL_STATIC_DRAW);
	bov_points_t *hullDraw = bov_points_new(my_hull, my_hull_size, GL_STATIC_DRAW);
	bov_points_set_color(coordDraw, (GLfloat[4]) {0.0, 0.0, 0.0, 1.0});
	bov_points_set_color(hullDraw, (GLfloat[4]) {1.0, 0.0, 0.0, 1.0});
	bov_points_set_outline_color(coordDraw, (GLfloat[4]) {0.3, 0.12, 0.0, 0.25});

	while(!bov_window_should_close(window)){

		bov_points_draw(window, coordDraw, 0, nPoints);
		bov_points_draw(window, hullDraw, 0, my_hull_size);
		bov_window_update(window);
	}

	bov_points_delete(coordDraw);
	bov_points_delete(hullDraw);
	free(coord);
	free(my_hull);
	bov_window_delete(window);

	return EXIT_SUCCESS;
}
