#pragma once

#include <vector>
#include "GraphTemplate/graphviewer.h"
#include "Local/Local.h"
#include "Person/Person.h"
#include "Utilities/Table.h"
#include "GraphTemplate/Graph.h"
#include "Wagon/Wagon.h"
#include "Map/Map.h"

/// The System Class
class System {
private:
    friend class Menu;

    /// @brief The file with the names of the other database files
    string fileName;

    /// @brief All People memory positions in a vector
    vector<Person *> people;

    vector<Wagon *> wagons;

    Map map;
public:
    /// @brief Creates a new system with the information provided by the file.
    /// @param fileName The path to the file where the information is stored.
    explicit System(const string &fileName/*,const string pass*/);

    /// @brief Writes the new information of the system on the files and frees the memory from the objects created during the execution of the program.
    ~System();

    /// @return Returns a vector with the people of the system.
    vector<Person *> &getPeople();

    /// @brief Finds the memory position of a Person.
    /// @param name A string equal to the name of the person. Not case sensitive.
    /// @param birthday A date equal to the person's birthday.
    /// @return Returns a iterator to the Person, people.end() otherwise.
    vector<Person *>::const_iterator findPerson(unsigned id) const;

    /// @brief Inserts a new person in the People vector
    /// @param Person The person to be inserted
    void createPerson(Person *Person);

    /// @brief Reads the information via console and creates a new Client with those attributes.
    void createPerson();

    /// @brief Asks information about the Person and shows the specified person.
    void readPerson() const;

    /// @brief Shows a list of people in a Table
    /// @param container A vector of the people to be showed
    void readPeople(const vector<Person *> &container) const;

    ///@brief Reads a client information via user input and removes it.
    void deletePerson();

    ///@brief It's called by deletePerson() and deletes a Client.
    ///@param name The name of the client to be deleted.
    ///@param birthday The birthday of the client to be deleted.
    void deletePerson(unsigned id);

    void updatePersonName(vector<Person *>::const_iterator iterator);

    void updatePersonContact(vector<Person *>::const_iterator iterator);

    void viewGraph();

    void viewPathBetween2Points(unsigned idFrom, unsigned idTo);

    void applyFloydWarshall();
};

///@brief Receives a vector of Person and transforms in a table.
Table<string> toTable(const vector<Person *> &container, const System *sys);


