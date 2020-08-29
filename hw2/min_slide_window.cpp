//
// Created by Nick on 2019-10-28.
//
#include <iostream>
#include <deque>
#include <vector>

using namespace std;
int main(){
    int M,K;
    cin >> M >> K;
    vector<int> dp(M);
    for(int i = 0; i < M; i++)
        cin >> dp[i];
    vector<int> stable_dp(M);
    deque<int> min_effort_j;
    vector<int> min_index(M);
    for(int i = 0; i < M; i++) {
        if (min_effort_j.empty()) {
            min_effort_j.push_back(i);
            min_index[i] = i;
        } else {
            if (min_effort_j.front() < i - 2*K) {
                min_effort_j.pop_front();
            }
            if (dp[i] <= dp[min_effort_j.back()]) {
                min_effort_j.pop_back();
                while (!min_effort_j.empty() && dp[i] <= dp[min_effort_j.back()])
                    min_effort_j.pop_back();
            }
            min_effort_j.push_back(i);
            min_index[i] = min_effort_j.front();
        }
    }
    for(int i = 0; i < M; i++)
        cout << min_index[i] << ":" << dp[min_index[i]] << " ";
    cout << endl;
    return 0;
}

