
/*
* Contains the functions needed to make a Graham scan
*/
#define DEBUG_MODE  1
#define ANIMATION_ON  1

#include "jarvis.h"

GLfloat xp_j, yp_j;  // Global variables



int alignedCase(GLfloat* a, GLfloat* p, GLfloat* b) {
    
    if (a[0] < p[0] && p[0] < b[0] || a[1] < p[1] && p[1] < b[1] || a[0] > p[0] && p[0] > b[0] || a[1] > p[1] && p[1] > b[1])
        return 1;
    if (a[0] == b[0] && a[1] == b[1])
        return 0;

    return -1;

}

//void jarvis_march(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat** hull)
void jarvis_march(GLfloat points[][2], GLsizei n_points, int* hull_size, GLfloat hull[][2])
{
    exactinit();
    // Find the point with lowest y-coord
    int min_index = 0;
    GLfloat y_min = points[min_index][1];
    for (int i = 1; i < n_points; i++) {
        if (points[i][1] < y_min) {       // Ignoring same x for now
            min_index = i;
            y_min = points[i][1];
        }
    }
    // Setting the global variables xp, yp
    xp_j = points[min_index][0];
    yp_j = points[min_index][1];

    GLfloat last[2] = { xp_j,yp_j };
    GLfloat candidate[2];
    int i = 0;
    do {
        hull[i][0] = last[0]; hull[i][1] = last[1];
        candidate[0] = points[0][0]; candidate[1] = points[0][1];
        for (int j = 0; j < n_points; j++) {
            float orient = orient2d(hull[i], points[j], candidate);
            if ((candidate[0] == last[0] && candidate[1] == last[1]) || orient > 0 || orient == 0 && alignedCase(hull[i], points[j], candidate) == 1) {
                candidate[0] = points[j][0]; candidate[1] = points[j][1];
            }
        }
        i++;
        last[0] = candidate[0]; last[1] = candidate[1];
    } while (!(candidate[0] == hull[0][0] && candidate[1] == hull[0][1]) && i < n_points);

    *hull_size = i;
}

//GLsizei r_tan(GLfloat hull[][2], GLsizei hull_size, GLfloat p[2]) {
//    int l = 0;
//    int r = hull_size-1;
//    float l_prev = orient2d(p, hull[0], hull[hull_size - 1]);
//    float l_next = orient2d(p, hull[0], hull[(1 + hull_size) % hull_size]);
//
//    while (l < r) {
//        int c = (l + r) / 2;
//        float c_prev = orient2d(p, hull[c], hull[(c - 1 + hull_size) % hull_size]);
//        float c_next = orient2d(p, hull[c], hull[(c + 1 + hull_size) % hull_size]);
//        float c_side = orient2d(p, hull[l], hull[c]);
//        if (c_prev >= 0 && c_next >= 0) {
//            if (hull[c][0] == p[0] && hull[c][1] == p[1])
//                return orient2d(hull[(c + 1) % hull_size], p, hull[(c - 1 + hull_size) % hull_size]) < 0 ? (c + 1) % hull_size : (c - 1 + hull_size) % hull_size;
//            return c;
//        }
//        else if (c_side > 0 && (l_next < 0 || l_prev > 0 && l_next > 0 || l_prev == 0 && l_next == 0) || c_side < 0 && c_prev < 0)
//            r = c;
//        else {
//            l = c + 1;
//            l_prev = -c_next;
//            l_next = orient2d(p, hull[l], hull[(l + 1 + hull_size) % hull_size]);
//        }
//    }
//    if (hull[l][0] == p[0] && hull[l][1] == p[1])
//        return orient2d(hull[(l + 1) % hull_size], p, hull[(l - 1 + hull_size) % hull_size]) < 0 ? (l + 1) % hull_size : (l - 1 + hull_size) % hull_size;
//    return l;
//}

//GLsizei r_tan(GLfloat hull[][2], GLsizei n, GLfloat p[2]) {
GLsizei r_tan(GLfloat** hull, GLsizei n, GLfloat p[2]) {

    exactinit();

    int a, b, c;
    int upA, dnC;

    if (n == 1)
        return 0;

    GLfloat p2[2], p3[2];
    int idx;

    idx = 1;
    p2[0] = hull[idx][0]; p2[1] = hull[idx][1];
    idx = 0;
    p3[0] = hull[idx][0]; p3[1] = hull[idx][1];


    if (orient2d(p, p2, p3) <= 0) {
        idx = n-1;
        p2[0] = hull[idx][0]; p2[1] = hull[idx][1];
        if (orient2d(p, p2, p3) < 0)
            return 0;
    }

    //a = 0;
    //b = n;
    for (a = 0, b = n; a < b; ) {
        c = (a + b) / 2;

        idx = (c + 1) % n;
        p2[0] = hull[idx][0]; p2[1] = hull[idx][1];
        idx = c;
        p3[0] = hull[idx][0]; p3[1] = hull[idx][1];
        dnC = orient2d(p, p2, p3) <= 0;

        idx = (c - 1 + n) % n;
        p2[0] = hull[idx][0]; p2[1] = hull[idx][1];
        if (dnC && orient2d(p, p2, p3) < 0)
            return c;

        idx = (a + 1) % n;
        p2[0] = hull[idx][0]; p2[1] = hull[idx][1];
        idx = a;
        p3[0] = hull[idx][0]; p3[1] = hull[idx][1];
        upA = orient2d(p, p2, p3) >= 0;
        if (upA) {
            if (dnC)
                b = c;
            else {
                idx = a;
                p2[0] = hull[idx][0]; p2[1] = hull[idx][1];
                idx = c;
                p3[0] = hull[idx][0]; p3[1] = hull[idx][1];
                if (orient2d(p, p2, p3) >= 0)
                    b = c;
                else
                    a = c;
            }
        }
        else {
            if (!dnC)
                a = c;
            else {
                idx = a;
                p2[0] = hull[idx][0]; p2[1] = hull[idx][1];
                idx = c;
                p3[0] = hull[idx][0]; p3[1] = hull[idx][1];
                if (orient2d(p, p2, p3) <= 0)
                    b = c;
                else
                    a = c;
            }
        }
    } 
    return a;

}
