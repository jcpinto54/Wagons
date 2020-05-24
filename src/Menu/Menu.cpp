#include "Menu.h"
#include "Utilities/utils.h"
#include "Utilities/Table.h"
#include "Utilities/InvalidInput.h"

using namespace std;
using namespace Util;
using namespace chrono;

string Menu::readOption() const {
    string input;
    getline(cin, input);
    input = upper(input);
    return input;
}

bool Menu::validOption(const string &option) const {
    if (option.size() != 1) return false;
    vector<vector<string>> o = getOptions();
    vector<string> v;
    v.reserve(o.size());
    for (auto elem : o)
        v.push_back(elem.at(0));
    auto it = find(v.begin(), v.end(), option);
    return it != v.end();
}

char Menu::option() const {
    Table<string> options({"Letter", "Option"}, this->getOptions());
    cout << options;
    cout << "Choose an option: ";
    string o = this->readOption();
    while (!validOption(o)) {
        cout << options;
        cout << "'" << o << "' is not a valid option, choose a valid option: ";
        o = this->readOption();
    };
    return o.at(0);
}

char Menu::getNext() const {
    return this->nextMenu;
}

Menu::Menu(System *system) {
    this->nextMenu = 'Q';
    this->sys = system;
}

InitMenu::InitMenu() : Menu(nullptr) {
    cout << "What Graph do you want to load?" << endl;
    do {
        char o = this->option();
        switch (o) {
            case '0' :
                try {
                    this->sys = new System("../data/GridGraphs/4x4/");
                } catch (InvalidInput &e) {
                    cout << e.getMsg() << endl;
                }
                new MainMenu(this->sys);
                break;
            case '1' :
                try {
                    this->sys = new System("../data/GridGraphs/4x4NotSC/");
                } catch (InvalidInput &e) {
                    cout << e.getMsg() << endl;
                }
                new MainMenu(this->sys);
                break;
            case '2' :
                try {
                    this->sys = new System("../data/GridGraphs/8x8/");
                } catch (InvalidInput &e) {
                    cout << e.getMsg() << endl;
                }
                new MainMenu(this->sys);
                break;
            case '3' :
                try {
                    this->sys = new System("../data/GridGraphs/16x16/");
                } catch (InvalidInput &e) {
                    cout << e.getMsg() << endl;
                }
                new MainMenu(this->sys);
                break;
            case '4' :
                try {
                    this->sys = new System("../data/PenafielFull/");
                } catch (InvalidInput &e) {
                    cout << e.getMsg() << endl;
                }
                new MainMenu(this->sys);
                break;
            case '5' :
                try {
                    this->sys = new System("../data/PenafielStrong/");
                } catch (InvalidInput &e) {
                    cout << e.getMsg() << endl;
                }
                new MainMenu(this->sys);
                break;
            case '6' :
                try {
                    this->sys = new System("../data/EspinhoFull/");
                } catch (InvalidInput &e) {
                    cout << e.getMsg() << endl;
                }
                new MainMenu(this->sys);
                break;
            case '7' :
                try {
                    this->sys = new System("../data/EspinhoStrong/");
                } catch (InvalidInput &e) {
                    cout << e.getMsg() << endl;
                }
                new MainMenu(this->sys);
                break;
            case '8' :
                try {
                    this->sys = new System("../data/PortoFull/");
                } catch (InvalidInput &e) {
                    cout << e.getMsg() << endl;
                }
                new MainMenu(this->sys);
                break;
            case '9' :
                try {
                    this->sys = new System("../data/PortoStrong/");
                } catch (InvalidInput &e) {
                    cout << e.getMsg() << endl;
                }
                new MainMenu(this->sys);
                break;
            case 'Q':
                exit(0);
            default:
                break;
        }
    } while (true);
}

vector<vector<string>> InitMenu::getOptions() const {
    return vector<vector<string>>({{"0", "Grid 4x4", "25", "40"},
                                   {"1", "Grid 4x4 Not Strongly Conected", "25", "33"},
                                   {"2", "Grid 8x8", "81", "144"},
                                   {"3", "Grid 16x16", "289", "544"},
                                   {"4", "Full Penafiel", "10365", "10916"},
                                   {"5", "Strongly Conected Penafiel", "3964", "4237"},
                                   {"6", "Full Espinho", "17772", "19260"},
                                   {"7", "Strongly Conected Espinho", "7180", "7938"},
                                   {"8", "Full Porto", "53621", "59526"},
                                   {"9", "Strongly Conected Porto", "26098", "29488"},
                                   {"Q", "Quit Program", "-", "-"}});
}

char InitMenu::option() const {
    Table<string> options({"Letter", "Graph", "Nr. Nodes", "Nr. Edges"}, this->getOptions());
    cout << options;
    cout << "Choose a option: ";
    string o = this->readOption();
    while (!validOption(o)) {
        cout << options;
        cout << "'" << o << "' is not a valid option, choose a valid option: ";
        o = this->readOption();
    };
    return o.at(0);
}

MainMenu::MainMenu(System *system) : Menu(system) {
    do {
        char o = this->option();
        switch (o) {
            case 'G' :
                new GraphMenu(system);
                break;
            case 'T' :
                new TripMenu(system);
                break;
            case 'M' :
                new MeatMenu(system);
                break;
            case 'A' :
                new AlgorithmMenu(system);
                break;
            case 'L' :
                new InitMenu();
                break;
            case 'Q':
                exit(0);
            default:
                break;
        }
    } while (true);
}

vector<vector<string>> MainMenu::getOptions() const {
    return vector<vector<string>>({{"G", "Graph Menu"},
                                   {"T", "Trip Menu"},
                                   {"M", "Meat Menu"},
                                   {"A", "Algorithm Menu"},
                                   {"L", "Load another Graph"},
                                   {"Q", "Quit Program"}});
}

GraphMenu::GraphMenu(System *system) : Menu(system) {
    while (true) {
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'V' : {
                sys->viewGraph();
            }
                break;
            case 'P' : {
                string idFromStr, idToStr;
                idFromStr = getInput(isNum, "Enter the start vertex id: ", "Invalid Number");
                if (idFromStr == ":q") break;
                try {
                    sys->getMap().findLocal(stoi(idFromStr));
                } catch (NonExistingVertex e) {
                    cout << e.getMsg() << endl;
                    break;
                }
                idToStr = getInput(isNum, "Enter the end vertex id: ", "Invalid Number");
                if (idToStr == ":q") break;
                try {
                    sys->getMap().findLocal(stoi(idToStr));
                } catch (NonExistingVertex e) {
                    cout << e.getMsg() << endl;
                    break;
                }
                int algo = sys->readAlgorithm();
                try {
                    sys->viewPathBetween2Points(stoi(idFromStr), stoi(idToStr), algo);
                } catch (...) {
                    cout << "Error in in sys->viewPathBetween2Points";
                }
            }
                break;
            case 'A' : {
                string allPairsConfirmation = "N";
                allPairsConfirmation = Util::getInput(Util::isYorN, "Applying this algorithm for city graphs is not recomended because it can take a while(several minutes).\nAre you sure you want to continue?(Y/N) ", "Invalid Input");
                if (Util::isY(allPairsConfirmation)) {
                    int algo = this->sys->readAllPairsAlgorithm();
                    sys->applyAllPairs(algo);
                }
            }
                break;
            case 'C' : {
                sys->getMap().viewGraphConectivity();
            }
                break;
            case 'I' : {
                cout << "Instructions for this Menu:" << endl
                    << "- This menu can be used to calculate paths between two points only" << endl
                    << "- Option 'V' makes you view the graph" << endl
                    << "- Selecting option 'F', the program will pre-process the graph with Floyd-Warshall Algorithm.\n\tHowever with large maps, this may take a while: some seconds, even minutes" << endl
                    << "- Selecting option 'P', the program will give you the path between 2 points.\n\t If the graph Floyd Warshall Algorithm has been solved, this only looks for the path in memory.\n\tIf the graph has not been pre-processed it will use Dijkstra Algorithm to get the shortest path" << endl
                    << "- If you pre-process the graph, future paths calculating will be much faster (also applies for Trip Menu calculations)" << endl
                    << "- When vieweing graph conectivity (option 'C'), nodes belonging to the same strongly connected component have the same color.\n\tSince the GraphViewer API only supports 13 different colors, there might be color repetition." << endl;
            }
                break;
            case 'S' : {
                sys->printSuggestions();
            }
                break;
            case 'M':
                return;
            case 'Q':
                exit(0);
            default:
                break;
        }
    }
}

vector<vector<string>> GraphMenu::getOptions() const {
    return vector<vector<string>>({{"V", "View Graph"},
                                   {"P", "Path between two vertexes"},
                                   {"A", "Path between all pairs of vertexes"},
                                   {"C", "View Graph Conectivity"},
                                   {"I", "Instruction for this Menu"},
                                   {"S", "ID Suggestions"},
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}


TripMenu::TripMenu(System *system) : Menu(system) {
        while (true) {
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'A' : {
                try {
                    sys->addPOI();
                } catch (ImpossiblePath e) {
                    cout << e.getMsg() << endl;
                } catch (NonExistingVertex e) {
                    cout << e.getMsg() << endl;
                }
            }
                break;
            case 'E' : {
                sys->erasePOI();
            }
                break;
            case 'R' : {
                sys->readPOIs();
            }
                break;
            case 'C' : {
                if (sys->getPoIs().size() < 2) {
                    cout << "Not enough POIs: You need at least 2!" << endl;
                    break;
                }
                Util::triplet<vector<Local *>, double, pair<Time, unsigned>> tour = sys->solvePOITour();
                if (tour.second == -1.0) {
                    cout << "There is no path the Wagon can take to make the tour on time!" << endl;
                    break;
                }
                string viewWithAPI = Util::getInput(Util::isYorN, "Do you want to view the path in a gui mode?(Y/N) ", "Invalid Input");
                sys->getMap().viewTour(tour.first, tour.second, tour.third, sys->getPoIs(), Util::isY(viewWithAPI));
            }
                break;
            case 'I' : {
                cout << "Instructions for this Menu:" << endl
                     << "- When a POI is added, it is stored in a Vector" << endl
                     << "- If you want more than one POI just add more than one POI" << endl
                     << "- You can leave this menu and the inserted POIs will still be here" << endl
                     << "- If you want to know the optimal path that passes by all the POIs, then calculate the path('C')" << endl;
            }
                break;
            case 'S' : {
                sys->printSuggestions();
            }
                break;
            case 'M':
                return;
            case 'Q':
                exit(0);
            default:
                break;
        }
    }
}

vector<vector<string>> TripMenu::getOptions() const {
    return vector<vector<string>>({{"A", "Add POI"},
                                   {"E", "Erase POI"},
                                   {"R", "Read POIs"},
                                   {"C", "Calculate Trips"},
                                   {"I", "Menu Instructions"},
                                   {"S", "ID Suggestions"},
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}


MeatMenu::MeatMenu(System *system) : Menu(system) {
    while (true) {
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'A' : {
                sys->addPrisionerTransport();
            }
                break;
            case 'E' : {
                sys->erasePrisioner();
            }
                break;
            case 'R' : {
                sys->readPrisionersTransports();
            }
                break;
            case 'C' : {
                if (sys->getPrisioners().size() < 1) {
                    cout << "Not enough : You need at least 1!" << endl;
                    break;
                }
                vector<vector<POI *>> poisToAPI;
                vector<Prisioner *> errors;
                vector<triplet<vector<Local *>, double, pair<Time, unsigned>>> tours = sys->solvePrisionersTour(errors, poisToAPI);
                unsigned errorCounters = 0;
                for (auto e : errors) {
                    if (e == nullptr) continue;
                    cout << "Prisioner with ID " << e->getId() << " has an infeasible transport" << endl;
                    errorCounters++;
                }
                cout << "There are " << tours.size() - errorCounters << " tours." << endl;
                int tourCounter = 1;
                for (int i = 0; i < tours.size(); i++) {
                    cout << "Tour " << tourCounter++ << endl;
                    string viewWithAPI = Util::getInput(Util::isYorN, "Do you want to view the path in a gui mode?(Y/N) ", "Invalid Input");
                    sys->getMap().viewTour(tours[i].first, tours[i].second, tours[i].third, poisToAPI[i], Util::isY(viewWithAPI));
                    getInput(isNext, "Write 'next' to advance: ", "You didn't write next!");
                }
            }
                break;
            case 'I' : {
                cout << "Instructions for this Menu:" << endl
                    << "- It works with a vector of Prisioners" << endl;
            }
                break;
            case 'S' : {
                sys->printSuggestions();
            }
                break;
            case 'M':
                return;
            case 'Q':
                exit(0);
            default:
                break;
        }
    }
}

vector<vector<string>> MeatMenu::getOptions() const {
    return vector<vector<string>>({{"A", "Add Prisioner"},
                                   {"E", "Erase Prisioner"},
                                   {"R", "Read Prisioners"},
                                   {"C", "Calculate Trips"},
                                   {"I", "Menu Instructions"},
                                   {"S", "ID Suggestions"},
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}

AlgorithmMenu::AlgorithmMenu(System *system) : Menu(system) {
    while (true) {
        sys->getMap().resetTarjanSolved();
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'S' : {
                new ShortestPathMenu(system);
                break;
            }
            case 'T' : {
                auto start = high_resolution_clock::now();
                sys->applyTarjan();
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                cout << "The execution time was: " << duration.count() << " microseconds" << endl;
                break;
            }
            case 'F' : {
                new FirstSearchMenu(system);
                break;
            }
            case 'M':
                return;
            case 'Q':
                exit(0);
            default:
                break;
        }
    }
}

vector<vector<string>> AlgorithmMenu::getOptions() const {
    return vector<vector<string>>({{"S", "Shortest Path Algorithms"},
                                   {"T", "Tarjan / Connectivity Analysis Algorithm"},
                                   {"F", "First Search Algorithms"},
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}

ShortestPathMenu::ShortestPathMenu(System *system) : Menu(system) {
    while (true) {
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'S' : {
                new SingleSourceMenu(system);
                break;
            }
            case 'A' : {
                new AllPairsMenu(system);
                break;
            }
            case 'M':
                new MainMenu(system);
                break;
            case 'Q':
                exit(0);
            default:
                break;
        }
    }
}

vector<vector<string>> ShortestPathMenu::getOptions() const {
    return vector<vector<string>>({{"S", "Single Source Algorithms"},
                                   {"A", "All Pairs Algorithms"},
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}

SingleSourceMenu::SingleSourceMenu(System *system) : Menu(system) {
    string idFromStr, idToStr;
    Local *from, *to;
    while (true) {
        idFromStr = getInput(isNum, "Enter the start vertex id: ", "Invalid Number");
        if (idFromStr == ":q") break;
        try {
            from = sys->getMap().findLocal(stoi(idFromStr));
        } catch (NonExistingVertex e) {
            cout << "This is not a vertex!" << endl;
            continue;
        }

        idToStr = getInput(isNum, "Enter the end vertex id: ", "Invalid Number");
        if (idToStr == ":q") break;
        try {
            to = sys->getMap().findLocal(stoi(idToStr));
        } catch (NonExistingVertex e) {
            cout << "This is not a vertex!" << endl;
            continue;
        }
        break;
    }
    int algo; 
    while (true) {
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'D' : {
                algo = 0;
                auto start = high_resolution_clock::now();
                sys->applyDijkstra(from);
                sys->applySingleSource(to);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                cout << "The execution time was: " << duration.count() << " microseconds" << endl;
                break;
            }
            case 'A' : {
                algo = 2;
                auto start = high_resolution_clock::now();
                sys->applyAStar(from, to);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                cout << "The execution time was: " << duration.count() << " microseconds" << endl;
                break;
            }
            case 'M':
                new MainMenu(system);
                break;
            case 'Q':
                exit(0);
            default:
                break;
        }
    }
}

vector<vector<string>> SingleSourceMenu::getOptions() const {
    return vector<vector<string>>({{"D", "Dijkstra Algorithm"},
                                   {"A", "A Star Algorithm"},
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}

AllPairsMenu::AllPairsMenu(System *system) : Menu(system) {
    string floydWarshallConfirmation = "N";
    floydWarshallConfirmation = Util::getInput(Util::isYorN,
                                               "Applying these algorithms for city graphs is not recomended because it can take a while(several minutes).\nAre you sure you want to continue?(Y/N) ",
                                               "Invalid Input");
    if (isN(floydWarshallConfirmation)) return;
    while (true) {
        sys->getMap().resetAllPairsSolved();
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'A' : {
                auto start = high_resolution_clock::now();
                sys->applyAllPairs(1);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                cout << "The execution time was: " << duration.count() << " microseconds" << endl;
                break;
            }
            case 'F' : {
                auto start = high_resolution_clock::now();
                sys->applyAllPairs(0);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                cout << "The execution time was: " << duration.count() << " microseconds" << endl;
                break;
            }
            case 'M':
                new MainMenu(system);
                break;
            case 'Q':
                exit(0);
            default:
                break;
        }
    }
}

vector<vector<string>> AllPairsMenu::getOptions() const {
    return vector<vector<string>>({{"A", "A Star Algorithm"},
                                   {"F", "Floyd Warshall Algorithm"},
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}

FirstSearchMenu::FirstSearchMenu(System *system) : Menu(system) {
    string idFromStr;
    Local *from;
    while (true) {
        idFromStr = getInput(isNum, "Enter the start vertex id: ", "Invalid Number");
        if (idFromStr == ":q") break;
        try {
            from = sys->getMap().findLocal(stoi(idFromStr));
        } catch (NonExistingVertex e) {
            cout << "This is not a vertex!" << endl;
            continue;
        }
        break;
    }
    while (true) {
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'B' : {
                auto start = high_resolution_clock::now();
                sys->applyBfs(from);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                cout << "The execution time was: " << duration.count() << " microseconds" << endl;
                break;
            }
            case 'D' : {
                auto start = high_resolution_clock::now();
                sys->applyDfs();
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                cout << "The execution time was: " << duration.count() << " microseconds" << endl;
                break;
            }
            case 'M': {
                new MainMenu(system);
                break;
            }
            case 'Q':
                exit(0);
            default:
                break;
        }
    }
}

vector<vector<string>> FirstSearchMenu::getOptions() const {
    return vector<vector<string>>({{"B", "Breadth-first search"},
                                   {"D", "Depth-first search"},
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}


bool isNext(const string &toTest) {
    return (toTest == "next");
}

