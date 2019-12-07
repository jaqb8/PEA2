//
// Created by kubaa on 07.12.2019.
//

#include "TSP.h"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <random>
#include <iomanip>

using namespace std;

void TSP::loadTSP(string path) {
    vector<int> buffer;

    fstream file(path, ios::in);

    if(!file.is_open()) throw runtime_error("Nie odnaleziono pliku!");

    string temp;
    file >> temp;

    unsigned long noOfVertices;

    try {
        noOfVertices = stoul(temp);
    } catch (const exception &e) {
        throw runtime_error("Bledny format pliku!");
    }

    while (file >> temp) {
        try {
            buffer.push_back(stoi(temp));
        } catch (const exception &e) {
            buffer.clear();
            throw runtime_error("Bledna zawartosc pliku!");
        }
    }

    if (buffer.empty()) throw runtime_error("Plik jest pusty!");

    if (noOfVertices == 0) {
        TSPData.clear();
        TSPData.resize(0);
        return;
    }

    TSPData.clear();
    TSPData.resize(noOfVertices);

    int i = 0;

    for (auto &row : TSPData) {
        for (int j = 0; j < noOfVertices; ++j) {
            if (buffer.size() > i) {
                row.push_back(buffer[i++]);
            } else {
                throw runtime_error("Za malo danych w pliku! ");
            }
        }
    }

    for (int j = 0; j < TSPData.size(); ++j) {
        TSPData[j][j] = -1;
    }
}

string TSP::printData() {
    if (TSPData.empty()) {
        return "Macierz wierzcholkow jest pusta!";
    }

    stringstream output;

    output << "Ilosc wierzcholkow: " << TSPData.size() << endl << endl;

    output << "    | ";
    for (int i = 0; i < TSPData.size(); ++i) {
        output << setw(3) << i << " | ";
    }
    output << endl;

    output << "    -";
    for (int i = 0; i < TSPData.size(); ++i) {
        output << "------";
    }
    output << endl;

    int i = 0;
    for (auto &row : TSPData) {
        output << setw(3) << i++ << " |";
        for (auto &element : row) {
            output << setw(5) << element << " ";
        }
        output << endl;
    }
    return output.str();
}

unsigned long TSP::getNumberOfVertices() const {
    return TSPData.size();
}

int TSP::getDistance(int start, int end) const {
    if (start < 0 || end < 0) throw runtime_error("Bledny indeks wierzcholka!");
    if (start >= TSPData.size() || end >= TSPData.size()) throw runtime_error("Wierzcholek nie istnieje!");

    return TSPData[start][end];
}

int TSP::getMinimumDistanceFrom(int vertex) const {
    int min = INT32_MAX;

    for (int i = 0; i < TSPData.size(); ++i) {
        if (TSPData[vertex][i] < min && TSPData[vertex][i] >= 0) min = TSPData[vertex][i];
    }

    return min;
}

int TSP::getMinimumDistanceTo(int vertex) const {
    int min = INT32_MAX;

    for (const auto &row : TSPData) {
        if (row[vertex] < min && row[vertex] >= 0) min = row[vertex];
    }

    return min;
}

void TSP::generateRandomData(unsigned long noOfVertices, int range) {
    if (noOfVertices < 0) {
        throw runtime_error("Liczba wierzcholkow nie może byc ujemna!");
    }
    if (noOfVertices == 0) {
        TSPData.clear();
        TSPData.resize(0);
        return;
    }
    if (range < 1) {
        throw runtime_error("Górna granica zakresu musi byc dodatnia!");
    }

    random_device seed;
    mt19937 randomGenerator(seed());
    uniform_int_distribution<> rangeTransformer(1, range);

    TSPData.clear();
    TSPData.resize(noOfVertices);

    for (auto &row : TSPData) {
        for (int j = 0; j < noOfVertices; ++j) {
            row.push_back(rangeTransformer(randomGenerator));
        }
    }

    for (int i = 0; i < TSPData.size(); ++i) {
        TSPData[i][i] = -1;
    }
}

vector<vector<int>> TSP::getMatrix() {
    return TSPData;
}
