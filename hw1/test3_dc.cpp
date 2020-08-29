//
// Created by Nick on 2019-10-14.
// Discussed with Rui? Zhou

#include <iostream>
#include <vector>

using namespace std;

int cross_count(vector<int> &P, int low, int mid, int high)
{
    vector<int> min_value(high - low + 1),max_value(high - low + 1);
    min_value[mid - low] = P[mid]; max_value[mid - low] = P[mid];
    min_value[mid + 1 - low] = P[mid + 1]; max_value[mid +1 - low] = P[mid + 1];
    for(int i = mid - 1; i >= low; i--)
    {
        min_value[i - low] = P[i] < min_value[i - low + 1]? P[i]:min_value[i - low + 1];
        max_value[i - low] = P[i] > max_value[i - low + 1]? P[i]:max_value[i - low + 1];
    }
    for(int i = mid + 2; i <= high; i++)
    {
        min_value[i - low] = P[i] < min_value[i - low - 1]? P[i]:min_value[i - low - 1];
        max_value[i - low] = P[i] > max_value[i - low - 1]? P[i]:max_value[i - low - 1];
    }
    //left_min,left_max : max - min = |S| - 1
    int count_res = 0;
    for(int i = mid; i >= low; i--)
    {
        int S = max_value[i - low] - min_value[i - low];
        if( S > 0 && i + S <= high && i + S > mid)
        {
            if(min_value[i - low] < min_value[i + S - low] && max_value[i - low] > max_value[i + S - low] )
                count_res ++;
        }
    }
    //right_max,right_min : max - min = |S| - 1
    for(int i = mid + 1; i <= high; i++)
    {
        int S = max_value[i - low] - min_value[i - low];
        if( S > 0 && i - S >= low && i - S <= mid)
        {
            if(min_value[i - low] < min_value[i - S - low] && max_value[i - low] > max_value[i - S - low] )
                count_res ++;
        }
    }
    //left_max, right_min : max(r) - min(l) = r - l => max(r) + l = min(l) + r
    for(int l = mid; l >= low; l--)
        for(int r = mid + 1; r <= high; r++)
        {
            if(max_value[l - low] - min_value[r - low] == r - l)
            {
                if(max_value[l - low] > max_value[r - low] && min_value[r - low] < min_value[l - low])
                    count_res++;
            }
        }
    //right_max, left_min : l - min(l) = r - max(r)
    for(int l = mid; l >= low; l--)
        for(int r = mid + 1; r <= high; r++)
        {
            if(max_value[r - low] - min_value[l - low] == r - l)
            {
                if(max_value[r - low] > max_value[l - low] && min_value[l - low] < min_value[r - low])
                    count_res++;
            }
        }
    return count_res;

}

int subset_count(vector<int> &P, int low,int high)
{
    if(low == high)
    {
        return 1;
    }
    int mid = (low + high - 1) / 2;
    int left_res = subset_count(P,low,mid);
    int right_res = subset_count(P,mid+1,high);
    int cross_res = cross_count(P,low,mid,high);
    return left_res + right_res + cross_res;
}

int main()
{
    int N;
    cin >> N;
    vector<int> P(N);
    for(int i = 0; i < N; i++)
    {
        cin >> P[i];
    }
    int res = subset_count(P,0,N-1);
    cout << res;
    return 0;
}
