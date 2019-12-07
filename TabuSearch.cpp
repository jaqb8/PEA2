//
// Created by kubaa on 07.12.2019.
//

#include "TabuSearch.h"
#include <algorithm>
#include <random>

TabuSearch::TabuSearch(std::shared_ptr<TSP> TravellingSalesmanProblem) : Algorithm(std::move(TravellingSalesmanProblem)) {
    setDefaultParameters();
}

TabuSearch::~TabuSearch() = default;

std::string TabuSearch::showInfoBeforeRunning() {
    std::string output;

    output += "Czas wykonywania algorytmu: " + std::to_string(timeToBreakSearch) + " (s)\n";

    return output;
}

std::string TabuSearch::run() {
    startTime = std::chrono::high_resolution_clock::now();
    prepareToRun();

    bool timeNoExceeded = true;

    while (timeNoExceeded) {
        std::tuple<int, int, int> nextTabuElement = enumerateNeighbourSolutions();

        currentDistance = calculateDistance(currentRoute);

        iterationWithoutImprovement++;

        if (currentDistance < bestDistance) {
            bestDistance = currentDistance;
            bestRoute = currentRoute;
            iterationWithoutImprovement = 0;
        }

        updateTabuList();
        tabuList.push_back(nextTabuElement);

        if (diversification && iterationWithoutImprovement >= iterationsToChangeNeighborhood) {
            currentRoute = generateRandomRoute();
            currentDistance = calculateDistance(currentRoute);
            iterationWithoutImprovement = 0;
        }

        endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

        if (duration > timeToBreakSearch) {
            timeNoExceeded = false;
        }
    }

    endTime = std::chrono::high_resolution_clock::now();
    return generateOutput();
}

void TabuSearch::setDefaultParameters() {
    cadency = TravellingSalesmanProblem->getNumberOfVertices() / 2;
    timeToBreakSearch = 10; // seconds
    aspiration = true;
    diversification = true;
    iterationsToChangeNeighborhood = 10000;
}

int TabuSearch::getCadency() const {
    return cadency;
}

double TabuSearch::getTimeToBreakSearch() const {
    return timeToBreakSearch;
}

bool TabuSearch::isAspiration() const {
    return aspiration;
}

bool TabuSearch::isDiversification() const {
    return diversification;
}

int TabuSearch::getIterationsToChangeNeighborhood() const {
    return iterationsToChangeNeighborhood;
}

void TabuSearch::setCadency(int cadency) {
    TabuSearch::cadency = cadency;
}

void TabuSearch::setTimeToBreakSearch(double timeToBreakSearch) {
    TabuSearch::timeToBreakSearch = timeToBreakSearch;
}

void TabuSearch::setAspiration(bool aspiration) {
    TabuSearch::aspiration = aspiration;
}

void TabuSearch::setDiversification(bool diversification) {
    TabuSearch::diversification = diversification;
}

void TabuSearch::setIterationsToChangeNeighborhood(int iterationsToChangeNeighborhood) {
    TabuSearch::iterationsToChangeNeighborhood = iterationsToChangeNeighborhood;
}

void TabuSearch::prepareToRun() {
    Algorithm::prepareToRun();
    tabuList.clear();
    iterationWithoutImprovement = 0;
    generateStartRoute();
    currentDistance = calculateDistance(currentRoute);
    bestRoute = currentRoute;
    bestDistance = currentDistance;
}

void TabuSearch::generateStartRoute() {
    std::vector<bool> visitedVertices;

    visitedVertices.clear();
    visitedVertices.resize(noOfVertices);

    currentRoute.clear();

    int currentVertex = startVertex;
    currentRoute.push_back(currentVertex);
    visitedVertices[currentVertex] = true;

    for (int i = 0; i < noOfVertices; ++i) {
        std::vector<std::pair<int, int>> distancesFromCurrentVertex;

        for (int j = 0; j < noOfVertices; ++j) {
            if (j == currentVertex) {
                continue;
            }
            distancesFromCurrentVertex.emplace_back(std::make_pair(TravellingSalesmanProblem->getDistance(currentVertex, j), j));
        }

        std::sort(distancesFromCurrentVertex.begin(), distancesFromCurrentVertex.end());

        for (auto &k : distancesFromCurrentVertex) {
            if (visitedVertices[k.second]) {
                continue;
            } else {
                currentVertex = k.second;
                break;
            }
        }

        currentRoute.push_back(currentVertex);
        visitedVertices[currentVertex] = true;
    }
}

std::tuple<int, int, int> TabuSearch::enumerateNeighbourSolutions() {
    std::vector<int> nextRoute;
    int nextRouteDistance = INT32_MAX;

    std::tuple<int, int, int> nextTabuElement = std::make_tuple(-1, -1, cadency);

    for (int i = 0; i < noOfVertices; ++i) {
        for (int j = 0; j < noOfVertices; ++j) {
            std::vector<int> neighbourRoute = currentRoute;

            bool moveInTabu = false;
            for (auto &tabuElement : tabuList) {
                if (std::get<0>(tabuElement) == i && std::get<1>(tabuElement) == j) {
                    moveInTabu = true;
                }
                if (std::get<1>(tabuElement) == i && std::get<0>(tabuElement) == j) {
                    moveInTabu = true;
                }
            }

            if (!aspiration && moveInTabu) {
                continue;
            }

            noOfChecks++;

            std::swap(neighbourRoute[i], neighbourRoute[j]);

            int neighbourRouteDistance = calculateDistance(neighbourRoute);

            if (aspiration && moveInTabu && neighbourRouteDistance >= bestDistance) {
                continue;
            }

            if (nextRouteDistance > neighbourRouteDistance) {
                nextRoute = neighbourRoute;
                nextRouteDistance = neighbourRouteDistance;

                std::get<0>(nextTabuElement) = i;
                std::get<1>(nextTabuElement) = j;
            }
        }
    }

    currentRoute = nextRoute;
    currentDistance = nextRouteDistance;
    return nextTabuElement;
}

void TabuSearch::updateTabuList() {
    for (auto &tabuElement : tabuList) {
        std::get<2>(tabuElement)--;
    }

    tabuList.erase(std::remove_if(tabuList.begin(),
            tabuList.end(),
            [&](const std::tuple<int, int, int> element) -> bool {
        return std::get<2>(element) <= 0;
    }),
            tabuList.end());
}

std::string TabuSearch::generateOutput() {
    std::string output;

    output += "Ilosc sprawdzonych rozwiazan: ";
    output += std::to_string(noOfChecks);
    output += "\n";

    output += "Dlugosc pracy algorytmu: ";
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    output += std::to_string(duration);
    output += "µs";
    output += "\n";

    if (bestRoute.empty()) {
        output += "Nie znaleziono zadnej trasy!\n";
    } else {
        output += "Najlepsza znaleziona droga: ";

        for (auto city : bestRoute) {
            output += std::to_string(city);
            output += " - ";
        }

        output += std::to_string(bestRoute[0]);
        output += "\n";

        output += "Dlugosc najlepszej znalezionej drogi: ";
        output += std::to_string(bestDistance);
        output += "\n";
    }

    return output;
}