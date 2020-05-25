#include "utils.h"
#include <iostream>

namespace Util {

    void pause() {
        string s;
        cout << "Press enter to continue...";
        getline(cin, s);
    }

    string ltrim(string toTrim) {
        size_t found = toTrim.find_first_not_of(' ');
        if (found != string::npos) {
            toTrim = toTrim.substr(found);
        }
        return toTrim;
    }

    string rtrim(string toTrim) {
        size_t found = toTrim.find_last_not_of(' ');
        if (found != string::npos) {
            toTrim = toTrim.substr(0, found + 1);
        }
        return toTrim;
    }

    string trim(string toTrim) {
        return rtrim(ltrim(move(toTrim)));
    }

    vector<string> trim(vector<string> toTrim) {
        for (auto &i : toTrim) {
            i = rtrim(ltrim(i));
        }
        return toTrim;
    }

    void trim(vector<string> *toTrim) {
        for (auto &i : *toTrim) {
            i = rtrim(ltrim(i));
        }
    }

    vector<string> split(const string &toSplit, const string &splitPoint) {
        vector<string> splitVec;
        string temp;
        for (char i : toSplit) {
            if (splitPoint.find(i) != string::npos) {
                if (temp.empty())continue;
                splitVec.push_back(trim(temp));
                temp = "";
            } else {
                temp += i;
            }
        }
        if (!temp.empty())splitVec.push_back(trim(temp));
        return splitVec;
    }

    bool isNum(const string &toConvert) {
        float f;
        stringstream st;
        st.str(toConvert);
        st >> f;
        return st.eof() && !st.fail();
    }

    string upper(const string &s) {
        string ns;
        for (char i : s)
            if (i >= 'a' && i <= 'z')
                ns.push_back(toupper(i));
            else
                ns.push_back(i);
        return ns;
    }

    string strPrecision(string num, unsigned decimalDigits) {
        stringstream res;
        bool afterPoint = false;
        string newNum;
        for (auto car : num) {
            newNum.push_back(car);
            if (afterPoint) decimalDigits--;
            if (car == '.') afterPoint = true;
            if (decimalDigits == 0) return newNum;
        }
        return num;
    }

    string getInput(bool (__valid)(const string &), const string &message, const string &fail) {
        string input;
        cout << message;
        getline(cin, input);
        if (input == ":q") return input;
        while (!__valid(input)) {
            cout << fail << endl << message;
            getline(cin, input);
            if (input == ":q") return input;
        };
        return input;
    }

    bool isYorN(const string &toTest) {
        return (toTest == "1" || toTest == "0" || toTest == "Y" || toTest == "N" || toTest == "y" || toTest == "n");
    }

    bool isY(const string &toTest) {
        if (toTest == "1" || toTest == "Y" || toTest == "y") return true;
        return !isN(toTest);
    }

    bool isN(const string &toTest) {
        return (toTest == "0" || toTest == "N" || toTest == "n");
    }

    bool notEmptyString(const string &toTest) {
        return toTest.size() > 0;
    }

    bool isName(const string &toTest) {
        for (char x : toTest) {
            x = toupper(x);
            if (!((x >= 'A' && x <= 'Z') || x == ' ' || x == '-'))
                return false;
        }
        return split(toTest, " ").size() >= 2;
    }

    bool isContact(const string &toTest) {
        return (isNum(toTest) && toTest.size() == 9);
    }
}