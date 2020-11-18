#include "utils.h"

GLfloat dot(GLfloat x[2], GLfloat y[2])
{
    GLfloat result;
    result = x[0]*y[0] + x[1]*y[1];
    return result;
}

GLfloat cross(GLfloat x[2], GLfloat y[2])
{
    GLfloat result;
    result = x[0]*y[1] - y[0]*x[1];
    return result;
}

GLfloat norm(GLfloat x[2])
{
    GLfloat result;
    result = sqrt(dot(x, x));
    return result;
}

void swap(int i, int j, GLfloat arr[][2]) {
    GLfloat xi = arr[i][0];
    GLfloat yi = arr[i][1];
    GLfloat xj = arr[j][0];
    GLfloat yj = arr[j][1];
    arr[i][0] = xj;
    arr[i][1] = yj;
    arr[j][0] = xi;
    arr[j][1] = yi;

}
