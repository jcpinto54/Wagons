#include "Person.h"
#include "Utilities/utils.h"
#include "Utilities/InvalidInput.h"


//Person

string Person::getName() const {
    return name;
}

Date Person::getBirthday() const{
    return birthday;
}

unsigned Person::getAge() const {
    Date now;
    unsigned res = now.getYear() - this->getBirthday().getYear() - 1;
    if(now.getMonth() == this->getBirthday().getMonth()){
        if(now.getDay() <= this->getBirthday().getDay()){
            res++;
        }
    }else if(now.getMonth() < this->getBirthday().getMonth()){
        res++;
    }
    return res;
}


unsigned Person::getContact() const{
    return contact;
}

void Person::setName(string n){
    name = move(n);
}

void Person::setContact(unsigned contact){
    this->contact = contact;
}

Person::Person(string name, Date birthday, unsigned contact, unsigned id) : name(move(name)), birthday(birthday), id(id) {
    this->contact=contact;
    if (id >= Person::nextId) Person::nextId = id + 1;
}

Person::Person(string name, Date birthday, unsigned contact) : name(move(name)), birthday(birthday), id(Person::nextId) {
    Person::nextId += 1;
    this->contact=contact;
}

Person::Person(const Person &person) : name(person.name), birthday(person.getBirthday()), contact(person.contact), id(person.id) {
    if (person.id >= Person::nextId) Person::nextId = id + 1;
}

bool Person::operator==(Person &person) {
    return this->id == person.id;
}

bool Person::operator!=(Person person) {
    return !(*this == person);
}

istream &operator>>(istream &in, Person ** person) {
   string aux;
   getline(in,aux);
   vector<string> auxvec = trim(split(aux,"|"));
   if (auxvec.size() != 4) throw InvalidInput("Input error in people.txt");
    *person = new Person(auxvec.at(0), Date(auxvec.at(1)), stoi(auxvec.at(2)), stoi(auxvec.at(3)));
    return in;
}

ostream &operator<<(ostream &out, Person *person) {
    out << person->name << " | " << person->birthday <<
        " | " << person->contact << " | " << person->id;
    return out;
}

bool operator<(const Person &lhs, const Person &rhs) {
    return lhs.id < rhs.id;
}

string Person::getType() {
    return "Person";
}

unsigned int Person::getId() const {
    return id;
}


bool compareName(const Person *person1, const Person *person2) {
    return person1->getName() < person2->getName();
}

bool compareBirthday(const Person *person1, const Person *person2) {
    return person1->getBirthday() < person2->getBirthday();
}

bool compareId(const Person *p1, const Person *p2) {
    return p1->getId() < p2->getId();
}

unsigned Person::nextId = 0;

Person::Person(const unsigned int id) : id(id), contact(0), birthday(Date()), name("") { }
