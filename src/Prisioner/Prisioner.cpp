//
// Created by joao on 23/05/20.
//

#include "Prisioner.h"

unsigned Prisioner::nextId = 0;

Prisioner::Prisioner(POI *start, POI *end, TransportType type) : start(start), end(end), type(type), id(nextId++) {}

POI *Prisioner::getStart() const {
    return start;
}

POI *Prisioner::getEnd() const {
    return end;
}

string Prisioner::getType() const {
    switch (this->type) {
        case PRISON_TRANSFER:
            return "Prison Transfer";
        case ATTEND_COURT:
            return "Attend Court";
        case POLICE_PRISON:
            return "Police to Prison";
        case COMMUNITY:
            return "Community Service";
    }
}

unsigned int Prisioner::getId() const {
    return id;
}

bool Prisioner::isReady() const {
    return ready;
}

void Prisioner::setReady(bool ready) {
    Prisioner::ready = ready;
}
