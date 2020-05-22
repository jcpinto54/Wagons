#include "Local/Local.h"
#include "GraphTemplate/Graph.h"
#include "GraphTemplate/graphviewer.h"
#include <unordered_map>
#include <utility>

#define PRECISION 10e9

enum ViewGraph {
    VIEW, PATH, CONECTIVITY
};

struct edgeMapHash
{
    int operator() (const pair<Local *, Local *> & key) const
    {
        return -key.first->getId() - key.second->getId();
    }

    bool operator() (const pair<Local *, Local *> & key1, const pair<Local *, Local *> & key2) const
    {
        return *key1.first == *key2.first && *key1.second == *key2.second;
    }
};

class Map {
    Graph<Local *> graph;
    GraphViewer *graphViewer;

    unordered_map<unsigned, Tag> locs;
    unordered_map<pair<Local *, Local *>, unsigned, edgeMapHash, edgeMapHash> edgeIds;
    vector<unsigned> greenEdges;

    bool directed = true;

    vector<Local *> *getPath(unsigned idFrom, unsigned idTo);

    double getWeight(unsigned idFrom, unsigned idTo);

    string giveColorToSSC(int ssc);
    unordered_map<int, string> sscToColor;


    double convertXToAPI(double x);
    double convertYToAPI(double y);
public:
    Map();

    void init(unordered_map<unsigned,Vertex<Local *> *> &map);

    void viewGraph(ViewGraph type);

    void setDirected(bool directed);

    bool isDirected() const;

    void viewPath(unsigned idFrom, unsigned idTo, bool api);
    void viewTour(vector<Local *> path, double weight, vector<Local *> pois, bool api);

    void applyFloydWarshall();

    double dist(Local *l1, Local *l2);

    int numVertex();

    int numEdges();

    void setNumEdges(int numEdges);


    bool areStronglyConected(vector<Local *> &POIs);
    bool areStronglyConected(unsigned id1, unsigned id2);

    Local *findLocal(unsigned id);

    void viewGraphConectivity();

    double getTotalWeight(vector<unsigned> &poi_ids);

    // TSP Algorithm
    pair<vector<Local *>, double> minimumWeightTour(vector<unsigned> *poi_ids);
};

/// NonExistingVertex Exception
class NonExistingVertex : public std::exception {
private:
    /// @brief The id of the not found vertex
    unsigned id;
    /// @brief The msg to be displayed when this exception occurs
    std::string msg;
public:
    /// @brief Constructs an NonExistingVertex exception
    /// @param id The id of the not found vertex
    explicit NonExistingVertex(unsigned id) : id(id) {
        this->msg = "The vertex with id " + to_string(id) + " doesn't exist";
    }

    NonExistingVertex(NonExistingVertex const &e) {
        this->id = e.id;
        this->msg = e.msg;
    }

    /// Gets the message to be showed
    /// @return Returns the msg attribute of the object
    std::string getMsg() { return this->msg; }

};

/// ImpossiblePath Exception
class ImpossiblePath : public std::exception {
private:
    Local * loc;
    /// @brief The msg to be displayed when this exception occurs
    std::string msg;
public:
    /// @brief Constructs an ImpossiblePath exception
    /// @param idFrom The id of the start vertex
    /// @param idTo The id of the end vertex
    explicit ImpossiblePath(Local * l) : loc(l){
        this->msg = "Adding this POI (ID:" + to_string(loc->getId()) + ") would make an impossible route!";
    }

    ImpossiblePath(ImpossiblePath const &e) {
        this->loc = e.loc;
        this->msg = e.msg;
    }

    /// Gets the message to be showed
    /// @return Returns the msg attribute of the object
    std::string getMsg() { return this->msg; }

};

string giveColorToSSC(int ssc);