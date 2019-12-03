//
// Created by kubaa on 03.12.2019.
//

#include "TabuSearch.h"
#include <iostream>

TabuSearch::TabuSearch() {}

TabuSearch::~TabuSearch() {
    if (shortestRoute != nullptr) {
        delete shortestRoute;
    }
}

void TabuSearch::initRoute() {
    if (shortestRoute != nullptr) {
        delete shortestRoute;
    }
    shortestRoute = new int[graph.getVerticesNumber()];
    shortestRoute[0] = startVertex;
    
    bool *alreadyVisited = new bool[graph.getVerticesNumber()];
    for (int i = 0; i < graph.getVerticesNumber(); i++) {
        alreadyVisited[i] = false;
    }
    alreadyVisited[startVertex] = true;

    for (int i = 0; i < graph.getVerticesNumber(); i++) {
        int min = INT32_MAX;
        int chosen = -1;
        for (int j = 0; j < graph.getVerticesNumber(); j++) {
            if (graph.getVertex(shortestRoute[i], j) < min && !alreadyVisited[j]) {
                min = graph.getVertex(shortestRoute[i], j);
                chosen = j;
            }
        }
        alreadyVisited[chosen] = true;
        shortestRoute[i + 1] = chosen;
    }
    shortestRouteValue = calculateRouteLength(shortestRoute);
}

void TabuSearch::copySolution(int *from, int *to) {
    for (int i = 0; i < graph.getVerticesNumber(); i++) {
        to[i] = from[i];
    }
}

void TabuSearch::swapElements(int *route, int i, int j) {
    int temp = route[i];
    route[i] = route[j];
    route[j] = temp;
}

int TabuSearch::calculateRouteLength(int *route) {
    int length = 0;
    for (int i = 0; i < graph.getVerticesNumber(); ++i) {
        length += graph.getVertex(route[i], route[i + 1 >= graph.getVerticesNumber() ? 0 : i + 1]);
    }
    return length;
}

int TabuSearch::setSource(string filename) {
    return graph.getFromFile(filename);
}

string TabuSearch::printSource() {
    return graph.printGraph();
}

string TabuSearch::getShortestRoute() {
    if (graph.isEmpty()) {
        return "Maciez jest pusta!";
    }

    string temp = "";
    for (int i = 0; i < graph.getVerticesNumber(); i++) {
        temp += to_string(shortestRoute[i]) + " -> ";
    }
    temp += to_string(shortestRoute[0]);
    return temp;
}

int TabuSearch::getShortestRouteValue() {
    return shortestRouteValue == INT32_MAX ? 0 : shortestRouteValue;
}

void TabuSearch::solve(int repetitions, int repetitionsTillTabuRemoval, int aspirationValue) {
    if (graph.isEmpty()) return;
    initRoute();

    int aspirationPlus = false;
    int aspirationCounter = 0;
    int maxAspirationCount = 10;

    int **tabuList = new int * [graph.getVerticesNumber()];
    for (int i = 0; i < graph.getVerticesNumber(); i++) {
        tabuList[i] = new int[graph.getVerticesNumber()];
        for (int j = 0; j < graph.getVerticesNumber(); j++) {
            tabuList[i][j] = 0;
        }
    }

    int *currentRoute = new int[graph.getVerticesNumber()];
    copySolution(shortestRoute, currentRoute);

    int *tempRoute = new int[graph.getVerticesNumber()];
    int tempShortestValue = shortestRouteValue;
    int bestSwapFrom, bestSwapTo, bestSwapValue;

    while (repetitions--) {
        bestSwapValue = INT32_MAX;
        aspirationPlus = false;
        for (int i = 1; i < graph.getVerticesNumber(); i++) {
            for (int j = i + 1; j < graph.getVerticesNumber(); j++) {
                copySolution(currentRoute, tempRoute);
                swapElements(tempRoute, i, j);
                int diff = calculateRouteLength(tempRoute) - calculateRouteLength(currentRoute);

                if (((diff < bestSwapValue) && tabuList[i][j] == 0) || diff < aspirationValue) {
                    bestSwapValue = diff;
                    bestSwapFrom = i;
                    bestSwapTo = j;
                    if (diff < aspirationValue) {
                        aspirationPlus = true;
                        aspirationCounter = 0;
                    }
                }
                aspirationCounter++;
                if (aspirationCounter == maxAspirationCount && aspirationPlus) {
                    goto aspHappend;
                }
            }
        }
aspHappend:
        for (int i = 0; i < graph.getVerticesNumber(); i++) {
            for (int j = 0; j < graph.getVerticesNumber(); j++) {
                if (tabuList[i][j] > 0) {
                    tabuList[i][j]--;
                }
            }
        }

        swapElements(currentRoute, bestSwapFrom, bestSwapTo);
        tabuList[bestSwapFrom][bestSwapTo] = repetitionsTillTabuRemoval;
        tabuList[bestSwapTo][bestSwapFrom] = repetitionsTillTabuRemoval;

        if (calculateRouteLength(currentRoute) < shortestRouteValue) {
            swapElements(shortestRoute, bestSwapFrom, bestSwapTo);
            shortestRouteValue = calculateRouteLength(shortestRoute);
        }
    }

    for (int i = 0; i < graph.getVerticesNumber(); i++) {
        delete [] tabuList[i];
    }
    delete [] tabuList;
    delete [] currentRoute;
    delete [] tempRoute;
}
