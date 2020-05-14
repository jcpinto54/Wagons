#include "Date.h"

using namespace std;

// ----------------------------------------------------------------------------------------------------
//                             Creates a new date equal to the actual date
// ----------------------------------------------------------------------------------------------------
Date::Date() {
    time_t t = time(nullptr);
    this->year = localtime(&t)->tm_year + 1900;
    this->month = localtime(&t)->tm_mon + 1;
    this->day = localtime(&t)->tm_mday;
}

// ----------------------------------------------------------------------------------------------------
//                                Creates a new date in the actual month
// ----------------------------------------------------------------------------------------------------
Date::Date(unsigned char day) {
    time_t t = time(nullptr);
    unsigned short year = localtime(&t)->tm_year + 1900;
    unsigned char month = localtime(&t)->tm_mon + 1;
    if (!validDate(day, month, year))
        throw InvalidDate(day, this->month, this->year);
    this->year = year;
    this->month = month;
    this->day = day;
}

// ----------------------------------------------------------------------------------------------------
//                                Creates a new date in the actual year
// ----------------------------------------------------------------------------------------------------
Date::Date(unsigned char day, unsigned char month) {
    time_t t = time(nullptr);
    unsigned short  year = localtime(&t)->tm_year + 1900;
    if (!validDate(day, month, year))
        throw InvalidDate(day, this->month, this->year);
    this->year = year;
    this->month = month;
    this->day = day;
}

// ----------------------------------------------------------------------------------------------------
//                                          Creates a new date
// ----------------------------------------------------------------------------------------------------
Date::Date(unsigned char day, unsigned char month, unsigned short year) {
    if (!validDate(day, month, year))
        throw InvalidDate(day, this->month, this->year);
    this->year = year;
    this->month = month;
    this->day = day;
}

// ----------------------------------------------------------------------------------------------------
//                      Creates a new date from a string in the format DD-MM-YYYY
// ----------------------------------------------------------------------------------------------------
Date::Date(const string & s) {
    unsigned char day, month;
    unsigned short year;
    vector<string> splitVec;
    string temp;
    string separators = "-/|";
    splitVec = trim(split(s, separators));
    if(splitVec.size() != 3) throw InvalidDate(0, 0, 0);
    try {
        day = stoi(splitVec.at(0));
        month = stoi(splitVec.at(1));
        year = stoi(splitVec.at(2));
    } catch (...) {
        throw InvalidDate(day, month, year);
    }
    if (!validDate(day, month, year))
        throw InvalidDate(day, month, year);
    this->year = year;
    this->month = month;
    this->day = day;
}

// ----------------------------------------------------------------------------------------------------
//                                       Returns the date's day
// ----------------------------------------------------------------------------------------------------
unsigned char Date::getDay() const {
    return this->day;
}

// ----------------------------------------------------------------------------------------------------
//                                      Returns the date's month
// ----------------------------------------------------------------------------------------------------
unsigned char Date::getMonth() const {
    return this->month;
}

// ----------------------------------------------------------------------------------------------------
//                                       Returns the date's year
// ----------------------------------------------------------------------------------------------------
unsigned short Date::getYear() const {
    return this->year;
}

// ----------------------------------------------------------------------------------------------------
//                                    Sets the date's day if possible
// ----------------------------------------------------------------------------------------------------
void Date::setDay(unsigned char day) {
    if (!validDate(day, this->month, this->year))
        throw InvalidDate(day, this->month, this->year);
    this->day = day;
}

// ----------------------------------------------------------------------------------------------------
//                                   Sets the date's month if possible
// ----------------------------------------------------------------------------------------------------
void Date::setMonth(unsigned char month) {
    if (!validDate(this->day, month, this->year))
        throw InvalidDate(this->day, month, this->year);
    this->month = month;
}

// ----------------------------------------------------------------------------------------------------
//                                   Sets the date's year if possible
// ----------------------------------------------------------------------------------------------------
void Date::setYear(unsigned short year) {
    if (!validDate(this->day, this->month, year))
        throw InvalidDate(this->day, this->month, year);
    this->year = year;
}

// ----------------------------------------------------------------------------------------------------
//                                   Sums a number of days in a date
// ----------------------------------------------------------------------------------------------------
Date Date::operator+(unsigned n) const {
    unsigned res = abs(* this);
    return date(res + n);
}

// ----------------------------------------------------------------------------------------------------
//                               Subtracts a number of days from a date
// ----------------------------------------------------------------------------------------------------
Date Date::operator-(unsigned n) const {
    unsigned res = abs(* this);
    return date(res - n);
}

// ----------------------------------------------------------------------------------------------------
//                             Returns the number of days between two dates
// ----------------------------------------------------------------------------------------------------
int Date::operator-(const Date param) const {
    Date x(this->day, this->month, this->year);
    unsigned actual = abs(x);
    int res = actual - abs(param);
    return res;
}

// ----------------------------------------------------------------------------------------------------
//                                   Sums a number of days in a date
// ----------------------------------------------------------------------------------------------------
void Date::operator+=(unsigned n) {
    unsigned aux = abs(* this);
    Date res = date(aux + n);
    this->day = res.getDay();
    this->month = res.getMonth();
    this->year = res.getYear();
}

// ----------------------------------------------------------------------------------------------------
//                                Subtracts a number of days from a date
// ----------------------------------------------------------------------------------------------------
void Date::operator-=(unsigned n) {
    *this += -(int)n;
}

// ----------------------------------------------------------------------------------------------------
//                        Compares two dates and returns true if they are equal
// ----------------------------------------------------------------------------------------------------
bool Date::operator==(const Date param) const {
    bool res = true;
    res = res && (this->year == param.getYear());
    res = res && (this->month == param.getMonth());
    res = res && (this->day == param.getDay());
    return res;
}

// ----------------------------------------------------------------------------------------------------
//                Compares two dates and returns true if the first is before the second
// ----------------------------------------------------------------------------------------------------
bool Date::operator<(const Date param) const {
    return abs(* this) < abs(param);
}

// ----------------------------------------------------------------------------------------------------
//                  Compares two dates and returns true if the first is after the second
// ----------------------------------------------------------------------------------------------------
bool Date::operator>(const Date param) const {
    return abs(* this) > abs(param);
}

// ----------------------------------------------------------------------------------------------------
//            Compares two dates and returns true if the first is before or equal the second
// ----------------------------------------------------------------------------------------------------
bool Date::operator<=(const Date param) const {
    return abs(* this) <= abs(param);
}

// ----------------------------------------------------------------------------------------------------
//             Compares two dates and returns true if the first is after or equal the second
// ----------------------------------------------------------------------------------------------------
bool Date::operator>=(const Date param) const {
    return abs(* this) >= abs(param);
}

// ----------------------------------------------------------------------------------------------------
//                      Compares two dates and returns true if they are not equal
// ----------------------------------------------------------------------------------------------------
bool Date::operator!= (const Date param) const {
    return !(* this == param);
}

// ----------------------------------------------------------------------------------------------------
//                                  Outputs the date in a nice format
// ----------------------------------------------------------------------------------------------------
ostream& operator<<(ostream & out, const Date & date) {
    out << (short) date.day << "/" << (short) date.month << "/" << date.year;
    return out;
}

istream &operator>>(istream &in, Date &date) {
    string input;
    cin >> input;
    date = Date(input);
    return in;
}


Date::Date(const Date &date) {
    this->month = date.month;
    this->year = date.year;
    this->day = date.day;
}

// ----------------------------------------------------------------------------------------------------
//                                Returns true if a year is a leap year
// ----------------------------------------------------------------------------------------------------
bool bissextile(unsigned short year) {
    if (year % 4 == 0){
        if (year % 100 != 0 || year % 400 == 0) {
            return true;
        }
    }
    return false;
}

// ----------------------------------------------------------------------------------------------------
//                                   Returns true if a date is valid
// ----------------------------------------------------------------------------------------------------
bool validDate(unsigned char day, unsigned char month, unsigned short year) {
    if (day <= 0 || month <= 0 || year == 0){
        return false;
    }
    if (month > 12) return false;
    if (day == 29 && month == 2){
        return bissextile(year);
    }
    return day <= daysMonth(month);
}

// ----------------------------------------------------------------------------------------------------
//                                   Returns true if a date is valid
// ----------------------------------------------------------------------------------------------------
bool isDate(const string& date) {
    try{
        Date temp(date);
        return true;
    } catch (...) {
        return false;
    }
}


// ----------------------------------------------------------------------------------------------------
//                       Returns the days of a specific month in a non leap year
// ----------------------------------------------------------------------------------------------------
unsigned char daysMonth(unsigned char month) {
    if (month < 8){
        if (month % 2 == 1){
            return 31;
        }else if(month != 2){
            return 30;
        }else{
            return 28;
        }
    } else {
        if (month % 2 == 0){
            return 31;
        }else{
            return 30;
        }
    }
}

// ----------------------------------------------------------------------------------------------------
//                                 Returns the days of a specific year
// ----------------------------------------------------------------------------------------------------
unsigned short daysYear(unsigned short year) {
    if (bissextile(year)){
        return 366;
    }else{
        return 365;
    }
}

// ----------------------------------------------------------------------------------------------------
//                         Returns the number of days of a date since the epoch
// ----------------------------------------------------------------------------------------------------
unsigned abs(const Date date) {
    unsigned short year = date.getYear();
    unsigned char month = date.getMonth();
    unsigned char day = date.getDay();
    unsigned total = 0;
    for (unsigned i = EPOCH; i < year; i++){
        total += daysYear(i);
    }
    for (unsigned i = 1; i < month; ++i){
        total += daysMonth(i);
    }
    if (bissextile(year)){
        total++;
    }
    total += day;
    return total;
}

// ----------------------------------------------------------------------------------------------------
//                   Returns the date that is after a number of days since the epoch
// ----------------------------------------------------------------------------------------------------
Date date(unsigned days) {
    unsigned short year = EPOCH;
    unsigned char month = 1;
    while (days > daysYear(year)){
        days -= daysYear(year++);
    }
    if (days > 31){
        days -= daysMonth(month++);
        if (days > 28){
            if (bissextile(year)){
                days -= daysMonth(month++) + 1;
            }else{
                days -= daysMonth(month++);
            }
            while(days > daysMonth(month)){
                days -= daysMonth(month++);
            }
        }else{
            if (bissextile(year)){
                days--;
            }
        }
    }else{
        if (bissextile(year)){
            days--;
        }
    }
    Date result(days, month, year);
    return result;
}


bool futureDate(Date date, Time time) {
    Date atual;
    Time atualT;
    long days1 = abs(date), days2 = abs(atual);
    long days = days1 - days2;
    if (days > 0) return true;
    else if (days == 0) {
        if (atualT < time) return true;
    }
    return false;
}

bool future8hours(Date date, Time time) {
    Date atual;
    Time atualT;
    long days = abs(date) - abs(atual);
    if (days == 0 || days == 1) {
        if ((time - atualT).getHour() < 8) return true;
    }
    return false;
}

