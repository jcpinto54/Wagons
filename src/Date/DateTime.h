//
// Created by joao on 23/05/20.
//

#ifndef Wagons_DATETIME_H
#define Wagons_DATETIME_H


#include "Date.h"
#include "Time.h"

class DateTime {
public:
    DateTime(const Date &date, const Time &time);

    DateTime operator+(Time &rhs);
    bool operator<(DateTime &rhs);
    Time time;
    Date date;
};


#endif //Wagons_DATETIME_H
