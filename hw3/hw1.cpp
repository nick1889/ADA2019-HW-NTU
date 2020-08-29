#include <iostream>
#include <vector>
#include <set>
using namespace std;
class Node{
public:
    int id;
    Node* successor{};
    vector<int> destructed;
    int eliminated = 0;
    int in_degree = 0;
    bool visited = false;
    explicit Node(int id){
        this->id = id;
        destructed.push_back(id);
    }
};
class Graph{
public:
    vector<Node *> nodes;
};
void destruct_graph_nodes(vector<Node*> &nodes,int i){
    nodes[i]->visited = true;
    if(nodes[i]->successor == nullptr){
        return;
    }
    if(nodes[i]->successor->id - 1 == i){
        return;
    }
    if(nodes[i]->successor->in_degree == 1){
        if(nodes[i]->successor->visited){
            nodes[i]->destructed.reserve(nodes[i]->destructed.size()+nodes[i]->successor->destructed.size());
            nodes[i]->destructed.insert(nodes[i]->destructed.end(),nodes[i]->successor->destructed.begin(),nodes[i]->successor->destructed.end());
        }
        else{
            destruct_graph_nodes(nodes,nodes[i]->successor->id - 1);
            nodes[i]->destructed.reserve(nodes[i]->destructed.size()+nodes[i]->successor->destructed.size());
            nodes[i]->destructed.insert(nodes[i]->destructed.end(),nodes[i]->successor->destructed.begin(),nodes[i]->successor->destructed.end());

        }
        if(nodes[i]->successor->eliminated != (i + 1) )
            nodes[i]->eliminated = nodes[i]->successor->eliminated;
    }
    else{
        if(nodes[i]->successor->eliminated != (i + 1) )
            nodes[i]->eliminated = nodes[i]->successor->id;
        nodes[i]->visited = true;
    }
}
int main() {
    int N, Q;
    cin >> N;
    Graph graph;
    graph.nodes = vector<Node*>(N);
    for(int i = 0; i < N; i++){
        int pi;
        cin >> pi;
        if(graph.nodes[i] == nullptr){
            graph.nodes[i] = new Node(i+1);
        }
        if(pi != 0){
            if(graph.nodes[pi - 1] == nullptr){
                graph.nodes[pi - 1] = new Node(pi);
                graph.nodes[pi - 1]->in_degree ++;
            }
            else{
                graph.nodes[pi - 1]->in_degree ++;
            }
            graph.nodes[i]->successor = graph.nodes[pi - 1];
        }
    }
    for(int i = 0; i < N; i++){
        if(!graph.nodes[i]->visited){
            destruct_graph_nodes(graph.nodes,i);
            graph.nodes[i]->visited = true;
        }
    }
    cin >> Q;
    for(int i = 0; i < Q; i++){
        int Qi;
        cin >> Qi;
        set<int> destructed_nodes;
        vector<int> map_q(N + 1);
        for(int qi = 0; qi < Qi; qi++){
            int q;
            cin >> q;
            for(int j : graph.nodes[q - 1]->destructed) {
                destructed_nodes.insert(j);
            }
            int key = graph.nodes[q - 1]->eliminated;
            if(key != 0){
                map_q[key] ++;
                while(map_q[key] == graph.nodes[key - 1]->in_degree){
                    for(int e : graph.nodes[key - 1]->destructed){
                        destructed_nodes.insert(e);
                    }
                    key = graph.nodes[key - 1]->eliminated;
                    if(map_q[key] != 0){
                        map_q[key] ++;
                    }
                }
            }
        }
        cout << destructed_nodes.size() << endl;
    }
    return 0;
}