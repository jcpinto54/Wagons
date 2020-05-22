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
    this->graphPath = graphPath;

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
                this->POIs.push_back(tempGraph[nodeId]->getInfo());
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

void System::viewPathBetween2Points(unsigned int idFrom, unsigned int idTo) {
    string viewWithAPI = "N";
    viewWithAPI = Util::getInput(Util::isYorN, "Do you want to view the path in a gui mode?(Y/N) ", "Invalid Input");
    this->map.viewPath(idFrom, idTo, isY(viewWithAPI));
}

void System::applyFloydWarshall() {
    this->map.applyFloydWarshall();
}


void System::addPOI() {
    string locIdStr = getInput(isNum, "Enter Local ID: ", "Invalid Number");
    if (locIdStr == ":q")
        return;
    unsigned locId = stoi(locIdStr);
    Local *loc;
    try {
        loc = this->map.findLocal(locId);
    } catch (NonExistingVertex e) {
        throw NonExistingVertex(e);
    }
    if (find(this->POIs.begin(), this->POIs.end(), loc) != this->POIs.end()) {
        cout << "This point has already been added" << endl;
        return;
    }

    this->POIs.push_back(loc);
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

vector<Local *>::iterator System::findPOI(unsigned int id) {
    for (auto it = this->POIs.begin(); it != this->POIs.end(); it++) {
        if (**it == Local(id)) {
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
    vector<unsigned> poisInIds;
    for (auto & POI : this->POIs) {
        poisInIds.push_back(POI->getId());
    }
    return this->map.minimumWeightTour(&poisInIds);
}

Map &System::getMap() {
    return map;
}

const vector<Local*> &System::getPoIs() const{
    return this->POIs;
}


Table<string> toTable(const vector<Local *> &container, const System *sys) {
    vector<string> header = {"ID", "X Coordinate", "Y Coordinate", "Tag"};
    vector<vector<string>> content;
    for (auto local : container) {
        vector<string> aux = {to_string(local->getId()),to_string(local->getX()),
                              to_string(local->getY()), tagToStr(local->getTag())};
        content.push_back(aux);
    }
    Table<string> data(header, content);
    return data;
}

