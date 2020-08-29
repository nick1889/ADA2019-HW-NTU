//
// Created by Nick on 2019-10-15.
//

//
// Created by Nick on 2019-10-14.
//
#include <iostream>
#include <map>

using namespace std;

int main()
{
    string s1;
    string s2;
    cin >> s1;
    cin >> s2;
    int l1 = s1.length(), l2 = s2.length();
    if(l1 > l2)
    {
        string t = s1;
        s1 = s2;
        s2 = t;
        l1 = s1.length();
        l2 = s2.length();
    }
    map<int,vector<int>> d;
    for(int i = 0; i < l1; i++)
    {

    }

    cout << 0;
}

