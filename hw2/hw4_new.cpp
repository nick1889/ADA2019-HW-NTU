//
// Created by Nick on 2019-10-27.
//
#include <iostream>
#include <vector>
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

    vector<int> my_comb(7);
    my_comb[0] = my_ship[0] + my_ship[1] + my_ship[2];
    my_comb[1] = my_ship[0] + my_ship[1];
    my_comb[2] = my_ship[0] + my_ship[2];
    bool bc_larger = true;
    if(my_ship[1] + my_ship[2] > my_ship[0]) {
        my_comb[3] = my_ship[1] + my_ship[2];
        my_comb[4] = my_ship[0];
    }
    else {
        bc_larger = false;
        my_comb[3] = my_ship[0];
        my_comb[4] = my_ship[1] + my_ship[2];
    }
    my_comb[5] = my_ship[1];
    my_comb[6] = my_ship[2];

    vector<int> enemy(N);
    vector<int> enemy_type_hist(7);
    bool succeed = true;
    for(int i = 0; i < N; i++){
        cin >> enemy[i];
        for(int j = 6; j >= 0; j--){
            if(my_comb[j] >= enemy[i]){
                enemy_type_hist[j]++;
                break;
            }
            if(j == 0){
                succeed = false;
                break;
            }
        }
    }
//    for(int i = 0; i < 7; i++) cout << my_comb[i] << " ";
//    cout << endl;
//    for(int i = 0; i < 7; i++)
//        cout << enemy_type_hist[i] << " ";
//    cout << endl;
    if(!succeed)
        cout << -1;
    else{
        int res = enemy_type_hist[0] + enemy_type_hist[1] + enemy_type_hist[2];
        enemy_type_hist[6] = max(enemy_type_hist[6] - enemy_type_hist[1], 0);
        if(enemy_type_hist[5] >= enemy_type_hist[2]){
            enemy_type_hist[5] -= enemy_type_hist[2];
        }
        else{
            enemy_type_hist[6] = max(0, enemy_type_hist[6] - enemy_type_hist[2] + enemy_type_hist[5]);
            enemy_type_hist[5] = 0;
        }

        int minab = 1e8;
        if(bc_larger){
            int w = enemy_type_hist[3];
            int x = enemy_type_hist[4];
            int y = enemy_type_hist[5];
            int z = enemy_type_hist[6];
            int alpha = enemy_type_hist[3];
            for(; alpha <= 2e5; alpha++){
                int b1 = w + x - 2 * alpha;
                int b2 = (int)ceil((double)(w + x + y - 2 * alpha) / 2);
                int b3 = (int)ceil((double)(w + x + y + z - 2 * alpha) / 3);
                int beta = max(max(max(b1,0), max(b2,0)), max(b3,0));
                if(alpha + beta < minab)
                    minab = alpha + beta;
            }
        }
        else{
            int w = enemy_type_hist[3];
            int x = enemy_type_hist[4];
            int y = enemy_type_hist[5];
            int z = enemy_type_hist[6];
            int alpha = 0;
            for(; alpha <= 2e5; alpha++){
                int b0 = w - alpha;
                int b1 = w + x - 2 * alpha;
                int b2 = (int)ceil((double)(w + x + y - 2 * alpha) / 2);
                int b3 = (int)ceil((double)(w + x + y + z - 2 * alpha) / 3);
                int beta = max(max(max(b1,0), max(b2,0)), max(max(b0,0),max(b3,0)));
                if(alpha + beta < minab)
                    minab = alpha + beta;
            }
        }
        res += minab;
        cout << res;
    }

return 0;
}