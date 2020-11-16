
/*
* Contains the functions needed to make a Graham scan
*/
#define DEBUG_MODE  1
#include "graham.h"

GLfloat xp, yp;  // Global variables (change ?)

int cmpfunc(const void *a, const void *b)
{
    // Casting the elements
    GLfloat (*val1)[2] = (GLfloat (*)[2])a;
    GLfloat (*val2)[2] = (GLfloat (*)[2])b;
    // Getting the elems
    GLfloat xa = *val1[0];
    GLfloat xb = *val2[0];
    GLfloat ya = *val1[1];
    GLfloat yb = *val2[1];
    // Dot products
    GLfloat dota = xa*xp + ya*yp;
    GLfloat dotb = xb*xp + yb*yp;
    // Norms
    GLfloat norma = sqrt(xa*xa + ya*ya);
    GLfloat normb = sqrt(xb*xb + yb*yb);
    GLfloat normp = sqrt(xp*xp + yp*yp);
    // cosines
    GLfloat cosa = dota/(norma*normp);
    GLfloat cosb = dotb/(normb*normp);
    // Compare
    if (cosa > cosb) {
        return -1;
    }
    else if (cosa < cosb) {
        return 1;
    }
    return 0;
}

void graham_scan(GLfloat points[][2], GLsizei n_points)
{
    // Find the minimum y-coordinate point
    GLfloat y_min = points[0][1];
    int y_index = 0;
    for (int i = 1; i < n_points; i++) {
        if (points[i][1] == y_min && points[i][0] < points[y_index][0]) {
            y_min = points[i][1];
            y_index = i;
        }
        else if (points[i][1] < y_min) {
            y_min = points[i][1];
            y_index = i;
        }
    }
#if DEBUG_MODE
    printf("The minimal y index and value are: %d, %f\n", y_index, y_min);
#endif
    // Put y_min at the beginning of the array
    GLfloat x0, y0;
    x0 = points[0][0];
    y0 = points[0][1];
    xp = points[y_index][0];
    yp = points[y_index][1];
    points[0][0] = xp;
    points[0][1] = yp;
    points[y_index][0] = x0;
    points[y_index][1] = y0;
    // Sort
    qsort(&(points[1]), n_points-1, sizeof(GLfloat[2]), cmpfunc);  // get the adress of the 2nd point

}
