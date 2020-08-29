//
// Created by Nick on 2019-10-14.
// Discuss with Chengyou Jia.
//
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    string s1 ;
    string s2 ;
    cin >> s1;
    cin >> s2;
    int l1 = s1.length(), l2 = s2.length();
    int l = min(l1,l2);
    vector< vector<int> > d(l1+1,vector<int>(l2+1));
    for(int i = 0; i <= l1+1; i++)
    {
        vector<int> s(l2+1);
        d.push_back(s);
    }
    d[l1][l2] = 0;
    for(int i = 1; i <= l1; i++)
        d[l1][0] = 0;
    for(int j = 1; j <= l2; j++)
        d[0][l2] = 0;

    for(int i = l1; i > 0; i--)
        for(int j = l2; j > 0; j--)
        {
            if(s1[i - 1] != s2[j - 1])
                d[i - 1][j - 1] = d[i][j];
            else
            {
                d[i - 1][j - 1] = min(min( d[i][j-1], d[i-1][j]), d[i][j]) + 1;
            }
        }
//    for(int i = 0;i <= l1; i++)
//    {
//        if(i > 0)
//            cout << endl;
//        for(int j = 0; j <= l2; j++)
//            cout << d[i][j] << " ";
//    }
//    cout << endl;
//    cout << s1 << endl;
//    cout << s2 << endl;
    cout << d[0][0];
    return 0;
}

