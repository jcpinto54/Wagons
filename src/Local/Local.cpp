//
// Created by joao on 15/05/20.
//

#include "Local.h"
#include "Utilities/utils.h"
#include "Utilities/InvalidInput.h"
#include <math.h>

using namespace Util;

Local::Local(unsigned id, int x, int y) : id(id), x(x), y(y) {
    this->tag = Tag::DEFAULT;
    if (x < Local::minX) {
        Local::minX = x;
    }
    if (x > Local::maxX) {
        Local::maxX = x;
    }

    if (y < Local::minY) {
        Local::minY = y;
    }
    if (y > Local::maxY) {
        Local::maxY = y;
    }

}

unsigned int Local::getId() const {
    return id;
}

void Local::setId(unsigned int id) {
    Local::id = id;
}

unsigned int Local::getX() const {
    return x;
}

unsigned int Local::getY() const {
    return y;
}

istream &operator>>(istream &in, Local ** local) {
    string aux;
    getline(in,aux);
    aux[0] = ' ';
    aux.pop_back();
    vector<string> auxvec = trim(split(aux,","));
    if (auxvec.size() != 3) throw InvalidInput("Input error reading a Local");
    int x = stoi(auxvec.at(1)), y = stoi(auxvec.at(2));
    *local = new Local(stoi(auxvec.at(0)), x, y);
    return in;
}

bool Local::operator==(const Local &rhs) const {
    return id == rhs.id;
}

bool Local::operator!=(const Local &rhs) const {
    return !(rhs == *this);
}

Local::Local(unsigned int id) : id(id) {}

int Local::minX = INT32_MAX;
int Local::minY = INT32_MAX;
int Local::maxX = INT32_MIN;
int Local::maxY = INT32_MIN;

int Local::getMinX() {
    return minX;
}

int Local::getMaxX() {
    return maxX;
}

int Local::getMinY() {
    return minY;
}

int Local::getMaxY() {
    return maxY;
}

void Local::setTag(string tag) {
    this->tag = strToTag(tag);
}

Tag Local::getTag() const {
    return tag;
}

double Local::dist(Local *l2) {
    int distY = this->y - l2->y;
    int distX = this->x - l2->x;
    double distXPow = pow((double)distX, 2);
    double distYPow = pow((double)distY, 2);
    return sqrt(distXPow + distYPow);
}

string tagToStr(Tag tag) {
    switch(tag) {
        case PRISON: return "prison";
        case POLICE: return "police";
        case COURT: return "court";
        case HQ: return "hq";
        case DEFAULT: return "default";
    }
    throw InvalidTagConversion();
}

Tag strToTag(string tagStr) {
    if (tagStr == "prison") return PRISON;
    else if (tagStr == "police") return POLICE;
    else if (tagStr == "court") return COURT;
    else if (tagStr == "hq") return HQ;
    throw InvalidTagConversion(tagStr);
}



