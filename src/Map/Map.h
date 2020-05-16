#include "Local/Local.h"
#include "GraphTemplate/Graph.h"
#include "GraphTemplate/graphviewer.h"
#include <unordered_map>

/*
struct apostaHash
{
	int operator() (const Aposta & ap) const
	{
		return ap.somaNumeros();
	}

	bool operator() (const Aposta & ap1, const Aposta & ap2) const
	{
		if ( ap1.tamanho() != ap2.tamanho() )
			return false;

		unsigned int n = ap1.calculaCertos(ap2.getNumeros());
		if ( n == ap1.tamanho() )
			return true;
		else
			return false;
	}
};



typedef unordered_set<Aposta, apostaHash, apostaHash> tabHAposta;
*/

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

    vector<Tag> locs;
    unordered_map<pair<Local *, Local *>, unsigned, edgeMapHash, edgeMapHash> edgeIds;

    bool directed = true;

    vector<Local *> getPath(unsigned idFrom, unsigned idTo);
public:
    Map();

    void init(unordered_map<unsigned,Vertex<Local *> *> &map);

    void viewGraph();

    void setDirected(bool directed);

    bool isDirected() const;

    void viewPath(unsigned idFrom, unsigned idTo);
};

/// NonExistingVertex Exception
class NonExistingVertex : public std::exception {
private:
    /// @brief The id of the not found vertex
    unsigned id;
    /// @brief The msg to be displayed when this exception occurs
    std::string msg = "The vertex with id " + to_string(id) + " doesn't exist";
public:
    /// @brief Constructs an NonExistingVertex exception
    /// @param id The id of the not found vertex
    explicit NonExistingVertex(unsigned id) : id(id) {}

    NonExistingVertex(NonExistingVertex const &e) {
        this->id = e.id;
    }

    /// Gets the message to be showed
    /// @return Returns the msg attribute of the object
    std::string getMsg() { return this->msg; }

};


