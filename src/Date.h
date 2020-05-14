#pragma once

#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include "Time.h"

#define EPOCH 1900

using namespace std;

/// @brief The Date Class
class Date {
private:
    /// @brief The day of the Date
    unsigned char day;
    /// @brief The month of the Date
    unsigned char month;
    /// @brief The year of the Date
    unsigned short year;
public:
    /// @brief Constructs a Date equal to the actual date.
    Date();

    /// @brief Constructs a Date equal to other date.
    Date(const Date &date);

    /// @brief Constructs a Date in the actual month.
    /// @param day The day of the Date.
    explicit Date(unsigned char day);

    /// @brief Constructs a Date in the actual year.
    /// @param day The day of the Date.
    /// @param month The month of the Date.
    Date(unsigned char day, unsigned char month);

    /// @brief Constructs a Date with the parameters.
    /// @param day The day of the Date.
    /// @param month The month of the Date.
    /// @param year The year of the Date.
    Date(unsigned char day, unsigned char month, unsigned short year);

    /// @brief Constructs a Date from a string.
    /// @param s A string in the format "DD-MM-YYYY".
    explicit Date(const string &s);

    /// @brief Gets the day of a Date.
    /// @return The day attribute of the Date.
    unsigned char getDay() const;

    /// @brief Gets the month of a Date.
    /// @return The month attribute of the Date.
    unsigned char getMonth() const;

    /// @brief Gets the year of a Date.
    /// @return The year attribute of the Date.
    unsigned short getYear() const;

    /// @brief Sets the day of a Date.
    /// @param day The day to be attributed to the Date.
    void setDay(unsigned char day);

    /// @brief Sets the month of a Date.
    /// @param month The month to be attributed to the Date.
    void setMonth(unsigned char month);

    /// @brief Sets the year of a Date.
    /// @param year The year to be attributed to the Date.
    void setYear(unsigned short year);

    /// @brief Returns a Date that is n days after another.
    /// @param n The number of days after the Date.
    /// @return A Date that is n days after the object.
    Date operator+(unsigned n) const;

    /// @brief Returns a Date that is n days before another.
    /// @param n The number of days before the Date.
    /// @return A Date that is n days before the object.
    Date operator-(unsigned n) const;

    /// @brief Returns the number of days between two Date objects.
    /// @param argument The date that is compared with the object.
    /// @return The number of days between the object and the argument.
    int operator-(const Date argument) const;

    /// @brief Adds a number of days to a Date object.
    /// @param n The number of days to be added to the date.
    void operator+=(unsigned n);

    /// @brief Subtracts a number of days to a Date object.
    /// @param n The number of days to be subtracted from the date.
    void operator-=(unsigned n);

    /// @param param A Date object.
    /// @return Returns true if the param attributes are equal to the object ones, false otherwise.
    bool operator==(const Date param) const;

    /// @param param A Date object.
    /// @return Returns true if the param is before the object, false otherwise.
    bool operator<(const Date param) const;

    /// @param param A Date object.
    /// @return Returns true if the param is after the object, false otherwise.
    bool operator>(const Date param) const;

    /// @param param A Date object.
    /// @return Returns true if the param is before or the same day as the object, false otherwise.
    bool operator<=(const Date param) const;

    /// @param param A Date object.
    /// @return Returns true if the param is after or the same day as the object, false otherwise.
    bool operator>=(const Date param) const;

    /// @param param A Date object.
    /// @return Returns true if the dates are different, false otherwise.
    bool operator!=(const Date param) const;

    /// @brief Outputs a date in the DD-MM-YYYY format
    /// @param out The ostream that will receive the information of the Date
    /// @param date The Date that will have its information send
    /// @return Returns the ostream object itself
    friend ostream &operator<<(ostream &out, const Date &date);

    /// @brief Inputs a date in the DD-MM-YYYY format from a istream object
    /// @param in The istream that has the information of the Date
    /// @param date The Date that will be changed based on the istream
    /// @return Returns the istream object itself
    friend istream &operator>>(istream &in, Date &date);
};

/// @param year The year to test.
/// @return Returns true if the year was a leap year in the Gregorian calendar.
bool bissextile(unsigned short year);

/// @param day The day of the date.
/// @param month The month of the date.
/// @param year The year of the date.
/// @return Returns true if tha date is possible in the Gregorian calendar.
bool validDate(unsigned char day, unsigned char month, unsigned short year);

/// @param date The date in the DD-MM-YYYY form.
/// @return Returns true if tha date is possible in the Gregorian calendar.
bool isDate(const string &date);

/// @param month The month to verify.
/// @return Returns the number of days of that month in a non-leap year.
unsigned char daysMonth(unsigned char month);

/// @param year The year to verify.
/// @return Returns the number of days of that year (365 or 366).
unsigned short daysYear(unsigned short year);

/// @param date The date to calculate.
/// @return Returns the number of days of a date since the epoch.
unsigned abs(const Date date);

/// @param days The number of days since the epoch.
/// @return Returns the date that is after a number of days since the epoch
Date date(unsigned days);

/// @param date The date to be tested
/// @param time The time to be tested
/// @return True if the datetime is in the future
bool futureDate(Date date, Time time);

/// @param date The date to be tested
/// @param time The time to be tested
/// @return True if the datetime will happen in the next 8 hours
bool future8hours(Date date, Time time);

/// @brief The Invalid Date exception
class InvalidDate : public exception {
private:
    /// @brief The day of the Invalid Date
    unsigned char day;

    /// @brief The month of the Invalid Date
    unsigned char month;

    /// @brief The year of the Invalid Date
    unsigned short year;
public:
    /// @brief Constructs a new Invalid Date Exception
    /// @param d The day of the new InvalidDate object
    /// @param m The month of the new Invalid Date object
    /// @param y The year of the new Invalid Date object
    InvalidDate(unsigned char d, unsigned char m, unsigned short y) :
            day(d), month(m), year(y) {};

    /// @brief Constructs a new Invalid Date from a string
    /// @param s String with a date in the format DD-MM-YYYY
    InvalidDate(string s) {
        unsigned char day = stoi(s.substr(0, 2));
        unsigned char month = stoi(s.substr(3, 2));
        unsigned short year = stoi(s.substr(6, 4));
        this->year = year;
        this->month = month;
        this->day = day;
    }
};
