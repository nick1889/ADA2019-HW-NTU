//
// Created by Nick on 2019-12-4.
//

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

enum class Color {WHITE,GRAY,BLACK};
enum class VType {START,END,BOTH};

class V{
public:
    int s;
    int t;
    long long d = 0;
    vector<V* > adj;
    Color color = Color::WHITE;
    VType vtype;
    V(int s1, int t1, VType vt):s(s1),t(t1),vtype(vt){}
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
        vector<unordered_map<long long,V*>> vertices(N,unordered_map<long long,V*>());
        vector<vector<int>> keys(N,vector<int>(0));
        bool cycle_flag = false;
        for(int i = 0; i < M; i++){
            int s1, t1, s2, t2;
            cin >> s1 >> t1 >> s2 >> t2;
            if(s1 == s2){
                if(t2 <= t1){
                    cycle_flag = true;
                }
                else{
                    continue;
                }
            }
            V* v1;
            V* v2;
            if(!vertices[s1 - 1].count(t1)){
                v1 = new V(s1,t1,VType::END);
                vertices[s1 - 1][t1] = v1;
            }
            else{
                v1 = vertices[s1 - 1][t1];
                if(v1->vtype == VType::START){
                    v1->vtype = VType::BOTH;
                }
            }
            if(!vertices[s2 - 1].count(t2)) {
                v2 = new V(s2, t2, VType::START);
                vertices[s2 - 1][t2] = v2;
            }
            else{
                v2 = vertices[s2 - 1][t2];
                if(v2->vtype == VType::END){
                    v2->vtype = VType::BOTH;
                }
            }
            v1->adj.push_back(v2);
        }
        if(cycle_flag){
            cout << "LoveLive!" << '\n';
            continue;
        }

        V* start = new V(0,1,VType::START);
        for(int i = 0; i < N; i++){
            if(vertices[i].empty()){
                V *v = new V(i + 1, a[i], VType::END);
                vertices[i][a[i]] = v;
            }
            start->adj.push_back(vertices[i].begin()->second);
            auto iter1 = vertices[i].begin();
            auto iter2 = ++vertices[i].begin();
            for(; iter2 != vertices[i].end();iter1++,iter2++){
                iter1->second ->adj.push_back(iter2 -> second);
            }
        }
/*
        vector<V*> orders(0);
        bool cycle = dfs(start, orders);
        if(cycle){
            cout << "LoveLive!" << '\n';
            continue;
        }
        else{
            long long res = 0;
            for(int i = orders.size() - 1; i >= 0; i-- ) {
                for (V *v:orders[i]->adj) {
                    long long extra = 0;
                    if (orders[i]->s == 0 or orders[i]->s == v->s) {
                        if (orders[i]->vtype == VType::START || orders[i]->vtype == VType::BOTH) {
                            if (!(v->vtype == VType::START)) {
                                extra++;
                            }
                        }
                        else{
                            if(v->vtype == VType::START){
                                extra--;
                            }
                        }
                        v->d = max(v->d, orders[i]->d + (v->t - orders[i]->t + extra));
                    }
                    else{
                        v->d = max(v->d, orders[i]->d);
                    }
                }
            }
            for(int i = 0; i < N; i++){
                V *last = (--vertices[i].end())->second;
                res = max(res, last->d + a[i] - last->t + 1);
            }
            cout << res << '\n';
        }
         */
    }

    return 0;
}