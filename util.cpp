#include <bits/types/FILE.h>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

#include "util.h"
#include "const.h"

int readToken(FILE *fp, int curr) {
  int c = fgetc(fp);
  if (c == EOF)
    return EOF;
  if (c == (int)'(' || c == (int)')' || c == (int)' ' || c == (int)'.')
    return curr;
  int val = (int)(c - (int)'0');
  return readToken(fp, curr == NUM_NOT_FOUND ? val : curr * 10 + val);
}

void reformatStore (std::set<int> store[], int rows, FILE* handle){
  fprintf(handle, "#hash(");
  for (int i = 0; i < rows; i++){
    int h = 0;
    int m = store[i].size() - 1;
    fprintf (handle, "(%d . (", i);
    for (auto x : store[i]) {
      if (h < m){
        fprintf(handle, "%d ", x); 
      } else{
        fprintf(handle, "%d", x);
      }
      h++;
    }
    fprintf(handle, "))");
  }
  fprintf(handle, ")");
}

void displayMatrix (int* matrix, int rows, int cols)
{
    int i, j;
    printf("[");
    for (i = 0; i < rows; i++) {
        printf("[");
        for (j = 0; j < cols; j++) {
          printf("%u", *(matrix + i*cols + j));
            if (j < cols-1)
      	      printf(" ");
            }
          printf ("]");
    	if(i < rows-1)
    	  printf("\n");
      if ((i + 1) % (((rows > 3) ? rows : 4) / 3) == 0)
        printf("\n");
    }
    printf("]\n");
}

void printStore (std::set<int> store[], int rows)
{
  for (int i = 0; i < rows; i++){
    std::cout << "Row: " << i << " > ";
    printSet(store[i]);
    std::cout << "\n";
  }
  std::cout << "\n\n";
}

void printArray(int *array, int size) {
  std::cout << "[";
  for (int i = 0; i < size; i++){
    std::cout << array[i] << ", ";
  }
  std::cout << "]\n";
}

void printSet(std::set<int> s){
  for (auto x : s){
    std::cout << x << " ";
  }
}

void printQ(std::queue<int> q) {
  while (!q.empty()) {
    std::cout << q.front() << ", ";
    q.pop();
  }
  std::cout << "\n";
}

void printDeps(std::vector<std::vector<bool>> deps) {
  for (unsigned long i = 0; i < deps.size(); i++) {
    for (unsigned long j = 0; j < deps[i].size(); j++) {
      std::cout << deps[i][j] << " ";
    }
    std::cout << "\n";
  }
}
