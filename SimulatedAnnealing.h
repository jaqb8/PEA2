//
// Created by kubaa on 05.12.2019.
//

#ifndef PEA2_SIMULATEDANNEALING_H
#define PEA2_SIMULATEDANNEALING_H

#include "Graph.h"
#include "TabuSearch.h"
#include <random>


class SimulatedAnnealing {
private:
    Graph graph;
    int* shortestRoute = nullptr;
    int shortestRouteValue = INT32_MAX;
    const int startVertex = 0;
    mt19937 randomGenerator;

    void initRoute();
    int calculateRouteLength(int *route);
    void copySolution(int *from, int *to);
    void invertElements(int *route, int i, int j);

public:
    SimulatedAnnealing();
    ~SimulatedAnnealing();

    int setSource(string filename);
    string printSource();

    void solve();
    string getShortestRoute();
    int getShortestRouteValue();
};


#endif //PEA2_SIMULATEDANNEALING_H
