#include <set>
#include <string>
#include <queue>
#include <bits/types/FILE.h>
#include <vector>
#include <map>

#ifndef UTIL_H
#define UTIL_H

void displayMatrix(int *matrix, int rows, int cols);
void printArray(int *array, int size);
void printDeps(std::map<int, std::vector<int>>deps);
void printQ(std::queue<int> q);
void printSet(std::set<int> set);
void printStore(std::set<int> store[], int rows);
int readToken(FILE *fp, int curr);
void reformatStore(std::set<int> store[], int rows, FILE *handle);
void makeGraph(std::set<int> store[], int arg1Vec[], int arg2Vec[],
               int callFun[], std::map<int, std::vector<int>> &deps, int calls,
               int lams, int iter, int callSite);
std::string exprId(int id, int lams);

#endif
