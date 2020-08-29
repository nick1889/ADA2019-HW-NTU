//
// Created by Nick on 2019-10-19.
//

#include <iostream>
using namespace std;
int main(){
    int W = 5; // capacity
    int N = 3;
    int obj_weight[] = {1,2,4};
    int obj_value[] = {4,9,17};
    int M[W+1];
    M[0] = 0;
    for(int i = 1; i <= W; i++){
        int max_value = 0;
        for(int j = 1; j <= N; j++){
            if(obj_weight[j - 1] <= i){
                max_value = max(M[i - obj_weight[j - 1]] + obj_value[j - 1], max_value);
            }
        }
        M[i] = max_value;
    }

    for(int i = 0; i <= W; i++){
        cout << M[i] << " ";
    }
    cout << endl;
    cout << M[W];

    return 0;
}