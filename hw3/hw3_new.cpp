//
// Created by Nick on 2019-12-4.
//
#pragma GCC optimize("O3")
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

enum class Color {WHITE,GRAY,BLACK};

class V{
public:
    int s;
    int t;
    long long d = 0;
    vector<V* > adj;
    Color color = Color::WHITE;
    V(int s1, int t1):s(s1),t(t1){}
};

bool dfs(V* v, vector<V*> &orders){
    bool cycle = false;
    if(v->s != 0) {
        v->color = Color::GRAY;
    }
    for(V* u:v->adj){
        if(u->color == Color::WHITE){
            cycle = dfs(u, orders);
            if(cycle){
                return true;
            }
        }
        else if(u->color == Color::GRAY){
            return true;
        }
    }
    if(v->s != 0) {
        v->color = Color::BLACK;
    }
    orders.push_back(v);
    return cycle;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int T, N, M;
    cin >> T;
    for(int t = 0; t < T; t++){
        cin >> N >> M;
        vector<int> a(N);
        for(int i = 0; i < N; i++){
            cin >> a[i];
        }
        vector<vector<int>> vertices(N,vector<int>(0));
        vector<pair < pair<int,int>, pair<int,int> > > inputs(0);
        vector<unordered_map<int,V*>> nodes(N,unordered_map<int,V*>());
        bool cycle_flag = false;
        inputs.reserve(M);
        for(int i = 0; i < M; i++) {
            int s1, s2;
            int t1, t2;
            cin >> s1 >> t1 >> s2 >> t2;
            if (s1 == s2) {
                if (t2 <= t1) {
                    cycle_flag = true;
                } else {
                    continue;
                }
            }
            inputs.emplace_back( make_pair(s1,t1),make_pair(s2,t2) );
            vertices[s1 - 1].push_back(t1);
            vertices[s2 - 1].push_back(t2);
        }
        if(cycle_flag){
            cout << "LoveLive!\n";
            continue;
        }
        V* start = new V(0, 1);
        V* end = new V(0, 0);
        for(int i = 0; i < N; i++) {
            sort(vertices[i].begin(),vertices[i].end());
            vertices[i].erase(unique(vertices[i].begin(),vertices[i].end()),vertices[i].end());
            V *last = nullptr;
            if(vertices[i].empty()){
                vertices[i].push_back(a[i]);
                last = new V(i + 1, a[i]);
                start->adj.push_back(last);
                nodes[i].insert(make_pair(a[i],last));
            }
            else{
                last = new V(i + 1, vertices[i][0]);
                start->adj.push_back(last);
                nodes[i].insert(make_pair(vertices[i][0],last));
                for(int j = 1; j < vertices[i].size(); j++) {
                    V* v = new V(i + 1,vertices[i][j]);
                    last->adj.push_back(v);
                    last = v;
                    nodes[i].insert(make_pair(vertices[i][j],v));
                }
                if(last->t != a[i]){
                    V* v = new V(i+1, a[i]);
                    last->adj.push_back(v);
                    last = v;
                }
            }
            last->adj.push_back(end);
        }

        for(auto & input : inputs) {
            V* v1 = nodes[(input.first.first) - 1].find(input.first.second)->second;
            V* v2 = nodes[(input.second.first) - 1].find(input.second.second)->second;
            v1->adj.push_back(v2);
        }
        vector<V*> orders(0);
        orders.reserve(2*inputs.size());
        bool cycle = dfs(start, orders);
        if(cycle){
            cout << "LoveLive!\n";
            continue;
        }
        else{
            long long res = 0;
            for(int i = (int)orders.size() - 1; i >= 0; i-- ) {
                for (V *v:orders[i]->adj) {
                    if(v == end){
                        res = max(res, orders[i]->d);
                        continue;
                    }
                    if (orders[i]->s == v->s) {
                        v->d = max(v->d, orders[i]->d + ((long long)v->t - (long long)orders[i]->t));
                    }
                    else if (orders[i]->s == 0){
                        v->d = max(v->d, orders[i]->d + ((long long)v->t - (long long)orders[i]->t + 1));
                    }
                    else{
                        v->d = max(v->d, orders[i]->d + 1);
                    }
                }
            }
            cout << res << '\n';
        }
    }

    return 0;
}