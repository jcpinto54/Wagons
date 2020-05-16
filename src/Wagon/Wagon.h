#pragma once

#include <ostream>
#include "Local/Local.h"
#include "GraphTemplate/Graph.h"

class Wagon {
    Vertex<Local *> *pos;
    const unsigned id;
public:
    Wagon(unsigned int id);

    Vertex<Local *> *getPos() const;

    void setPos(Vertex<Local *> *pos);

    friend istream &operator>>(istream &is, Wagon **wagon);

    unsigned int getId() const;
};
