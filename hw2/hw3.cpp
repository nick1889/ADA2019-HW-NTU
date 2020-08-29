//
// Created by Nick on 2019-11-5.
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Pokemon{
public:
    int A;
    int B;
    Pokemon(int a, int b):A(a),B(b){}
};
bool comp(const Pokemon &a, const Pokemon &b)
{
    if (a.B == 0 && b.B != 0)
        return false;
    else if(a.B != 0 && b.B == 0)
        return true;
    else if(a.B == 0 && b.B == 0)
        return a.A < b.A;
    else
        return (float)a.A/(float)a.B < (float)b.A/(float)b.B;
}

int main(){
    int K, N;
    cin >> N >> K;
    vector<Pokemon> candidates;
    for(int i = 0; i < N; i++){
        int a, b;
        cin >> a >> b;
        Pokemon *p = new Pokemon(a,b);
        candidates.push_back(*p);
    }
    sort(candidates.begin(),candidates.end(),comp);
    if(N == K){
        int res = 0;
        int E = candidates[0].B;
        for(int i = 1; i < N; i++){
            res += candidates[i].A * E;
            E += candidates[i].B;
        }
        cout << res;
    }
    else{
        vector<vector<int>> dp(110);
        for(int i = 0; i < 110;i++){
            dp[i] = vector<int>(10100);
            for(int j = 1; j < 10100; j++){
                dp[i][j] = -2e9;
            }
        }
        for(int n = 0; n < N; n++){
            for(int i = min(n + 1,K); i >= 1; i--){
                for(int j = 0; j < 10100; j++){
                    if(j >= candidates[n].B){
                        dp[i][j] = max(dp[i][j], dp[i-1][j-candidates[n].B]+candidates[n].A*(j-candidates[n].B));
                    }
                }
            }
        }
        int res = 0;
        for(int j = 0; j < 10100; j++)
            res = max(res, dp[K][j]);
        cout << res;
    }

    return 0;
}