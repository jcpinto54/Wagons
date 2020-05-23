//
// Created by joao on 22/05/20.
//

#include "POI.h"

POI::POI(Local *loc, const Date &date, const Time &time) : loc(loc), dt(DateTime(date, time)) {}

Local *POI::getLoc() const {
    return loc;
}

bool POI::operator==(const POI &rhs) const {
    return loc == rhs.loc &&
           dt.date == rhs.dt.date;
}

DateTime POI::getDt() const{
    return (dt);
}
