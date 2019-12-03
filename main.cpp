#include <iostream>
#include "Graph.h"
#include "TabuSearch.h"

int main() {
    TabuSearch ts;
    ts.setSource("br17.txt");
    ts.solve();
    cout << ts.getShortestRoute() << endl;
    cout << ts.getShortestRouteValue() << endl;

    return 0;
}