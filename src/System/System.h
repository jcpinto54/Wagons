#pragma once

#include <vector>
#include "GraphTemplate/graphviewer.h"
#include "Local/Local.h"
#include "Utilities/Table.h"
#include "GraphTemplate/Graph.h"
#include "Wagon/Wagon.h"
#include "Map/Map.h"

/// The System Class
class System {
private:

    /// @brief The file with the names of the other database files
    string graphPath;

    vector<Wagon *> wagons;

    vector<Local *> POIs;

    Map map;
public:
    friend class Menu;

    /// @brief Creates a new system with the information provided by the file.
    /// @param fileName The path to the file where the information is stored.
    explicit System(const string &graphPath);

    /// @brief Writes the new information of the system on the files and frees the memory from the objects created during the execution of the program.
    ~System();

    void viewGraph();

    void viewPathBetween2Points(unsigned idFrom, unsigned idTo);

    void applyFloydWarshall();

    void addPOI();

    void erasePOI();

    vector<Local *>::iterator findPOI(unsigned id);

    void readPOIs();

    pair<vector<Local *>, double> solvePOITour();

    Map &getMap();

    const vector<Local*> &getPoIs() const;
};

Table<string> toTable(const vector<Local *> &container, const System *sys);


