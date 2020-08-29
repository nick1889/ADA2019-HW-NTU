//
// Created by Nick on 2019-10-24.
//
#include <iostream>
#include <vector>
#include <list>
#include <bits/stdc++.h>
#define MY_SHIP_NUM 3
using namespace std;
int main(){
    int N;
    cin >> N;
    vector<int> my_ship(MY_SHIP_NUM);
    for(int i = 0; i < MY_SHIP_NUM; i++)
        cin >> my_ship[i];
    sort(my_ship.begin(),my_ship.end(),greater<>());
    vector<int> enemy(N);
    for(int i = 0; i < N; i++)
        cin >> enemy[i];
    sort(enemy.begin(),enemy.end(),greater<>());
    vector<vector<int>> my_comb(5);
    my_comb[0] = vector<int>(1);
    my_comb[0][0] = my_ship[0] + my_ship[1] + my_ship[2];
    my_comb[1] = vector<int>(2);
    my_comb[1][0] = my_ship[0] + my_ship[1];
    my_comb[1][1] = my_ship[2];
    my_comb[2] = vector<int>(2);
    my_comb[2][0] = my_ship[0]+my_ship[2];
    my_comb[2][1] = my_ship[1];
    if(my_ship[0] <= my_ship[1] + my_ship[2]){
        my_comb[3] = vector<int>(2);
        my_comb[3][0] = my_ship[1]+my_ship[2];
        my_comb[3][1] = my_ship[0];
        my_comb[4] = vector<int>(3);
        my_comb[4][0] = my_ship[0];
        my_comb[4][1] = my_ship[1];
        my_comb[4][2] = my_ship[2];
    }
    else{
        my_comb[3] = vector<int>(3);
        my_comb[3][0] = my_ship[0];
        my_comb[3][1] = my_ship[1];
        my_comb[3][2] = my_ship[2];
        my_comb[4] = vector<int>(2);
        my_comb[4][0] = my_ship[1]+my_ship[2];
        my_comb[4][1] = my_ship[0];
    }
    int res = 0;
    if(enemy[0] > my_comb[0][0])
        res = -1;
    else{
        int first_i = -1;
        int second_i = -1;
        for(int i = 0; i < N; i++){
            if(enemy[i] > my_comb[1][0]){
                res += 1;
                first_i = i;
            }
            if(enemy[i] > my_ship[0]){
                second_i = i;
            }
        }

        vector<bool>flag(N);

        for(int i = first_i + 1; i < N; i++){
            if(flag[i] == true)
                continue;
            int j = 0;
            for(j = 0; j <= 4 && my_comb[j][0] >= enemy[i]; j++)
                ;
            int comb_i = j - 1;
            flag[i] = true;
            res += 1;
            int inner_i = second_i + 1;
            bool flag_flag = true;
            for(int k = 1; k < my_comb[comb_i].size();k++){
                int left = second_i + 1;
                int right = N - 1;
                int killed_idx = 0;
                while(right - left >= 1) {
                    int mid = (left + right - 1) / 2;
                    if (enemy[mid] == my_comb[comb_i][k]) {
                        killed_idx = mid;
                        break;
                    }
                    if (my_comb[comb_i][k] > enemy[mid]) {
                        right = mid;
                    } else {
                        left = mid;
                    }
                }
                    if(flag[inner_i] == true){
                        if(flag_flag)
                            second_i += 1;

                    }
                    else{
                        flag_flag = false;
                        if(my_comb[comb_i][k] >= enemy[inner_i]){
                            flag[inner_i] = true;
                            inner_i++;
                            break;
                        }
                    }
                    inner_i++;
                }
            }
        }
    }
    cout << res <<endl;
    return 0;
}

