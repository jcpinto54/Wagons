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
    Local *hq;

    vector<Wagon *> wagons;

    vector<POI *> POIs;

    Map map;
public:
    friend class Menu;

    explicit System(const string &graphPath);

    ~System();

    void viewGraph();

    void viewPathBetween2Points(unsigned idFrom, unsigned idTo);

    void applyFloydWarshall();

    void addPOI();

    void erasePOI();

    vector<POI *>::iterator findPOI(unsigned id);

    void readPOIs();

    pair<vector<Local *>, double> solvePOITour();

    Map &getMap();

    const vector<POI*> &getPoIs() const;

    vector<POI *>::iterator findPOI(const POI * poi);

    Wagon* chooseWagon();
};

Table<string> toTable(const vector<POI *> &container, const System *sys);

bool isWagonOption(const string &toTest);

