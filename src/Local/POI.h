#pragma once
#include "Local.h"
#include "Date/DateTime.h"
#include "Wagon/Wagon.h"

class POI {
    Local *loc;
    DateTime dt;
public:
    POI(Local *loc, const Date &date, const Time &time);

    POI();

    Local *getLoc() const;

    DateTime getDt() const;

    void setLoc(Local *loc);

    void setDt(const DateTime &dt);

    bool operator==(const POI &rhs) const;

};

