//
// Created by Nick on 2020-1-5.
//

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

set<int> Q, QMax;

class Node{
public:
    int id;
    vector<Node *> adj;
    int degree = 0;
    explicit Node(int i):id(i){};
};

bool CompLess (const Node* x, const Node* y) {
    return x->degree > y->degree;
}

vector<Node*> nodes;

void intersect(vector<int> &Ck, vector<Node *>  &adj, vector<int> &ret){
    for(Node *node: adj){
        for(int i: Ck){
            if(node->id == i){
                ret.push_back(i);
            }
        }
    }
}

void intersectRP(vector<Node*> &R, vector<Node*> &adj,vector<Node*> &ret){
    for(Node *node: adj){
        for(Node *r: R){
            if(node->id == r->id){
                ret.push_back(r);
            }
        }
    }
}

void colorSort(vector<Node*> &R, vector<int> &C){
    int max_no = 1;
    int k_min = QMax.size() - Q.size() + 1;
    if(k_min <= 0) k_min = 1;
    int j = 0;
    vector<vector<int>> Ck(2);
    for(int i = 0; i < R.size(); i++){
        Node p = *R[i];
        int k = 1;
        vector<int> ret;
        intersect(Ck[k - 1], p.adj, ret);
        while(!ret.empty()){
            k ++;
            ret.clear();
            intersect(Ck[k - 1], p.adj, ret);
        }
        if(k > max_no){
            max_no = k;
            Ck.emplace_back(vector<int>());
        }
        Ck[k - 1].push_back(p.id);
        if(k < k_min){
            R[j] = R[i];
            j++;
        }
    }
    C[j - 1] = 0;
    for(int k = k_min; k <= max_no; k++){
        for(int i = 1; i <= Ck[k].size(); i++){
            R[j] = nodes[Ck[k - 1][i - 1] - 1];
            C[j] = k;
            j++;
        }
    }
}

void maxClique(vector<Node*> &R, vector<int> &C){
    while(!R.empty()){
        colorSort(R, C);
        int max_value = 0, idx = 0;
        for(int i = 0; i < C.size(); i++){
            if(C[i] > max_value){
                idx = i;
            }
        }
        Node p = *R[idx];
        for(int i = 0; i < R.size(); i++){
            if(R[i]->id == p.id){
                R.erase(R.begin() + i);
                break;
            }
        }
        if((Q.size() + C[p.id - 1]) > QMax.size()){
            Q.insert(p.id);
            vector<Node*> ret;
            intersectRP(R, p.adj, ret);
            if(!ret.empty()){
                vector<int> CPrime(ret.size());
//                colorSort(R, CPrime);
                maxClique(ret, CPrime);
            }
            else if(Q.size() > QMax.size()) {
                QMax = Q;
            }
            Q.erase(p.id);
        } else{
            return;
        }
    }
}

int main(){
    int N;
    cin >> N;
    vector<int> colors(N);
    vector<Node*> R;
    for(int i = 0; i < N; i++){
        Node* node = new Node(i+1);
        nodes.push_back(node);
        R.push_back(node);
    }
    for(int i = 0; i < N; i++){
        char ch[N + 1];
        cin >> ch;
        for(int j = 0; j < N; j++){
            if(ch[j] - '0' == 1){
                nodes[i]->adj.push_back(nodes[j]);
                nodes[i]->degree++;
            }
        }
    }
    sort(R.begin(), R.end(), CompLess);
    maxClique(R, colors);
    for(int qi : QMax){
        cout << qi << ' ';
    }
    return 0;
}