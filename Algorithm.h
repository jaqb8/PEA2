//
// Created by kubaa on 07.12.2019.
//

#ifndef PEA2_ALGORITHM_H
#define PEA2_ALGORITHM_H

#include "TSP.h"

#include <utility>
#include <string>
#include <memory>
#include <chrono>

using namespace std;

class Algorithm {
public:
    explicit Algorithm(shared_ptr<TSP> TravellingSalesmanProblem);
    virtual ~Algorithm();
    virtual string showInfoBeforeRunning() = 0;
    virtual string run() = 0;

protected:
    virtual void prepareToRun();
    virtual string generateOutput() = 0;
    int calculateDistance(vector<int> &route);
    vector<int> generateRandomRoute();

    chrono::high_resolution_clock::time_point startTime;
    chrono::high_resolution_clock::time_point endTime;

    shared_ptr<TSP> TravellingSalesmanProblem;
    int noOfChecks;
    unsigned long noOfVertices;
    int startVertex;
    vector<int> currentRoute;
    int currentDistance;
    vector<int> bestRoute;
    int bestDistance;

private:
    Algorithm() = default;
};


#endif //PEA2_ALGORITHM_H
