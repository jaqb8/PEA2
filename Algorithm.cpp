//
// Created by kubaa on 07.12.2019.
//

#include "Algorithm.h"

#include <random>


using namespace std;


Algorithm::Algorithm(shared_ptr<TSP> TravellingSalesmanProblem) : TravellingSalesmanProblem(move(TravellingSalesmanProblem)) {}

Algorithm::~Algorithm() = default;

void Algorithm::prepareToRun() {
    noOfChecks = 0;

    noOfVertices = TravellingSalesmanProblem->getNumberOfVertices();
    if (noOfVertices < 2) throw runtime_error("Macierz wierzcholkow jest pusta lub zawiera tylko jedno miasto!");

    currentRoute.clear();
    bestRoute.clear();

    currentDistance = 0;
    bestDistance = INT32_MAX;

    startVertex = 0;
}

int Algorithm::calculateDistance(vector<int> &route) {
    int currentVertex = -1;
    int nextVertex = -1;
    int distance = 0;

    for (int i = 0; i < route.size(); ++i) {
        currentVertex = route[i];
        if (i != route.size() - 1) nextVertex = route[i + 1];
        else nextVertex = route[0];

        distance += TravellingSalesmanProblem->getDistance(currentVertex, nextVertex);
    }

    return distance;
}

vector<int> Algorithm::generateRandomRoute() {
    vector<bool> visitedVertices;
    vector<int> route;

    visitedVertices.clear();
    visitedVertices.resize(noOfVertices);

    route.clear();

    int currentVertex = startVertex;
    route.push_back(currentVertex);
    visitedVertices[currentVertex] = true;

    int nextVertex = -1;

    random_device seed;
    mt19937 randomGenerator(seed());
    uniform_int_distribution<> rangeTransformer(0, noOfVertices - 1);

    for (int i = 0; i < noOfVertices - 1; ++i) {
        do {
            nextVertex = rangeTransformer(randomGenerator);
        } while (visitedVertices[nextVertex]);

        currentVertex = nextVertex;

        route.push_back(currentVertex);
        visitedVertices[currentVertex] = true;
    }

    return route;
}
