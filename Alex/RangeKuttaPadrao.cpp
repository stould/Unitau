#include <bits/stdc++.h>
#define ld long double

using namespace std;


//Put the function
ld func(ld T, ld Y){
    return T - Y;
}

ld getK(ld h, ld T, ld Y){
    return h * func(T, Y);
}

void printLine(int &N, ld Tn, ld K1, ld K2, ld Yn){
    cout<< fixed <<setprecision(6) << N << " " << Tn << " " << K1 << " " << K2 << " " << Yn << endl;
    N++;
}

void rangeKutta(ld Y, ld h, ld left, ld right){
    ld Yn = Y;
    int n = 0;
    cout << "N | Tn | K1 | K2 | Yn" << endl;
    for(ld t = left; t <= right; t += h){
        ld K1 = getK(h, t, Yn);
        ld K2 = getK(h, t + h, Yn + K1);
        Yn = Yn + (K1 + K2) / (ld) 2.0;
        printLine(n, t, K1, K2, Yn);
    }
}

int main(){
    //Set {y(0); h; left, right}
    //(0 <= t <= 4) 0 = left bound, 4 = right bound
    rangeKutta(2, 0.2, 0, 0.6);
    return 0;
}
