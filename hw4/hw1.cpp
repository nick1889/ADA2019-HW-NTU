#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cryptominisat5/cryptominisat.h>
using namespace std;
using namespace CMSat;

void addClause(SATSolver &solver, vector<vector<int>> clauses){

}

void appendResult(vector<int>& vecInt, vector<int> &t, vector<int> &ret){
    for(int i = 0; i < vecInt.size(); ++i){
        if(vecInt[i] == 1){
            ret.push_back(t[i]);
        }
    }
}

bool compare(int a, int b){
    return a > b;
}

void combination(vector<int> &t, int c, int total, vector<vector<int>> &ret){
    //initial first combination like:1,1,0,0,0
    vector<int> vecInt(total,0);
    for(int i = 0; i < c; ++i){
        vecInt[i] = 1;
    }
    vector<int> temp;
    appendResult(vecInt, t, temp);
    ret.push_back(temp);
    for(int i = 0; i < total - 1; ++i){
        if(vecInt[i] == 1 && vecInt[i+1] == 0){
            //1. first exchange 1 and 0 to 0 1
            swap(vecInt[i], vecInt[i+1]);
            //2.move all 1 before vecInt[i] to left
            sort(vecInt.begin(),vecInt.begin() + i, compare);
            //after step 1 and 2, a new combination is exist
            vector<int> temp2;
            appendResult(vecInt, t, temp2);
            ret.push_back(temp2);
            //try do step 1 and 2 from front
            i = -1;
        }
    }
}

class VDC{
public:
    int r;
    int c;
    int v;
    int h;
    int num_wall;
    int max_time;
    vector<vector<int>> grid;
    VDC(int r, int c, vector<vector<int>> &grid){
        this->r = r;
        this->c = c;
        this->grid = grid;
        v = (c + 1) * r;
        h = (r + 1) * c;
        num_wall = v + h;
        max_time = num_wall;
    }
    bool wallIsInRange(int ri, int ci, int o){
        if (ri < 0 or ci < 0)
            return false;
        if (ri > r or ci > c)
            return false;
        if (o == 0 and ri == r)
            return false;
        return !(o == 1 and ci == c);
    }
    int wall(int ri, int ci, int o){
        if (o == 0)
            return 1 + ci * r + ri;
        else
            return 1 + v + ri * c + ci;
    }
    void rcoFromWall(int w, vector<int> &rco){
        w -= 1;
        int o, ci, ri;
        if (w < v){
            o = 0;
            ci = w / r;
            ri = w % r;
        }
        else{
            w -= v;
            o = 1;
            ri = w / c;
            ci = w % c;
        }
        rco = {ri, ci, o};
    }
    int reachable(int ri,int ci,int o,int time){
        int pref = num_wall + 1;
        return pref + time * num_wall + wall(ri, ci, o);
    }
    void cellWalls(int ri, int ci, vector<int> &ret){
        ret = {wall(ri, ci, 0), wall(ri, ci, 1), wall(ri + 1, ci, 1), wall(ri, ci + 1, 0)};
    }
    void nodeWalls(int ri, int ci, vector<int> &ret){
        if (wallIsInRange(ri, ci, 0))
            ret.push_back(wall(ri, ci, 0));
        if (wallIsInRange(ri, ci, 1))
            ret.push_back(wall(ri, ci, 1));
        if (wallIsInRange(ri - 1, ci, 0))
            ret.push_back(wall(ri - 1, ci, 0));
        if (wallIsInRange(ri, ci - 1, 1))
            ret.push_back(wall(ri, ci - 1, 1));
    }
    void wallNeighbors(int ri, int ci, int o, vector<int> &ret){
        nodeWalls(ri, ci, ret);
        if(o == 0){
            nodeWalls(ri + 1, ci, ret);
        }
        else{
            nodeWalls(ri, ci + 1, ret);
        }
        sort(ret.begin(),ret.end());
        ret.erase(unique(ret.begin(), ret.end()), ret.end());
    }
    void atMostN(vector<int> &vs, int n, vector<vector<int>> &ret){
        vector<vector<int>> comb;
        combination(vs, n + 1, vs.size(), comb);
        for(vector<int> &cs : comb){
            for(int & ci : cs){
                ci = -ci;
            }
            ret.push_back(cs);
        }
    }
    void atLeastN(vector<int> &vs, int n, vector<vector<int>> &ret){
        vector<vector<int>> comb;
        combination(vs, vs.size() - n + 1, vs.size(), comb);
        for(vector<int> &cs : comb){
            ret.push_back(cs);
        }
    }
    void exactlyN(vector<int> &vs, int n, vector<vector<int>> &ret){
        vector<vector<int>> ret1;
        vector<vector<int>> ret2;
        atMostN(vs, n, ret1);
        atLeastN(vs, n, ret2);
        ret.insert(ret.end(), ret1.begin(), ret1.end());
        ret.insert(ret.end(), ret2.begin(), ret2.end());
    }
    void zeroOrTwo(vector<int> &vs, vector<vector<int>> &ret){
        for(int i = 0; i < pow(2, vs.size()); i++){
            char temp[vs.size() + 1];
            itoa(i, temp, 2);
            int j = 0, sum = 0, max_k = 0;
            while(temp[j] != '\0'){
                if(temp[j] == '1')
                    sum ++;
                j++;
            }
            max_k = j;
            if(sum != 0 and sum != 2){
                vector<int> res(vs.size());
                for(int k = 0; k < vs.size(); k++){
                    if(k < max_k)
                        res[k] = vs[k] * pow(-1, temp[k] - '0');
                    else
                        res[k] = vs[k];
                }
                ret.push_back(res);
            }
        }
    }
    void implies(vector<int> &A, vector<int> &B, vector<vector<int>> &ret){
        vector<int> res(A.size() + B.size());
        for(int i = 0; i < A.size(); i++){
            res[i] = -A[i];
        }
        for(int i = 0; i < B.size(); i++){
            res[i + A.size()] = B[i];
        }
        ret.push_back(res);
    }
};

int main() {
    int r,c;
    cin >> r >> c;
    vector<vector<int>> grid(r);
    for(int i = 0; i < r; i++){
        grid[i] = vector<int>(c);
        char str[c + 1];
        cin >> str;
        for (int j = 0; j < c; j++){
            if(str[j] == '.'){
                grid[i][j] = -1;
            }
            else{
                grid[i][j] = str[j] - '0';
            }
        }
    }


    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid[0].size(); j++){
            cout << grid[i][j] << ' ';
        }
        cout << '\n';
    }
    return 0;
}
