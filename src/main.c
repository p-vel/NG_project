#include "inputs.h"
#include "algorithms.h"
#include "animations.h"
#include <time.h>


#define ANIMATION_ON 1
#define SHOW_INFO 1
#define HEURISTIC 0

#define N_POINTS 1000

// Global vars for the sandbox mode
GLfloat (*my_points)[2];
int count;
int realloc_count;

// Mouse Input
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		if (count < (N_POINTS*(realloc_count+1))) {
			my_points[count][0] = (GLfloat)xpos/400.0 - 1.0; // cursor coords != pixel coords
			my_points[count][1] = -(GLfloat)ypos/400.0 + 1.0;
			count++;
		}
		else {
			printf("reallocating\n");
			my_points = realloc(my_points, 2*N_POINTS*sizeof(my_points[0]));
			realloc_count++;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && count > 3) {
		glfwGetCursorPos(window, &xpos, &ypos);
		for (int i = 0; i < N_POINTS; i++) {
			GLfloat x_pts = (GLfloat)xpos/400.0 - 1.0;
			GLfloat y_pts = -(GLfloat)ypos/400.0 + 1.0;
			if (fabs(my_points[i][0]-x_pts) < 0.05 && fabs(my_points[i][1]-y_pts) < 0.05) {
				swap(i, count-1, my_points);
				count--;
				break;
			}
		}
	}
}

// Keyboard input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		my_points[0][0] = 0.0;  // back to one point
		my_points[0][1] = 0.0;
		count = 1;
	}
}

// Main
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

	else if (argc == 4){

		int which_set = atoi(argv[1]);
		int which_algo = atoi(argv[2]);

		bov_window_t* window = bov_window_new(-1, -1, "Hello there Jackie");
		bov_window_set_color(window, COLOUR_MIDNIGHT_BLUE);
		// bov_window_set_color(window, (GLfloat[]){0.9f, 0.85f, 0.8f, 1.0f});

		// ##### Jackie w/ heuristic ######
		int n_points_max = 100000;
		int n_points;
		GLfloat(*coord)[2] = malloc(sizeof(GLfloat[2]) * n_points_max);

		double scale, speed;
		if (which_set == 0) {
			scanFile("../small_points.txt", coord, &n_points);
			scale = 2.0;
			speed = 0.01;
		}
		else if (which_set == 1) {
			scanFile("../JackieChanWTF.txt", coord, &n_points);
			scale = 0.1;
			speed = (which_algo == 0) ? 0.00001 : 0.01;
		}
		else if (which_set == 2) {
			scanFile("../JackieChan.txt", coord, &n_points);
			scale = 0.1;
			speed = (which_algo == 0) ? 0.00001 : 0.01;
		}
		else if (which_set == 3) {
			scanFile("../tree.txt", coord, &n_points);
			scale = 1.0;
			speed = (which_algo == 0) ? 0.01 : 1;
		}

		GLfloat(*hull)[2] = malloc(sizeof(GLfloat[2]) * n_points);
		int hull_size;

		int animation_on = atoi(argv[3]);

		if (animation_on == 1) {
			if (which_algo == 0) {
				graham_scan_animation(coord, n_points, &hull_size, hull, window, scale, speed);
			}
			else if (which_algo == 1) {
				chan_animation(coord, n_points, &hull_size, hull, window, scale, speed);
			}
		}

		else if (animation_on == 0) {
			int rem_points;
			akl_toussaint(coord, n_points, &rem_points);
			if (which_algo == 0) {
				graham_scan(coord, rem_points, &hull_size, hull);
			}
			else if (which_algo == 1) {
				chan(coord, rem_points, &hull_size, hull);
			}
		}

		bov_points_t* pointsDraw = bov_points_new(coord, n_points, GL_DYNAMIC_DRAW);
		bov_points_t* hullDraw = bov_points_new(hull, hull_size, GL_DYNAMIC_DRAW);
		bov_points_set_color(pointsDraw, COLOUR_GREEN_STYLE);
		bov_points_set_width(pointsDraw, 0.01 * scale);
		bov_points_set_color(hullDraw, (GLfloat[4]) {1.0, 0.0, 0.0, 1.0});
		bov_points_set_width(hullDraw, 0.02 * scale);

		while (!bov_window_should_close(window) && glfwGetKey(window->self,GLFW_KEY_ESCAPE) != GLFW_PRESS) {
			bov_points_draw(window, pointsDraw, 0, n_points);
			bov_points_draw(window, hullDraw, 0, hull_size);
			bov_fast_line_loop_draw(window, hullDraw, 0, hull_size);
			bov_window_update(window);
		}

		bov_points_delete(pointsDraw);
		bov_points_delete(hullDraw);
		bov_window_delete(window);
		free(coord);
		free(hull);
	}

	else {
		bov_window_t *window = bov_window_new(800, 800, "Hello there");
		bov_window_set_color(window, (GLfloat[]){0.9f, 0.85f, 0.8f, 1.0f});
		// Init. some graphical objects
		bov_text_t* hw_obj = bov_text_new((GLubyte[]) {""}, GL_DYNAMIC_DRAW);
		bov_text_set_pos(hw_obj, (GLfloat[2]) {-1.0, 0.9});
		bov_text_t* help = bov_text_new((GLubyte[]) {"Press [c] to clear the points"}, GL_DYNAMIC_DRAW);
		bov_text_set_pos(help, (GLfloat[2]) {-1.0, 0.8});
		bov_text_t* info = bov_text_new((GLubyte[]) {"Number of points: 3\nSize of the hull: 3\n"}, GL_DYNAMIC_DRAW);
		bov_text_set_pos(info, (GLfloat[2]) {-1.0, 0.7});
		// Init point array
		my_points = malloc(sizeof(my_points[0])*N_POINTS);
		GLfloat (*my_hull)[2] = malloc(sizeof(my_hull[0])*N_POINTS);
		my_points[0][0] = 0.0; my_points[0][1] = 0.0;
		my_points[1][0] = 0.2; my_points[1][1] = 0.2;
		my_points[2][0] = 0.1; my_points[2][1] = 0.2;
		bov_points_t *pointsDraw = bov_points_new(my_points, N_POINTS, GL_DYNAMIC_DRAW);
		bov_points_t *hullDraw = bov_points_new(my_hull, N_POINTS, GL_DYNAMIC_DRAW);
		bov_points_set_color(pointsDraw, (GLfloat[4]) {0.0, 0.0, 0.0, 1.0});
		bov_points_set_color(hullDraw, (GLfloat[4]) {1.0, 0.0, 0.0, 1.0});
		// Some vars
		double x, y;
		int state;
		count = 3;
		realloc_count = 0;
		// Main loop
		while (!bov_window_should_close(window) && glfwGetKey(window->self,GLFW_KEY_ESCAPE) != GLFW_PRESS) {
			glfwSetMouseButtonCallback(window->self, mouse_button_callback);
			glfwSetKeyCallback(window->self, key_callback);
			// get cursor position
			glfwGetCursorPos(window->self, &x, &y);
			// init. the message to print
			char str[30];
			sprintf(str, "Cursor pos. : (%.3f, %.3f)", x, y);
			hw_obj = bov_text_update(hw_obj, str);
			bov_text_draw(window, hw_obj);
			bov_text_draw(window, help);
			// convex hull
			int hull_size;
			graham_scan(my_points, count, &hull_size, my_hull);
			char str2[60];
			sprintf(str2, "Number of points: %d\nSize of the hull: %d\n", count, hull_size);
			info = bov_text_update(info, str2);
			bov_text_draw(window, info);
			// points
			bov_points_partial_update(pointsDraw, my_points, 0, count, 0);
			bov_points_partial_update(hullDraw, my_hull, 0, hull_size, 0);
			bov_points_draw(window, pointsDraw, 0, count);
			bov_points_draw(window, hullDraw, 0, hull_size);
			bov_fast_line_loop_draw(window, hullDraw, 0, hull_size);
			// update
			bov_window_update(window);
		}
		// Free memory
		bov_text_delete(hw_obj);
		bov_points_delete(pointsDraw);
		bov_points_delete(hullDraw);
		bov_window_delete(window);
		free(my_points);
		free(my_hull);
		return EXIT_SUCCESS;
	}
}
