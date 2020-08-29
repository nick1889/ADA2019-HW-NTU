#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

/*
  INPUT: Graph G = (V,E) must given : V ... ARRAY[1..N] of vertices 
                                      E ... ARRAY of edges for each vertex (there are as many arrays as there are vertices)
*/

#define MAX_VERTICES 4000

class ELEMENT {
public:
  int point;
  int N;
};

class ARRAY {
public:
  int *ele;
  int size;
  int first;
};

class ARRAY2 {
public:
  ELEMENT *ele;
  int size;
  int first;
};

class ARRAY_FIX {
public:
  int ele[MAX_VERTICES];
  int size;
  int first;
};


ARRAY_FIX C[MAX_VERTICES];
ARRAY E[MAX_VERTICES];
ARRAY2 V;
ARRAY_FIX Q;
ARRAY_FIX QMAX;
int maxno;
int max_degree;
int pk, nk, nk_size;
int e[MAX_VERTICES][MAX_VERTICES];
ARRAY2 S;

double num_level, dens;

int CUT1(int p, ARRAY_FIX &B) {
  /*
    return 1 if intersection of A and B is not empty
    return 0 if there are no elements in common to A and B
    in C we return the intersecting elements
  */
  int j;
  for (j = 0; j < B.size; j++)
    if (e[p][B.ele[j]] == 1)
      break;
  if (j == B.size)
    return 0;
  else 
    return 1;
}

int CUT2(int p, ARRAY2 B, ARRAY2 &C) {
  /*
    return 1 if intersection of A and B is not empty
    return 0 if there are no elements in common to A and B
    in C we return the intersecting elements
  */
  int j;
  C.size = 0;
  for (j = 0; j < B.size - 1; j++) {
    if (e[p][B.ele[j].point] == 1)
      C.ele[C.size++].point = B.ele[j].point;
  }
  if (C.size == 0)
    return 0;
  else 
    return 1;
}

void NUMBER_SORT(ARRAY2 &R) {
  int i, j, k;
  int p, min_k;
  maxno = 1;
  min_k = QMAX.size - Q.size + 1;
  C[1].size = 0;
  C[2].size = 0;
  i = 0;
  j = 0;
  while (i < R.size) {
    p = R.ele[i].point;
    k = 1;
    while (CUT1(p, C[k]) != 0)
      k++;
    if (k > maxno) {
      maxno = k;
      C[maxno+1].size = 0;
    }
    C[k].ele[C[k].size++] = R.ele[i].point;
    if (k < min_k) {
      R.ele[j++].point = R.ele[i].point;
    }
    i++;
  }
  //  R.ele[j - 1].N = 1;
  if (j > 0) R.ele[j-1].N = 0;
  if (min_k <= 0) min_k = 1;
  for (k = min_k; k <= maxno; k++)
    for (i = 0; i < C[k].size; i++) {
      R.ele[j].point = C[k].ele[i];
      R.ele[j++].N = k;
    }
}

void COPY(ARRAY_FIX &A, ARRAY_FIX B) {
  for (int i = 0; i < B.size; i++)
    A.ele[i] = B.ele[i];
  A.size = B.size;
}

void DEGREES_SORT(ARRAY2 R) {
  int tmp, k;
  ARRAY E2;
  E2.ele = (int*) calloc(R.size, sizeof(int));
  for (int i = 0; i < R.size; i++) {
    E2.ele[i] = 0;
    for (int j = 0; j < i; j++)
      if (e[R.ele[i].point][R.ele[j].point]) {
        E2.ele[i]++;
        E2.ele[j]++;
      }
  }
  for (int i = 0; i < R.size; i++) {  //  Sort vertices V in a descending order with respect to their degree
    k = i;
    for (int j = i + 1; j < R.size; j++) 
      if (E2.ele[k] < E2.ele[j]) {
        k = j;
      }
    tmp = R.ele[i].point;
    R.ele[i].point = R.ele[k].point;
    R.ele[k].point = tmp;
    tmp = E2.ele[i];
    E2.ele[i] = E2.ele[k];
    E2.ele[k] = tmp;
  }
  free(E2.ele);
}


void EXPAND(ARRAY2 R, int level) {
  /*
    First level is level = 1;
  */
  int p;
  ARRAY2 Rp;
  S.ele[level].point = S.ele[level].point + S.ele[level - 1].point - S.ele[level].N;
  S.ele[level].N = S.ele[level - 1].point;
  
  while (R.size != 0) {
    p = R.ele[R.size - 1].point;  // p is assigned the last vertex in R
    if (Q.size + R.ele[R.size - 1].N > QMAX.size) {
      Q.ele[Q.size++] = p;
      Rp.ele = (ELEMENT*) calloc(R.size, sizeof(ELEMENT));
      if (CUT2(p, R, Rp) != 0) {
        //        cout << S.ele[level].point << endl;
        if ((double)S.ele[level].point/pk < num_level) {
          nk++;
          nk_size+=Rp.size;
          DEGREES_SORT(Rp);
        }
        NUMBER_SORT(Rp);
        S.ele[level].point++;
        pk++;
        EXPAND(Rp, level + 1);
      }
      else if (Q.size > QMAX.size) { 
        cout << "na koraku " << pk << " sem nasel qmax dolzine " << Q.size << endl; 
        COPY(QMAX, Q); 
      }    
      free(Rp.ele);
      Q.size--;
    }
    else {
      return;
    }
    R.size--;
  }
}
        
void MCQ() {
  int tmp, k, max_k;
  Q.size = 0;
  QMAX.size = 0;

  for (int i = 0; i < V.size; i++)   //  Sort vertices V in a descending order with respect to their degree
    for (int j = i + 1; j < V.size; j++) 
      if (E[V.ele[i].point].size < E[V.ele[j].point].size) {
        tmp = V.ele[i].point;
        V.ele[i].point = V.ele[j].point;
        V.ele[j].point = tmp;
      }
  /*
  for (int i = 0; i < V.size; i++) {  //  Sort vertices V in a descending order with respect to their degree
    k = i;
    max_k = E[V.ele[k].point].size;
    for (int j = i + 1; j < V.size; j++) 
      if (E[V.ele[j].point].size > max_k) {
        k = j;
        max_k = E[V.ele[k].point].size;
      }
    tmp = V.ele[i].point;
    V.ele[i].point = V.ele[k].point;
    V.ele[k].point = tmp;
  }
  */



  max_degree = E[V.ele[0].point].size;
  cout << "max degree = " << max_degree << endl;
  for (int i = 0; i < max_degree; i++)
    V.ele[i].N = i + 1;
  for (int i = max_degree; i < V.size; i++)
    V.ele[i].N = max_degree + 1;


  S.ele = (ELEMENT*)  calloc(V.size, sizeof(ELEMENT));
  for (int i = 0; i < V.size; i++) {
    S.ele[i].point = 0;
    S.ele[i].N = 0;
  }
  EXPAND(V, 1);
  free(S.ele);
}

void read_dimacs(char name[]) {
  ifstream f (name);
  char buffer[256], token[20];
  int i, j;
  int vi, vj;
  int num_edges = 0;

  if (!f.is_open()) { 
    cout << "Error opening file!" << endl; 
    exit (1); 
  }
  for (i = 0; i < MAX_VERTICES; i++) 
    for (j = 0; j < MAX_VERTICES; j++) 
      e[i][j] = 0;

  while (!f.eof() ) {

    f.getline (buffer,250);
    if (buffer[0] == 'p') {
      cout << buffer << endl;
      sscanf(&buffer[7], "%d", &V.size);
      cout << V.size << endl;
      V.ele = (ELEMENT*)  calloc(V.size, sizeof(ELEMENT));
      for (i = 0; i < V.size; i++) {
        V.ele[i].point = i;
        E[i].size = 0;
        E[i].ele = (int*)  calloc(V.size, sizeof(int));
      }
    }
    if (buffer[0] == 'e') {
      num_edges++;
      i = 2;
      j = 0;
      while (buffer[i] != ' ') { token[j++] = buffer[i]; i++; }
      token[j] = '\0';
      vi = atoi(token);
      i++;
      j = 0;
      while (buffer[i] != ' ') { token[j++] = buffer[i]; i++; }
      token[j] = '\0';
      vj = atoi(token);
      vi--;
      vj--;
      E[vi].ele[E[vi].size++] = vj;
      E[vj].ele[E[vj].size++] = vi;
      e[vi][vj] = 1;
      e[vj][vi] = 1;
      //      cout << vi << " " << vj << endl;
    }
  }
  dens = (double) num_edges / (V.size * (V.size - 1) / 2);
  cout << "|E| = " << num_edges << "  |V| = " << V.size <<"  gostota = " << dens << endl;
  f.close();
}
  

int main(int argc, char *argv[]) {
  clock_t start1, start2;
  int i;
  cout << "args = " << argv[0] << " " << argv[1] << " " << argv[2] << endl;
  num_level = atof(argv[2]);
  cout << "num_level = " << num_level << endl;

  read_dimacs(argv[1]);

  start1 = time(NULL);
  start2 = clock();

  pk = 0;
  nk = 0;
  nk_size = 0;
  
  MCQ();
  i = 1;
  if (((double) (clock() - start2))/CLOCKS_PER_SEC < 1.0)
    while (i < 20) {
      pk = 0;
      nk = 0;
      nk_size = 0;
      MCQ();
      i++;
    }

  cout << "Stevilo korakov = " << pk << "    dolocanj degree-jev = " << nk << "   obseg dolocanj = " << nk_size << endl;
  
  for (int j = 0; j < QMAX.size; j++) 
    cout << QMAX.ele[j] << " ";
  cout << endl;
  
  cout << "Maximum clique length = " << QMAX.size << endl;

  if (difftime(time(NULL), start1) > 2000)
    cout << "Time = " << difftime(time(NULL), start1) << endl << endl;
  else
    cout << "Time = " << ((double) (clock() - start2)) / CLOCKS_PER_SEC / i << endl << endl;

  free(V.ele);
  for (int i = 0; i < V.size; i++)
    free(E[i].ele);
  
  return 0;
}
