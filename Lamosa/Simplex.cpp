#include <bits/stdc++.h>
#define MAXN 40001
#define INF 0x3f3f3f3f

typedef long long ll;

using namespace std;

//consts
const int NMAX = 100;

int N, M, Xn;
double matrix[NMAX][NMAX];
double copyZ[NMAX];

void simplex(){
    int cnt = 5;
    int last = -1;
    while(cnt-- > 0){
        //matrix[0] = goal function = Z, checking if Z has some element < 0
        int bestCol = -1, valCol = INF;
        for(int i = 0; i < M - 1; i++){
            if(matrix[0][i] < valCol && matrix[0][i] < 0){
                bestCol = i;
                valCol = matrix[0][i];
            }
        }
        if(valCol < 0){//Ok, we have at least a element to be optimized.
            //now we need to get the lest (matrix[i][XN+EN - 1] / matrix[i][bestCol]) for matrix[i][bestCol] != 0.
            int bestLine = -1;
            double bestVal = INF;//except for (matrix[0][i] == Z).
            for(int i = 1; i < N; i++){
                if(matrix[i][bestCol] != 0 && i != last){
                    const double currVal = matrix[i][M - 1] / matrix[i][bestCol];
                    //printf("%.2f / %.2f = %.2f\n", matrix[i][bestCol], matrix[i][XN+EN - 1], currVal);
                    if(currVal < bestVal){
                        bestLine = i;
                        bestVal = currVal;
                    }
                }
            }
            cout << "L = " << bestLine << " COL = " << bestCol << endl;
            //cout << bestLine << "  " << bestCol << " = " << matrix[bestLine][bestCol] << endl;
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
            for(int i = 0; i < N; i++){
                for(int j = 0; j < M; j++){
                    printf("%.2f ", matrix[i][j]);
                }
                printf("\n");
            }
            printf("\n");
        }else{//That is it, we need to stop here.
            break;
        }
    }

}

void readMatrix(){
    cin >> N >> M;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            cin >> matrix[i][j];
            //cout << matrix[i][j] << " ";
        }
        //cout << endl;
    }
    for(int i = 0; i < M; i++){
        copyZ[i] = matrix[0][i];
    }
}

int main() {
    freopen("in.in", "r", stdin);
    readMatrix();
    simplex();
    return 0;
}
