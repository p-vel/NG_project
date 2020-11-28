
/*
* 
*/

#include "chan.h"

void chan(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2]) {

	GLfloat xp, yp;

	// Find the point with lowest y-coord
	int min_index = 0;
	GLfloat y_min = points[min_index][1];
	for (int i = 1; i < n_points; i++) {
		if (points[i][1] < y_min) {       // Ignoring same x for now
			min_index = i;
			y_min = points[i][1];
		}
	}
	xp = points[min_index][0]; yp = y_min;

	//Compute the subhulls
	int m = 35;
	int nHulls = ceil((float)n_points / m);
	
	struct subHull** subHulls = malloc(sizeof(struct subHull*) * nHulls);

	for (int i = 0; i < nHulls; i++) {
		subHulls[i] = (struct subHull*)newSubHull(&(points[i*m]),min(m,n_points-m*i));
	}

	//Merge the hulls
	GLfloat current[2] = { xp,yp };
	*hull_size = 0;
	for (int i = 0; i < m; i++) {
		hull[i][0] = current[0]; hull[i][1] = current[1];
		(*hull_size)++;
		GLfloat maxTan[2] = { xp - 1., yp };
		for (int j = 0; j < nHulls; j++) {
			int t_idx = r_tan((subHulls[j])->hull, (subHulls[j])->hull_size, current);
			GLfloat currentTan[2] = { (subHulls[j])->hull[t_idx][0],(subHulls[j])->hull[t_idx][1] };
			if (compare(currentTan, maxTan) > 0) {
				maxTan[0] = currentTan[0]; maxTan[1] = currentTan[1];
			}
		}
		current[0] = maxTan[0]; current[1] = maxTan[1];
		if (current[0] == xp && current[1] == yp)
			break;
	}

	for (int i = 0; i < nHulls; i++) {
		delete_sh(subHulls[i]);
	}
	free(subHulls);
}