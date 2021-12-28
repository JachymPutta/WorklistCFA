#include <bits/types/FILE.h>
#include <cstddef>
#include <cstdlib>
#include <endian.h>
#include <iostream>
#include <algorithm>
#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "const.h"
#include "util.h"

void populateMatrix(int* matrix, int rows, int cols, const char* path) {
  FILE* fp = fopen(path, "r");
  int token = readToken(fp, NUM_NOT_FOUND);
  for (int i = 0; token != EOF; ) {
	  if (token != NUM_NOT_FOUND) {
      matrix[i] = token;
      i++;
	  }
	  token = readToken(fp, NUM_NOT_FOUND);
	}
  fclose(fp);
}

void addOrAppend(std::map<int, std::vector<int>> &deps, int key, int value) {
  std::map<int, std::vector<int>>::iterator it = deps.find(key);
  if (it != deps.end()) {
    it->second.push_back(value);
  } else {
    deps[key].push_back(value);
  }
}

void makeDepGraph(std::map<int, std::vector<int>> &deps, int* arg1, int* arg2, int* callfun, int calls) {
  for (int i = 0; i < calls; i++){
    // fprintf(stderr, "Handling index %i: \n arg1[i] = %i,  arg2[i] = %i, callfun[i] = %i\n", i , arg1[i], arg2[i], callfun[i]);
    addOrAppend(deps, arg1[i], i);
    addOrAppend(deps, arg2[i], i);
    addOrAppend(deps, callfun[i], i);
  }
}

template <typename Iter, typename Q>
void push_range(Q &q, Iter begin, Iter end) {
  for (; begin != end; ++begin) {
    q.push(*begin);
  }
}

void update(std::set<int> store[],std::queue<int> &workList, std::map<int, std::vector<int>> deps, int arg, int var, int callsite) {
  // Get flow set from the store
  std::set<int> fsVar = store[var];
  std::set<int> fsArg = store[arg];

  std::set<int> newFs;
  newFs.insert(fsArg.begin(), fsArg.end());
  newFs.insert(fsVar.begin(), fsVar.end());

  if (fsVar.size() != newFs.size()){
    // Update happened
    store[var] = newFs;
    std::map<int, std::vector<int>>::iterator it = deps.find(arg);
    push_range(workList, it->second.begin(), it->second.end());
    workList.push(callsite);
  }
}

void runAnalysis(std::set<int> store[], int arg1Vec[], int arg2Vec[], int callFun[], std::map<int, std::vector<int>> &deps, int calls, int lams) {
  //Create worklist and enqueue all callsites.
  std::queue<int> workList;
  for (int i = 0; i < calls; i++){
    workList.push(i);
  }
  int iter = 0;
 
  while (!workList.empty() /* || iter < 200*/){
    // if (workList.empty()){
    //   for (int i = 0; i < calls; i++) {
    //     workList.push(i);
    //   }
    // }
    int callSite = workList.front();
    workList.pop();

    int fun = callFun[callSite];
    int arg1 = arg1Vec[callSite];
    int arg2 = arg2Vec[callSite];

    // makeGraph(store, arg1Vec, arg2Vec, callFun, deps, calls, lams, iter, callSite);
    for (auto x : store[fun]) {
      int var1 = x + lams;
      int var2 = x + 2 * lams;
      // int wlSize = workList.size();
      // if (var1 == 87){
      //   fprintf(stdout, "%d - callsite\n", callSite);
      // }

      // Push successors into the worklist
      update(store, workList, deps, arg1, var1, callSite);
      update(store, workList, deps, arg2, var2, callSite);
      // if (wlSize != workList.size()){
      //   workList.push(x);
      // }
    }
    iter++;
    // std::cout << "Worklist after iter: " << iter << "\n";
    // printQ(workList);
 }
}

int main(int argc, char** argv) {
  std::string testId = DEFAULT_TEST;

  if (argc == 1){
  } else if (argc == 2){
    testId = argv[1];
  } else {
    return 1;
  }

  std::string outputPath = OUTPUT_DIR + testId;
  std::string paramsPath = INPUT_DIR + testId + PARAMS_PATH;
  std::string arg1Path = INPUT_DIR + testId + ARG1_PATH;
  std::string arg2Path = INPUT_DIR + testId + ARG2_PATH;
  std::string funPath = INPUT_DIR + testId + FUN_PATH;
  // fprintf(stdout, "outPath = %s, paramPath = %s, arg1P = %s, arg2P = %s, funP  = %s",
  //         outputPath.c_str(), paramsPath.c_str(), arg1Path.c_str(), arg2Path.c_str(), funPath.c_str());

  int lams, vars, calls;
  {
    FILE *fp = fopen(paramsPath.c_str(), "r");
    fscanf(fp, "%d %d %d\n", &lams, &vars, &calls);
    fclose(fp);
  }

  const int vals = 3 * lams;
  
  // fprintf (stderr, "Program parameters\n");
  // fprintf (stderr, "------------------\n");
  // fprintf (stderr, 
	// 	   "lams: %d\nvars: %d\nvals: %d\ncalls: %d\nterms: %d\n", 
	// 	   lams, vars, vals, calls, lams+vars+calls);


  std::map<int, std::vector<int>> deps;
  int *callFun = (int*)malloc(calls * sizeof(int));
  int *callArg1 = (int*)malloc(calls * sizeof(int));
  int *callArg2 = (int*)malloc(calls * sizeof(int));

  //Populate store
  std::set<int> store[vals];
  for (int i = 0; i < lams; i++) {
    store[i].insert(i);
  }
  // printStore(store, vals);

  // Read in the FUN matrix
  // fprintf(stderr, "Reading CALLFUN (%d x %d) ... ", calls, 1);
  populateMatrix(callFun, calls, 2, funPath.c_str());
  // fprintf(stderr, "Populated FUN\n");
  // displayMatrix(callFun, 1, calls);

  // Read in the ARG1 matrix
  // fprintf(stderr, "Reading ARG1 (%d x %d) ... ", calls, 1);
  populateMatrix(callArg1, calls, 2, arg1Path.c_str());
  // fprintf(stderr, "Populated ARG1\n");
  // displayMatrix(callArg1, 1, calls);


  // Read in the ARG2 matrix
  // fprintf(stderr, "Reading ARG2 (%d x %d) ... ", calls, 1);
  populateMatrix(callArg2, calls, 2, arg2Path.c_str());
  // fprintf(stderr, "Populated ARG2\n");
  // displayMatrix(callArg2, 1, calls);


  // Construct the dependency graph
  // fprintf(stderr, "Constructing dependency graph (%d x %d) ... ", calls, 1);
  makeDepGraph(deps,callArg1, callArg2, callFun, calls);
  // fprintf(stderr, "Graph constructed\n");
  // printDeps(deps);

  // for (std::map<int,std::vector<int>>::iterator iter = deps.begin(); iter != deps.end();
  //      ++iter) {
  //   std::vector<int> v = iter->second;
  //   for (auto &i : v){
  //     if (i == 13){
  //       fprintf(stdout, "%d - links to 13\n", iter->first);
  //     }
  //   }
  // }

  // Run the analysis
  runAnalysis(store, callArg1, callArg2, callFun, deps, calls, lams);
  // printStore(store, vals);

  // Deallocate memory -- Causes error with unfinished algorithm
  free(callFun);
  free(callArg1);
  free(callArg2);

  // Write out the result
  fprintf(stderr, "Writing %s\n", outputPath.c_str());
  FILE* resFp = fopen(outputPath.c_str(), "w");
  reformatStore(store, vals, resFp);
  fclose(resFp);

  return EXIT_SUCCESS;
}

