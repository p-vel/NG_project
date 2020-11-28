
#include "subHull.h"


struct subHull* newSubHull(GLfloat points[][2], GLsizei n_points)
{
    struct subHull *pt = (struct subHull*)malloc(sizeof(struct subHull));

    pt->hull = (GLfloat*)malloc(sizeof(GLfloat*) * n_points);

    graham_scan(points, n_points, &(pt->hull_size), &(pt->hull));

    return pt;
}

void delete_sh(struct subHull* pt)
{
    free(pt->hull);
    free(pt);
}

