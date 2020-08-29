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
    vector<int> min_lr(1e6);
    vector<int> min_ll(1e6);
    int bias = 3e5;
    min_lr[min_value[mid + 1 - low] + mid + 1] = 1;
    min_ll[mid - min_value[mid - low] + bias] = 1;
    for(int i = mid - 1; i >= low; i--)
    {
        min_value[i - low] = P[i] < min_value[i - low + 1]? P[i]:min_value[i - low + 1];
        max_value[i - low] = P[i] > max_value[i - low + 1]? P[i]:max_value[i - low + 1];
        min_ll[i - min_value[i - low] + bias] += 1;
    }
    for(int i = mid + 2; i <= high; i++)
    {
        min_value[i - low] = P[i] < min_value[i - low - 1]? P[i]:min_value[i - low - 1];
        max_value[i - low] = P[i] > max_value[i - low - 1]? P[i]:max_value[i - low - 1];
        min_lr[min_value[i - low] + i] += 1;
    }
    //left_min,left_max : max - min = |S| - 1
    int count_res = 0;
    for(int i = mid; i >= 0; i--)
    {
        int S = max_value[i - low] - min_value[i - low];
        if( S > 0 && i + S <= high && i + S > mid)
        {
            if(min_value[i - low] < min_value[i + S - low] && max_value[i - low] > max_value[i + S - low] )
                count_res ++;
        }
    }
    //right_max,right_min : max - min = |S| - 1
    int count_rr = 0;
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
    int right_i = mid + 1, left_i = mid + 1;
    int i = mid;
    while(right_i <= high && max_value[right_i - low] < max_value[mid - low])
        right_i++;
    while(left_i <= high && min_value[left_i - low] > min_value[mid - low])
        left_i++;
    if(left_i < right_i)
        count_res += min_lr[max_value[mid - low] + mid];
    for(int j = mid - 1; j >= low; j--)
    {
        while(max_value[j - low] > max_value[right_i - low])
            right_i++;
        while(min_value[j - low] < min_value[left_i - low])
        {
            min_lr[min_value[left_i - low] + left_i]--;
            left_i++;
        }
        if(left_i < right_i)
            count_res += min_lr[max_value[j - low] + j];
    }
    //right_max, left_min : l - min(l) = r - max(r)
    left_i = mid + 1, right_i = mid + 1;
    while(left_i <= high && max_value[left_i - low] > max_value[mid - low])
        left_i++;
    while(right_i <= high && min_value[right_i - low] < min_value[mid - low])
        right_i++;
    if(left_i < right_i)
        count_res += min_ll[mid - min_value[mid - low] + bias];
    for(int j = mid - 1; j >= low; j--)
    {
        while(max_value[j - low] < max_value[left_i - low])
        {
            left_i++;
            min_ll[left_i - max_value[left_i] + bias]--;
        }
        while(min_value[j - low] > min_value[right_i - low])
        {
            //min_lr[min_value[left_i - low] + left_i]--;
            right_i++;
        }
        if(left_i < right_i)
            count_res += min_ll[j - min_value[j - low] + bias];
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
