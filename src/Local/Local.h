#pragma once

#include <iostream>
#include <map>

using namespace std;

enum Tag {
    DEFAULT, HQ, POLICE, PRISON, COURT
};

string tagToStr(Tag tag);
Tag strToTag(string tagStr);

class Local {
private:
    unsigned id;
    int x, y;
    Tag tag;
    static int minX, maxX, minY, maxY;

public:
    Local(unsigned int id, int x, int y);

    Local(unsigned int id);

    unsigned int getId() const;

    void setId(unsigned int id);

    static int getMinX();

    void setTag(string tag);

    static int getMaxX();

    static int getMinY();

    static int getMaxY();

    unsigned int getX() const;

    unsigned int getY() const;

    ///@brief Reads an object person from the input stream in.
    ///@param in Stream from where the person will be read.
    ///@param person Pointer to pointer to a person object.
    ///@return Input stream.
    friend istream &operator>>(istream &in, Local **local);

    bool operator==(const Local &rhs) const;

    bool operator!=(const Local &rhs) const;

    Tag getTag() const;

};


/// Invalid Tag Conversion Exception
class InvalidTagConversion : public std::exception {
private:
    /// @brief The msg to be displayed when this exception occurs
    std::string msg = "Invalid Tag Conversion!";
public:
    /// @brief Constructs an InvalidTagConversion exception
    /// @param msg The msg of the new InvalidTagConversion exception
    InvalidTagConversion(std::string msg) : msg(move(msg)) {};

    /// @brief Construct an InvalidTagConversion exception with an empty msg
    InvalidTagConversion() = default;

    /// Gets the message to be showed
    /// @return Returns the msg attribute of the object
    std::string getMsg() { return this->msg; }
};


