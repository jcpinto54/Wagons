#pragma once

#include <string>
#include <vector>
#include <sstream>

using namespace std;

namespace Util {

///@brief Pauses the program by requesting user input.
    void pause();

///@brief Removes all spaces to the left of the string.
///@param toTrim String to be left trimmed.
///@return String left trimmed.
    string ltrim(string toTrim);

///@brief Removes all spaces to the right of the string.
///@param toTrim String to be right trimmed.
///@return String right trimmed.
    string rtrim(string toTrim);

///@brief Removes all spaces to the left and to the right of the string.
///@param toTrim String to be trimmed.
///@return String trimmed.
    string trim(string toTrim);

///@brief Removes all spaces to the left and to the right of the elements of the vector.
///@param toTrim Vector to have its elements trimmed.
///@return Vector trimmed.
    vector<string> trim(vector<string> toTrim);


///@brief Creates a vector split by the string the user entered.
///@param toSplit String to be separated.
///@param splitPoint Set of char that decides how the string should be split.
///@return Vector of strings
    vector<string> split(const string &toSplit, const string &splitPoint = " ");

///@brief Recognizes if a string is a number.
///@param toConvert String to check if it's a number.
///@return Returns true if the string is convertible to number;
    bool isNum(const string &toConvert);


///@brief Joins the elements of a vector in a string.
///@param vecToJoin Vector to be joined.
///@param character Char to separate the strings joined.
///@return Returns a string joined by the param character.
    template<class T>
    string join(vector<T> vecToJoin, char character = ' ') {
        stringstream res;
        for (auto elem :vecToJoin) {
            res << elem << character;
        }
        return res.str();
    }

///@brief Converts a string to uppercase.
///@param s String to be uppercase.
///@return Uppercased string.
    string upper(const string &s);

///@brief Changes the decimal cases on a string that is a number.
///@param num Number to be converted
///@param decimalDigits How many digits we desire after the floating point.
///@return String with "decimalDigits" digits after the floating point.
    string strPrecision(string num, unsigned decimalDigits = 2);

///@brief Asks for user input and returns a string if the what the user inputed makes the first parameter true.
///@param __valid Function passed as argument that has to return a bool.
///@param message Message to be shown when the function asks for user input.
///@param fail Message to be shown when what the user inputed isn't valid.
///@return A string.
    string getInput(bool (__valid)(const string &), const string &message = "Choose a option: ",
                    const string &fail = "Invalid input.");

///@brief Tests if the input is a '1' or a '0'
///@param toTest String to be tested.
///@return A bool.
    bool isYorN(const string &toTest);

///@brief Tests if the input string is a valid contact number.
///@param toTest String to be tested.
///@return A bool.
    bool isContact(const string &toTest);

///@brief Tests if the input is name.
///@param toTest String to be tested.
///@return A bool.
    bool isName(const string &toTest);

///@brief Tests if the input string is empty.
///@param toTest String to be tested.
///@return A bool.
    bool notEmptyString(const string &toTest);


///@brief
///@param
///@return
    template<class T, template<class, class = allocator<T>> class Container>
    Container<T> filter(const Container<T> &container, bool (filterFunction)(const T)) {
        Container<T> res;
        for (auto it = container.begin(); it != container.end(); it++)
            if (filterFunction(*it))
                res.push_back(*it);
        return res;
    };

///@brief Tests if the the first template parameter is in the second template vector.
///@param first The element to check if it is in the second parameter.
///@param second The vector to be checked if it contains the first parameter.
///@return A pointer to the element in the second parameter that is equal to the first element.
    template<class T>
    T *firstInSecond(T *first, vector<T *> second) {
        for (size_t i = 0; i < second.size(); i++) {
            if (*first == *second.at(i)) return second.at(i);
        }
        return nullptr;
    }
}

