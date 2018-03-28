#include <bits/stdc++.h>
#define EPS 1e-6

using namespace std;

long double function_x(long double x){
    return -19*(x-0.5)*(x-1) + exp(x) - exp(-2*x);
}

double smart_mutiply(long double a, long double b){
    if(a == 0.0 || b == 0.0){
        return 0;
    }else if(a < 0.0 && b > 0.0){
        return -1;
    }else if(b < 0.0 && a > 0.0){
        return -1;
    }else{
        return 1;
    }
}

long double bisection(long double low, long double high, int iter){
    long double mid = low + (high - low) / 2.0;
    if(abs(low - high) > EPS){//checking the |error|
        return mid;
    }else{
        long double f_low = function_x(low);
        long double f_mid = function_x(mid);
        long double result = smart_mutiply(f_low, f_mid);
		cout << low << " " << mid << " " << high << " " << f_low << " " << f_mid << endl;
		string a;
		cin >> a;
        if(result <= 0){
            //need to cut the right part, means that the interval pass through X axis.
            return bisection(low, mid, iter + 1);
        }else{
            //need to cut the left part, means that the interval won't pass through X axis.
            return bisection(mid, high, iter + 1);
        }
    }
}

long double l, r;

int main(){
	bisection(6, 7, 0);
    return 0;
}
