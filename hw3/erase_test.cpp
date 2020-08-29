//
// Created by Nick on 2019-12-6.
//
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(){
    vector<vector<int>> v={{4,5,6,0,1,2,4,5,6},{4,5,6,4,5,6,5,1,2}};
    sort(v[0].begin(),v[0].end());
    sort(v[1].begin(),v[1].end());
    v[0].erase(unique(v[0].begin(),v[0].end()),v[0].end());
    v[1].erase(unique(v[1].begin(),v[1].end()),v[1].end());
    for(int j = 0; j < v.size(); j++){
        for(int i = 0; i < v[j].size(); i++){
            cout << v[j][i] << ' ';
        }
        cout << '\n';
    }
    cout << v[1].back() << '\n';
    vector<int> a;
    a.reserve(10);
    cout << a.size() << '\n';
    return 0;
}
