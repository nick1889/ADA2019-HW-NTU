//
// Created by Nick on 2019-12-30.
//
#include <iostream>
#include <vector>
#include <cryptominisat5/cryptominisat.h>

using namespace CMSat;
enum orientation{UP,DOWN,LEFT,RIGHT};
enum operation{NONE,AT_MOST_ONE,AT_LEAST_ONE,EXACT_ONE,EXACT_TWO,EXACT_ONE_NOT};
std::vector<orientation> orientations = {UP,DOWN,LEFT,RIGHT};
int r, c;

int ij2id(int i, int j, orientation o){
    int ret = -1;
    switch (o){
        case UP: ret = (2 * c + 1) * i + j;break;
        case DOWN: ret = (2 * c + 1) * (i + 1) + j;break;
        case LEFT: ret = (2 * c + 1) * i + j + c;break;
        case RIGHT: ret = (2 * c + 1) * i + j + c + 1;break;
    }
    return ret;
}

int ij2f(int i, int j){
    return 2*r*c + r + c + i * c + j;
}

int ijkl2R(int i, int j, int k, int l){
    return 3*r*c + r + c + (i * c + j) *(r * c) + (k * c + l);
}

void preHij(int i, int j, orientation o, std::vector<int> &pre){
    if(o == UP){
        pre.push_back(ij2id(i, j , LEFT));
        if(i > 0) pre.push_back(ij2id(i - 1, j, LEFT));
        if(j > 0) pre.push_back(ij2id(i, j - 1, UP));
    }
    else{
        pre.push_back(ij2id(i, j, LEFT));
        if(i < r - 1) pre.push_back(ij2id(i + 1, j, LEFT));
        if(j > 0) pre.push_back(ij2id(i, j - 1, DOWN));
    }
}

void succHij(int i, int j, orientation o, std::vector<int> &succ){
    if(o == UP){
        succ.push_back(ij2id(i, j , RIGHT));
        if(i > 0) succ.push_back(ij2id(i - 1, j, RIGHT));
        if(j < c - 1) succ.push_back(ij2id(i, j + 1, UP));
    }
    else{
        succ.push_back(ij2id(i, j, RIGHT));
        if(i < r - 1) succ.push_back(ij2id(i + 1, j, RIGHT));
        if(j < c - 1) succ.push_back(ij2id(i, j + 1, DOWN));
    }
}

void preVij(int i, int j, orientation o, std::vector<int> &pre){
    if(o == LEFT){
        pre.push_back(ij2id(i, j, UP));
        if(i > 0) pre.push_back(ij2id(i - 1, j, LEFT));
        if(j > 0) pre.push_back(ij2id(i, j - 1, UP));
    }
    else{
        pre.push_back(ij2id(i, j, UP));
        if(i > 0) pre.push_back(ij2id(i - 1, j, RIGHT));
        if(j < c - 1) pre.push_back(ij2id(i, j + 1, UP));
    }
}

void succVij(int i, int j, orientation o, std::vector<int>& succ){
    if(o == LEFT){
        succ.push_back(ij2id(i, j, DOWN));
        if(i < r - 1) succ.push_back(ij2id(i + 1, j, LEFT));
        if(j > 0) succ.push_back(ij2id(i, j - 1, DOWN));
    }
    else{
        succ.push_back(ij2id(i, j, DOWN));
        if(i < r - 1) succ.push_back(ij2id(i + 1, j, RIGHT));
        if(j < c - 1) succ.push_back(ij2id(i, j + 1, DOWN));
    }
}

void noneA(std::vector<int> &ids, SATSolver &solver){
    std::vector<Lit> clause;
    for(int id: ids){
        clause.push_back(Lit(id, true));
        solver.add_clause(clause);
        clause.clear();
    }
}

void atLeastOne(std::vector<int> &ids, SATSolver &solver){
    std::vector<Lit> clause;
    for(int id: ids){
        clause.push_back(Lit(id, false));
    }
    solver.add_clause(clause);
}

void atMostOne(std::vector<int> &ids, SATSolver &solver){
    std::vector<Lit> clause;
    for(int i1 = 0; i1 < ids.size(); i1++){
        for(int i2 = i1 + 1; i2 < ids.size(); i2++){
            clause.push_back(Lit(ids[i1], true));
            clause.push_back(Lit(ids[i2], true));
            solver.add_clause(clause);
            clause.clear();
        }
    }
}

void exactlyOne(std::vector<int> &ids, SATSolver &solver){
    atLeastOne(ids, solver);
    if(ids.size() > 1)
        atMostOne(ids, solver);
}

void exactlyOneNot(std::vector<int> &ids, SATSolver &solver) {
    std::vector<Lit> clause;
    for(int id: ids){
        clause.push_back(Lit(id, true));
    }
    solver.add_clause(clause);
    clause.clear();
    if(ids.size() > 1){
        for(int i1 = 0; i1 < ids.size(); i1++){
            for(int i2 = i1 + 1; i2 < ids.size(); i2++){
                clause.push_back(Lit(ids[i1], false));
                clause.push_back(Lit(ids[i2], false));
                solver.add_clause(clause);
                clause.clear();
            }
        }
    }
}

void exactlyTwo(std::vector<int> &ids, SATSolver &solver){
    std::vector<Lit> clause;
    for(int i1 = 0; i1 < ids.size(); i1++){
        for(int i2 = i1 + 1; i2 < ids.size(); i2++){
            for (int i3 = i2 + 1; i3 < ids.size(); i3++){
                clause.push_back(Lit(ids[i1], false));
                clause.push_back(Lit(ids[i2], false));
                clause.push_back(Lit(ids[i3], false));
                solver.add_clause(clause);
                clause.clear();
                clause.push_back(Lit(ids[i1], true));
                clause.push_back(Lit(ids[i2], true));
                clause.push_back(Lit(ids[i3], true));
                solver.add_clause(clause);
                clause.clear();
            }
        }
    }
}

void boxOperation(int i, int j, SATSolver &solver, operation op){
    std::vector<int> ids;
    ids.reserve(4);
    for(orientation o: orientations){
        ids.push_back(ij2id(i, j, o));
    }
    switch(op){
        case NONE:
            noneA(ids, solver);
            break;
        case AT_MOST_ONE:
            atMostOne(ids, solver);
            break;
        case AT_LEAST_ONE:
            atLeastOne(ids, solver);
            break;
        case EXACT_ONE:
            exactlyOne(ids, solver);
            break;
        case EXACT_TWO:
            exactlyTwo(ids, solver);
            break;
        case EXACT_ONE_NOT:
            exactlyOneNot(ids, solver);
            break;
    }
}

void bordersOperations(SATSolver &solver){
    std::vector<Lit> clause;
    for(int j = 0; j < c; j++){
        clause.push_back(Lit(ij2f(0, j), true));
        clause.push_back(Lit(ij2id(0, j, UP), true));
        solver.add_clause(clause);
        clause.clear();
        clause.push_back(Lit(ij2f(0, j), false));
        clause.push_back(Lit(ij2id(0, j, UP), false));
        solver.add_clause(clause);
        clause.clear();

        clause.push_back(Lit(ij2f(r - 1, j), true));
        clause.push_back(Lit(ij2id(r - 1, j, DOWN), true));
        solver.add_clause(clause);
        clause.clear();
        clause.push_back(Lit(ij2f(r - 1, j), false));
        clause.push_back(Lit(ij2id(r - 1, j, DOWN), false));
        solver.add_clause(clause);
        clause.clear();
    }
    for(int i = 0; i < r; i++){
        clause.push_back(Lit(ij2f(i, 0), true));
        clause.push_back(Lit(ij2id(i, 0, LEFT), true));
        solver.add_clause(clause);
        clause.clear();
        clause.push_back(Lit(ij2f(i, 0), false));
        clause.push_back(Lit(ij2id(i, 0, LEFT), false));
        solver.add_clause(clause);
        clause.clear();

        clause.push_back(Lit(ij2f(i, c - 1), true));
        clause.push_back(Lit(ij2id(i, c - 1, RIGHT), true));
        solver.add_clause(clause);
        clause.clear();
        clause.push_back(Lit(ij2f(i, c - 1), false));
        clause.push_back(Lit(ij2id(i, c - 1, RIGHT), false));
        solver.add_clause(clause);
        clause.clear();
    }
}

void innerReaching(SATSolver &solver){
    std::vector<Lit> clause;
    for(int i = 1; i < r; i++){
        for(int j = 0; j < c; j++){
            clause.push_back(Lit(ij2f(i - 1, j), false));
            clause.push_back(Lit(ij2f(i, j), true));
            clause.push_back(Lit(ij2id(i, j, UP), false));
            solver.add_clause(clause);
            clause.clear();
            clause.push_back(Lit(ij2f(i - 1, j), true));
            clause.push_back(Lit(ij2f(i, j), false));
            clause.push_back(Lit(ij2id(i, j, UP), false));
            solver.add_clause(clause);
            clause.clear();

            clause.push_back(Lit(ij2f(i - 1, j), false));
            clause.push_back(Lit(ij2f(i, j), false));
            clause.push_back(Lit(ij2id(i, j, UP), true));
            solver.add_clause(clause);
            clause.clear();
            clause.push_back(Lit(ij2f(i - 1, j), true));
            clause.push_back(Lit(ij2f(i, j), true));
            clause.push_back(Lit(ij2id(i, j, UP), true));
            solver.add_clause(clause);
            clause.clear();
        }
    }
    for(int i = 0; i < r; i++){
        for(int j = 1; j < c; j++){
            std::vector<Lit> clause;
            clause.push_back(Lit(ij2f(i, j - 1), false));
            clause.push_back(Lit(ij2f(i, j), true));
            clause.push_back(Lit(ij2id(i, j, LEFT), false));
            solver.add_clause(clause);
            clause.clear();
            clause.push_back(Lit(ij2f(i, j - 1), true));
            clause.push_back(Lit(ij2f(i, j), false));
            clause.push_back(Lit(ij2id(i, j, LEFT), false));
            solver.add_clause(clause);
            clause.clear();
            clause.push_back(Lit(ij2f(i, j - 1), false));
            clause.push_back(Lit(ij2f(i, j), false));
            clause.push_back(Lit(ij2id(i, j, LEFT), true));
            solver.add_clause(clause);
            clause.clear();
            clause.push_back(Lit(ij2f(i, j - 1), true));
            clause.push_back(Lit(ij2f(i, j), true));
            clause.push_back(Lit(ij2id(i, j, LEFT), true));
            solver.add_clause(clause);
            clause.clear();
        }
    }
}

void connectedCellsWithoutMax(SATSolver &solver){
    std::vector<Lit> clause;
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            for(int k = 0; k < r; k++){
                for(int l = 0; l < c; l++){
                    if(i < k or j < l){
                        clause.push_back(Lit(ij2f(i, j),true));
                        clause.push_back(Lit(ij2f(k, l),true));
                        clause.push_back(Lit(ijkl2R(i, j, k, l), false));
                        solver.add_clause(clause);
                        clause.clear();
                    }
                }
            }
        }
    }
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++){
            clause.push_back(Lit(ij2f(i, j), false));
            for(int k1 = 0,k2 = r - 1,l = 0; l < c; l++){
                if(i < k1 or j < l)
                    clause.push_back(Lit(ijkl2R(i, j, k1, l), false));
                if(i < k2 or j < l)
                    clause.push_back(Lit(ijkl2R(i, j, k2, l), false));
            }
            for(int l1 = 0, l2 = c - 1, k = 0; k < r; k++){
                if(i < k or j < l1)
                    clause.push_back(Lit(ijkl2R(i, j, k, l1),false));
                if(i < k or j < l2)
                    clause.push_back(Lit(ijkl2R(i, j, k, l2),false));
            }
            solver.add_clause(clause);
            clause.clear();
        }
    }
}

void connectedCellsWithMax(SATSolver &solver, std::vector<std::pair<int,int>> &NBH){
    std::vector<Lit> clause;
    for(std::pair<int,int> &nb: NBH){
        for(int k = 0; k < r; k++){
            for(int l = 0; l < c; l++){
                if(nb.first < k or nb.second < l){
                    clause.push_back(Lit(ij2f(nb.first, nb.second), true));
                    clause.push_back(Lit(ij2f(k, l), true));
                    clause.push_back(Lit(ijkl2R(nb.first, nb.second, k, l), false));
                    solver.add_clause(clause);
                    clause.clear();
                }

                if(nb.first > 0 and (nb.first - 1 < k or nb.second < l)){
                    clause.push_back(Lit(ij2f(nb.first - 1, nb.second), true));
                    clause.push_back(Lit(ij2f(k, l), true));
                    clause.push_back(Lit(ijkl2R(nb.first - 1, nb.second, k, l), false));
                    solver.add_clause(clause);
                    clause.clear();
                }

                if(nb.first < r - 1 and (nb.first + 1 < k or nb.second < l)){
                    clause.push_back(Lit(ij2f(nb.first + 1, nb.second), true));
                    clause.push_back(Lit(ij2f(k, l), true));
                    clause.push_back(Lit(ijkl2R(nb.first + 1, nb.second, k, l), false));
                    solver.add_clause(clause);
                    clause.clear();
                }

                if(nb.second > 0 and (nb.first < k or nb.second - 1 < l)){
                    clause.push_back(Lit(ij2f(nb.first, nb.second - 1), true));
                    clause.push_back(Lit(ij2f(k, l), true));
                    clause.push_back(Lit(ijkl2R(nb.first, nb.second - 1, k, l), false));
                    solver.add_clause(clause);
                    clause.clear();
                }

                if(nb.second < c - 1 and (nb.first < k or nb.second + 1 < l)){
                    clause.push_back(Lit(ij2f(nb.first, nb.second + 1), true));
                    clause.push_back(Lit(ij2f(k, l), true));
                    clause.push_back(Lit(ijkl2R(nb.first, nb.second + 1, k, l), false));
                    solver.add_clause(clause);
                    clause.clear();
                }
            }
        }
    }
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++){
            clause.push_back(Lit(ij2f(i, j), false));
            for(int k1 = 0,k2 = r - 1,l = 0; l < c; l++){
                if(i < k1 or j < l)
                    clause.push_back(Lit(ijkl2R(i, j, k1, l), false));
                if(i < k2 or j < l)
                    clause.push_back(Lit(ijkl2R(i, j, k2, l), false));
            }
            for(int l1 = 0, l2 = c - 1, k = 0; k < r; k++){
                if(i < k or j < l1)
                    clause.push_back(Lit(ijkl2R(i, j, k, l1),false));
                if(i < k or j < l2)
                    clause.push_back(Lit(ijkl2R(i, j, k, l2),false));
            }
            solver.add_clause(clause);
            clause.clear();
        }
    }
}

void exactlyOneContinuity(std::vector<int> &ids, int eid, SATSolver &solver){
    std::vector<Lit> clause;
    //At Least One
    for(int id: ids){
        clause.push_back(Lit(id, false));
    }
    clause.push_back(Lit(eid, true));
    solver.add_clause(clause);
    clause.clear();
    //At Most One
    if(ids.size() > 1){
        for(int i1 = 0; i1 < ids.size(); i1++){
            for(int i2 = i1 + 1; i2 < ids.size(); i2++){
                clause.push_back(Lit(ids[i1], true));
                clause.push_back(Lit(ids[i2], true));
                clause.push_back(Lit(eid, true));
                solver.add_clause(clause);
                clause.clear();
            }
        }
    }
}
void continuityOfLine(SATSolver &solver){
    std::vector<Lit> clause;
    std::vector<int> prehij;
    std::vector<int> previj;
    std::vector<int> succhij;
    std::vector<int> succvij;
    int eidH, eidV;
    for(int k = 0; k < r; k++){
        for(int j = 0; j < c; j++){
            eidH = ij2id(k, j, UP);
            eidV = ij2id(k, j, LEFT);
            preHij(k, j, UP, prehij);
            succHij(k, j, UP, succhij);
            preVij(k, j, LEFT, previj);
            succVij(k, j, LEFT, succvij);
            exactlyOneContinuity(prehij, eidH, solver);
            exactlyOneContinuity(succhij, eidH, solver);
            exactlyOneContinuity(previj, eidV, solver);
            exactlyOneContinuity(succvij, eidV, solver);
            prehij.clear();
            succhij.clear();
            previj.clear();
            succvij.clear();
        }
    }
    for(int j = 0; j < c; j++){
        eidH = ij2id(r - 1, j, DOWN);
        preHij(r - 1, j, DOWN, prehij);
        succHij(r - 1, j, DOWN, succhij);
        exactlyOneContinuity(prehij, eidH, solver);
        exactlyOneContinuity(succhij, eidH, solver);
        prehij.clear();
        succhij.clear();
    }
    for(int k = 0; k < r; k++){
        eidV = ij2id(k, c - 1, RIGHT);
        preVij(k, c - 1, RIGHT, previj);
        succVij(k, c - 1, RIGHT, succvij);
        exactlyOneContinuity(previj, eidV, solver);
        exactlyOneContinuity(succvij, eidV, solver);
        previj.clear();
        succvij.clear();
    }
}

int main() {
    std::cin >> r >> c;
    std::vector<std::vector<int>> grid(r);
    int max_value = -1;
    for(int i = 0; i < r; i++){
        grid[i] = std::vector<int>(c);
        char str[c + 1];
        std::cin >> str;
        for (int j = 0; j < c; j++){
            if(str[j] == '.'){
                grid[i][j] = -1;
            }
            else{
                grid[i][j] = str[j] - '0';
                if(grid[i][j] > max_value){
                    max_value = grid[i][j];
                }
            }
        }
    }
    SATSolver solver;
    solver.new_vars(3*r*c + r + c + r*c*r*c);
    bool flag = false;
    std::vector<std::pair<int,int>> NBH;
    for(int i = 0; i < r; i++){
        for (int j = 0; j < c; j++){
            switch(grid[i][j]){
                case 0:
                    boxOperation(i, j, solver, NONE);
                    break;
                case 1:
                    boxOperation(i, j, solver, EXACT_ONE);
                    break;
                case 2:
                    boxOperation(i, j, solver, EXACT_TWO);
                    break;
                case 3:
                    boxOperation(i, j, solver, EXACT_ONE_NOT);
                    break;
                default:
                    break;
            }
            if(grid[i][j] == max_value){
                if(!(i == 0 and (j == 0 or j == c - 1) ) and !( i == r - 1 and ( j == 0 or j == c - 1) ) ){
                    flag = true;
                    NBH.push_back({i, j});
                }
            }
        }
    }
    continuityOfLine(solver);
    bordersOperations(solver);
    innerReaching(solver);
//    std::cout << flag << '\n';
//    if(!flag)
//        connectedCellsWithoutMax(solver);
//    else
//        connectedCellsWithMax(solver, NBH);
    lbool ret = solver.solve();
//    std::cout << ret << '\n';
    if(ret == l_True){
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++) {
                std::cout << ((solver.get_model()[ij2id(i, j, UP)] == l_True) ? 1 : 0);
            }
            std::cout << '\n';
            for(int j = 0; j < c; j++){
                std::cout << ((solver.get_model()[ij2id(i, j, LEFT)] == l_True)?1:0);
            }
            std::cout << ((solver.get_model()[ij2id(i, c - 1, RIGHT)] == l_True)?1:0);
            std::cout << '\n';
        }
        for(int j = 0; j < c; j++){
            std::cout << ((solver.get_model()[ij2id(r - 1, j, DOWN)] == l_True)?1:0);
        }
    }
    else{
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++) {
                std::cout << ij2id(i, j, UP) << ' ';
            }
            std::cout << '\n';
            for(int j = 0; j < c; j++){
                std::cout << ij2id(i, j, LEFT) << ' ';
            }
            std::cout << ij2id(i, c - 1, RIGHT)<< ' ';
            std::cout << '\n';
        }
        for(int j = 0; j < c; j++){
            std::cout << ij2id(r - 1, j, DOWN) << ' ';
        }
    }
    std::cout << '\n';
    return 0;
}
