#include "System.h"
#include <fstream>
#include "Utilities/InvalidInput.h"

#if defined(__linux__) || defined(__APPLE__)
#include <sys/wait.h>
#endif

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
    #if defined(__linux__) || defined(__APPLE__)
    wait(NULL);
    #endif
}

void System::viewGraph() {
    map.viewGraph(VIEW);
}

void System::viewPathBetween2Points(unsigned int idFrom, unsigned int idTo, int &algo) {
    string viewWithAPI = "N";
    viewWithAPI = Util::getInput(Util::isYorN, "Do you want to view the path in a gui mode?(Y/N) ", "Invalid Input");
    this->map.viewPath(idFrom, idTo, isY(viewWithAPI), algo);
}

void System::applyDijkstra(Local* const &origin){
    this->map.applyDijkstra(origin);
}

void System::applySingleSource(Local* const &destiny){
    this->map.applySingleSource(destiny);
}

void System::applyAStar(Local* const &origin, Local* const &destiny){
    this->map.applyAStar(origin, destiny);
}

void System::applyTarjan(){
    this->map.applyTarjan();
}

void System::applyAllPairs(int algo) {
    this->map.applyAllPairs(algo);
}

void System::applyDfs() {
    this->map.applyDfs();
}

void System::applyBfs(Local* const &source) {
    this->map.applyBfs(source);
}

void System::addPOINoTime() {
    string locIdStr;
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

        this->POIs.push_back(new POI(loc, Date() + 1 + this->POIs.size(), Time()));
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


    POI *poi = new POI(loc, Date() + 1 + this->POIs.size(), Time());
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
    int algo = this->readAlgorithm();
    return this->map.minimumWeightTour(&this->POIs, wagon, algo);
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
    aux = {"2", "A* - May have Problems"};
    content.push_back(aux);
    Table<string> data(header, content);
    cout << data;
    string option = Util::getInput(isAlgo, "Choose a Algorithm: ", "Invalid Choice");
    if (option == ":q")
        return -1;
    return stoi(option);
}

int System::readAllPairsAlgorithm()
{
    vector<string> header = {"Option","Algorithm"};
    vector<vector<string>> content;
    vector<string> aux = {"0", "Floyd-Warshall"};
    content.push_back(aux);
    aux = {"1", "A* - May have Problems"};
    content.push_back(aux);
    Table<string> data(header, content);
    cout << data;
    string option = Util::getInput(isAllPairsAlgo, "Choose a Algorithm: ", "Invalid Choice");
    if (option == ":q")
        return -1;
    return stoi(option);
}

Table<string> toTable(const vector<POI *> &container, const System *sys) {
    vector<string> header = {"Local ID", "X Coordinate", "Y Coordinate", "Tag"};
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
                              to_string(local->getLoc()->getY()), tagToStr(local->getLoc()->getTag())};
        content.push_back(aux);
    }
    Table<string> data(header, content);
    return data;
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

vector<vector<string>> System::getSugestions() const {
    if(this->graphPath == "../data/EspinhoFull/"){ //police | prison | court | hq
        return vector<vector<string>>({{"39", "5105", "4780", "6861"},
                                       {"123", "14382", "7731", "-"},
                                       {"7246", "5098", "14183", "-"}});
    } else if(this->graphPath == "../data/EspinhoStrong/"){
        return vector<vector<string>>({{"6480", "4598", "1", "6927"},
                                       {"3751", "6186", "4690", "-"},
                                       {"4217", "6374", "6854", "-"}});
    } else if(this->graphPath == "../data/PenafielFull/"){
        return vector<vector<string>>({{"7486", "5871", "10112", "159"},
                                       {"6", "3520", "247", "-"},
                                       {"1440", "2114", "1938", "-"}});
    } else if(this->graphPath == "../data/PenafielStrong/"){
        return vector<vector<string>>({{"3881", "2846", "2596", "3620"},
                                       {"74", "525", "2218", "-"},
                                       {"3570", "2527", "8", "-"}});
    } else if(this->graphPath == "../data/PortoFull/"){
        return vector<vector<string>>({{"6011", "9198", "6141", "19548"},
                                       {"20226", "28920", "525", "-"},
                                       {"29963", "5531", "12691", "-"},
                                       {"22909", "21626", "11597", "-"}});
    } else if(this->graphPath == "../data/PortoStrong/"){
        return vector<vector<string>>({{"4438", "14376", "8343", "14979"},
                                       {"13611", "22656", "1196", "-"},
                                       {"13832", "17069", "8787", "-"},
                                       {"15616", "18793", "2356", "-"}});
    } else if (this->graphPath == "../data/GridGraphs/4x4/") {
        return vector<vector<string>>({{"10", "3", "11", "0"},
                                       {"17", "23", "20", "-"}});
    } else if (this->graphPath == "../data/GridGraphs/4x4NotSC/") {
        return vector<vector<string>>({{"10", "3", "11", "0"},
                                       {"17", "23", "20", "-"}});
    } else if (this->graphPath == "../data/GridGraphs/8x8/") {
        return vector<vector<string>>({{"17", "3", "11", "40"},
                                       {"24", "71", "20", "-"},
                                       {"64", "33", "4", "-"}});
    } else if (this->graphPath == "../data/GridGraphs/16x16/") {
        return vector<vector<string>>({{"107", "13", "11", "155"},
                                       {"52", "203", "131", "-"},
                                       {"34", "233", "171", "-"}});
    }
    return vector<vector<string>>();
}

void System::printSuggestions() {
    Table<string> sugestions({"Police", "Prison", "Court", "HQ"}, this->getSugestions());
    cout << "Here's some POI Id's examples:\n" << sugestions;
}

void System::addPrisionerTransport() {
    vector<string> header = {"Option","Type of transport"};
    vector<vector<string>> content;
    vector<string> aux = {"0", "Prison transfer"};
    content.push_back(aux);
    aux = {"1", "Attend Court"};
    content.push_back(aux);
    aux = {"2", "Police Station to Prison"};
    content.push_back(aux);
    aux = {"3", "Community Service"};
    content.push_back(aux);
    Table<string> data(header, content);
    cout << data;
    string optionStr = Util::getInput(isTransportOption, "Choose a transport type: ", "Invalid Choice");

    if (optionStr == ":q")
        return;
    int option = stoi(optionStr);

    switch(option) {
        case 0:
            this->prisonTransfer();
            break;
        case 1:
            this->attendCourt();
            break;
        case 2:
            this->policeToPrison();
            break;
        case 3:
            this->communityService();
            break;
    }

}

void System::erasePrisioner() {
    string idStr = getInput(isNum, "Enter the ID of the transport you want to delete: ", "Invalid Input");
    if (idStr == ":q") return;
    for (auto it = this->prisioners.begin(); it != this->prisioners.end(); it++) {
        if ((*it)->getId() == stoi(idStr)) {
            this->prisioners.erase(it);
            cout << "The prisioner with ID " << idStr << " has been erased" << endl;
            return;
        }
    }
    cout << "The prisioner with ID " << idStr << " has not been found" << endl;
}

void System::readPrisionersTransports() {
    vector<string> header = {"ID", "Type", "Start POI ID", "Start Date", "Start Time", "End POI ID", "Time difference", "Day Difference"};
    vector<vector<string>> content;
    vector<string> aux;
    for (auto prisioner : this->prisioners) {
        aux.push_back(to_string(prisioner->getId()));
        aux.push_back(prisioner->getType());
        aux.push_back(to_string(prisioner->getStart()->getLoc()->getId()));
        aux.push_back(prisioner->getStart()->getDt().date.str());
        aux.push_back(prisioner->getStart()->getDt().time.str());
        aux.push_back(to_string(prisioner->getEnd()->getLoc()->getId()));
        aux.push_back("+" + (prisioner->getEnd()->getDt().time - prisioner->getStart()->getDt().time).str());
        aux.push_back(to_string(prisioner->getEnd()->getDt().date - prisioner->getStart()->getDt().date));
        content.push_back(aux);
        aux.clear();
    }
    Table<string> data(header, content);
    cout << data << endl;
}

void System::prisonTransfer() {
    string dateStr, timeStr;
    while (true) {
        dateStr = getInput(isDate, "What day do you want to start the transport(DD/MM/YYYY)? ", "Invalid Date");
        if (dateStr == ":q") {
            return;
        }
        if (Date() <= Date(dateStr)) break;
        cout << "This day was in the past!" << endl;
    }

    while (true) {
        timeStr = getInput(isTime,"What time do you want to start the transport(HH:MM:SS)? ", "Invalid Time");
        if (timeStr == ":q") {
            return;
        }
        if (Date() <= Date(dateStr) || Date() == Date(dateStr) && Time() < Time(timeStr)) break;
        cout << "Too late to plan for this time!" << endl;
    }
    DateTime dt = DateTime(Date(dateStr), Time(timeStr));

    string residenceId = getInput(isNum, "Enter the prisioner's old prison ID: ", "Invalid Input");
    if (residenceId == ":q") return;
    unsigned resId = stoi(residenceId);
    POI *res = new POI();
    try {
        res->setLoc(this->map.findLocal(resId));
    } catch (NonExistingVertex e) {
        cout << e.getMsg() << endl;
        return;
    }
    if (this->map.getTag(resId) != PRISON) {
        cout << "The prisioner's residence must be a prison!"  << endl;
        return;
    }
    res->setDt(dt);

    string objectiveId = getInput(isNum, "Enter the prisioner's new prison ID: ", "Invalid Input");
    if (objectiveId == ":q") return;
    unsigned objId = stoi(objectiveId);
    POI *objective = new POI();
    try {
        objective->setLoc(this->map.findLocal(objId));
    } catch (NonExistingVertex e) {
        cout << e.getMsg() << endl;
        return;
    }
    if (this->map.getTag(objId) != PRISON) {
        cout << "The prisioner's objective must be a prison!"  << endl;
        return;
    }
    if (!this->map.areStronglyConected(resId, objId)) {
        cout << "These 2 points don't belong to the same strongly connected component!" << endl;
        return;
    }

    timeStr = getInput(isTime, "Enter the time it can take to reach the other prison(HH:MM:SS): ", "Invalid Time");
    if (timeStr == ":q") return;
    Time t(timeStr);
    DateTime dtObjective = dt + t;
    objective->setDt(dtObjective);


        this->prisioners.push_back(new Prisioner(res, objective, PRISON_TRANSFER));
}

void System::attendCourt() {
    string dateStr, timeStr;
    while (true) {
        dateStr = getInput(isDate, "What day do you want to start the transport(DD/MM/YYYY)? ", "Invalid Date");
        if (dateStr == ":q") {
            return;
        }
        if (Date() <= Date(dateStr)) break;
        cout << "This day was in the past!" << endl;
    }

    while (true) {
        timeStr = getInput(isTime,"What time do you want to start the transport(HH:MM:SS)? ", "Invalid Time");
        if (timeStr == ":q") {
            return;
        }
        if (Date() <= Date(dateStr) || Date() == Date(dateStr) && Time() < Time(timeStr)) break;
        cout << "Too late to plan for this time!" << endl;
    }
    DateTime dt = DateTime(Date(dateStr), Time(timeStr));

    string residenceId = getInput(isNum, "Enter the prisioner's prison/police station ID: ", "Invalid Input");
    if (residenceId == ":q") return;
    unsigned resId = stoi(residenceId);
    POI *res = new POI();
    try {
        res->setLoc(this->map.findLocal(resId));
    } catch (NonExistingVertex e) {
        cout << e.getMsg() << endl;
        return;
    }
    if (this->map.getTag(resId) != PRISON && this->map.getTag(resId) != POLICE) {
        cout << "The prisioner's residence must be a prison or a police station!"  << endl;
        return;
    }
    res->setDt(dt);

    string objectiveId = getInput(isNum, "Enter the court's ID: ", "Invalid Input");
    if (objectiveId == ":q") return;
    unsigned objId = stoi(objectiveId);
    POI *objective = new POI();
    try {
        objective->setLoc(this->map.findLocal(objId));
    } catch (NonExistingVertex e) {
        cout << e.getMsg() << endl;
        return;
    }
    if (this->map.getTag(objId) != COURT) {
        cout << "The prisioner's objective must be a court!"  << endl;
        return;
    }
    if (!this->map.areStronglyConected(resId, objId)) {
        cout << "These 2 points don't belong to the same strongly connected component!" << endl;
        return;
    }

    timeStr = getInput(isTime, "Enter the time it can take to reach the court(HH:MM:SS): ", "Invalid Time");
    if (timeStr == ":q") return;
    Time t(timeStr);
    DateTime dtObjective = dt + t;
    objective->setDt(dtObjective);

    this->prisioners.push_back(new Prisioner(res, objective, ATTEND_COURT));
}

void System::policeToPrison() {
    string dateStr, timeStr;
    while (true) {
        dateStr = getInput(isDate, "What day do you want to start the transport(DD/MM/YYYY)? ", "Invalid Date");
        if (dateStr == ":q") {
            return;
        }
        if (Date() <= Date(dateStr)) break;
        cout << "This day was in the past!" << endl;
    }

    while (true) {
        timeStr = getInput(isTime,"What time do you want to start the transport(HH:MM:SS)? ", "Invalid Time");
        if (timeStr == ":q") {
            return;
        }
        if (Date() <= Date(dateStr) || Date() == Date(dateStr) && Time() < Time(timeStr)) break;
        cout << "Too late to plan for this time!" << endl;
    }
    DateTime dt = DateTime(Date(dateStr), Time(timeStr));

    string residenceId = getInput(isNum, "Enter the prisioner's police station ID: ", "Invalid Input");
    if (residenceId == ":q") return;
    unsigned resId = stoi(residenceId);
    POI *res = new POI();
    try {
        res->setLoc(this->map.findLocal(resId));
    } catch (NonExistingVertex e) {
        cout << e.getMsg() << endl;
        return;
    }
    if (this->map.getTag(resId) != POLICE) {
        cout << "The prisioner's residence must be a police station!"  << endl;
        return;
    }
    res->setDt(dt);

    string objectiveId = getInput(isNum, "Enter the prison's ID: ", "Invalid Input");
    if (objectiveId == ":q") return;
    unsigned objId = stoi(objectiveId);
    POI *objective = new POI();
    try {
        objective->setLoc(this->map.findLocal(objId));
    } catch (NonExistingVertex e) {
        cout << e.getMsg() << endl;
        return;
    }
    if (this->map.getTag(objId) != PRISON) {
        cout << "The prisioner's objective must be a prison!"  << endl;
        return;
    }
    if (!this->map.areStronglyConected(resId, objId)) {
        cout << "These 2 points don't belong to the same strongly connected component!" << endl;
        return;
    }

    timeStr = getInput(isTime, "Enter the time it can take to reach the prison(HH:MM:SS): ", "Invalid Time");
    if (timeStr == ":q") return;
    Time t(timeStr);
    DateTime dtObjective = dt + t;
    objective->setDt(dtObjective);

    this->prisioners.push_back(new Prisioner(res, objective, POLICE_PRISON));
}

void System::communityService() {
    string dateStr, timeStr;
    while (true) {
        dateStr = getInput(isDate, "What day do you want to start the transport(DD/MM/YYYY)? ", "Invalid Date");
        if (dateStr == ":q") {
            return;
        }
        if (Date() <= Date(dateStr)) break;
        cout << "This day was in the past!" << endl;
    }

    while (true) {
        timeStr = getInput(isTime,"What time do you want to start the transport(HH:MM:SS)? ", "Invalid Time");
        if (timeStr == ":q") {
            return;
        }
        if (Date() <= Date(dateStr) || Date() == Date(dateStr) && Time() < Time(timeStr)) break;
        cout << "Too late to plan for this time!" << endl;
    }
    DateTime dt = DateTime(Date(dateStr), Time(timeStr));

    string residenceId = getInput(isNum, "Enter the prisioner's prison/police station ID: ", "Invalid Input");
    if (residenceId == ":q") return;
    unsigned resId = stoi(residenceId);
    POI *res = new POI();
    try {
        res->setLoc(this->map.findLocal(resId));
    } catch (NonExistingVertex e) {
        cout << e.getMsg() << endl;
        return;
    }
    if (this->map.getTag(resId) != PRISON && this->map.getTag(resId) != POLICE) {
        cout << "The prisioner's residence must be a prison or a police station!"  << endl;
        return;
    }
    res->setDt(dt);

    string objectiveId = getInput(isNum, "Enter the place's ID where the prisioner will work: ", "Invalid Input");
    if (objectiveId == ":q") return;
    unsigned objId = stoi(objectiveId);
    POI *objective = new POI();
    try {
        objective->setLoc(this->map.findLocal(objId));
    } catch (NonExistingVertex e) {
        cout << e.getMsg() << endl;
        return;
    }
    if (!this->map.areStronglyConected(resId, objId)) {
        cout << "Point with ID you chose does not belong to the same strongly connected component!\nSuggestions:\nPorto - 17309\nPenafiel - 524\nEspinho - 45" << endl;
        return;
    }

    timeStr = getInput(isTime, "Enter the time it can take to reach the place(HH:MM:SS): ", "Invalid Time");
    if (timeStr == ":q") return;
    Time t(timeStr);
    DateTime dtObjective = dt + t;
    objective->setDt(dtObjective);

    this->prisioners.push_back(new Prisioner(res, objective, COMMUNITY));
}

const vector<Prisioner *> & System::getPrisioners() const{
    return prisioners;
}

// This function manages the transport of prisioners and if it divides the transports in more than 1 trip
vector<triplet<vector<Local *>, double, pair<Time, unsigned>>> System::solvePrisionersTour(vector<Prisioner *> &prisVecForErrors, vector<vector<POI *>> &toAPI) {
    // Choose a type of wagon
    Wagon * wagon = this->chooseWagon();
    // Choose what algorithm to use to compute paths
    int algo = this->readAlgorithm();
    vector<triplet<vector<Local *>, double, pair<Time, unsigned>>> tours;
    // Matrix that contains the path that each wagon will make. Size of the first dimension is the max number of wagons
    //      Note: We won't ever need more wagons than prisioners!
    vector<vector<POI *>> wagonsPois(this->prisioners.size());
    // Counter of "ready-to-go" prisioners. Being ready means a prisioner is associated with a wagon
    unsigned readyCounter = 0;
    for (int i = 0; i < this->prisioners.size(); i++) {
        // If all prisioners are "ready-to-go", stop
        if (readyCounter == this->prisioners.size()) break;

        for (auto prisioner : this->prisioners) {
            // If prisioner is ready, jump him.
            if (prisioner->isReady()) {
                continue;
            }
            // If this prisioner is the first one to go to this wagon, then add him without checking compatibility
            if (wagonsPois[i].empty()) {
                wagonsPois[i].push_back(prisioner->getStart());
                wagonsPois[i].push_back(prisioner->getEnd());
                prisioner->setReady(true);
                readyCounter++;
            } else { // wagon is not empty so check if new route is compatible
                // If Prisioner's Start POI is compatible, then temporarily add the first POI to the wagon tour
                if (this->map.isStartPoiCompatible(prisioner->getStart(), wagonsPois[i], wagon, algo)) {
                    wagonsPois[i].push_back(prisioner->getStart());
                    // If Prisioner's End POI is also compatible, then definetly add both POIs
                    if (this->map.isEndPoiCompatible(prisioner->getEnd(), wagonsPois[i], wagon, algo)) {
                        wagonsPois[i].push_back(prisioner->getEnd());
                        prisioner->setReady(true);
                        readyCounter++;
                    }
                    // If Prisioner's End POI is not compatible, then remove the Start POI that you temporarily added
                    else {
                        wagonsPois[i].pop_back();
                    }
                }
            }
        }
        // If wagon has no prisioners, continue to next wagon
        if (wagonsPois[i].empty()) break;
        // Calculate a wagon tour
        tours.push_back(this->map.minimumWeightTourWithTime(&wagonsPois[i], wagon, algo));
        // Vector of POIs that will be passed to the API so it can draw the tour
        toAPI.push_back(wagonsPois[i]);
        // If tour is unfeasable, this means: there isn't a path that doesn't reach late to a POI.
        if (tours.back().second == -1.0) {
            if (wagonsPois[i].size() != 3) {
                cout << "wagonPois[i].size() should be 3!" << endl;
                exit(1);
            }
            Prisioner *p = this->findPrisioner(wagonsPois[i][0], wagonsPois[i][1]);
            if (p == nullptr) {
                cout << "Prisioner doesn't exists!" << endl;
                exit(1);
            }
            // Store unfeasable tour's information
            prisVecForErrors.push_back(p);
        }
        else prisVecForErrors.push_back(nullptr);
    }

    // Reset prisioners being ready for later calculations
    for (auto p : this->prisioners) {
        p->setReady(false);
    }
    return tours;
}

Prisioner *System::findPrisioner(POI *start, POI *end) {
    Prisioner *p = nullptr;
    for (auto pris : this->prisioners) {
        if (*(pris->getStart()) == *start && *(pris->getEnd()) == *end) {
            p = pris;
            break;
        }
    }
    return p;
}

bool isAlgo(const string &toTest){
    if (!isNum(toTest)) return false;
    if (toTest == ":q") return false;
    int n = stoi(toTest);
    return n == 0 || n == 1 || n == 2;
}

bool isTransportOption(const string &toTest) {
    if (!isNum(toTest)) return false;
    if (toTest == ":q") return false;
    int n = stoi(toTest);
    return n == 0 || n == 1 || n == 2 || n == 3;

}
