//
// Created by kubaa on 05.12.2019.
//

#ifndef PEA2_SIMULATEDANNEALING_H
#define PEA2_SIMULATEDANNEALING_H

#include "Graph.h"
#include "Algorithm.h"
#include "TabuSearch.h"
#include <random>


class SimulatedAnnealing : public Algorithm {
private:
    vector<vector<int>> matrix;
    int* shortestRoute = nullptr;
    int shortestRouteValue = INT32_MAX;
    const int startVertex = 0;
    mt19937 randomGenerator;
    double timeToBreakSearch;
    double coolingValue;
    long long iterationWithoutChange;
    double temperature;

    void initRoute();
    int calculateRouteLength(int *route);
    void copySolution(std::vector<int> from, std::vector<int> to);
    void invertElements(std::vector<int> route, int i, int j);
    std::string generateOutput() override;
    void prepareToRun() override;
    void generateStartRoute();

public:
    explicit SimulatedAnnealing(std::shared_ptr<TSP> TravellingSalesmanProblem);
    ~SimulatedAnnealing() override;

    std::string showInfoBeforeRunning() override;
    std::string run() override;

    int setSource(string filename);
    string printSource();
    double getTimeToBreakSearch() const;
    void setTimeToBreakSearch(double timeToBreakSearch);
    void setCoolingValue(double coolingValue);
    double getCoolingValue() const;
    void setDefaultParameters();

    void solve();
    string getShortestRoute();
    int getShortestRouteValue();
};


#endif //PEA2_SIMULATEDANNEALING_H
