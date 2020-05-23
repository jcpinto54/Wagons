//
// Created by joao on 23/05/20.
//

#ifndef MEATWAGONS_DATETIME_H
#define MEATWAGONS_DATETIME_H


#include "Date.h"
#include "Time.h"

class DateTime {
private:
    Date date;
    Time time;
public:
    DateTime(const Date &date, const Time &time);

    void operator
    bool operator<(DateTime &rhs);
};


#endif //MEATWAGONS_DATETIME_H
