//
// Created by kubaa on 03.12.2019.
//

#include "Graph.h"

Graph::Graph() {

}

Graph::~Graph() {
    deleteMatrix();
}

void Graph::createMatrix() {
    matrixPtr = new int * [noOfVertices];
    for (int i = 0; i < noOfVertices; i++) {
        matrixPtr[i] = new int [noOfVertices];
        for (int j = 0; j < noOfVertices; j++) {
            matrixPtr[i][j] = INT32_MAX;
        }
    }
}

void Graph::deleteMatrix() {
    if (matrixPtr != nullptr) {
        for (int i = 0; i < noOfVertices; i++) {
            delete [] matrixPtr[i];
        }
        delete [] matrixPtr;
        matrixPtr = nullptr;
    }
}

int Graph::getFromFile(string filename) {
    deleteMatrix();
    ifstream input(filename.c_str());
    if (!input) {
        return 0;
    }
    input >> noOfVertices;

    createMatrix();

    for (int i = 0; i < noOfVertices; i++) {
        for (int j = 0; j < noOfVertices; j++) {
            int temp;
            input >> temp;
            matrixPtr[i][j] = temp;
            if (i == j) matrixPtr[i][j] = INT32_MAX;
        }
    }

    input.close();
    return 1;
}

string Graph::printGraph() {
    if(matrixPtr == nullptr) {
        return "Maciez jest pusta!\n";
    }

    stringstream output;

    output << "Ilosc wierzcholkow: " << getVerticesNumber() << endl << endl;

    output << "    | ";
    for (int i = 0; i < noOfVertices; ++i) {
        output << setw(3) << i << " | ";
    }
    output << endl;

    output << "    -";
    for (int i = 0; i < noOfVertices; ++i) {
        output << "------";
    }
    output << endl;


    for (int i = 0; i < noOfVertices; i++) {
        output << setw(3) << i << " |";
        for (int j = 0; j < noOfVertices; j++) {
            if (matrixPtr[i][j] == INT32_MAX) {
                output << setw(5) << " - ";
            } else {
                output << setw(5) << matrixPtr[i][j] << " ";
            }
        }
        output << endl;
    }
    return output.str();
}

int Graph::getVerticesNumber() {
    return noOfVertices;
}

int Graph::getVertex(int n, int m) {
    return matrixPtr[n][m];
}

int** Graph::getMatrixPtr() {
    return matrixPtr;
}

bool Graph::isEmpty() {
    return matrixPtr == nullptr;
}
