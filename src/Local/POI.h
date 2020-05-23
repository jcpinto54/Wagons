#pragma once
#include "Local.h"
#include "Date/DateTime.h"

class POI {
    Local *loc;
    DateTime dt;
public:
    POI(Local *loc, const Date &date, const Time &time);

    Local *getLoc() const;

    DateTime getDt() const;

    bool operator==(const POI &rhs) const;
};


