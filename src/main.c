#include "inputs.h"
#include "graham.h"
#include <time.h>

#define N_POINTS 20
// Global vars
GLfloat (*my_points)[2];
int count;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
		double xpos, ypos;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    	glfwGetCursorPos(window, &xpos, &ypos);
			if (count < N_POINTS) {
				my_points[count][0] = (GLfloat)xpos/400.0 - 1.0; // cursor coords != pixel coords
				my_points[count][1] = -(GLfloat)ypos/400.0 + 1.0;
				count++;
			}
			else if (count == N_POINTS) {
				printf("sorry can't add more points\n");
			}
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
				my_points[0][0] = 0.0;  // back to one point
				my_points[0][1] = 0.0;
				count = 1;
		}
}

int main()
{
	// Create the window
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
	my_points[0][0] = 0.0;
	my_points[0][1] = 0.0;
	bov_points_t *pointsDraw = bov_points_new(my_points, N_POINTS, GL_DYNAMIC_DRAW);
	bov_points_t *hullDraw = bov_points_new(my_hull, N_POINTS, GL_DYNAMIC_DRAW);
	bov_points_set_color(pointsDraw, (GLfloat[4]) {0.0, 0.0, 0.0, 1.0});
	bov_points_set_color(hullDraw, (GLfloat[4]) {1.0, 0.0, 0.0, 1.0});
	// Some vars
	double x, y;
	int state;
	count = 1;
	// Main loop
	while (!bov_window_should_close(window)) {
		glfwSetMouseButtonCallback(window->self, mouse_button_callback);
		glfwSetKeyCallback(window->self, key_callback);
		// get cursor position
		glfwGetCursorPos(window->self, &x, &y);
		// init. the message to print
    char str[20]; // not sure if correct
		sprintf(str, "Cursor pos. : (%f, %f)", x, y);
		hw_obj = bov_text_update(hw_obj, str);
    bov_text_draw(window, hw_obj);
    bov_text_draw(window, help);
		// convex hull
		int hull_size;
		graham_scan(my_points, count, &hull_size, my_hull);
		GLubyte str2[40];
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
	for (int i = 0; i < 10; i++) {
		printf("%f, %f\n", my_points[i][0], my_points[i][1]);
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
