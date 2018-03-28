#include <bits/stdc++.h>
#define MAXN 40001
#define INF 0x3f3f3f3f

typedef long long ll;

using namespace std;

//consts
const int NMAX = 100;

int N, M, Xn;
double matrix[NMAX][NMAX];

void print(){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            printf("%.1f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void simplex(){
    //print();
    int last = -1;
    while(1){
        //matrix[0] = goal function = Z, checking if Z has some element < 0
        int bestCol = -1;
        double valCol = INF;
        for(int i = 0; i < M - 1; i++){
            if(matrix[0][i] < valCol && matrix[0][i] < 0){
                bestCol = i;
                valCol = matrix[0][i];
            }
        }


        if(valCol < 0){
            //Ok, we have at least a element to be optimized.
            //now we need to get the lest (matrix[i][XN+EN - 1] / matrix[i][bestCol]) for matrix[i][bestCol] != 0.
            int bestLine = -1;
            double bestVal = INF;//except for (matrix[0][i] == Z).
            for(int i = 1; i < N; i++){
                if(matrix[i][bestCol] != 0 && i != last){
                    const double currVal = matrix[i][M - 1] / matrix[i][bestCol];
                    if(currVal < bestVal){
                        bestLine = i;
                        bestVal = currVal;
                    }
                }
            }

            //all lines will lead to division by 0
            if(bestLine == -1){
                break;
            }
            last = bestLine;
            const double PIVOT = matrix[bestLine][bestCol];
            for(int i = 0; i < M; i++){
                matrix[bestLine][i] /= PIVOT;
            }
            for(int i = 0; i < N; i++){
                if(i != bestLine){
                    const double COEFICIENT = matrix[i][bestCol];
                    for(int j = 0; j < M; j++){
                        matrix[i][j] -= (COEFICIENT * matrix[bestLine][j]);
                    }
                }
            }
            //print();
        }else{
            //That is it, we need to stop here.
            break;
        }
    }

}

bool readInput(){
    if(cin >> N >> M){
        for(int i = 0; i < N; i++){
            for(int j = 0; j < M; j++){
                cin >> matrix[i][j];
            }
        }
        return 1;
    }
    return 0;
}

int main() {
    freopen("in", "r", stdin);
    freopen("out", "w", stdout);
    while(readInput()){
        simplex();
        printf("%.2f\n", matrix[0][M - 1]);
    }
    return 0;
}
