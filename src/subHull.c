
#include "subHull.h"


struct subHull* newSubHull(GLfloat points[][2], GLsizei n_points)
{
    struct subHull *pt = (struct subHull*)malloc(sizeof(struct subHull));

    pt->hull = (GLfloat**)malloc(sizeof(GLfloat*) * n_points);
    for (int i = 0; i < n_points; i++)
        (pt->hull)[i] = (GLfloat*)malloc(sizeof(GLfloat) * 2);

    graham_scan(points, n_points, &(pt->hull_size), pt->hull);
    //jarvis_march(points, n_points, &(pt->hull_size), pt->hull);

    return pt;
}

void delete_sh(struct subHull* pt)
{
    for (int i = 0; i < pt->hull_size; i++)
        free((pt->hull)[i]);
    free(pt->hull);
    free(pt);
}

