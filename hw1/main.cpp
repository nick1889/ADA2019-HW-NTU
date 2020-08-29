/*
#include<iostream>
#include<vector>
using namespace std;

int merge(vector<int> pref, vector<int> fl, int start, int mid, int end)
{
    vector<int> left(pref.begin() + start, pref.begin() + mid);
    vector<int> right(pref.begin() + mid + 1, pref.begin() + end);
    vector<int> left_fl(fl.begin() + start, fl.begin() + mid);
    vector<int> right_fl(fl.begin() + mid + 1, fl.begin() + end);

    int i_left = left.size() - 1, i_right = right.size() - 1;
    int res = 0;
    for (int i = end; i >= start; i--)
    {
        if ((i_left >= 0) && (left[i_left] - left_fl[i_left] <= right[i_right]))
        {
            res += i_right + 1;
            //pref[i] = left[i_left];
            i_left--;
        }
        else
        {
            //pref[i] = right[i_right];
            i_right--;
        }
    }
    i_left = left.size() - 1, i_right = right.size() - 1;
    for (int i = end; i >= start; i--)
    {
        if ((i_left >= 0) && (left[i_left] + left_fl[i_left] >= right[i_right]))
        {
            res += i_right + 1;
            //pref[i] = left[i_left];
            i_left--;
        }
        else
        {
            //pref[i] = right[i_right];
            i_right--;
        }
    }
    i_left = 0, i_right = 0;
    for (int i = start; i <= end; i++)
    {
        if ((i_left <= left.size()-1) && (left[i_left] <= right[i_right]))
        {
            pref[i] = left[i_left];
            i_left++;
        }
        else
        {
            pref[i] = right[i_right];
            i_right++;
        }
    }

    return res;
}

int dc(vector<int> pref, vector<int> fl, int start, int end)
{
    int res = 0;
    if (start < end)
    {
        int mid = (start + end) / 2;

        res += dc(pref, fl, start, mid);
        res += dc(pref, fl, mid + 1, end);
        res += merge(pref, fl, start, mid, end);
    }
    return res;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    cin >> N;
    vector<int> pref;
    vector<int> fl;
    for (int i = 0; i < N; i++)
    {
        cin >> pref[i];
    }
    for (int i = 0; i < N; i++)
    {
        cin >> fl[i];
    }
    int res = 0;
    res = dc(pref, fl, 0, N - 1);
    cout << res;
    return 0;
}
 */