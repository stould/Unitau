#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <string.h>

const int ARRAY_SIZE = 1762;//# of elements floats per file
const int FILE_COUNT = 2015;//number of files


//This function returns a copy of an array
char* string_copy(char *from) {
    int n = strlen(from);
    char *to = malloc((n + 1));
    int i = 0;
    while (i < n){
        to[i] = from[i];
        i++;
    }
    to[i] = '\0';
    return to;
}

//This functions returns the name of all the files/folder/etc in a directory, don't dive into subdirectories.
char** getFiles(char* path){
    DIR *dir;
    struct dirent *ent;
    char **ans = (char **) malloc(sizeof(char*) * (FILE_COUNT + 3));
    int n = 0;
    if ((dir = opendir (path)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            ans[n++] = string_copy(ent->d_name);
        }
        closedir (dir);
    } else {//could not open directory
        perror ("");
    }
    return ans;
}

//This function checks if two strings are the same.
int isSame(char *a, char *b){
    int sizea = strlen(a), sizeb = strlen(b);
    if(sizea != sizeb){
        return 0;
    }else{
        for(int i = 0; i < sizea; i++){
            if(a[i] != b[i]){
                return 0;
            }
        }
        return 1;
    }
}

//This function returns the number of digits of a number
int getSize(int num){
    int numSize = 0;
    for(int i = 0; num > 0; i++){
        numSize++;
        num /= 10;
    }
    return numSize == 0 ? 1 : numSize;
}

/*
    This function makes current_file+num as string, eg:
    makeFileName("hello.txt", 331) = "331_hello.txt"
*/
char *makeFileName(char *current, int num){
    int numSize = getSize(num);
    int currentSize = strlen(current);

    char* ans = malloc((currentSize + numSize + 2));
    int idx = 0;
    if(num == 0){
        ans[0] = '0';
        idx++;
    }
    while(num > 0){
        ans[numSize - idx - 1] = '0' + num % 10;
        idx++;
        num /= 10;
    }
    ans[idx++] = '_';
    int j = 0;
    while(j < currentSize){
        ans[idx++] = current[j++];
    }
    ans[idx] = '\0';
    return ans;
}

/*
    Check if a string ends with some pattern, eg:
    endsWith("try hard", "ard") = true
    endsWith("try hard", "erd") = false
*/
int endsWith(char *str, char* pattern){
    int len_s = strlen(str) - 1;
    int len_p = strlen(pattern) - 1;
    while(len_s >= 0 && len_p >= 0){
        if(str[len_s] != pattern[len_p]){
            return 0;
        }
        len_s--;
        len_p--;
    }
    return 1;
}

/*
    This function basically concatenates dir+file, eg:
    makePath("hello/", "331.txt") = "hello/331.txt"
*/
char* makePath(char *dir, char *file){
    int n = strlen(dir);
    int m = strlen(file);

    char *result = malloc( (n + m + 1));
    int j = 0, idx = 0;
    while(j < n){
        result[idx++] = dir[j++];
    }
    j = 0;
    while(j < m){
        result[idx++] = file[j++];
    }
    result[idx] = '\0';
    return result;
}

/*
    This program reads several files, process them and write new files with the modified values.

*/
int main(int argc, char **argv){

    //reading out the files
    char** fileName = getFiles("/mnt/c/Users/Vinicius/Documents/Unitau/ProcessamentoDeAltoDesempenho/data");
    float buffer[ARRAY_SIZE];
    float newValues[ARRAY_SIZE];

    for(int i = 0; i < FILE_COUNT; i++){
        if(endsWith(fileName[i], ".bin")){//check if it is a valid file

            //Opening the file and storing values into buffer
            char *tmpPath = makePath("data/", fileName[i]);
            FILE *currentFile = fopen(tmpPath, "rb");//read binary
            fread(&buffer, sizeof(float), ARRAY_SIZE, currentFile);
            fclose(currentFile);

            //Process the values into new ones
            for(int j = 0; j < ARRAY_SIZE; j++){
                newValues[j] = buffer[j] + 273;
            }

            //creates a new file with modified values
            char *tmpName = makeFileName(fileName[i], i);
            char *tmpNewPath = makePath("newData/", tmpName);
            FILE *newFile = fopen(tmpNewPath, "wb");
            fwrite(newValues, sizeof(float), ARRAY_SIZE, newFile);
            fclose(newFile);

            //releasing memory
            free(tmpNewPath);
            free(tmpName);
            free(tmpPath);
            free(fileName[i]);
        }
    }
    //releasing memory
    free(fileName);
    return 0;
}
