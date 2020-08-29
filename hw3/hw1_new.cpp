#pragma GCC optimize("O3")

#include <iostream>
#include <vector>
#include <map>
using namespace std;

int main() {
    ios::sync_with_stdio(0);cin.tie();
    int N, Q;
    cin >> N;
    vector<int> p(N); // successor
    vector<int> hist(N + 1); // in-degree
    vector<int> linked_res(N + 1); // result increment of destructing node
    //vector<int> in_degree_1_node(N);
    //input and compute in-degree
    for(int i = 0; i < N; i++){
        cin >> p[i];
        hist[p[i]] ++;
    }
    //make i->j->k become i->k and compute linked_res[i]
    for(int i = 0; i < N; i++){
        if(p[i] != 0){
            while(p[i] != 0 && hist[p[i]] == 1){
                hist[p[i]] = 0;
                linked_res[i + 1] += linked_res[p[i]] + 1;
                int old_successor = p[p[i] - 1];
                p[p[i] - 1] = 0;
                p[i] = old_successor;
            }
        }
    }


    cin >> Q;
    vector<vector<int>> queries(Q);
    for(int i = 0; i < Q; i++) {
        int Qi;
        cin >> Qi;
        vector<int> qq(Qi);
        for (int qi = 0; qi < Qi; qi++) {
            int q;
            cin >> q;
            qq[qi] = q;
        }
        queries[i] = qq;
    }

    for(int i = 0; i < Q;i++) {
        int res = 0;
        map<int,int> map_q; // record <node,#destructed in-degree>
        for(int q : queries[i]) {
            res += linked_res[q] + 1;
            int key = p[q - 1];
            if (key != 0) {
                if (map_q.count(key))
                    map_q[key] += 1;
                else
                    map_q[key] = 1;
                //map_q[key] += 1;
                // if the #destructed in-degree is equal to #in-degree, the node should be destructed.
                while (key != 0 && map_q[key] == hist[key]) {
                    res += linked_res[key] + 1;
                    map_q.erase(key);
                    key = p[key - 1];
                    if (map_q.count(key))
                        map_q[key] += 1;
                    else
                        map_q[key] = 1;
                        //map_q[key] += 1;
                }
            }
        }
        cout << res << endl;
    }
    return 0;
}

