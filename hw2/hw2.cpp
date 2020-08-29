//
// Created by Nick on 2019-10-27.
//
#include <iostream>
#include <vector>
#include <deque>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M, K;
    cin >> N >> M >> K;
    vector<int> notes(N);
    for(int i = 0; i < N; i++)
        cin >> notes[i];
    vector<vector<int>> dp(N + 1);
    dp[0] = vector<int>(M);

    for(int t = 1; t <= N; t++){
        dp[t] = vector<int>(M);
        vector<int> change_dp(M);
        int min_effort_i = 2 * N * M;
        for(int i = 0; i < M; i ++){
            int effort_i = max(abs(i - notes[t - 1]) - K,0) + dp[t - 1][i];
            if(effort_i < min_effort_i ){
                min_effort_i = effort_i;
            }
        }
        for(int j = 0; j < M; j++){
            if(t == 1)
                change_dp[j] = min_effort_i;
            else
                change_dp[j] = min_effort_i + max(abs(j - notes[t - 2]) - K,0);
        }

        vector<int> stable_dp(M);
        deque<int> min_effort_j;
        vector<int> min_index(M+K);
        for(int i = 0; i < M; i++) {
            if (min_effort_j.empty()) {
                min_effort_j.push_back(i);
                min_index[i] = i;
            } else {
                while (min_effort_j.front() < i - 2*K) {
                    min_effort_j.pop_front();
                }
                while (!min_effort_j.empty() && dp[t - 1][i] < dp[t - 1][min_effort_j.back()])
                    min_effort_j.pop_back();
                min_effort_j.push_back(i);
                min_index[i] = min_effort_j.front();
            }
        }
        for(int i = M; i < M + K; i++){
            if (min_effort_j.empty()) {
                min_effort_j.push_back(i);
                min_index[i] = i;
            } else {
                while (min_effort_j.front() < i - 2*K) {
                    min_effort_j.pop_front();
                }

                min_index[i] = min_effort_j.front();
            }
        }
        for(int i = 0; i < M; i++){
            if (t == 1) {
                dp[t][i] = 0;
                continue;
            }

            stable_dp[i] = dp[t - 1][min_index[min(i + K,M + K- 1)]] + max(abs(notes[t - 2] - notes[t - 1])-K,0);

            dp[t][i] = min(stable_dp[i], change_dp[i]);
            if(M > 1){
                dp[t][notes[t - 1]] = 2*N*M;
            }
        }
    }

    int res = 2*N*M;
    for(int i = 0; i < M; i++){
        if(dp[N][i] < res)
            res = dp[N][i];
    }
    cout << res;
    return 0;
}
