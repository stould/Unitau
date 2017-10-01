#include <bits/stdc++.h>
#define EPS 1e-6
#define MAXN 100
#define ld long double
using namespace std;

ld matrix[MAXN][MAXN], b[MAXN], Xn[MAXN], ans[MAXN];

void printMat(int n){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			cout << matrix[i][j] << " ";
		}
		cout << b[i] << endl;
	}
}

void printXn(int n){
	for(int i = 0; i < n; i++){
		cout << Xn[i] << endl;
	}
}

void schedule(int n){
	for(int i = 0; i < n; i++){
		for(int j = i+1; j < n; j++){
			ld ratio = 0;
			ratio = matrix[j][i] / matrix[i][i];
			for(int k = i; k < n; k++){
				matrix[j][k] = matrix[j][k] - (ratio * matrix[i][k]);
			}
			b[j] = b[j] - (ratio * b[i]);
		}
	}
}

void backsubstitution(int n){
	for(int i = n-1; i >= 0; i--){
		ld tmp = 0;
		for(int j = i+1; j < n; j++){
			tmp += matrix[i][j] * Xn[j];
		}
		if(matrix[i][i] == 0) continue;
		Xn[i] = (b[i] - tmp) / matrix[i][i];
	}

}

int main(){
	int n = 3;
	matrix[0][0] = 3;
	matrix[0][1] = 2;
	matrix[0][2] = 4;

	matrix[1][0] = 0;
	matrix[1][1] = 0;
	matrix[1][2] = 1;

	matrix[2][0] = 0;
	matrix[2][1] = 1;
	matrix[2][2] = 0;

	b[0] = 40;
	b[1] = 3;
	b[2] = 5;

	schedule(n);
	printMat(n);
	backsubstitution(n);
	printXn(n);
    return 0;
}
