//
// Created by Nick on 2019-10-20.
//
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
    int M[W+1];
    int old_M[W+1];
    for(int i = 0; i < W + 1; i++){
        M[i] = 0;
        old_M[i] = 0;
    }
    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= W; j++){
            if(obj_weight[i - 1] > j)
                ;//M[j] = old_M[j];
            else{
                M[j] = max( old_M[j], old_M[j - obj_weight[i - 1]]+obj_value[i - 1]);
            }
        }
        for(int k = 0; k <= W; k++){
            old_M[k] = M[k];
            cout << M[k] <<" ";
        }
        cout << endl;

    }

    for(int i = 0; i <= W; i++)
            cout << M[i] << " ";
    cout << endl;
    cout << M[W];

    return 0;
}
