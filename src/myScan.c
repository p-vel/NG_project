#include "myScan.h"


GLfloat** scanFile(const char* filename) {

    FILE* myfile;

    myfile = fopen(filename, "r");
    int size;
    fscanf(myfile, "%d", &size);

    GLfloat (*coord)[2] = malloc(sizeof(coord[0]) * size);


    for (int i = 0; i < size; i++) {
        fscanf(myfile, "%f", &(coord[i][0]));
        fscanf(myfile, "%f", &(coord[i][1]));
    }
    fclose(myfile);

    return coord;

}
