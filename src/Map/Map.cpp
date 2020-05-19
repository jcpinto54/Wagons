//
// Created by joao on 16/05/20.
//

#include "Map.h"
#include "Utilities/utils.h"

void Map::viewGraph(bool toContinue) {

    if (this->graph.getNumVertex() > 300) {
        cout << "The graph is too big to represent in the gui mode!" << endl;
        return;
    }
    graphViewer->closeWindow();
    graphViewer->createWindow(graphViewer->getWidth(), graphViewer->getHeight());
    if (!this->directed)
        graphViewer->defineEdgeCurved(false);

    graphViewer->defineVertexColor("white");

    for (auto vertex : graph.getVertexSet()) {
        graphViewer->addNode(vertex->getInfo()->getId(), vertex->getInfo()->getX() - Local::getMinX(), vertex->getInfo()->getY()-Local::getMinY());
        switch (vertex->getInfo()->getTag()) {
            case Tag::COURT:
                graphViewer->setVertexColor(vertex->getInfo()->getId(), "orange");
                graphViewer->setVertexLabel(vertex->getInfo()->getId(), "Court");
                graphViewer->setVertexSize(vertex->getInfo()->getId(), 15);
                break;
            case Tag::POLICE:
                graphViewer->setVertexColor(vertex->getInfo()->getId(), "blue");
                graphViewer->setVertexSize(vertex->getInfo()->getId(), 15);
                graphViewer->setVertexLabel(vertex->getInfo()->getId(), "Police");
                break;
            case Tag::PRISON:
                graphViewer->setVertexColor(vertex->getInfo()->getId(), "black");
                graphViewer->setVertexSize(vertex->getInfo()->getId(), 15);
                graphViewer->setVertexLabel(vertex->getInfo()->getId(), "Prison");
                break;
            case Tag::HQ:
                graphViewer->setVertexColor(vertex->getInfo()->getId(), "cyan");
                graphViewer->setVertexSize(vertex->getInfo()->getId(), 25);
                graphViewer->setVertexLabel(vertex->getInfo()->getId(), "HQ");
                break;
            case Tag::DEFAULT:
                graphViewer->setVertexSize(vertex->getInfo()->getId(),5);
        }
    }
    int i = 0;
    for (auto vertex : graph.getVertexSet()) {
        for (auto edge : vertex->getAdj()) {
            if (directed)
                graphViewer->addEdge(i, vertex->getInfo()->getId(), edge.getDest()->getInfo()->getId(), EdgeType::DIRECTED);
            else
                graphViewer->addEdge(i, vertex->getInfo()->getId(), edge.getDest()->getInfo()->getId(), EdgeType::UNDIRECTED);
            edgeIds[pair<Local *, Local *>(vertex->getInfo(), edge.getDest()->getInfo())] = i;
            i++;
        }
    }
    if (!toContinue)
        graphViewer->rearrange();
}

Map::Map() {}

void Map::init(unordered_map<unsigned int, Vertex<Local *> *> &map) {
    for (auto & pair : map) {
        this->graph.addVertex(pair.second);
        locs[pair.first] = pair.second->getInfo()->getTag();
    }

    int width = (Local::getMaxX() - Local::getMinX()) + 50;
    int height = (Local::getMaxY() - Local::getMinY()) + 50;

    this->graphViewer = new GraphViewer(width, height, false);
}

void Map::setDirected(bool directed) {
    Map::directed = directed;
}

bool Map::isDirected() const {
    return directed;
}

vector<Local *> Map::getPath(unsigned int idFrom, unsigned int idTo) {
    Vertex<Local *> *from, *to;
    from = this->graph.findVertex(new Local(idFrom));
    to = this->graph.findVertex(new Local(idTo));
    if (from == NULL) {
        throw NonExistingVertex(idFrom);
    }
    if (to == NULL) {
        throw NonExistingVertex(idTo);
    }

    if (this->graph.isFloydWarshallSolved())
        return this->graph.getfloydWarshallPath(from->getInfo(), to->getInfo());

    this->graph.dijkstraShortestPath(from->getInfo());
    return this->graph.getPathTo(to->getInfo());
}

void Map::viewPath(unsigned int idFrom, unsigned int idTo, bool api) {
    vector<Local *> path;
    try {
        path = this->getPath(idFrom, idTo);
    } catch (NonExistingVertex e) {
        throw e;
    }

    if (!api) {
        for (int i = 0; i < path.size()-1; i++) {
            cout << path[i]->getId() << " -> ";
            if (i % 20 == 0 && i != 0) {
                cout << endl;
            }
        }
        cout << path.back()->getId() << endl;
    }


    if (api) {
        graphViewer->setVertexColor(idFrom, "green");
        graphViewer->setVertexSize(idFrom, 30);
        switch (this->locs[idFrom]) {
            case Tag::DEFAULT:
                graphViewer->setVertexLabel(idFrom, "Start");
                break;
            case Tag::PRISON:
                graphViewer->setVertexLabel(idFrom, "Start/Prison");
                break;
            case Tag::POLICE:
                graphViewer->setVertexLabel(idFrom, "Start/Police");
                break;
            case Tag::COURT:
                graphViewer->setVertexLabel(idFrom, "Start/Court");
                break;
            case Tag::HQ:
                graphViewer->setVertexLabel(idFrom, "Start/HQ");
                break;
        }
        graphViewer->setVertexColor(idTo, "red");
        graphViewer->setVertexSize(idTo, 30);
        switch (this->locs[idTo]) {
            case Tag::DEFAULT:
                graphViewer->setVertexLabel(idTo, "End");
                break;
            case Tag::PRISON:
                graphViewer->setVertexLabel(idTo, "End/Prison");
                break;
            case Tag::POLICE:
                graphViewer->setVertexLabel(idTo, "End/Police");
                break;
            case Tag::COURT:
                graphViewer->setVertexLabel(idTo, "End/Court");
                break;
            case Tag::HQ:
                graphViewer->setVertexLabel(idTo, "End/HQ");
                break;
        }
        for (auto it = path.begin(); it != path.end() - 1; it++) {
            unsigned idEdge = edgeIds[pair<Local *, Local *>(*it, *(it + 1))];
            graphViewer->setEdgeThickness(idEdge, 5);
            graphViewer->setEdgeColor(idEdge, "green");
            if (!this->directed) {
                unsigned idEdgeBack = edgeIds[pair<Local *, Local *>(*(it + 1), *it)];
                graphViewer->setEdgeThickness(idEdgeBack, 5);
                graphViewer->setEdgeColor(idEdgeBack, "green");
            }
            if (it == path.begin()) continue;

            graphViewer->setVertexSize((*it)->getId(), 10);
            switch (this->locs[(*it)->getId()]) {
                case Tag::DEFAULT:
                    break;
                case Tag::HQ:
                    graphViewer->setVertexSize((*it)->getId(), 25);
                    break;
                default:
                    graphViewer->setVertexSize((*it)->getId(), 20);
            }
        }
        graphViewer->rearrange();
    }
}

void Map::applyFloydWarshall() {
    this->graph.floydWarshallShortestPath();
}

double Map::dist(Local *l1, Local *l2) {
    return sqrt(pow(l1->getX() - l2->getX(), 2) + pow(l1->getY() - l2->getY(), 2));
}


int Map::numEdges() {
    return this->graph.getEdgeCounter();
}

int Map::numVertex() {
    return this->graph.getNumVertex();
}

void Map::setNumEdges(int numEdges) {
    this->graph.setEdgeCounter(numEdges);
}

void Map::solveTarjanAlgorithm() {
    this->graph.tarjanStronglyConnectedComponents();
}

bool Map::areStronglyConected(vector<Local *> &POIs) {
    if (!this->graph.isTarjanSolved()) this->solveTarjanAlgorithm();

    bool connected = true;
    bool firstTime = true;
    unsigned lowlink;

    for (auto loc : POIs) {
        Vertex<Local *> * v = this->graph.findVertex(loc);
        if (v == NULL) throw NonExistingVertex(loc->getId());
        if (firstTime) {
            firstTime = false;
            lowlink = v->getSSC();
        }
        if (lowlink != v->getSSC()) {
            connected = false;
            break;
        }
    }

    return connected;
}

Local *Map::findLocal(unsigned int id) {
    Vertex<Local *> *v = this->graph.findVertex(new Local(id));
    if (v == NULL) throw NonExistingVertex(id);
    return v->getInfo();
}
