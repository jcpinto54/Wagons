#pragma once

#include <vector>
#include "GraphTemplate/graphviewer.h"
#include "Local/Local.h"
#include "Local/POI.h"
#include "Utilities/Table.h"
#include "GraphTemplate/Graph.h"
#include "Wagon/Wagon.h"
#include "Map/Map.h"
#include "Prisioner/Prisioner.h"
#include <chrono>

using namespace chrono;

/// The System Class
class System {
private:
    string graphPath;

    Local *hq;

    vector<POI *> POIs;

    vector<Prisioner *> prisioners;

    Map map;

    vector<vector<string>> getSugestions() const;
public:
    friend class Menu;

    explicit System(const string &graphPath);

    ~System();

    Map &getMap();

    //GUI interface
    void viewGraph();
    void viewPathBetween2Points(unsigned idFrom, unsigned idTo, int &algo);

    // Apply graph
    void applyDijkstra(Local* const &origin);
    void applySingleSource(Local* const &destiny);
    void applyAStar(Local* const &origin, Local* const &destiny);
    void applyTarjan();
    void applyAllPairs(int algo);
    void applyDfs();
    void applyBfs(Local* const &source);

    // POIS
    void addPOI();
    void erasePOI();
    vector<POI *>::iterator findPOI(unsigned id);
    void readPOIs();
    const vector<POI*> &getPoIs() const;
    vector<POI *>::iterator findPOI(const POI * poi);
    Util::triplet<vector<Local *>, double, pair<Time, unsigned>> solvePOITour();

    // Wagon
    Wagon* chooseWagon();

    // Prisioners
    const vector<Prisioner *> & getPrisioners() const;
    vector<triplet<vector<Local *>, double, pair<Time, unsigned>>> solvePrisionersTour(vector<Prisioner *> &prisVecForErrors, vector<vector<POI *>> &toAPI);
    void addPrisionerTransport();
    void prisonTransfer();
    void attendCourt();
    void policeToPrison();
    void communityService();
    void erasePrisioner();
    void readPrisionersTransports();
    Prisioner *findPrisioner(POI*start, POI*end);
    
    int readAllPairsAlgorithm();
    void printSuggestions();
    int readAlgorithm();
};

Table<string> toTable(const vector<POI *> &container, const System *sys);

bool isAlgo(const string &toTest);
bool isAllPairsAlgo(const string &toTest);
bool isWagonOption(const string &toTest);
bool isTransportOption(const string &toTest);
