#pragma once
#include "Local.h"
#include "Date/Date.h"
#include "Date/Time.h"

class POI {
    Local *loc;
    Date date;
    Time time;
public:
    POI(Local *loc, const Date &date, const Time &time);

    Local *getLoc() const;

    const Date &getDate() const;

    const Time &getTime() const;

    bool operator==(const POI &rhs) const;
};


