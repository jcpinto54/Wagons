#include "System.h"
#include "Utilities/utils.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <utility>
#include "Utilities/InvalidInput.h"
#include <unordered_map>
#include <sys/wait.h>
#include <Wagon/Wagon.h>


using namespace std;
using namespace Util;

System::System(const string &graphPath) {
    this->graphPath = graphPath;
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

void System::applyAllPairs(int algo) {
    this->map.applyAllPairs(algo);
}

void System::addPOI() {
    string dateStr, timeStr, locIdStr;
    string addHQ;
    Local *loc;
    if (this->POIs.empty()) {
        addHQ = getInput(isYorN, "Do you want to add the company's hq to the POI(Y/N)?", "Invalid Input");
        if (isN(addHQ)) {
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

        while (true) {
            dateStr = getInput(isDate, "What day do you want to start the tour(DD/MM/YYYY)? ", "Invallid Date");
            if (dateStr == ":q") {
                return;
            }
            if (Date() < Date(dateStr)) break;
            cout << "This day was in the past!" << endl;
        }

        while (true) {
            timeStr = getInput(isTime,"What time do you want to start the tour(HH:MM:SS)? ", "Invalid Time");
            if (timeStr == ":q") {
                return;
            }
            if (Date() < Date(dateStr) || Date() == Date(dateStr) && Time() < Time(timeStr)) break;
            cout << "Too late to plan for this time!" << endl;
        }
        Time t = Time(timeStr);
        this->POIs.push_back(new POI(loc, Date(dateStr), t));
        return;
    }

    locIdStr = getInput(isNum, "Enter Local ID: ", "Invalid Number");
    if (locIdStr == ":q")
        return;
    unsigned locId = stoi(locIdStr);
    try {
        loc = this->map.findLocal(locId);
    } catch (NonExistingVertex e) {
        throw NonExistingVertex(e);
    }

    timeStr = getInput(isTime, "How much time does the wagon have to get from the start POI to here(HH:MM:SS)? ", "Invalid Time");
    if (timeStr == ":q")
        return;
    Date POIDate = this->POIs[0]->getDt().date;
    Time initTime = this->POIs[0]->getDt().time, time = Time(timeStr);
    if ((initTime + time).second) POIDate = Date(this->POIs[0]->getDt().date + 1);

    POI *poi = new POI(loc, POIDate, (initTime + time).first);
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

Util::triplet<vector<Local *>, double, pair<Time, unsigned>> System::solvePOITour() {
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
    cout << data;
    string option = Util::getInput(isWagonOption, "Choose a Wagon to use: ", "Invalid Choice");
    if (option == ":q")
        return new Wagon(-1);
    switch(stoi(option)) {
        case 0:
            return new Wagon(40.0/3.6);
        case 1:
            return new Wagon(50.0/3.6);
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

int System::readAllPairsAlgorithm()
{
    vector<string> header = {"Option","Algorithm"};
    vector<vector<string>> content;
    vector<string> aux = {"0", "Floyd-Warshall"};
    content.push_back(aux);
    aux = {"1", "A*"};
    content.push_back(aux);
    Table<string> data(header, content);
    cout << data;
    string option = Util::getInput(isAllPairsAlgo, "Choose a Algorithm: ", "Invalid Choice");

    return stoi(option);
}


Table<string> toTable(const vector<POI *> &container, const System *sys) {
    vector<string> header = {"Local ID", "X Coordinate", "Y Coordinate", "Tag", "Time to pass"};
    vector<vector<string>> content;
    for (auto local : container) {
        vector<string> aux;
        if (local == container[0]) {
            aux = {to_string(local->getLoc()->getId()),to_string(local->getLoc()->getX()),
                                  to_string(local->getLoc()->getY()), tagToStr(local->getLoc()->getTag()),
                                  "-"};
        }
        else
            aux = {to_string(local->getLoc()->getId()),to_string(local->getLoc()->getX()),
                              to_string(local->getLoc()->getY()), tagToStr(local->getLoc()->getTag()),
                              "+"+(local->getDt().time - container[0]->getDt().time).str()};
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

bool isAllPairsAlgo(const string &toTest){
    if (!isNum(toTest)) return false;
    int n = stoi(toTest);
    return n == 0 || n == 1;
}

bool isWagonOption(const string &toTest) {
    if (toTest == ":q") return true;
    if (!isNum(toTest)) return false;
    int n = stoi(toTest);
    return n == 0 || n == 1;
}

string System::getGraphPath(){
    return this->graphPath;
}


vector<vector<string>> System::getSugestions() const {
    if(this->graphPath == "../data/EspinhoFull/"){
        return vector<vector<string>>({{"15892", "13504", "17126", "8498"},
                                       {"9882", "9243", "13357", "-"},
                                       {"305", "10031", "8001", "-"}});
    } else if(this->graphPath == "../data/EspinhoStrong/"){
        return vector<vector<string>>({{"6042", "2374", "4818", "6927"},
                                       {"4377", "4447", "230", "-"},
                                       {"1211", "75", "4148", "-"}});
    } else if(this->graphPath == "../data/PenafielFull/"){
        return vector<vector<string>>({{"5531", "9432", "9051", "2485"},
                                       {"2167", "5116", "10218", "-"},
                                       {"8885", "6104", "8313", "-"}});
    } else if(this->graphPath == "../data/PenafielStrong/"){
        return vector<vector<string>>({{"962", "3421", "1146", "3620"},
                                       {"3255", "1326", "3185", "-"},
                                       {"3469", "3623", "774", "-"}});
    } else if(this->graphPath == "../data/PortoFull/"){
        return vector<vector<string>>({{"3754", "4746", "49178", "6638"},
                                       {"18813", "5764", "38069", "-"},
                                       {"19020", "15107", "547", "-"}});
    } else if(this->graphPath == "../data/PortoStrong/"){
        return vector<vector<string>>({{"9447", "22347", "13991", "14979"},
                                       {"16993", "16760", "18897", "-"},
                                       {"16573", "60", "10413", "-"}});
    }
}

void System::printSuggestions() {
    Table<string> sugestions({"Police", "Prison", "Court", "HQ"}, this->getSugestions());
    cout << "Here's some POI Id's examples:\n" << sugestions;
}
