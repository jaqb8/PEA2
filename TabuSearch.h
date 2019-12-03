//
// Created by kubaa on 03.12.2019.
//

#ifndef PEA2_TABUSEARCH_H
#define PEA2_TABUSEARCH_H

#include "Graph.h"

class TabuSearch {
private:
    Graph graph;
    int *shortestRoute = nullptr;
    int shortestRouteValue = INT32_MAX;
    const int startVertex = 0;
    void initRoute();
    void copySolution(int *from, int *to);
    void swapElements(int *route, int i, int j);
    int calculateRouteLength(int *route);


public:
    TabuSearch();
    ~TabuSearch();
    int setSource(string filename);
    string printSource();
    string getShortestRoute();
    int getShortestRouteValue();
    void solve(int repetitions = 200, int repetitionsTillTabuRemoval = 10, int aspirationValue = -10);
};


#endif //PEA2_TABUSEARCH_H
