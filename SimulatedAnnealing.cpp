//
// Created by kubaa on 05.12.2019.
//

#include "SimulatedAnnealing.h"
#include <iostream>
#include <random>
#include <math.h>
#include <algorithm>

SimulatedAnnealing::SimulatedAnnealing(std::shared_ptr<TSP> TravellingSalesmanProblem) : Algorithm(
        std::move(TravellingSalesmanProblem)) {
    randomGenerator.seed(std::random_device()());
    setDefaultParameters();
}

SimulatedAnnealing::~SimulatedAnnealing() = default;

std::string SimulatedAnnealing::showInfoBeforeRunning() {
    std::string output;

    output += "Czas wykonywania algorytmu: " + std::to_string(timeToBreakSearch) + " (s)\n";

    return output;
}

//int SimulatedAnnealing::setSource(string filename)
//{
//    return graph.getFromFile(filename);
//}

//string SimulatedAnnealing::printSource()
//{
//    return graph.printGraph();
//}

void SimulatedAnnealing::generateStartRoute() {
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
            distancesFromCurrentVertex.emplace_back(
                    std::make_pair(TravellingSalesmanProblem->getDistance(currentVertex, j), j));
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

void SimulatedAnnealing::prepareToRun() {
    Algorithm::prepareToRun();
    generateStartRoute();
    iterationWithoutChange = 0;
    currentDistance = calculateDistance(currentRoute);
    bestRoute = currentRoute;
    bestDistance = currentDistance;
}

//int SimulatedAnnealing::calculateRouteLength(int * route)
//{
//    int length = 0;
//    for (int i = 0; i < noOfVertices; i++) {
//        length += matrix[route[i]][route[i + 1 >= noOfVertices ? 0 : i + 1]];
//    }
//    return length;
//}

//int SimulatedAnnealing::getShortestRouteValue()
//{
//    return shortestRouteValue == INT32_MAX ? 0 : shortestRouteValue;
//}

void SimulatedAnnealing::invertElements(std::vector<int> route, int i, int j) {
    for (; i < j; j--) {
        int temp = route[j];
        route[j] = route[i];
        route[i] = temp;
    }
}

//void SimulatedAnnealing::copySolution(std::vector<int> from, std::vector<int> to)
//{
//      to = from;
////    for (int i = 0; i < noOfVertices; i++) {
////        to[i] = from[i];
////    }
//}

//string SimulatedAnnealing::getShortestRoute() {
//    if (matrix.empty()) {
//        return "Maciez jest pusta!";
//    }
//
//    string temp = "";
//    for (int i = 0; i < noOfVertices; i++) {
//        temp += to_string(shortestRoute[i]) + " -> ";
//    }
//    temp += to_string(shortestRoute[0]);
//    return temp;
//}

double SimulatedAnnealing::getTimeToBreakSearch() const {
    return timeToBreakSearch;
}

void SimulatedAnnealing::setTimeToBreakSearch(double timeToBreakSearch) {
    SimulatedAnnealing::timeToBreakSearch = timeToBreakSearch;
}

void SimulatedAnnealing::setCoolingValue(double coolingValue) {
    SimulatedAnnealing::coolingValue = coolingValue;
}

double SimulatedAnnealing::getCoolingValue() const {
    return coolingValue;
}

void SimulatedAnnealing::setDefaultParameters() {
    timeToBreakSearch = 10;
    coolingValue = 0.9999;
}

std::string SimulatedAnnealing::run() {
    startTime = std::chrono::high_resolution_clock::now();
    prepareToRun();

    temperature = bestDistance / noOfVertices * noOfVertices;
    bool timeNotExceeded = true;
    long long maxIterations = pow(noOfVertices, 4);
//    long long iterationWithoutChange = 0;
    long long maxIterationWithoutChange = maxIterations / 1000;
    std::uniform_int_distribution<std::mt19937::result_type> distVertex(1, noOfVertices - 1);
    std::uniform_real_distribution<double> distProb(0.0, 1.0);
    //cout << temperature << '\t' << maxIterations << '\t' << maxIterationWithoutChange << endl;

    std::vector<int> newRoute;
    std::vector<int> tempShortestRoute;
    //int *newRoute = new int[noOfVertices];
    //int *bestRoute = new int[noOfVertices];
    int bestRouteValue = bestDistance;
//    copySolution(bestRoute, tempShortestRoute);
    tempShortestRoute = bestRoute;

    while (timeNotExceeded) {
        while (maxIterations-- && iterationWithoutChange < maxIterationWithoutChange && temperature > 0.5) {
//        copySolution(tempShortestRoute, newRoute);
            newRoute = tempShortestRoute;
            int swapFrom = distVertex(randomGenerator);
            int swapTo = distVertex(randomGenerator);
            while (swapTo == swapFrom) {
                swapTo = distVertex(randomGenerator);
            }

            invertElements(newRoute, swapFrom, swapTo);
            int newRouteValue = calculateDistance(newRoute);

            if (newRouteValue < bestRouteValue) {
                bestRouteValue = newRouteValue;
//            copySolution(newRoute, bestRoute);
                bestRoute = newRoute;
                iterationWithoutChange = 0;
            } else {
                int diff = bestRouteValue - newRouteValue;
                double prob = exp(diff / temperature);
                if (distProb(randomGenerator) <= prob) {
                    bestRouteValue = newRouteValue;
//                copySolution(newRoute, bestRoute);
                    bestRoute = newRoute;
                    iterationWithoutChange = 0;
                } else {
                    iterationWithoutChange++;
                }
            }
            temperature *= coolingValue;
            if (bestRouteValue < shortestRouteValue) {
//            copySolution(tempShortestRoute, bestRoute);
                bestRoute = tempShortestRoute;
                shortestRouteValue = bestRouteValue;
            }
        }

        endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

        if (duration > timeToBreakSearch) {
            timeNotExceeded = false;
        }

    }

    //cout << temperature << '\t' << maxIterations << '\t' << maxIterationWithoutChange << '\t' << iterationWithoutChange << endl;
//    delete [] newRoute;
//    delete [] bestRoute;

    endTime = std::chrono::high_resolution_clock::now();
    return generateOutput();
}

std::string SimulatedAnnealing::generateOutput() {
    std::string output;

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

        output += "Temperatura koncowa: ";
        output += std::to_string(temperature);
        output += "\n";
    }

    return output;
}