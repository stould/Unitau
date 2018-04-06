#include <stdio.h>
#include <stdlib.h>
#include <math.h>


const int ARRAY_SIZE = 1762;

/*
    Simple program to show content of a binary file
*/
int main(int argc, char **argv){
    char dirName[100];
    scanf("%s", dirName);
    FILE *ptr = fopen(dirName, "rb");
    float buffer[ARRAY_SIZE];
    fread(&buffer, sizeof(float), ARRAY_SIZE, ptr);
    fclose(ptr);

    for(int i = 0; i < ARRAY_SIZE; i++){
        printf("%.3f\n", buffer[i]);
    }

    return 0;
}
