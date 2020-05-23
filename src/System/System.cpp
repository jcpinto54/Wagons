#include "System.h"
#include "Utilities/utils.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <utility>
#include "Utilities/InvalidInput.h"
#include <unordered_map>
#include <sys/wait.h>


using namespace std;
using namespace Util;

System::System(const string &graphPath) {
    cout << "Loading Graph...\n";

    ifstream file;

    if (graphPath.find("GridGraphs") != string::npos)
        this->map.setDirected(false);

    string graphNodesFile = graphPath + "nodes.txt";
    int nInputs;

    unordered_map<unsigned, Vertex<Local *> *> tempGraph;

    file.open(graphNodesFile);
    if (!file.is_open()) throw InvalidInput("graph nodes is not open");
    file >> nInputs;
    file.ignore(100, '\n');
    for (int i = 0; i < nInputs; i++) {
        Local *l;
        file >> &l;
        tempGraph[l->getId()] = new Vertex<Local *>(l);
    }
    file.close();

    vector<string> aux;
    int edgeCounter = 0;
    string graphEdgesFile = graphPath + "edges.txt";
    string line;
    file.open(graphEdgesFile);
    file >> nInputs;
    file.ignore(100, '\n');
    for (int i = 0; i < nInputs; i++) {

        getline(file, line);
        line[0] = ' ';
        line.pop_back();
        aux = trim(split(line, ","));
        if (aux.size() != 2) throw InvalidInput("Invalid edge reading");

        unsigned idFrom = stoi(aux.at(0));
        unsigned idTo = stoi(aux.at(1));
        double distEdge = this->map.dist(tempGraph[idFrom]->getInfo(), tempGraph[idTo]->getInfo());
        tempGraph[idFrom]->getAdj().emplace_back(tempGraph[idTo], distEdge);
        edgeCounter++;
        if (!this->map.isDirected()) {
            tempGraph[idTo]->getAdj().emplace_back(tempGraph[idFrom], distEdge);
            edgeCounter++;
        }
    }
    file.close();

    string graphTagsFile = graphPath + "tags.txt";

    file.open(graphTagsFile);
    file >> nInputs;
    file.ignore(100, '\n');
    string tagName;
    int nTags, nodeId;
    for (int i = 0; i < nInputs; i++) {
        getline(file, tagName);
        file >> nTags;
        file.ignore(100, '\n');
        for (int j = 0; j < nTags; j++) {
            getline(file, line);
            nodeId = stoi(line);
            tempGraph[nodeId]->getInfo()->setTag(tagName);
            if (tagName == "hq")
                this->hq = (tempGraph[nodeId]->getInfo());
        }
    }
    file.close();

    this->map.init(tempGraph);
    this->map.setNumEdges(edgeCounter);
}

System::~System() {
    wait(NULL);
}

void System::viewGraph() {
    map.viewGraph(VIEW);
}

void System::viewPathBetween2Points(unsigned int idFrom, unsigned int idTo, int &algo) {
    string viewWithAPI = "N";
    viewWithAPI = Util::getInput(Util::isYorN, "Do you want to view the path in a gui mode?(Y/N) ", "Invalid Input");
    this->map.viewPath(idFrom, idTo, isY(viewWithAPI), algo);
}

void System::applyFloydWarshall() {
    this->map.applyFloydWarshall();
}


void System::addPOI() {
    string dateStr, timeStr, locIdStr;
    string addHQ;
    Local *loc;
    if (this->POIs.empty()) {
        addHQ = getInput(isYorN, "Do you want to add the company's hq to the POI(Y/N)?", "Invalid Input");
    }
    if (!isY(addHQ) || !this->POIs.empty()) {
        locIdStr = getInput(isNum, "Enter Local ID: ", "Invalid Number");
        if (locIdStr == ":q")
            return;
        unsigned locId = stoi(locIdStr);
        try {
            loc = this->map.findLocal(locId);
        } catch (NonExistingVertex e) {
            throw NonExistingVertex(e);
        }
    }
    else {
        loc = this->hq;
    }
    dateStr = getInput(isDate, "Enter the max date a wagon needs to pass here(DD/MM/YYYY): ", "Invalid Time");
    if (dateStr == ":q")
        return;
    timeStr = getInput(isTime, "Enter the max time a wagon needs to pass here(HH:MM): ", "Invalid Time");
    if (timeStr == ":q")
        return;
    POI *poi = new POI(loc, Date(dateStr), Time(timeStr));
    if (this->findPOI(poi) != this->POIs.end()) {
        cout << "This point has already been added" << endl;
        return;
    }

    this->POIs.push_back(poi);
    bool connected = this->map.areStronglyConected(this->POIs);
    if (!connected) {
        this->POIs.pop_back();
        throw ImpossiblePath(loc);
    }
}

void System::erasePOI() {
    string locIdStr = getInput(isNum, "Enter POI Local ID: ", "Invalid Number");
    if (locIdStr == ":q")
        return;

    auto itPoi = this->findPOI(stoi(locIdStr));
    if (itPoi == this->POIs.end()) {
        cout << "ID is not of a POI!" << endl;
        return;
    }

    this->POIs.erase(itPoi);
}

vector<POI *>::iterator System::findPOI(unsigned int id) {
    for (auto it = this->POIs.begin(); it != this->POIs.end(); it++) {
        if ((*(*it)->getLoc()) == Local(id)) {
            return it;
        }
    }
    return this->POIs.end();
}

void System::readPOIs() {
    if (this->POIs.empty()) {
        cout << "The container of POIs is empty" << endl;
        return;
    }
    auto read = toTable(this->POIs, this);
    cout << read;
    Util::pause();
}

pair<vector<Local *>, double> System::solvePOITour() {
    Wagon * wagon = this->chooseWagon();
    return this->map.minimumWeightTour(&this->POIs, wagon);
}

Map &System::getMap() {
    return map;
}

const vector<POI*> &System::getPoIs() const{
    return this->POIs;
}

vector<POI *>::iterator System::findPOI(const POI *poi) {
    for (auto it = this->POIs.begin(); it != this->POIs.end(); it++) {
        if (**it == *poi) return it;
    }
    return this->POIs.end();
}

Wagon* System::chooseWagon() {
    vector<string> header = {"Option","Wagon", "Avg. Speed"};
    vector<vector<string>> content;
    vector<string> aux = {"0", "Regular","40km/h"};
    content.push_back(aux);
    aux = {"1", "Speedy","50km/h"};
    content.push_back(aux);
    Table<string> data(header, content);
    string option = Util::getInput(isWagonOption, "Choose a Wagon to use: ", "Invalid Choice");
    if (option == ":q")
        return new Wagon(-1);
    switch(stoi(option)) {
        case 0:
            return new Wagon(40.0/60.0*1000);
        case 1:
            return new Wagon(50.0/60.0*1000);
    }
    return new Wagon(0);
}

int System::readAlgorithm()
{
    vector<string> header = {"Option","Algorithm"};
    vector<vector<string>> content;
    vector<string> aux = {"0", "Dijkstra"};
    content.push_back(aux);
    aux = {"1", "Floyd-Warshall"};
    content.push_back(aux);
    aux = {"2", "A*"};
    content.push_back(aux);
    Table<string> data(header, content);
    cout << data;
    string option = Util::getInput(isAlgo, "Choose a Algorithm: ", "Invalid Choice");

    return stoi(option);
}


Table<string> toTable(const vector<POI *> &container, const System *sys) {
    vector<string> header = {"Local ID", "X Coordinate", "Y Coordinate", "Tag", "Date to Pass", "Time to Pass"};
    vector<vector<string>> content;
    for (auto local : container) {
        vector<string> aux = {to_string(local->getLoc()->getId()),to_string(local->getLoc()->getX()),
                              to_string(local->getLoc()->getY()), tagToStr(local->getLoc()->getTag()),
                              local->getDate().str(), local->getTime().str()};
        content.push_back(aux);
    }
    Table<string> data(header, content);
    return data;
}

bool isAlgo(const string &toTest){
    if (!isNum(toTest)) return false;
    int n = stoi(toTest);
    return n == 0 || n == 1 || n == 2;
}

bool isWagonOption(const string &toTest) {
    if (toTest == ":q") return true;
    if (!isNum(toTest)) return false;
    int n = stoi(toTest);
    return n == 0 || n == 1;
}
