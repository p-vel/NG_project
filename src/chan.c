

#include "algorithms.h"


int my_pow(int t) {
    int temp_res = 1 << t;
    int res = 1 << temp_res;
    return res;
}

/*
* Function implementing Chan's Algorithm
* Arguments
*   points   : the set of points we wants to convexhullise
*   n_points : the number of points in points
*   hull_size: the size of the hull that will be created (passed by ref)
*   hull     : without any surprises... the future hull
*
*/
void chan(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2]) {
    exactinit();

    GLfloat xp, yp;

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
    int m = 2;  // Initial value for size of subsets
    int finalM; // Boolean
    // Allocation of extra arrays for subdivisions
    GLfloat(*partition)[2] = malloc(sizeof(partition[0]) * n_points); // points of the convex hulls
    int* start_indices = malloc(sizeof(int) * n_points);              // stores the starting index for each subset
    int* size_indices = malloc(sizeof(int) * n_points);               // stores the size of each subset (convenience)

    GLfloat(*jarvis_hull)[2] = malloc(sizeof(GLfloat[2]) * n_points);

    int t = 1;
    // Main Loop
    do {
        finalM = m == n_points ? 1 : 0;
        int nHulls = n_points % m == 0 ? n_points / m : (n_points / m) + 1;

        // Subdivision step
        int temp_hull_size;
        int hull_count = 0;

        for (int i = 0; i < nHulls; i++) {
            // Computes the hull
            graham_scan(&(points[i * m]), min(m, n_points - (i * m)), &temp_hull_size, &(partition[hull_count]));
            start_indices[i] = hull_count;
            size_indices[i] = temp_hull_size;
            hull_count += temp_hull_size;
        }
        int n_full_subsets = n_points / m;
        int n_rest = n_points % m;

        GLfloat current[2] = { xp, yp };
        int hullOfCurrent = min_index / m;
        int t_idxOfCurrrent = 0;
        *hull_size = 0;
        for (int i = 0; i < m; i++) {
            hull[i][0] = current[0]; hull[i][1] = current[1];
            (*hull_size)++;
            GLfloat maxTan[2] = { current[0],current[1] };
            int hullOfMaxTan, t_idxOfMaxTan;
            for (int j = 0; j < nHulls; j++) {
                int t_idx;
                if (j == hullOfCurrent)
                    t_idx = (t_idxOfCurrrent + 1 + (size_indices[j])) % (size_indices[j]);
                else
                    t_idx = r_tan(&partition[start_indices[j]], size_indices[j], current);
                GLfloat currentTan[2] = { partition[start_indices[j] + t_idx][0], partition[start_indices[j] + t_idx][1] };
                float orient = orient2d(currentTan, current, maxTan);
                if (orient < 0 || (orient == 0 && alignedCase(current, currentTan, maxTan) == 1) || maxTan[0] == current[0] && maxTan[1] == current[1]) {
                    maxTan[0] = currentTan[0]; maxTan[1] = currentTan[1];
                    hullOfMaxTan = j;
                    t_idxOfMaxTan = t_idx;
                }
            }
            current[0] = maxTan[0]; current[1] = maxTan[1];
            hullOfCurrent = hullOfMaxTan;
            t_idxOfCurrrent = t_idxOfMaxTan;
            if (current[0] == xp && current[1] == yp) {
                converged = 1;
                break;
            }
        }

        m = min(2 * m, n_points); // doubling scheme
        // m = min(pow(2,pow(2,t)), n_points); // power scheme
        // m =  min(my_pow(t), n_points);
        t += 1;
        // printf("%d\n", m);
    } while (converged == 0 && !finalM);
    // Free the memory
    free(partition);
    free(jarvis_hull);
    free(start_indices);
    free(size_indices);

}


/*
* Function that finds the `right' tangent to a convex hull based on [1]
* Arguments
*   hull : the convex hull¹
*   n    : number of points constituing the hull
*   p    : the point where the tangent should start ! MUST BE STRICTLTY OUTSIDE THE HULL !
* Returns
*   the index in hull of the point of the hull that is hit by the tangent
*
* References
*   [1] Dan Sunday, "Tangents to & between 2D Polygons", http://geomalgorithms.com/a15-_tangents.html
* _________________________________
* ¹ hull is here defined as a pointer but can be defined as an array if more convenient.
*   ! If you use Chan's algorithm, please use the pointer definition. !
*/
GLsizei r_tan(GLfloat hull[][2], GLsizei n, GLfloat p[2]) {
    //GLsizei r_tan(GLfloat** hull, GLsizei n, GLfloat p[2]) {

    exactinit();

    int a, b, c;
    int upA, dnC;

    if (n == 1)
        return 0;

    if (orient2d(p, hull[1], hull[0]) <= 0 && orient2d(p, hull[n - 1], hull[0]) < 0)
        return 0;

    for (a = 0, b = n; a < b; ) {
        c = (a + b) / 2;

        dnC = orient2d(p, hull[(c + 1) % n], hull[c]) <= 0;

        if (dnC && orient2d(p, hull[(c - 1 + n) % n], hull[c]) < 0)
            return c;

        upA = orient2d(p, hull[(a + 1) % n], hull[a]) >= 0;
        if (upA) {
            if (dnC)
                b = c;
            else {
                if (orient2d(p, hull[a], hull[c]) >= 0)
                    b = c;
                else
                    a = c;
            }
        }
        else {
            if (!dnC)
                a = c;
            else {
                if (orient2d(p, hull[a], hull[c]) <= 0)
                    b = c;
                else
                    a = c;
            }
        }
    }
    return a;

}