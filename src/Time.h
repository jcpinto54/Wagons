#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include "utils.h"

using namespace std;

/// @brief The Time Class
class Time {
protected:
    /// @brief The hour of the Time
    char hour;

    /// @brief The minute of the Time
    char minute;
public:
    /// @brief Constructs a new Time equal to the actual time
    Time();

    /// @brief Constructs a new Time object
    /// @param h The hour of the new Time
    /// @param m The minutes of the new Time
    Time(char h, char m);

    /// @brief Constructs a new Time from a string
    /// @param str String in the format HH:MM
    explicit Time(string &str);

    /// @return Returns the hour of the Time object
    char getHour() const;

    /// @return Returns the minute of the Time object
    char getMinute() const;

    /// @brief Subtracts two Times
    /// @param left The first Time
    /// @param right The time to be subtracted from the first
    /// @return Returns a Time object with the difference between two Times
    friend Time operator-(Time left, Time right);

    /// @brief Compares two Times
    /// @param left The first Time object to be compared
    /// @param right The second Time object to be compared
    /// @return Returns true if the first Time is smaller than the second one
    friend bool operator<(Time left, Time right);

    /// @brief Sends a Time information to a ostream object in the HH:MM format
    /// @param out The ostream object that will receive the information from the Time
    /// @param t The Time whose information is gonna be send
    /// @return Returns the ostream object itself
    friend ostream &operator<<(ostream &out, const Time &t);

    /// @brief Changes a Time object with the information that is in a istream object
    /// @param in The istream object that will send the information to the Time
    /// @param t The Time that will receive information from the istream
    /// @return Returns the istream object itself
    friend istream &operator>>(istream &in, Time &t);

    /// @brief Verifies if a Time is in the future. (Compares time only!!!)
    /// @param time The Time that will be verified
    /// @return Returns true if the Time is after the actual Time
    friend bool futureTimeInSameDay(Time time);
};


/// Invalid Time Exception
class InvalidTime : public exception {
    /// @brief The hours of the supposed Time
    char hour;

    /// @brief The minutes of the supposed Time
    char minute;
public:
    /// @brief Constructs a new Invalid Time object
    /// @param h The hours of the new Invalid Time
    /// @param m The minutes of the new Invalid Time
    InvalidTime(char h, char m) : hour(h), minute(m) {}

    /// @brief Constructs a new Invalid Time from a string
    /// @param str A string in the format HH:MM
    explicit InvalidTime(const string &str) {
        vector<string> aux;
        aux = trim(split(str, ":"));
        if (aux.size() != 2) {
            this->minute = -1;
            this->hour = -1;
        }
        if (!isNum(aux.at(0)) || !isNum(aux.at(1))) {
            this->minute = -1;
            this->hour = -1;
        }
        hour = (char) stoi(aux.at(0));
        minute = (char) stoi(aux.at(1));
    }

    /// @brief Sends a Invalid Time information to a ostream object
    /// @param out The ostream object that will receive the Invali Time information
    /// @param t The Invalid Time whose information will be send
    /// @return Returns the ostream object itself
    friend ostream &operator<<(ostream &out, InvalidTime &t) {
        out << "The time " << t.hour << ":" << t.minute << " is invalid!";
        return out;
    }
};

/// @param str The string to be verified
/// @return Returns true if the string is in the format HH:MM
bool isTime(const string &str);

bool futureTimeInSameDay(Time time);
