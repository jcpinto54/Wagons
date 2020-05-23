//
// Created by joao on 22/05/20.
//

#include "POI.h"

POI::POI(Local *loc, const Date &date, const Time &time) : loc(loc), date(date), time(time) {}

Local *POI::getLoc() const {
    return loc;
}

const Date &POI::getDate() const {
    return date;
}

const Time &POI::getTime() const {
    return time;
}

bool POI::operator==(const POI &rhs) const {
    return loc == rhs.loc &&
           date == rhs.date;
}
