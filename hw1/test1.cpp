//
// Created by Nick on 2019-10-10.
//
#include <iostream>
#include <vector>

using namespace std;

long merge(vector<int> &pref,vector<int> &fl, vector<int> &lower,vector<int> &upper, int start, int mid, int end)
{
    int left_size = mid - start +1;
    int right_size = end - mid;
    vector<int> left_p(left_size);
    vector<int> right_p(right_size);
    vector<int> left_fl(left_size);
    vector<int> right_fl(right_size);
    vector<int> left_lower(left_size);
    vector<int> left_upper(left_size);
    vector<int> right_lower(right_size);
    vector<int> right_upper(right_size);
    vector<int> res(left_size);

    for(int i = start; i <= mid ; i++)
    {
        left_p[i - start] = pref[i];
        left_fl[i - start] = fl[i];
        left_lower[i - start] = lower[i];
        left_upper[i - start] = upper[i];
    }
    for(int i = mid + 1; i <= end; i++)
    {
        right_p[i - (mid + 1)] = pref[i];
        right_fl[i - (mid + 1)] = fl[i];
        right_lower[i - (mid + 1)] = lower[i];
        right_upper[i - (mid + 1)] = upper[i];
    }
    int i_left = 0, i_right = 0;
    for(int i = start;i <= end; i++)
    {
        if(i_right == right_size || (i_left < left_size && left_p[i_left] <= right_upper[i_right]))
        {
            res[i_left] = i_right;
            i_left++;
        }
        else
        {
            i_right++;
        }
    }
    int j_left = left_size - 1, j_right = right_size - 1;
    for(int i = start; i <= end; i++)
    {
        if(j_right == -1 || (j_left >= 0 && left_p[j_left] >= right_lower[j_right] ) )
        {
            res[j_left] = max(j_right + 1 - res[j_left],0);
            j_left--;
        }
        else
        {
            j_right--;
        }
    }

    int i_left_p = 0, i_right_p = 0, i_left_lower = 0, i_left_upper = 0, i_right_lower = 0, i_right_upper = 0;
    long res_sum = 0;
    //cout << "res: ";
    for(int i = start;i <= end; i++)
    {
        if(i <= mid)
        {
            res_sum += res[i - start];
            //cout << res[i - start] << " ";
        }

        if(i_right_p == right_size || (i_left_p < left_size && left_p[i_left_p] <= right_p[i_right_p]))
        {
            pref[i] = left_p[i_left_p];
            i_left_p++;
        }
        else
        {
            pref[i] = right_p[i_right_p];
            i_right_p++;
        }

        if(i_right_upper == right_size || (i_left_upper < left_size && left_upper[i_left_upper] <= right_upper[i_right_upper]))
        {
            upper[i] = left_upper[i_left_upper];
            i_left_upper++;
        }
        else
        {
            upper[i] = right_upper[i_right_upper];
            i_right_upper++;
        }

        if(i_right_lower == right_size || (i_left_lower < left_size && left_lower[i_left_lower] <= right_lower[i_right_lower]))
        {
            lower[i] = left_lower[i_left_lower];
            i_left_lower++;
        }
        else
        {
            lower[i] = right_lower[i_right_lower];
            i_right_lower++;
        }
    }
    //cout << endl;
    return res_sum;
}

long dc(vector<int> &pref,vector<int> &fl,vector<int> &lower,vector<int> &upper, int start, int end )
{
    if(start == end)
        return 0;
    long res = 0;
    int mid = (start + end - 1) / 2;
//    cout << "--------------------------------------" << endl;
//    cout << "start dc: start,mid,end = " << start << "," << mid << "," << end << endl;
//    cout << "i" << endl;
//    for(int i = start; i <= end; i++)
//        cout << i << " ";
//    cout << endl << "pref:";
//    for(int i = start; i <= end; i++)
//        cout << pref[i] << " ";
//    cout << endl << "lower:";
//    for(int i = start; i <= end; i++)
//        cout << lower[i] << " ";
//    cout << endl << "upper:";
//    for(int i = start; i <= end; i++)
//        cout << upper[i] << " ";
//    cout << endl;
//    cout << "--------------------------------------" << endl;

    res += dc(pref, fl, lower, upper, start, mid);

//    cout << "After left ( " << start << " , " <<mid << " ) res: " << res <<endl;

    res += dc(pref, fl, lower, upper, mid+1, end);

//    cout << "After right ( " << mid + 1 << " , " << end << " )res: " << res <<endl;

    res += merge(pref, fl, lower, upper, start, mid, end);

//    cout << "--------------------------------------" << endl;
//    cout << "i" << endl;
//    for(int i = start; i <= end; i++)
//        cout << i << " ";
//    cout << endl << "pref:";
//    for(int i = start; i <= end; i++)
//        cout << pref[i] << " ";
//    cout << endl << "lower:";
//    for(int i = start; i <= end; i++)
//        cout << lower[i] << " ";
//    cout << endl << "upper:";
//    for(int i = start; i <= end; i++)
//        cout << upper[i] << " ";
//    cout << endl;
//    cout << "end dc: start,mid,end,res = " << start << "," << mid << "," << end << "," << res <<endl;
//    cout << "--------------------------------------" << endl;
    return res;
}

int test1()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    cin >> N;
    vector<int> pref(N);
    vector<int> fl(N);
    vector<int> lower(N);
    vector<int> upper(N);
    for (int i = 0; i < N; i++) {
        cin >> pref[i];
        //pref[i] = rand();
    }
    for (int i = 0; i < N; i++) {
        cin >> fl[i];
        //fl[i] = rand();
        lower[i] = pref[i] - fl[i];
        upper[i] = pref[i] + fl[i];
    }
    long res = dc(pref, fl, lower, upper, 0, N - 1);
    cout << res;
    return 0;
}
