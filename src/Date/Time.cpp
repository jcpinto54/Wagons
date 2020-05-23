#include "Time.h"


using namespace std;

Time::Time() {
    unsigned long t = time(nullptr);
    t %= (24 * 3600);
    hour = t / 3600;
    t %= 3600;
    minute = t / 60;
}

Time::Time(char h, char m) {
    if (h < 0 || h >= 24 || m < 0 || m >= 60) throw InvalidTime(h, m);
    this->hour = h;
    this->minute = m;
    this->seconds = 0;
}

Time::Time(char h, char m, char s) {
    if (h < 0 || h >= 24 || m < 0 || m >= 60 || s < 0 || s >= 60) throw InvalidTime(h, m);
    this->hour = h;
    this->minute = m;
    this->seconds = s;
}

Time::Time(string &str) {
    if (!isTime(str)) throw InvalidTime(str);
    vector<string> aux;
    aux = trim(split(str, ":"));
    hour = stoi(aux.at(0));
    minute = stoi(aux.at(1));
    seconds = stoi(aux.at(2));
}

char Time::getHour() const {
    return this->hour;
}

char Time::getMinute() const {
    return this->minute;
}

Time operator-(Time left, Time right) {
    signed char resH, resM, resS;
    resM = left.minute - right.minute;
    if (resM < 0) {
        left.hour--;
        resM = 60 + resM;
    }
    resH = left.hour - right.hour;
    if (resH < 0) {
        resH = 24 + resH;
    }
    resS = left.seconds - right.seconds;
    if (resS < 0) {
        left.hour--;
        resS = 60 + resM;
    }
    return Time((char) resH, (char) resM, (char) resS);
}

istream &operator>>(istream &in, Time &t) {
    string timeStr;
    in >> timeStr;
    if (!isTime(timeStr)) throw InvalidTime(timeStr);
    t = Time(timeStr);
    return in;
}

ostream &operator<<(ostream &out, const Time &t) {
    string h = to_string(t.hour), m = to_string(t.minute), s = to_string(t.seconds);
    if (t.hour < 10) h = "0" + h;
    if (t.minute < 10) m = "0" + m;
    if (t.seconds < 10) s = "0" + s;
    out << h << ":" << m << ":" << s;
    return out;
}

bool isTime(const string &str) {
    if (str == ":q") return true;
    vector<string> aux;
    aux = trim(split(str, ":"));
    if (aux.size() != 3) return false;
    if (!isNum(aux.at(0)) || !isNum(aux.at(1)) || !isNum(aux.at(2))) return false;
    char h = stoi(aux.at(0)), m = stoi(aux.at(1)), s = stoi(aux.at(2));
    return (h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60);
}

bool futureTimeInSameDay(Time time) {
    Time atual;
    if (atual.hour > time.hour) return true;
    else if (atual.hour == time.hour) {
        if (atual.minute >= time.minute) return true;
    }
    return false;
}

bool operator<(Time left, Time right) {
    if (left.hour < right.hour) return true;
    else if (left.hour == right.hour) {
        if (left.minute < right.minute) return true;
        else if (left.minute == right.minute) {
            if (left.seconds < right.seconds) return true;
        }
    }
    return false;
}

string Time::str() const {
    stringstream s;
    s << (*this);
    return s.str();
}

pair<Time, bool> Time::operator+(Time &rhs) {
    bool anthDay = false;
    unsigned s = this->seconds, m = this->minute, h = this->hour;
    s += rhs.seconds;
    if (s>=60) {
        m++;
        s%=60;
    }
    m += rhs.minute;
    if (m>=60) {
        h++;
        m%=60;
    }
    h += + rhs.hour;
    if (h>=24) {
        anthDay = true;
        h%= 24;
    }
    return pair<Time,bool>(Time(h, m, s), anthDay);
}
