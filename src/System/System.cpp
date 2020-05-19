#include "System.h"
#include "Utilities/utils.h"
#include "Person/Person.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <utility>
#include "Utilities/InvalidInput.h"
#include <unordered_map>
#include <sys/wait.h>


using namespace std;

System::System(const string &fileName) {
    this->fileName = fileName;

    ifstream file;
    vector<string> aux = split(fileName, "/");
    aux.pop_back();
    string path = join(aux, '/');
    string peopleFile, wagonsFile, graphPath;

    file.open(fileName);
    getline(file, peopleFile);
    getline(file, wagonsFile);
    getline(file, graphPath);
    file.close();

    if (graphPath.find("GridGraphs") != string::npos)
        this->map.setDirected(false);

    peopleFile = path + peopleFile;
    wagonsFile = path + wagonsFile;
    graphPath = path + graphPath;

    file.open(peopleFile);

    while (file.peek() != -1) {
        Person *p;
        file >> &p;
        this->people.push_back(p);
    }

    file.close();

    file.open(wagonsFile);

    while (file.peek() != -1) {
        Wagon *w;
        file >> &w;
        this->wagons.push_back(w);
    }

    file.close();

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

    this->map.init(tempGraph);
    this->map.setNumEdges(edgeCounter);
}

void System::readPerson() const {
    string idStr = getInput(isNum, "Type the id of the Client: ", "Invalid id");
    if (idStr == ":q") return;
    Person *personPtr = *findPerson(stoi(idStr));
    if (personPtr == *this->people.end()) {
        vector<Person *> aux = {};
        readPeople(aux);
    } else
        readPeople({personPtr});
}

void System::readPeople(const vector<Person *> &container) const {
    if (container.empty()) {
        cout << "The container is empty" << endl;
        return;
    }
    auto read = toTable(container, this);
    cout << read;
    Util::pause();
}


vector<Person *>::const_iterator System::findPerson(unsigned id) const {
    Person tempP = Person(id);
    for (auto person = people.begin(); person != people.end(); ++person) {
        if (tempP == **person)
            return person;
    }
    return people.end();
}

System::~System() {
    sort(this->people.begin(), this->people.end());

    fstream file;
    vector<string> aux = split(this->fileName, "/");
    aux.pop_back();
    string path = join(aux, '/');
    string peopleFile;

    file.open(this->fileName);
    //getline(file,this->pass);
    file >> peopleFile;
    peopleFile = path + peopleFile;
    file.close();

    file.open(peopleFile, ofstream::out | ofstream::trunc);
    auto itp = this->people.begin(), itpl = this->people.end();
    bool firstTime = true;
    for (; itp != itpl; itp++) {
        if (firstTime) {
            file << (*itp);
            firstTime = false;
            continue;
        }
        file << endl << *itp;
    }
    file.close();

    wait(NULL);

}

void System::createPerson() {
    string name = getInput(isName, "Input the person's name: ", "Invalid name");
    if (name == ":q")
        return;
    string bDay = getInput(isDate, "Input the person's birthday: ", "Invalid Date");
    if (bDay == ":q")
        return;
    Date birthdate = Date(bDay);
    string contactStr = getInput(isContact, "Input the person's contact: ", "Invalid Contact");
    if (contactStr == ":q")
        return;
    unsigned contact = stoi(contactStr);
    Person *toAdd = new Person(name, birthdate, contact);
    this->createPerson(toAdd);
}

void System::createPerson(Person *person) {
    if (findPerson(person->getId()) == people.end()) {
        this->people.push_back(person);
        return;
    }
    throw ExistingPerson(*person);
}

vector<Person *> &System::getPeople() {
    return this->people;
}

void System::deletePerson(unsigned id) {
    auto toRemoveP = findPerson(id);
    if (toRemoveP == people.end()) {
        cout << "Person not found!\n";
        return;
    }
    this->people.erase(toRemoveP);
    cout << "Person deleted!\n";
}

void System::deletePerson() {
    string idStr = getInput(isNum, "Type the id of the Client: ", "Invalid Number");
    if (idStr == ":q") return;
    deletePerson(stoi(idStr));
}

void System::updatePersonName(vector<Person *>::const_iterator person) {
    cout << "Old Name: " << (*person)->getName() << endl;
    string name = getInput(isName, "Introduce the person name: ", "Invalid Name");
    if (name == ":q") {
        return;
    }
    (*person)->setName(name);
    cout << "Person name changed to : " << name << " successfully!" << endl;
    Util::pause();
}

void System::updatePersonContact(vector<Person *>::const_iterator person) {
    string contStr = getInput(isContact, "Enter the person new contact: ", "Invalid contact");
    if (contStr == ":q") {
        return;
    }
    unsigned cont = stoi(contStr);
    (*person)->setContact(cont);
    cout << "Person contact changed successfully!" << endl;
    Util::pause();
}

void System::viewGraph() {
    map.viewGraph();
}

void System::viewPathBetween2Points(unsigned int idFrom, unsigned int idTo) {
    string viewWithAPI = "N";
    if (map.numVertex() < 300)
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


Table<string> toTable(const vector<Person *> &container, const System *sys) {
    vector<string> header = {"Name", "Birthday", "Contact", "ID"};
    vector<vector<string>> content;
    for (auto person : container) {
        stringstream birthday;
        birthday << person->getBirthday();
        vector<string> aux = {person->getName(), birthday.str(),
                              to_string(person->getContact()), to_string(person->getId())};
        content.push_back(aux);
    }
    Table<string> data(header, content);
    return data;
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

