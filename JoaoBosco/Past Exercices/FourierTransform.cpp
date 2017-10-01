#include <bits/stdc++.h>
#define MAXM 100001
#define MAXN 101
#define INF 0x3f3f3f3f
#define PI acos(-1)

typedef long long ll;

using namespace std;

vector<vector<double> > dft(vector<double> &x, int N){
    vector<vector<double> > y(N, vector<double>(2));
    for(int k = 0; k < N; k += 1){
        for(int n = 0; n < N; n += 1){
            double a = 2.0 * PI * (double) k * (double) n / (double) N;
            y[k][0] += x[n] * cos(a);
            y[k][1] += x[n] * -sin(a);
        }
    }
    return y;
}

int main() {
    vector<double> x;
    x.push_back(5);
    x.push_back(5);
    x.push_back(5);
    x.push_back(5);
    vector<vector<double> > ans = dft(x,x.size());
    for(int i = 0; i < ans.size(); i++){
        cout << fixed;
        cout << setprecision(2) << ans[i][0] << " + j * ( " << ans[i][1] << " )" << endl;
    }
    return 0;
}

