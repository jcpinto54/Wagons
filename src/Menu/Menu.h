#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
#include "System/System.h"

///Menu Class
class Menu {
protected:
    friend class System;

    /// @brief The next menu to be called
    char nextMenu;

    /// @brief The system that the menu will interact with
    System *sys;
public:
    ///@brief Constructs a new menu
    ///@param system The system that the menu will interact with
    Menu(System *system);

    ///@brief Gets Menu in organized manner
    ///@return Returns a matrix(n*2). The first column is a letter representing an option, and the second is the corresponding
    virtual vector<vector<string>> getOptions() const = 0;

    /// @brief Returns true if the option is valid in the menu that called it
    /// @param option The option that is gonna be verified
    /// @return Returns true if valid and false otherwise
    bool validOption(const string &option) const;

    /// @brief Reads a string from via user input
    /// @return Returns the string read
    string readOption() const;

    /// @brief Reads the input until it's a valid option for the menu that called it
    /// @return Returns the option chosen by the user
    char option() const;

    /// @brief Gets the nextMenu attribute
    /// @return Returns a char representing the next menu that needs to be called
    char getNext() const;
};

///Main menu
class MainMenu : public Menu {
public:
    ///@brief Constructs a new main menu
    ///@param system The system that the menu will interact with
    MainMenu(System *system);

    ///@brief Gets MainMenu in organized manner
    ///@return Returns a matrix(n*2). The first column is a letter representing an option, and the second is the corresponding
    vector<vector<string>> getOptions() const override;
};


///@brief Person menu
class PersonMenu : public Menu {
public:
    ///@brief PersonMenu Constructor
    PersonMenu(System *system);

    ///@brief Gets PersonMenu in organized manner
    ///@return Returns a matrix(n*2). The first column is a letter representing an option, and the second is the corresponding
    vector<vector<string>> getOptions() const override;
};

///@brief Update person menu
class UpdatePersonMenu : public Menu {
public:
    ///@brief UpdatePersonMenu Constructor
    UpdatePersonMenu(System *system);

    ///@brief Gets UpdatePersonMenu in organized manner
    ///@return Returns a matrix(n*2). The first column is a letter representing an option, and the second is the corresponding
    vector<vector<string>> getOptions() const override;
};

///@brief Read menu
template<class T>
class ReadMenu : public Menu {
public:
    ///@brief ReadMenu Constructor
    explicit ReadMenu(System *system) : Menu(system) {};
};

///@brief Read person menu
class ReadPersonMenu : public ReadMenu<Person> {
public:
    ///@brief ReadPersonMenu Constructor
    explicit ReadPersonMenu(System *system);

    ///@brief Gets ReadPersonMenu in organized manner
    ///@return Returns a matrix(n*2). The first column is a letter representing an option, and the second is the corresponding
    vector<vector<string>> getOptions() const override;
};


/////@brief Graph menu
//class GraphMenu : public Menu {
//public:
//    ///@brief GraphMenu Constructor
//    GraphMenu(System *system);
//
//    ///@brief Gets GraphMenu in organized manner
//    ///@return Returns a matrix(n*2). The first column is a letter representing an option, and the second is the corresponding
//    vector<vector<string>> getOptions() const override;
//};
