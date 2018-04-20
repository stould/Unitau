#include <stdio.h>
#include <mpi.h>

#define MASTER_ID 0
const int TAG = 1;

//write your function like: e^(-x), x^2, etc
double function(double x){
    return x*x;
}

//Getting the Trapezoid area between x1 and x2
double getTrapezoidArea(double h, double x1, double x2){
    double fx1 = function(x1);
    double fx2 = function(x2);
    return h * (fx1 + fx2) / 2.0;
}


//Integrating using Trapezoidal rule
double integrate(int n, int pid, int size, double lim_inf, double h){
        int chunkSize = n / size;
        int start, end;
        start = chunkSize * pid;
        end = chunkSize * (pid + 1);
        if(n % size != 0 && pid == size - 1){
            end = n;
        }
        double area = 0.0;
        //each thread must take the proper part of the function [start, end]
        for(int i = start; i < end; i++){
            double x1 = lim_inf + (double) i * h;
            double x2 = x1 + h;
            area += getTrapezoidArea(h, x1, x2);
        }

    return area;
}

/*
	This program calculates the area (or integrate it) of a function
    The job is divided among the threads.
    - Definite integral
*/
int main(int argc, char **argv){
    int ierr, size, pid;
    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

    //inferior limit and superior limit for definite integral
    double lim_inf, lim_sup;
    int n;//number of lanes, creates n + 1 areas to be added up
    double h;//height

    if(pid == MASTER_ID){
        if(scanf("%lf %lf %d", &lim_inf, &lim_sup, &n) > 0);
        h = (lim_sup - lim_inf) / (double) n;
        double area = integrate(n, pid, size, lim_inf, h);
        printf("Process[%d], %f\n", pid, area);
        MPI_Bcast(&lim_inf, 1, MPI_DOUBLE, MASTER_ID, MPI_COMM_WORLD);
        MPI_Bcast(&lim_sup, 1, MPI_DOUBLE, MASTER_ID, MPI_COMM_WORLD);
        MPI_Bcast(&h, 1, MPI_DOUBLE, MASTER_ID, MPI_COMM_WORLD);
        MPI_Bcast(&n, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
        for(int i = 1; i < size; i++){
            double piece;
			ierr = MPI_Recv(&piece, 1, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            area += piece;
        }
        printf("%f\n", area);
    }else{
        MPI_Bcast(&lim_inf, 1, MPI_DOUBLE, MASTER_ID, MPI_COMM_WORLD);
        MPI_Bcast(&lim_sup, 1, MPI_DOUBLE, MASTER_ID, MPI_COMM_WORLD);
        MPI_Bcast(&h, 1, MPI_DOUBLE, MASTER_ID, MPI_COMM_WORLD);
        MPI_Bcast(&n, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
        double currArea = integrate(n, pid, size, lim_inf, h);
        printf("Process[%d], %f\n", pid, currArea);
        ierr = MPI_Send(&currArea, 1, MPI_DOUBLE, MASTER_ID, TAG, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
