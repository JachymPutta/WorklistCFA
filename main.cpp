#include <bits/types/FILE.h>
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
#include <vector>

#include "const.h"
#include "util.h"

void populateMatrix (int* matrix, int rows, int cols, const char* path)
{
  FILE* fp = fopen(path, "r");
    int i;
    int token = readToken(fp, NUM_NOT_FOUND);
    for (i = 0; token != EOF; ) {
	  if (token != NUM_NOT_FOUND) {
		*(matrix + i++) = token;
	  }
	  token = readToken(fp, NUM_NOT_FOUND);
	}
    fclose(fp);
}
void addOrAppend(std::map<int, std::vector<int>> &deps, int key, int value) {
  std::map<int, std::vector<int>>::iterator it;
  it = deps.find(key);
  if (it != deps.end()) {
    it->second.push_back(value);
  }
  else {
    deps[key].push_back(value);
  }
}
void makeDepGraph(std::map<int, std::vector<int>> &deps, int* arg1, int* arg2, int* callfun, int calls){


  for (int i = 0; i < calls; i++){
    // fprintf(stderr, "Handling index %i: \n arg1[i] = %i,  arg2[i] = %i, callfun[i] = %i\n", i , arg1[i], arg2[i], callfun[i]);
    // deps[i][arg1[i]] = 1;
    // deps[i][arg2[i]] = 1;
    // deps[i][callfun[i]] = 1;
    addOrAppend(deps, arg1[i], i);
    addOrAppend(deps, arg2[i], i);
    addOrAppend(deps, callfun[i], i);
  }
}

bool update(std::set<int> store[], int arg, int var) {

  std::set<int> fsVar;
  std::set<int> fsArg;
  std::set<int> newFs;

  // Get flow set from the store
  fsVar = store[var];
  fsArg = store[arg];
  newFs = store[var];

  //Merge sets
  newFs.insert(fsArg.begin(), fsArg.end());

  if (fsVar.size() == newFs.size()){
    //No update
    return false;
  } else {
    store[var] = newFs;
    //Update happened
    return true;
  }
}
void runAnalysis(std::set<int> store[], int *arg1Vec, int *arg2Vec, int *callFun, std::map<int, std::vector<int>> &deps, int calls, int lams) {
  //Create worklist and enqueue all callsites.
  std::queue<int> workList;
  for (int i = 0; i < calls; i++){
    workList.push(i);
  }
  int iter = 0;
 
  while (!workList.empty()){

    int callSite = workList.front();
    bool ch1 = false;
    bool ch2 = false;
    workList.pop();

    int fun = callFun[callSite];
    int arg1 = arg1Vec[callSite];
    int arg2 = arg2Vec[callSite];

    std::set<int> funL = store[fun];
    
    for (auto x : funL) {
      int var1 = x + lams;
      int var2 = x + 2 * lams;
      // std::cout << "Calling at callsite: " << callSite << " with: \n"
      //           << "Var1: " << var1 << " ,Var2: " << var2 << "\n"
      //           << "Arg1: " << arg1 << " ,Arg2: " << arg2 << "\n";

      ch1 = update(store, arg1, var1);
      ch2 = update(store, arg2, var2);

      // Push successors into the worklist
      if (ch1 || ch2) {
        std::map<int, std::vector<int>>::iterator it1;
        std::map<int, std::vector<int>>::iterator it2;
        it1 = deps.find(arg1);
        it2 = deps.find(arg2);

        if (it1 != deps.end()) {
          for (auto ii = it1->second.begin(); ii != it1->second.end(); ii++) {
            workList.push(*ii);
          }
        }
        if (it2 != deps.end()) {
          for (auto ii = it2->second.begin(); ii != it2->second.end(); ii++) {
            workList.push(*ii);
          }
        }
      }
    }
    iter++;
    // std::cout << "Worklist after iter: " << iter << "\n";
    // printQ(workList);
 }
}

int main(int argc, char** argv)
{
  int lams, vars, calls, vals;
  std::string testDir = DIR;
  std::string resName = DIR;

  if (argc == 2){
    testDir = argv[1];
    resName = argv[1];
  }

  resName.pop_back();
  std::size_t found = resName.find_last_of("/\\");
  std::string resDir = RES + resName.substr(found + 1);

  std::string paramsPath = testDir + PARAMS_PATH;
  std::string arg1Path = testDir + ARG1_PATH;
  std::string arg2Path = testDir + ARG2_PATH;
  std::string funPath = testDir + FUN_PATH;

  FILE *fp = fopen(paramsPath.c_str(), "r");
  fscanf(fp, "%d %d %d\n", &lams, &vars, &calls);
  fclose(fp);

  vals = 3 * lams;
  
  // fprintf (stderr, "Program parameters\n");
  // fprintf (stderr, "------------------\n");
  // fprintf (stderr, 
	// 	   "lams: %d\nvars: %d\nvals: %d\ncalls: %d\nterms: %d\n", 
	// 	   lams, vars, vals, calls, lams+vars+calls);


  std::set<int> store[vals];
  // std::vector<std::vector<bool>> deps(calls, std::vector<bool>(vals));
  std::map<int, std::vector<int>> deps;
  int *callFun = (int*)malloc(calls * sizeof(int));
  int *callArg1 = (int*)malloc(calls * sizeof(int));
  int *callArg2 = (int*)malloc(calls * sizeof(int));
  // int *deps = (int *)malloc(calls * sizeof(int));

  //Populate store
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

  // Run the analysis
  runAnalysis(store, callArg1, callArg2, callFun, deps, calls, lams);
  printStore(store, vals);

  // Deallocate memory -- Causes error with unfinished algorithm
  free(callFun);
  free(callArg1);
  free(callArg2);

  // Write out the result
  fprintf(stderr, "Writing %s\n", resDir.c_str());
  FILE* resFp = fopen(resDir.c_str(), "w");
  reformatStore(store, vals, resFp);
  fclose(resFp);

  return (EXIT_SUCCESS);
}

