//
// Created by Nick on 2019-10-14.
//
#include <iostream>
#include <vector>

using namespace std;

long find_max_subarray(vector<int> &A, int N)
{
    long max_sum = 0;
    long cur_sum = 0;
    for(int i = 0; i <= N - 1; i++)
    {
        if(cur_sum < 0)
            cur_sum = A[i];
        else
            cur_sum += A[i];
        if(cur_sum > max_sum)
            max_sum = cur_sum;
    }
    return max_sum;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N = 0, Q = 0;
    cin >> N >> Q;
    vector<int> A(N);
    vector<int> p(Q),v(Q);
    for(int i = 0; i < N; i++)
    {
        cin >> A[i];
    }
    for(int i = 0; i < Q; i++)
    {
        cin >> p[i] >> v[i];
    }
    long res = find_max_subarray(A,N);
    if(res <= 0)
        cout << 0 << endl;
    else
        cout << res << endl;

    for(int i = 0; i < Q; i++)
    {
        A[p[i] - 1] = v[i];
        res = find_max_subarray(A,N);
        if(res <= 0)
            cout << 0 << endl;
        else
            cout << res <<endl;
    }
    return 0;
}