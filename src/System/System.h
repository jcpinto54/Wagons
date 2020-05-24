#pragma once

#include <vector>
#include "GraphTemplate/graphviewer.h"
#include "Local/Local.h"
#include "Local/POI.h"
#include "Utilities/Table.h"
#include "GraphTemplate/Graph.h"
#include "Wagon/Wagon.h"
#include "Map/Map.h"

/// The System Class
class System {
private:
    string graphPath;

    Local *hq;

    vector<Wagon *> wagons;

    vector<POI *> POIs;

    Map map;

    vector<vector<string>> getSugestions() const;
public:
    string getGraphPath();

    friend class Menu;

    explicit System(const string &graphPath);

    ~System();

    void viewGraph();

    void viewPathBetween2Points(unsigned idFrom, unsigned idTo, int &algo);

    void applyAllPairs(int algo);

    void addPOI();

    void erasePOI();

    vector<POI *>::iterator findPOI(unsigned id);

    void readPOIs();

    Util::triplet<vector<Local *>, double, pair<Time, unsigned>> solvePOITour();

    Map &getMap();

    const vector<POI*> &getPoIs() const;

    vector<POI *>::iterator findPOI(const POI * poi);

    Wagon* chooseWagon();

    void printSuggestions();
    int readAlgorithm();

    int readAllPairsAlgorithm();
};

Table<string> toTable(const vector<POI *> &container, const System *sys);

bool isAlgo(const string &toTest);
bool isAllPairsAlgo(const string &toTest);
bool isWagonOption(const string &toTest);

