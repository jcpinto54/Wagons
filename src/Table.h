#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

const unsigned char PADDING = 3;
using namespace std;

/// @brief Adds spaces after and before a string until it reaches a length
/// @tparam Printable Any class with the operator<< defined
/// @param s The object that is gonna be made into a normalized string
/// @param n The length of the resultant string
/// @return Returns a string with the length n that has the information of s
template<class Printable>
string normalize(Printable s, unsigned n);

/// @brief The Table Class
/// @tparam Printable Any class that has the operator<< defined
template<class Printable>
class Table {
private:
    /// @brief The header of the table (first row)
    vector<Printable> header;

    /// @brief The content of the table (all rows except the first)
    vector<vector<Printable>> content;
public:
    /// @brief Cronstructs a table from a header and a content vectors
    /// @param h The header of the new Table
    /// @param c The content of the new Table
    Table(vector<Printable> h, vector<vector<Printable>> c);

    /// @return Returns the header of the Table
    vector<Printable> getHeader() const;

    /// @return Returns the content of the Table
    vector<vector<Printable>> getContent() const;

    /// @brief Sets the header of the Table
    /// @param h The new header of the Table
    void setHeader(vector<Printable> h);

    /// @brief Sets the content of the Table
    /// @param c The new content of the Table
    void setContent(vector<vector<Printable>> c);

    /// @brief Sends the information of a Table to a ostream
    /// @param out The ostream object to where the Table information is gonna be send
    /// @param table The Table that has the informationthat is gonna be send
    /// @return The ostream object itself
    friend ostream &operator<<(ostream &out, Table<Printable> &table) {
        vector<unsigned> sizes;
        for (auto col : table.getHeader()) {
            ostringstream aux;
            aux << col;
            sizes.push_back(aux.str().size());
        }
        for (auto row: table.getContent()) {
            for (unsigned long i = 0; i < sizes.size(); ++i) {
                ostringstream aux;
                aux << row.at(i);
                if (aux.str().size() > sizes.at(i))
                    sizes.at(i) = aux.str().size();
            }
        }
        string line = "+";
        for (unsigned size : sizes) {
            line += string((size + PADDING * 2), '-') + "+";
        }
        out << line << endl;
        for (unsigned i = 0; i < table.header.size(); ++i) {
            out << "|" << string(PADDING, ' ') << normalize(table.header.at(i), sizes.at(i)) << string(PADDING, ' ');
        }
        out << "|" << endl << line << endl;
        for (unsigned i = 0; i < table.content.size(); ++i) {
            for (unsigned long j = 0; j < sizes.size(); ++j) {
                out << "|" << string(PADDING, ' ') << normalize(table.content.at(i).at(j), sizes.at(j))
                    << string(PADDING, ' ');
            }
            out << "|" << endl;
        }
        out << line << endl;
        return out;
    }
};

template<class Printable>
Table<Printable>::Table(vector<Printable> h, vector<vector<Printable>> c):
        header(h), content(c) {}

template<class Printable>
vector<Printable> Table<Printable>::getHeader() const {
    return header;
}

template<class Printable>
vector<vector<Printable>> Table<Printable>::getContent() const {
    return content;
}

template<class Printable>
void Table<Printable>::setHeader(vector<Printable> h) {
    header = h;
}

template<class Printable>
void Table<Printable>::setContent(vector<vector<Printable>> c) {
    content = c;
}

template<class Printable>
string normalize(const Printable s, unsigned n) {
    ostringstream aux;
    aux << s;
    unsigned diff = n - aux.str().length();
    ostringstream result;
    result << string(diff / 2 + diff % 2, ' ');
    result << s;
    result << string(diff / 2, ' ');
    return result.str();
}