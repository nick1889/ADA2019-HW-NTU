#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>
#include <ctime>
#include <cfloat>
#include <cstring>
#include <set>
#include <cstdio>
#include "hex.h"

using namespace std;
typedef pair<int, int> pii;

const int maxn = 11;

bool board[maxn][maxn];
bool myboard[maxn][maxn];
bool rivalboard[maxn][maxn];

int n;

void init(int _n) {
    n = _n;
    memset(board, 0, sizeof board);
    memset(myboard, 0, sizeof myboard);
    srand(12345);
}
pii decide(pii p) {
    if (p != make_pair(-1, -1)){
        board[p.first][p.second] = true;
        rivalboard[p.first][p.second] = true;
    }

    vector<pii> unused;
    pii res;
    if(n == 3){
        if( p.first == -1 and p.second == -1){
            res = {1, 1};
        }
        else{
            if(p.first == 1 or p.first == 0){
                if(!myboard[0][1] and !myboard[0][2])
                    if(!board[0][1]) res = {0,1}; else res = {0, 2};
            }
            else{
                if(!myboard[2][0] and !myboard[2][1])
                    if(!board[2][0]) res = {2,0}; else res = {2, 1};
            }
        }
    }
    if(n == 4){
        if( p.first == -1 and p.second == -1){
            res = {1, 2};
        }
        else{
            if(p.first == 0 or p.first == 1){
                if(!myboard[0][2] and !myboard[0][3]) {
                    if(!board[0][2]) res = {0, 2}; else res = {0, 3};
                }else if(!myboard[2][1] and !myboard[2][2]){
                    if(!board[2][1]) res = {2, 1}; else res = {2, 2};
                }else{
                    if(myboard[2][1] and !board[3][0]) res = {3, 0};
                    if(myboard[2][1] and !board[3][1]) res = {3, 1};
                    if(myboard[2][2] and !board[3][1]) res = {3, 1};
                    if(myboard[2][2] and !board[3][2]) res = {3, 2};
                }
            }
            else if(p.first == 2){
                if(!myboard[2][1] and !myboard[2][2]) {
                    if (!board[2][1]) res = {2, 1}; else res = {2, 2};
                } else if(!myboard[0][2] and !myboard[0][3]){
                    if(!board[0][2]) res = {0, 2}; else res = {0, 3};
                } else{
                    if(myboard[2][1] and !board[3][0]) res = {3, 0};
                    if(myboard[2][1] and !board[3][1]) res = {3, 1};
                    if(myboard[2][2] and !board[3][1]) res = {3, 1};
                    if(myboard[2][2] and !board[3][2]) res = {3, 2};
                }
            }
            else{
                if(!myboard[3][1] and !myboard[3][2]){
                    if(!board[3][1]) res = {3, 1}; else res = {3, 2};
                }else if(!myboard[0][2] and !myboard[0][3]){
                    if(!board[0][2]) res = {0, 2}; else res = {0, 3};
                } else{
                    if(!board[2][1]) res = {2, 1}; else res = {2, 2};
                }
            }
        }
    }
    else{
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(!board[i][j]){
                    unused.push_back({i,j});
                }
            }
        }
        res = unused[rand() % unused.size()];
    }
    board[res.first][res.second] = true;
    myboard[res.first][res.second] = true;
    return res;
}
