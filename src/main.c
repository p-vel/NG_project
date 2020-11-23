#include "inputs.h"
#include "graham.h"
#include <time.h>

// Global vars
GLfloat (*my_points)[2];
int count;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
		double xpos, ypos;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    	glfwGetCursorPos(window, &xpos, &ypos);
			if (count < 10) {
				my_points[count][0] = (GLfloat)xpos/800.0-0.5; // cursor coords != pixel coords
				my_points[count][1] = -(GLfloat)ypos/800.0+0.5;
				count++;
			}
			else if (count == 10) {
				printf("sorry can't add more points\n");
			}
	}
}

int main()
{
	// Create the window
	bov_window_t *window = bov_window_new(800, 800, "Hello there");
	// Init. some graphical objects
	bov_text_t* hw_obj = bov_text_new((GLubyte[]) {"General Kenobi...."}, GL_DYNAMIC_DRAW);
	bov_text_set_pos(hw_obj, (GLfloat[2]) {-1.0, 0.9});
	// Init point array
	my_points = malloc(sizeof(my_points[0])*10);
	my_points[0][0] = 0.0;
	my_points[0][1] = 0.0;
	bov_points_t *pointsDraw = bov_points_new(my_points, 10, GL_DYNAMIC_DRAW);
	// Some vars
	double x, y;
	int state;
	count = 1;
	// Main loop
	while (!bov_window_should_close(window)) {
		glfwSetMouseButtonCallback(window->self, mouse_button_callback);
		// get cursor position
		glfwGetCursorPos(window->self, &x, &y);
		// Detects left-clicks
		// state = glfwGetMouseButton(window->self, GLFW_MOUSE_BUTTON_LEFT);
		// if (state == GLFW_PRESS) {
		// 	if (count < 10) {
		// 		my_points[count][0] = (GLfloat)x/800.0; // cursor coords != pixel coords
		// 		my_points[count][1] = (GLfloat)y/800.0;
		// 		count++;
		// 	}
		// 	else {
		// 		printf("Sorry, can't add more points\n");
		// 	}
		// }
		// init. the message to print
    GLubyte str[2];
		sprintf(str, "Cursor pos. : (%f, %f)", x, y);
		hw_obj = bov_text_update(hw_obj, str);
    bov_text_draw(window, hw_obj);
		// points
		bov_points_partial_update(pointsDraw, my_points, 0, count, 0);
		bov_points_draw(window, pointsDraw, 0, count);
		// update
		bov_window_update(window);
	}
	for (int i = 0; i < 10; i++) {
		printf("%f, %f\n", my_points[i][0], my_points[i][1]);
	}
	// Free memory
	bov_text_delete(hw_obj);
	bov_points_delete(pointsDraw);
	bov_window_delete(window);
	free(my_points);
	return EXIT_SUCCESS;
}
