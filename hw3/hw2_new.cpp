//
// Created by Nick on 2019-12-1.
//

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class E{
public:
    long long di;
    long long li;
    int yi;
    E(int y, long long d, long long l):yi(y),di(d),li(l){}
};

class V{
public:
    int id;
    vector<E*> adj;
    long long d = 1e18;
    long long l = 1e18;
    explicit V(int id):id(id){}
};

class Compare_VL_Pointer{
public:
    bool operator() (V* &x, V* &y) const {
        return x->l > y->l;
    }
};
class Compare_VD_Pointer{
public:
    bool operator() (V* &x, V* &y) const {
        return x->d > y->d;
    }
};

int main(){
    int n, m, s, t;
    cin >> n >> m;
    cin >> s >> t;
    priority_queue<V*,vector<V*>,Compare_VL_Pointer> Q;
    vector<V*> vertices(0);
    for(int i = 0; i < n; i++){
        V* v = new V(i);
        if(i == s){
            v->d = 0;
            v->l = 0;
        }
        Q.push(v);
        vertices.push_back(v);
    }
    for(int i = 0; i < m; i++){
        int xi,yi;
        long long di,li;
        cin >> xi >> yi;
        cin >> di >> li;
        E* e1 = new E(yi, di, li);
        E* e2 = new E(xi, di, li);
        vertices[xi]->adj.push_back(e1);
        vertices[yi]->adj.push_back(e2);
    }

    while(!Q.empty()){
        V *u = Q.top();
        //cout << "Pop " << u->id << '\n';
        Q.pop();
        for(E* v:u->adj){
            if(vertices[v->yi]->l > max(u->l,v->li)){

                //cout << "vertex " << v->yi << "'s l: " << vertices[v->yi]->l << " --> " << max(u->l,v->li) << '\n';
                vertices[v->yi]->l = max(u->l,v->li);
                Q.push(vertices[v->yi]);
            }
        }
    }
    long long min_l = vertices[t]->l;
    priority_queue<V*,vector<V*>,Compare_VD_Pointer> QD;
    for(int i = 0; i < n; i++){
        QD.push(vertices[i]);
    }
    while(!QD.empty()){
        V *u = QD.top();
        //cout << "Pop " << u->id << '\n';
        QD.pop();
        for(E* v:u->adj){
            if( (v->li <= min_l) && vertices[v->yi]->d > (u->d + v->di)){

                //cout << "vertex " << v->yi << "'s l: " << vertices[v->yi]->l << " --> " << max(u->l,v->li) << '\n';
                vertices[v->yi]->d = u->d + v->di;
                QD.push(vertices[v->yi]);
            }
        }
    }

    cout << vertices[t]->d << ' ' << vertices[t]->l;
    return 0;
}