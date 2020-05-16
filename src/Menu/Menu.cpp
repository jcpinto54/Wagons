#include "Menu.h"
#include "Utilities/utils.h"
#include "Person/Person.h"
#include "Utilities/Table.h"

using namespace std;

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

MainMenu::MainMenu(System *system) : Menu(system) {
    do {
        char o = this->option();
        switch (o) {
            case 'P':
                new PersonMenu(system);
                break;
            case 'V' : {
                sys->viewGraph();
            }
                break;
            case 'G' : {
                new GraphMenu(system);
            }
                break;
            case 'Q':
                return;
            default:
                break;
        }
    } while (true);
}

vector<vector<string>> MainMenu::getOptions() const {
    return vector<vector<string>>({{"P", "Person Menu"},
                                   {"V", "View Graph"},
                                   {"G", "Graph Menu"},
                                   {"Q", "Quit Program"}});
}

PersonMenu::PersonMenu(System *system) : Menu(system) {
    while (true) {
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'C' : {
                sys->createPerson();
            }
                break;
            case 'R' : {
                new ReadPersonMenu(system);
            }
                break;
            case 'U' : {
                new UpdatePersonMenu(system);
            }
                break;
            case 'D' : {
                sys->deletePerson();
            }
                break;
            case 'M':
                return;
            case 'Q':
                return;
            default:
                break;
        }
    }
}

vector<vector<string>> PersonMenu::getOptions() const {
    return vector<vector<string>>({{"C", "Create Person"},
                                   {"R", "Read People"},
                                   {"U", "Update Person"},
                                   {"D", "Delete Person"},
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}

UpdatePersonMenu::UpdatePersonMenu(System *system) : Menu(system) {
    string aux;
    aux = getInput(isNum, "Introduce an id: ", "Invalid Number");
    if (aux == ":q") return;
    unsigned id = stoi(aux);
    auto prs = sys->findPerson(id);
    if (prs == sys->getPeople().end()) {
        cout << "This person doesn't exist!" << endl;
        Util::pause();
        return;
    }
    while (true) {
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'N' : {
                sys->updatePersonName(prs);
            }
                break;
            case 'C' : {
                sys->updatePersonContact(prs);
            }
                break;
            case 'G' : {
                return;
            }
            default:
                break;
        }
    }
}

vector<vector<string>> UpdatePersonMenu::getOptions() const {
    return vector<vector<string>>({{"N", "Update Name"},
                                   {"C", "Update Contact"},
                                   {"G", "Go Back"}});
}


ReadPersonMenu::ReadPersonMenu(System *system) : ReadMenu<Person>(system) {
    do {
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'N' : {
                sort(sys->getPeople().begin(), sys->getPeople().end(), compareName);
                sys->readPeople(sys->getPeople());
            }
                break;
            case 'B' : {
                sort(sys->getPeople().begin(), sys->getPeople().end(), compareBirthday);
                sys->readPeople(sys->getPeople());
            }
                break;
            case 'I' : {
                sort(sys->getPeople().begin(), sys->getPeople().end(), compareId);
                sys->readPeople(sys->getPeople());
            }
                break;
            case 'V' : {
                sys->readPerson();
            }
                break;
            case 'G':
                return;
            default:
                break;
        }
    } while (true);
}

vector<vector<string>> ReadPersonMenu::getOptions() const {
    return vector<vector<string>>({{"N", "Sort by Name"},
                                   {"B", "Sort by Birthday"},
                                   {"I", "Sort by ID"},
                                   {"V", "View just one Person"},
                                   {"G", "Go Back"}});
}

GraphMenu::GraphMenu(System *system) : Menu(system) {
    while (true) {
        this->nextMenu = this->option();
        switch (this->nextMenu) {
            case 'P' : {
                string idFromStr, idToStr;
                idFromStr = getInput(isNum, "Enter the start vertex id: ", "Invalid Number");
                idToStr = getInput(isNum, "Enter the end vertex id: ", "Invalid Number");
                try {
                    sys->viewPathBetween2Points(stoi(idFromStr), stoi(idToStr));
                } catch (NonExistingVertex e) {
                    cout << e.getMsg() << endl;
                }
            }
                break;
            case 'M':
                return;
            case 'Q':
                return;
            default:
                break;
        }
    }
}

vector<vector<string>> GraphMenu::getOptions() const {
    return vector<vector<string>>({{"P", "Path between two Vertices"},
                                   {"M", "Main Menu"},
                                   {"Q", "Quit Program"}});
}