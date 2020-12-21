
/*
* 
*/

#include "chan.h"


void chan(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2]) {
	exactinit();

	GLfloat xp, yp;

	// Find the point with lowest y-coord
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
	int m = 2;
	int finalM; // Boolean
	do {
		
		finalM = m == n_points ? 1 : 0;
		int nHulls = ceil((float)n_points / m);

		struct subHull** subHulls = malloc(sizeof(struct subHull*) * nHulls);

		for (int i = 0; i < nHulls; i++) {
			subHulls[i] = newSubHull(&(points[i * m]), min(m, n_points - m * i));
		}

		//Merge the hulls
		GLfloat current[2] = { xp,yp }; // Last added point in hull
		int hullOfCurrent = min_index / m;
		int t_idxOfCurrrent = 0;
		*hull_size = 0;
		for (int i = 0; i < m; i++) {
			hull[i][0] = current[0]; hull[i][1] = current[1];
			(*hull_size)++;
			GLfloat maxTan[2]; // Candidate 
			int hullOfMaxTan, t_idxOfMaxTan;
			for (int j = 0; j < nHulls; j++) {
				int t_idx;
				if (j == hullOfCurrent)
					t_idx = (t_idxOfCurrrent + 1 + (subHulls[j]->hull_size)) % (subHulls[j]->hull_size);
				else
					t_idx = r_tan((subHulls[j])->hull, (subHulls[j])->hull_size, current);
				GLfloat currentTan[2] = { (subHulls[j])->hull[t_idx][0],(subHulls[j])->hull[t_idx][1] };
				float orient = -1.;
				if (j > 0)
					orient = orient2d(current, maxTan, currentTan);
				if (orient < 0 ) { //|| (orient == 0 && alignedCase(current, currentTan, maxTan) == 1)
					maxTan[0] = currentTan[0]; maxTan[1] = currentTan[1];
					hullOfMaxTan = j;
					t_idxOfMaxTan = t_idx;
				}
				
			}
			current[0] = maxTan[0]; current[1] = maxTan[1];
			hullOfCurrent = hullOfMaxTan; t_idxOfCurrrent = t_idxOfMaxTan;
			if (current[0] == xp && current[1] == yp) {
				printf("m : %d\n", m);
				converged = 1;
				break;
			}
		}

		for (int i = 0; i < nHulls; i++) {
			delete_sh(subHulls[i]);
		}
		free(subHulls);

		m = min(2 * m, n_points);

	} while (converged == 0 && !finalM);
}