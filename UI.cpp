//
// Created by kubaa on 07.12.2019.
//

#include "UI.h"

#include <limits>

UI::UI() : TravellingSalesmanProblem(std::make_shared<TSP>()), TS(TravellingSalesmanProblem), SA(TravellingSalesmanProblem) {}

void UI::start() {
    char selection = 0;
    std::string path;
    unsigned long noOfVertices = 0;
    int range = 0;
    std::string output;

    do {
        printMenu();

        std::cin >> selection;
        std::cout << std::endl;

        switch (selection) {
            case '1':
                std::cout << "Podaj sciezke pliku: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                getline(cin, path);

                try {
                    TravellingSalesmanProblem->loadTSP(path);
                } catch (const runtime_error &e) {
                    cerr << e.what() << endl;
                }
                break;
            case '2':
                output = TravellingSalesmanProblem->printData();
                cout << output << endl;
                break;
            case '3':
                do {
                    std::cout << "Podaj ilość miast: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (!(std::cin >> noOfVertices));
                do {
                    std::cout << "Podaj zakres maksymalnej długości drogi (0 - zakres]: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (!(std::cin >> range));

                try {
                    TravellingSalesmanProblem->generateRandomData(noOfVertices, range);
                } catch (const std::runtime_error &e) {
                    std::cerr << e.what() << std::endl;
                }
                break;

            case '4':
                try {
                    runTSSettingsMenu();
                } catch (const std::runtime_error &e) {
                    std::cerr << e.what() << std::endl;
                }
                break;

            case '5':
                try {
                    output = TS.showInfoBeforeRunning();
                    std::cout << output << std::endl;

                    output = TS.run();
                    std::cout << output << std::endl;
                } catch (const std::runtime_error &e) {
                    std::cerr << e.what() << std::endl;
                }
                break;

            case '6':
                try {
                    runSASettingsMenu();
                } catch (const std::runtime_error &e) {
                    std::cerr << e.what() << std::endl;
                }
                break;

            case '7':
                try {
                    output = SA.showInfoBeforeRunning();
                    std::cout << output << std::endl;

                    output = SA.run();
                    std::cout << output << std::endl;
                } catch (const std::runtime_error &e) {
                    std::cerr << e.what() << std::endl;
                }

            case '0':
                break;

            default:
                std::cerr << "Wybrana opcja nie istnieje!" << std::endl;
                break;
        }

        } while (selection != '0');
}

void UI::printMenu() {
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "--- PROBLEM KOMIWOJAZERA ---" << std::endl;
    std::cout << "1. Wczytaj macierz wierzcholkow z pliku" << std::endl;
    std::cout << "2. Wyswietl macierz wierzcholkow" << std::endl;
    std::cout << "3. Wygeneruj losowa macierz" << std::endl;
    std::cout << "4. Zmien ustawienia algorytmu Tabu Search" << std::endl;
    std::cout << "5. Uruchom algorytm Tabu Search" << std::endl;
    std::cout << "6. Zmien ustawienia algorytmu Symulowanego Wyzarzania" << std::endl;
    std::cout << "7. Uruchom algorytm Symulowanego Wyzarzania" << std::endl;
    std::cout << "0. Wyjscie" << std::endl;
    std::cout << "Wybor: ";
}

void UI::runTSSettingsMenu() {
    char selection = 0;
    std::string output;
    double tempDouble = 0;

    do {
        printTSSettingsMenu();

        std::cin >> selection;
        std::cout << std::endl;

        switch (selection) {
            case '1':
                TS.setDefaultParameters();
                break;

            case '2':
                do {
                    std::cout << "Podaj czas wykonywania algorytmu w sekundach: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (!(std::cin >> tempDouble));
                TS.setTimeToBreakSearch(tempDouble);
                break;

            case '0':
                break;

            default:
                std::cerr << "Wybrana opcja nie istnieje!" << std::endl;
                break;
        }
    } while (selection != '0');
}

void UI::printTSSettingsMenu() {
    std::cout << "--- Ustawienia algorytmu Tabu Search --" << std::endl;
    std::cout << std::endl;
    std::cout << "Aktualne ustawienia:" << std::endl;
    std::cout << "> Czas wykonywania algorytmu: " << TS.getTimeToBreakSearch() << " (s)" << std::endl;
    std::cout << std::endl;
    std::cout << "1. Ustawienia domyslne" << std::endl;
    std::cout << "2. Zmien czas wykonywania algorytmu" << std::endl;
    std::cout << "0. Powrot" << std::endl;
    std::cout << "Wybor: ";
}

void UI::runSASettingsMenu() {
    char selection = 0;
    std::string output;
    double tempDouble = 0;

    do {
        printSASettingsMenu();
        std::cin >> selection;
        std::cout << std::endl;

        switch (selection) {
            case '1':
                SA.setDefaultParameters();
                break;

            case '2':
                do {
                    std::cout << "Podaj czas wykonywania algorytmu w sekundach: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } while (!(std::cin >> tempDouble));
                SA.setTimeToBreakSearch(tempDouble);
                break;

            case '3':
                do {
                    std::cout << "Podaj wartosc wspolczynnika zmiany temperatury: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize >::max(), '\n');
                } while (!(std::cin >> tempDouble));
                SA.setCoolingValue(tempDouble);
                break;
        }
    } while (selection != '0');
}

void UI::printSASettingsMenu() {
    std::cout << "--- Ustawienia algorytmu Symulowanego Wyzarzania --" << std::endl;
    std::cout << std::endl;
    std::cout << "Aktualne ustawienia:" << std::endl;
    std::cout << "> Czas wykonywania algorytmu: " << SA.getTimeToBreakSearch() << " (s)" << std::endl;
    std::cout << "> Wspolczynnik zmiany temperatury: " << SA.getCoolingValue() << std::endl;
    std::cout << std::endl;
    std::cout << "1. Ustawienia domyslne" << std::endl;
    std::cout << "2. Zmien czas wykonywania algorytmu" << std::endl;
    std::cout << "3. Zmien wartosc wspolczynnika zmieny temperatury" << std::endl;
    std::cout << "0. Powrot" << std::endl;
    std::cout << "Wybor: ";
}