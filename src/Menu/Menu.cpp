#include "Menu.h"
#include "Utilities/utils.h"
#include "Utilities/Table.h"
#include "Utilities/InvalidInput.h"

using namespace std;
using namespace Util;

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
    cout << "Choose a option: ";
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
            case 'L' :
//                delete this->sys;
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
                idToStr = getInput(isNum, "Enter the end vertex id: ", "Invalid Number");
                if (idToStr == ":q") break;
                try {
                    sys->viewPathBetween2Points(stoi(idFromStr), stoi(idToStr));
                } catch (NonExistingVertex e) {
                    cout << e.getMsg() << endl;
                }
            }
                break;
            case 'F' : {
                string floydWarshallConfirmation = "N";
                floydWarshallConfirmation = Util::getInput(Util::isYorN, "Applying this algorithm for city graphs is not recomended because it can take a while(several minutes).\nAre you sure you want to continue?(Y/N) ", "Invalid Input");
                if (Util::isY(floydWarshallConfirmation))
                    sys->applyFloydWarshall();
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
                                   {"P", "Path between two Vertices"},
                                   {"F", "Apply Floyd-Warshall algortihm"},
                                   {"C", "View Graph Conectivity"},
                                   {"I", "Instruction for this Menu"},
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}


TripMenu::TripMenu(System *system) : Menu(system) {
        while (true) {
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'A' : {
                try {
                    Table<string> sugestions({"Police", "Prison", "Court", "HQ"}, this->getSugestions(system));
                    cout << "Here's some POI Id's examples:\n" << sugestions;
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
                if (sys->getPoIs().size() <= 1) {
                    cout << "Not enough POIs: You need at least 2!" << endl;
                    break;
                }
                pair<vector<Local *>, double> tour = sys->solvePOITour();
                string viewWithAPI = Util::getInput(Util::isYorN, "Do you want to view the path in a gui mode?(Y/N) ", "Invalid Input");
                sys->getMap().viewTour(tour.first, tour.second, sys->getPoIs(), Util::isY(viewWithAPI));
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
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}

vector<vector<string>> TripMenu::getSugestions(System *system) const {
    if(system->getGraphPath() == "../data/EspinhoFull/"){
        return vector<vector<string>>({{"15892", "13504", "17126", "8498"},
                                       {"9882", "9243", "13357", "1"},
                                       {"305", "10031", "8001", ""}});
    } else if(system->getGraphPath() == "../data/EspinhoStrong/"){
        return vector<vector<string>>({{"6042", "2374", "4818", "6927"},
                                       {"4377", "4447", "230", "1"},
                                       {"1211", "75", "4148", ""}});
    } else if(system->getGraphPath() == "../data/PenafielFull/"){
        return vector<vector<string>>({{"5531", "9432", "9051", "2485"},
                                       {"2167", "5116", "10218", "1"},
                                       {"8885", "6104", "8313", ""}});
    } else if(system->getGraphPath() == "../data/PenafielStrong/"){
        return vector<vector<string>>({{"962", "3421", "1146", "3620"},
                                       {"3255", "1326", "3185", "1"},
                                       {"3469", "3623", "774", ""}});
    } else if(system->getGraphPath() == "../data/PortoFull/"){
        return vector<vector<string>>({{"3754", "4746", "49178", "6638"},
                                       {"18813", "5764", "38069", "1"},
                                       {"19020", "15107", "547", ""}});
    } else if(system->getGraphPath() == "../data/PortoStrong/"){
        return vector<vector<string>>({{"9447", "22347", "13991", "14979"},
                                       {"16993", "16760", "18897", "1"},
                                       {"16573", "60", "10413", ""}});
    }
}

