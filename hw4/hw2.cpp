//
// Created by Nick on 2020-1-1.
//
#include <iostream>
#include <vector>
#define T_rand 300

using namespace std;

enum Color {WHITE, GRAY, BLACK};
class V{
public:
    int id;
    vector<V*> succ;
    vector<int> rand_num;
    int in_degree = 0;
    V(int i):id(i), rand_num(T_rand){
        for(int j = 0; j < T_rand; j++){
            rand_num[j] = rand();
        }
    };
};

void dfs(V* v, vector<Color> &colors){
    colors[v->id - 1] = GRAY;
    for(V* u:v->succ){
        if(colors[u->id - 1] == WHITE){
            dfs(u, colors);
        }
        for(int i = 0; i < T_rand; i++){
            v->rand_num[i] = min(v->rand_num[i], u->rand_num[i]);
        }
    }
    colors[v->id - 1] = BLACK;
//    orders.push_back(v);
}

int main(){
    int N, M, Q;
    cin >> N >> M >> Q;
    vector<V*> vertices(N);
    vector<Color> colors(N, WHITE);
    for(int i = 0; i < N; i++){
        vertices[i] = new V(i+1);
    }
    int s, t;
    for(int i = 0; i < M; i++){
        cin >> s >> t;
        vertices[s - 1]->succ.push_back(vertices[t - 1]);
        vertices[t - 1]->in_degree++;
    }
//    vector<V*> orders;
//    orders.reserve(N);
    for(int i = 0; i < N; i++){
        if(colors[i] == WHITE)
            dfs(vertices[i], colors);
    }

    int q1, q2, count;
    for(int q = 0; q < Q; q++){
        cin >> q1 >> q2;
        count = 0;
        if((vertices[q1 - 1]->succ.empty() and vertices[q1 - 1]->in_degree == 0) or
        (vertices[q2 - 1]->succ.empty() and vertices[q2 - 1]->in_degree == 0) ){
            cout << 0 << '\n';
            continue;
        }
        for(int k = 0; k < T_rand; k++){
            if(vertices[q1 - 1]->rand_num[k] == vertices[q2 - 1]->rand_num[k]){
                count ++;
            }
        }
        float ret = (float)count / (float)T_rand;
        cout << ret << '\n';
    }
    return 0;
}
