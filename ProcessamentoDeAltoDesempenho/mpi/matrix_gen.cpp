#include <bits/stdc++.h>
#define MAXN 40001
#define INF 0x3f3f3f3f

using namespace std;


int main() {
    srand(time(NULL));
    int n = rand() % 50 + 1, m = 28;
    cout << n << " " << m << endl;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(j == m - 1)
                cout << rand() % 100 + 1;
            else
                cout << rand() % 100 + 1 << " ";
        }
        cout << endl;
    }
    return 0;
}
