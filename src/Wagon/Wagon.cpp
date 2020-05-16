//
// Created by joao on 16/05/20.
//

#include "Wagon.h"
#include "Utilities/utils.h"
#include "Utilities/InvalidInput.h"

using namespace Util;

istream &operator>>(istream &is, Wagon **wagon) {
    string aux;
    getline(is, aux);
    vector<string> auxVec = trim(split(aux, "|"));
    if (auxVec.size() != 1) throw InvalidInput("Wagons");
    *wagon = new Wagon(stoi(auxVec.at(0)));
    return is;
}

Wagon::Wagon(unsigned int id) : id(id) {}

Vertex<Local *> *Wagon::getPos() const {
    return pos;
}

unsigned int Wagon::getId() const {
    return id;
}

void Wagon::setPos(Vertex<Local *> *pos) {
    Wagon::pos = pos;
}
