#include "System.h"
#include "Utilities/utils.h"
#include "Person/Person.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <utility>

using namespace std;

System::System(const string &fileName) {
    this->fileName = fileName;
    ifstream file;
    vector<string> aux = split(fileName, "/");
    aux.pop_back();
    string path = join(aux, '/');
    string peopleFile;
    file.open(fileName);
    file >> peopleFile;
    peopleFile = path + peopleFile;
    file.close();

    file.open(peopleFile);

    while (file.peek() != -1) {
        Person *p;
        file >> &p;
        this->people.push_back(p);
    }

    file.close();
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
        cout << "The search is empty :(" << endl;
        return;
    }
    auto read = toTable(container, this);
    cout << read;
    pause();
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

vector<Person *> System::getPeople() const {
    return this->people;
}

void System::deletePerson(const unsigned id) {
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
    pause();
}

void System::updatePersonContact(vector<Person *>::const_iterator person) {
    string contStr = getInput(isContact, "Enter the person new contact: ", "Invalid contact");
    if (contStr == ":q") {
        return;
    }
    unsigned cont = stoi(contStr);
    (*person)->setContact(cont);
    cout << "Person contact changed successfully!" << endl;
    pause();
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

