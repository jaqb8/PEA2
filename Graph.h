//
// Created by kubaa on 03.12.2019.
//

#ifndef PEA2_GRAPH_H
#define PEA2_GRAPH_H
#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


#ifndef TO_STRING
#define TO_STRING
template<class s>
string to_string(s data) {
    stringstream ss;
    ss << data;
    return ss.str();
}
#endif

class Graph {
private:
    int noOfVertices;
    int **matrixPtr = nullptr;

    void createMatrix();
    void deleteMatrix();

public:
    Graph();
    ~Graph();
    int getFromFile(string filename);
    string printGraph();
    int getVerticesNumber();
    int getVertex(int n, int m);
    int **getMatrixPtr();
    bool isEmpty();

};


#endif //PEA2_GRAPH_H
