//
// Created by joao on 16/05/20.
//

#include "Wagon.h"
#include "Utilities/utils.h"
#include "Utilities/InvalidInput.h"

using namespace Util;

Wagon::Wagon(double avgSpeed) : avgSpeed(avgSpeed) {
}


double Wagon::getAvgSpeed() const {
    return avgSpeed;
}

pair<Time, unsigned> Wagon::distToTime(double dist) {
    unsigned s = dist/avgSpeed;
    unsigned m = s/60;
    unsigned h = s/3600;
    unsigned days = h / 24;
    m %= 60;
    h %= 24;
    s %= 60;
    return pair<Time, unsigned>(Time(h, m, s), days);
}
