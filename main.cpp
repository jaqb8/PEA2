#include <iostream>
#include "Graph.h"
#include "TabuSearch.h"
#include "SimulatedAnnealing.h"

int main() {
    SimulatedAnnealing SA;
    SA.setSource("br17.txt");
    SA.solve();
    cout << SA.getShortestRoute() << endl;
    cout << SA.getShortestRouteValue();

//    TabuSearch TS;
//    TS.setSource("br17.txt");
//    TS.solve();
//    cout << TS.getShortestRoute() << endl;
//    cout << TS.getShortestRouteValue();


    return 0;
}