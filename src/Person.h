#pragma once

#include <ostream>
#include <utility>
#include "Date.h"


///@brief The Person Class
class Person {
private:
    /// @brief The name of the Person
    string name;

    /// @brief The birthday of the Person
    const Date birthday;

    /// @brief The phone number of the Person
    unsigned contact;

    const unsigned id;

    static unsigned nextId;
public:
    /// @brief Constructs a new Person from the arguments
    /// @param name The name of the new Person
    /// @param birthday The birthday of the new Person
    /// @param contact The contact of the new Person
    Person(string name, Date birthday, unsigned contact, unsigned id);

    /// @brief Constructs a new Person from the arguments
    /// @param name The name of the new Person
    /// @param birthday The birthday of the new Person
    /// @param contact The contact of the new Person
    Person(string name, Date birthday, unsigned contact);

    /// @brief Creates a person equal to other person
    /// @param person The person to be copied to the object that called the function
    Person(const Person &person);

    /// @brief Gets the name of person.
    /// @return The name attribute of person.
    string getName() const;

    /// @brief Gets the birthday date of the person.
    /// @return The Birthday date attribute of the person.
    Date getBirthday() const;

    ///@brief Gets the age of the person.
    ///@return The age of the person.
    unsigned getAge() const;

    ///@brief Gets the contact date of the person.
    ///@return The Contact attribute of the person.
    unsigned getContact() const;

    ///@brief Sets the name of the card.
    ///@param name The name to be attributed to the card.
    void setName(string name);

    ///@brief Sets the Contact of the card.
    ///@param contact The contact to be attributed to the card.
    void setContact(unsigned contact);

    Person(unsigned int id);

    ///@brief Reads an object person from the input stream in.
    ///@param in Stream from where the person will be read.
    ///@param person Pointer to pointer to a person object.
    ///@return Input stream.
    friend istream &operator>>(istream &in, Person **person);

    ///@brief Prints an object person to the output strean out.
    ///@param out Stream where person's data will be printed.
    ///@param person Person to be printed.
    ///@return Output stream with data printed.
    friend ostream &operator<<(ostream &out, Person *person);

    ///@brief Compares two persons.
    ///@param person Person to be compared with the *this person.
    ///@return True if *this and person have the same name. False otherwise.
    bool operator==(Person & person);

    ///@brief Compares two persons.
    ///@param person Person to be compared with the *this person.
    ///@return True if *this and person have a different name or a different birthday. False otherwise.
    bool operator!=(Person person);

        ///@brief Gets the type of the Person.
    ///@return A string specifing the type of client/person. If the object is a Person and not a client, it returns "Not a Client".
    virtual string getType();

    unsigned int getId() const;

    friend bool operator<(const Person &lhs, const Person &rhs);

};

///@brief The ExistingPerson exception
class ExistingPerson : public exception, public Person {
public:

    ///@brief Constructs an ExistingPerson object.
    ///@param p Person that already exists.
    ExistingPerson(Person p) : Person(p) {}
};

bool compareName(const Person *person1, const Person *person2);

///@brief Compares the birthday of two persons.
///@param person1 Person whose birthday will be compared.
///@param person2 Person whose birthday will be compared.
///@return True if the birthdays are the same. False otherwise.
bool compareBirthday(const Person *person1, const Person *person2);

bool compareId(const Person *p1, const Person *p2);