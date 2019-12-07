//
// Created by kubaa on 07.12.2019.
//

#ifndef PEA2_UI_H
#define PEA2_UI_H

#include "TSP.h"
#include "TabuSearch.h"
#include "SimulatedAnnealing.h"

#include <iostream>
#include <memory>

class UI {
public:
    UI();
    void start();

private:
    void printMenu();
    void runTestMenu();
    void printTestMenu();
    void runTSSettingsMenu();
    void printTSSettingsMenu();
    void runSASettingsMenu();
    void printSASettingsMenu();

    std::shared_ptr<TSP> TravellingSalesmanProblem;

    TabuSearch TS;
    SimulatedAnnealing SA;
};


#endif //PEA2_UI_H
