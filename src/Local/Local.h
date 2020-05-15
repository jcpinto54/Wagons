#pragma once

#include <iostream>

using namespace std;

class Local {
private:
    unsigned id;
    int x, y;
    static int minX, maxX, minY, maxY;
    //adicionar tags, uma boa solução é um enum

public:
    Local(unsigned int id, int x, int y);

    Local(unsigned int id);

    unsigned int getId() const;

    void setId(unsigned int id);

    static int getMinX();

    static int getMaxX();

    static int getMinY();

    static int getMaxY();

    unsigned int getX() const;

    void setX(unsigned int x);

    unsigned int getY() const;

    void setY(unsigned int y);

    ///@brief Reads an object person from the input stream in.
    ///@param in Stream from where the person will be read.
    ///@param person Pointer to pointer to a person object.
    ///@return Input stream.
    friend istream &operator>>(istream &in, Local **local);

    bool operator==(const Local &rhs) const;

    bool operator!=(const Local &rhs) const;

};


