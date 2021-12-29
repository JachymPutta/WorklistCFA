#include <bits/types/FILE.h>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <set>

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
    std::vector<int> v = it->second;
    if (std::find(v.begin(), v.end(), value) != v.end()) {
    } else {
      deps[key].push_back(value);
    }
  } else {
    deps[key].push_back(value);
  }
}

void makeDepGraph(std::map<int, std::vector<int>> &deps, int* arg1, int* arg2, int callfun[], int calls) {
  for (int i = 0; i < calls; i++){
    // fprintf(stderr, "Handling index %i: \n arg1[i] = %i,  arg2[i] = %i, callfun[i] = %i\n", i , arg1[i], arg2[i], callfun[i]);
    addOrAppend(deps, arg1[i], i);
    addOrAppend(deps, arg2[i], i);
    addOrAppend(deps, callfun[i], i);
  }
}

void update(std::set<int> store[],std::queue<int> &workList, std::map<int, std::vector<int>> &deps, int arg, int var, int callsite) {
  // Get flow set from the store
  std::set<int> fsVar = store[var];
  std::set<int> fsArg = store[arg];

  std::set<int> newFs;
  newFs.insert(fsArg.begin(), fsArg.end());
  newFs.insert(fsVar.begin(), fsVar.end());

  if (fsVar.size() != newFs.size()){
    store[var] = newFs;
    std::map<int, std::vector<int>>::iterator it = deps.find(var);
    if (it != deps.end()){
      for(int x: it->second) {
        addOrAppend(deps, var, x);
      }
      pushRange(workList, it->second.begin(), it->second.end());
    }
  }
}

void runAnalysis(std::set<int> store[], int arg1Vec[], int arg2Vec[], int callFun[], std::map<int, std::vector<int>> &deps, int calls, int lams) {
  //Create worklist and enqueue all callsites.
  std::queue<int> workList;
  for (int i = 0; i < calls ; i++) {
    workList.push(i);
  }
  int iter = 0;

  while (!workList.empty()) {
    int callSite = workList.front();
    workList.pop();

    int fun = callFun[callSite];
    int arg1 = arg1Vec[callSite];
    int arg2 = arg2Vec[callSite];

    // makeGraph(store, arg1Vec, arg2Vec, callFun, deps, calls, lams, iter, callSite);
    for (auto x : store[fun]) {
      int var1 = x + lams;
      int var2 = x + 2 * lams;

      // fprintf(stdout, "Updating callsite %d: Arg1 = %d, Var1 = %d, Arg2 = %d, Var2 = %d",
      //         callSite, arg1, var1, arg2, var2);
      update(store, workList, deps, arg1, var1, callSite);
      update(store, workList, deps, arg2, var2, callSite);
    }
    iter++;
    // std::cout << "\nWorklist after iter: " << iter << "\n";
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

  // Read in the FUN matrix
  // fprintf(stderr, "Reading CALLFUN (%d x %d) ... ", calls, 1);
  populateMatrix(callFun, calls, 2, funPath.c_str());
  // fprintf(stderr, "Populated FUN\n");
  // printMatrix(callFun, 1, calls);

  // Read in the ARG1 matrix
  // fprintf(stderr, "Reading ARG1 (%d x %d) ... ", calls, 1);
  populateMatrix(callArg1, calls, 2, arg1Path.c_str());
  // fprintf(stderr, "Populated ARG1\n");
  // printMatrix(callArg1, 1, calls);


  // Read in the ARG2 matrix
  // fprintf(stderr, "Reading ARG2 (%d x %d) ... ", calls, 1);
  populateMatrix(callArg2, calls, 2, arg2Path.c_str());
  // fprintf(stderr, "Populated ARG2\n");
  // printMatrix(callArg2, 1, calls);

  // Construct the dependency graph
  // fprintf(stderr, "Constructing dependency graph (%d x %d) ... ", calls, 1);
  makeDepGraph(deps,callArg1, callArg2, callFun, calls);
  // fprintf(stderr, "Graph constructed\n");
  // printDeps(deps);

  // Run the analysis
  runAnalysis(store, callArg1, callArg2, callFun, deps, calls, lams);
  // printStore(store, vals);
  // printDeps(deps);

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

