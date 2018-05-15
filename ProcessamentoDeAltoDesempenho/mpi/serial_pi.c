#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MASTER_ID 0

const int TAG = 1;

double func(double i, double step) {
    double x = (i+0.5)*step;
    return (4.0 / (1.0 + x * x));
}

double getSubSum(int iterations, int pid, int size){
    int chunkSize = iterations / size;
    int start = pid * chunkSize, end = start + chunkSize - 1;
    if(pid == size - 1) end = iterations - 1;
    double ans = 0;
    double step = 1.0 / (double) iterations;
    while(start < end){
        ans += func(start++, step);
    }
    return ans;
}
/*
	This program calculates PI using integration and a fixed number of steps.
*/
int main(){
    int iterations = 500000000;
    double answer = getSubSum(iterations, 0, 1);
    printf("PI = %.8f\n", answer * (1.0 / iterations));
	return 0;
}
