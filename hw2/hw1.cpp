//
// Created by Nick on 2019-10-22.
//
#include <array>
#include <iostream>
#include <random>

namespace ada {

    class Xoroshiro128 {
    public:
        using result_type = uint32_t;
        static constexpr result_type(min)() { return 0; }
        static constexpr result_type(max)() { return UINT32_MAX; }
        static inline result_type rotl(const result_type x, int k) {
            return (x << k) | (x >> (32 - k));
        }
        Xoroshiro128() : Xoroshiro128(1, 2, 3, 4) {}
        Xoroshiro128(result_type a, result_type b, result_type c, result_type d)
                : s{a, b, c, d} {}
        result_type operator()() {
            const result_type result = rotl(s[0] + s[3], 7) + s[0];
            const result_type t = s[1] << 9;
            s[2] ^= s[0];
            s[3] ^= s[1];
            s[1] ^= s[2];
            s[0] ^= s[3];
            s[2] ^= t;
            s[3] = rotl(s[3], 11);
            return result;
        }

    private:
        std::array<result_type, 4> s;
    };

    namespace {
        int c_lead, c_team;
        Xoroshiro128 rng;
    }  // namespace

    int Init() {
        int n;
        uint32_t s1, s2, s3, s4;
        std::cin >> n >> c_lead >> c_team >> s1 >> s2 >> s3 >> s4;
        rng = Xoroshiro128(s1, s2, s3, s4);
        return n;
    }

    int GetLeadership() { return uint64_t(rng()) * c_lead >> 32; }

    int GetTeamValue() {
        int tmp = int(uint64_t(rng()) * c_team >> 32) + 1;
        return int(c_team / sqrt(tmp));
    }

}  // namespace ada

/*******************************************************************************
 * Example usage:
 * const int kN = 2000000;
 * int leadership[kN], team_value[kN];
 * int main() {
 *   int n = ada::Init(); // Get N
 *   for (int i = 0; i < n; i++) leadership[i] = ada::GetLeadership();
 *   for (int i = 0; i < n; i++) team_value[i] = ada::GetTeamValue();
 * }
 ******************************************************************************/

#include <vector>
using namespace std;

const int kN = 2000000;
int leadership[kN], team_value[kN];

int main(){
    int n = ada::Init(); // Get N

    for (int i = 0; i < n; i++) leadership[i] = ada::GetLeadership();
    for (int i = 0; i < n; i++) team_value[i] = ada::GetTeamValue();

//    int n;
//    cin >> n;
//    for (int i = 0; i < n; i++) cin >> leadership[i];
//    for (int i = 0; i < n; i++) cin >> team_value[i];

//    for (int i = 0; i < n; i++) cout << leadership[i] << " ";
//    cout << endl;
//    for (int i = 0; i < n; i++) cout << team_value[i] << " ";
//    cout << endl;

    vector<long long> dp(n + 1);
    vector<long long> sum_team_value(n + 1);
    vector<long long> sum_dp(n + 1);
    dp[n] = 1;
    sum_team_value[n] = 0;
    sum_dp[n] = 1;
    for(int i = n - 1; i >= 0; i--){
        sum_team_value[i] = sum_team_value[i + 1] + (long long)team_value[i];
        dp[i] = sum_dp[i + 1];
        if (i < n - 1){
            int start = i + 1;
            int end = n - 1;
            int mid = start + (end - start)/2;
            int ret = i + 1;
            while(end >= start){
                mid = start + (end - start)/2;
                if(sum_team_value[i + 1] - sum_team_value[mid + 1] < (long long)leadership[i]){
                    start = mid + 1;
                    ret = start;
                }
                else if(sum_team_value[i + 1] - sum_team_value[mid + 1] > (long long)leadership[i]){
                    end = mid - 1;
                }
                else {
                    ret = mid + 1;
                    break;
                }
            }
            if(ret + 1 <= n){
                if(dp[i] > sum_dp[ret + 1]){
                    dp[i] -= sum_dp[ret + 1];
                    dp[i] %= (long long)(1e9+7);
                }
                else{
                    dp[i] += (long long)(1e9+7);
                    dp[i] -= sum_dp[ret + 1];
                    dp[i] %= (long long)(1e9+7);
                }
            }

        }
        sum_dp[i] = (sum_dp[i + 1] + dp[i])%(long long)(1e9+7);
    }
    cout << dp[0]%(long long)(1e9+7);
    return 0;
}
