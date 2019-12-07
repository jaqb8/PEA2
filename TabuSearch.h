//
// Created by kubaa on 07.12.2019.
//

#ifndef PEA2_TABUSEARCH_H
#define PEA2_TABUSEARCH_H

#include "Algorithm.h"

class TabuSearch : public Algorithm {
public:
    explicit TabuSearch(std::shared_ptr<TSP> TravellingSalesmanProblem);

    ~TabuSearch() override;

    std::string showInfoBeforeRunning() override;

    std::string run() override;

    void setDefaultParameters();

    int getCadency() const;

    double getTimeToBreakSearch() const;

    bool isAspiration() const;

    bool isDiversification() const;

    int getIterationsToChangeNeighborhood() const;

    void setCadency(int cadency);

    void setTimeToBreakSearch(double timeToBreakSearch);

    void setAspiration(bool aspiration);

    void setDiversification(bool diversification);

    void setIterationsToChangeNeighborhood(int iterationsToChangeNeighborhood);

private:
    void prepareToRun() override;

    void generateStartRoute();

    std::tuple<int, int, int> enumerateNeighbourSolutions();

    void updateTabuList();

    std::string generateOutput() override;

    // city 1, city 2, cadency; blocks exchanging the pair of cities
    std::vector<std::tuple<int, int, int>> tabuList;

    int cadency;
    double timeToBreakSearch;
    bool aspiration;
    bool diversification;
    int iterationsToChangeNeighborhood;

    int iterationWithoutImprovement;
};


#endif //PEA2_TABUSEARCH_H
