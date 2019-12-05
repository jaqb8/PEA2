//
// Created by kubaa on 05.12.2019.
//

#include "SimulatedAnnealing.h"
#include <iostream>
#include <random>
#include <math.h>

SimulatedAnnealing::SimulatedAnnealing() {
    randomGenerator.seed(std::random_device()());
}

SimulatedAnnealing::~SimulatedAnnealing() {
    if (shortestRoute != nullptr) {
        delete shortestRoute;
    }
}

int SimulatedAnnealing::setSource(string filename)
{
    return graph.getFromFile(filename);
}

string SimulatedAnnealing::printSource()
{
    return graph.printGraph();
}

void SimulatedAnnealing::initRoute()
{
    if (shortestRoute != nullptr) {
        delete shortestRoute;
    }
    shortestRoute = new int[graph.getVerticesNumber()];
    shortestRoute[0] = startVertex;

    bool* alreadyVisited = new bool[graph.getVerticesNumber()];
    for (int i = 0; i < graph.getVerticesNumber(); i++) {
        alreadyVisited[i] = false;
    }
    alreadyVisited[startVertex] = true;

    for (int i = 0; i < graph.getVerticesNumber() - 1; i++) {
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

int SimulatedAnnealing::calculateRouteLength(int * route)
{
    int length = 0;
    for (int i = 0; i < graph.getVerticesNumber(); i++) {
        length += graph.getVertex(route[i], route[i + 1 >= graph.getVerticesNumber() ? 0 : i + 1]);
    }
    return length;
}

int SimulatedAnnealing::getShortestRouteValue()
{
    return shortestRouteValue == INT32_MAX ? 0 : shortestRouteValue;
}

void SimulatedAnnealing::invertElements(int *route, int i, int j) {
    for (; i < j; j--) {
        int temp = route[j];
        route[j] = route[i];
        route[i] = temp;
    }
}

void SimulatedAnnealing::copySolution(int * from, int * to)
{
    for (int i = 0; i < graph.getVerticesNumber(); i++) {
        to[i] = from[i];
    }
}

string SimulatedAnnealing::getShortestRoute() {
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

void SimulatedAnnealing::solve() {
    if (graph.isEmpty()) {
        return;
    }
    initRoute();

    double temperature = shortestRouteValue / graph.getVerticesNumber() * graph.getVerticesNumber();
    double coolingValue = 0.9999;
    long long maxIterations = pow(graph.getVerticesNumber(), 4);
    long long iterationWithoutChange = 0;
    long long maxIterationWithoutChange = maxIterations / 1000;
    std::uniform_int_distribution<std::mt19937::result_type> distVertex(1, graph.getVerticesNumber() - 1);
    std::uniform_real_distribution<double> distProb(0.0, 1.0);
    cout << temperature << '\t' << maxIterations << '\t' << maxIterationWithoutChange << endl;

    int *newRoute = new int[graph.getVerticesNumber()];
    int *bestRoute = new int[graph.getVerticesNumber()];
    int bestRouteValue = shortestRouteValue;
    copySolution(shortestRoute, bestRoute);

    while (maxIterations-- && iterationWithoutChange < maxIterationWithoutChange && temperature > 0.5) {
        copySolution(bestRoute, newRoute);
        int swapFrom = distVertex(randomGenerator);
        int swapTo = distVertex(randomGenerator);
        while (swapTo == swapFrom) {
            swapTo = distVertex(randomGenerator);
        }

        invertElements(newRoute, swapFrom, swapTo);
        int newRouteValue = calculateRouteLength(newRoute);

        if (newRouteValue < bestRouteValue) {
            bestRouteValue = newRouteValue;
            copySolution(newRoute, shortestRoute);
            iterationWithoutChange = 0;
        } else {
            int diff = bestRouteValue - newRouteValue;
            double prob = exp(diff / temperature);
            if (distProb(randomGenerator) <= prob) {
                bestRouteValue = newRouteValue;
                copySolution(newRoute, shortestRoute);
                iterationWithoutChange = 0;
            } else {
                iterationWithoutChange++;
            }
        }
        temperature *= coolingValue;
        if (bestRouteValue < shortestRouteValue) {
            copySolution(bestRoute, shortestRoute);
            shortestRouteValue = bestRouteValue;
        }
    }
    cout << temperature << '\t' << maxIterations << '\t' << maxIterationWithoutChange << '\t' << iterationWithoutChange << endl;
    delete [] newRoute;
    delete [] bestRoute;
}