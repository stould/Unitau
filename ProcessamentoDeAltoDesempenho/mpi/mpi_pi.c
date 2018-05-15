#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define MASTER_ID 0

const int TAG = 1;

double func(double i, double step) {
    double x = (i+0.5)*step;
    return (4.0 / (1.0 + x * x));
}

double getSubSum(int iterations, int pid, int size){
    int chunkSize = iterations / size;
    int start = pid * chunkSize, end = start + chunkSize;
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
    MPI_Status status;

    int ierr, size, pid;

    ierr = MPI_Init(NULL, NULL);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);


	if(pid == MASTER_ID){
        int iterations = 500000000;
        MPI_Bcast(&iterations, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
        double answer = getSubSum(iterations, 0, size);
        for(int i = 1; i < size; i++){
            double subSum = 0;
            ierr = MPI_Recv(&subSum, 1, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            answer += subSum;
        }
        printf("PI = %.8f\n", answer * (1.0 / iterations));
	}else{
        int iterations;
        MPI_Bcast(&iterations, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
        double subSum = getSubSum(iterations, pid, size);
        ierr = MPI_Send(&subSum, 1, MPI_DOUBLE, MASTER_ID, TAG, MPI_COMM_WORLD);
	}
    MPI_Finalize();
	return 0;
}
