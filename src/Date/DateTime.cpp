//
// Created by joao on 23/05/20.
//

#include "DateTime.h"

DateTime::DateTime(const Date &date, const Time &time) : date(date), time(time) {}

bool DateTime::operator<(DateTime &rhs) {
    long days1 = abs(this->date), days2 = abs(rhs.date);
    long days = days1 - days2;
    if (days < 0) return true;
    else if (days == 0) {
        if (this->time < rhs.time) return true;
    }
    return false;
}

DateTime DateTime::operator+(Time &rhs) {
    pair<Time, bool> sum = this->time + rhs;
    Date newDate = this->date;
    if (sum.second) newDate += 1;
    return DateTime(newDate, sum.first);
}
