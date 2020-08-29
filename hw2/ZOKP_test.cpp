//
// Created by Nick on 2019-10-19.
//

#include <iostream>
 using namespace std;
int main(){
    int W = 15; // capacity
    int N = 7;
    int obj_weight[] = {1,3,4,5,8,10,11};
    int obj_value[] = {3,7,10,12,17,19,21};
    int M[N+1][W+1];
    for(int i = 0; i < N + 1; i++){
        M[i][0] = 0;
    }
    for(int i = 0; i < W + 1; i++){
        M[0][i] = 0;
    }
    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= W; j++){
            if(obj_weight[i - 1] > j)
                M[i][j] = M[i - 1][j];
            else{
                M[i][j] = max( M[i - 1][j], M[i - 1][j - obj_weight[i - 1]]+obj_value[i - 1]);
            }
        }
    }

    for(int i = 0; i <= N; i++){
        for(int j = 0; j <= W; j++)
            cout << M[i][j] << " ";
        cout << endl;
    }
    cout << M[N][W];

    return 0;
}