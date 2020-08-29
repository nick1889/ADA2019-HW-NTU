//
// Created by Nick on 2019-10-19.
//
#include <iostream>

using namespace std;

int main()
{
    int N = 6;
    int values[] = {1,3,3,4,1,3};
    int finish_time[] = {2,4,5,6,7,8};
    int start_time[] = {0,0,2,2,6,5};
    int p[N];
    int max_values[N + 1];
    for(int i = N - 1; i >= 0; i--)
    {
        p[i] = 0;
        for(int j = i - 1; j >= 0; j--)
        {
            if(finish_time[j] <= start_time[i])
            {
                p[i] = j + 1;
                break;
            }
        }
    }
    for(int i = 0; i < N; i++)
        cout << p[i] << " ";
    cout << endl;
    max_values[0] = 0;
    for(int i = 1; i <= N; i++)
    {
        max_values[i] = max(values[i - 1] + max_values[p[i - 1]],max_values[i - 1]);
    }
    for(int i = 0; i < N + 1; i++)
        cout << max_values[i] << " ";
}
