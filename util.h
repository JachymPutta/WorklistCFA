#include <set>
#include <queue>
#include <bits/types/FILE.h>
#include <vector>

#ifndef UTIL_H
#define UTIL_H

void displayMatrix(int *matrix, int rows, int cols);
void printArray(int *array, int size);
void printDeps(std::vector<std::vector<bool>> deps);
void printQ(std::queue<int> q);
void printSet(std::set<int> set);
void printStore(std::set<int> store[], int rows);
int readToken(FILE *fp, int curr);
void reformatStore(std::set<int> store[], int rows, FILE *handle);

#endif
