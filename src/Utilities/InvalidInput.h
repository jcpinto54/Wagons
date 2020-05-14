#pragma once

#include <string>

/// Invalid Input Exception
class InvalidInput : public std::exception {
private:
    /// @brief The msg to be displayed when this exception occurs
    std::string msg;
public:
    /// @brief Constructs an InvalidInput exception
    /// @param msg The msg of the new InvalidInput exception
    InvalidInput(std::string msg) : msg(move(msg)) {};

    /// @brief Construct an InvalidInput exception with an empty msg
    InvalidInput() = default;

    /// Gets the message to be showed
    /// @return Returns the msg attribute of the object
    std::string getMsg() { return this->msg; }
};
