#include "Local/Local.h"
#include "GraphTemplate/Graph.h"
#include "GraphTemplate/graphviewer.h"
#include <unordered_map>

class Map {
    Graph<Local *> graph;
    GraphViewer *graphViewer;

public:
    Map();

    Map(unordered_map<unsigned,Vertex<Local *> *> &map);

    void viewGraph();


};

