#include <stdio.h>
#include <omp.h>

//write your function like: e^(-x), x^2, etc
double function(double x){
    return x*x*x -2.*x + 1.;
}

//Getting the Trapezoid area between x1 and x2
double getTrapezoidArea(double h, double x1, double x2){
    double fx1 = function(x1);
    double fx2 = function(x2);
    return h * (fx1 + fx2) / 2.0;
}

//Integrating using Trapezoidal rule
double integrate(double lim_inf, double lim_sup, int n){

    //getting height
    double h = (lim_sup - lim_inf) / (double) n;
    double area = 0.0;

    int nthreads;

    #pragma omp parallel default(shared) firstprivate(h)
    {
        int id = omp_get_thread_num();

        #pragma omp single
        {
            nthreads = omp_get_num_threads();
        }
        
        //each thread must take the proper part of the function [start, end]
        #pragma omp for reduction(+:area)
        for(int i = 0; i < n; i++){
            double x1 = lim_inf + (double) i * h;
            double x2 = x1 + h;
            area += getTrapezoidArea(h, x1, x2);
        }
    }
    return area;
}

/*
	This program calculates the area (or integrate it) of a function
    The job is divided among the threads.
    - Definite integral
*/
int main(){
    //inferior limit and superior limit for definite integral
    double lim_inf, lim_sup;
    int n;//number of lanes, creates n + 1 areas to be added up
    if(scanf("%lf %lf %d", &lim_inf, &lim_sup, &n) > 0);
    double area = integrate(lim_inf, lim_sup, n);
    printf("%.3f\n", area);
    return 0;
}
