//
// Created by joao on 16/05/20.
//

#include "Map.h"
#include "Utilities/utils.h"

void Map::viewGraph(ViewGraph type) {

    graphViewer->closeWindow();

    if (type == PATH) {
        graphViewer->defineVertexSize(5);
    }

    graphViewer->createWindow(graphViewer->getWidth(), graphViewer->getHeight());
    graphViewer->defineEdgeCurved(false);

    graphViewer->defineVertexColor("white");

    for (auto vertex : graph.getVertexSet()) {
        graphViewer->addNode(vertex->getInfo()->getId(), convertXToAPI(vertex->getInfo()->getX()), convertYToAPI(vertex->getInfo()->getY()));
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
    if (type == VIEW)
        graphViewer->rearrange();
}

Map::Map() {}

void Map::init(unordered_map<unsigned int, Vertex<Local *> *> &map) {
    for (auto & pair : map) {
        this->graph.addVertex(pair.second);
        locs[pair.first] = pair.second->getInfo()->getTag();
    }

    this->graph.tarjanStronglyConnectedComponents();

    this->graphViewer = new GraphViewer(600, 600, false);
}

void Map::setDirected(bool directed) {
    Map::directed = directed;
}

bool Map::isDirected() const {
    return directed;
}

vector<Local *> *Map::getPath(unsigned int idFrom, unsigned int idTo) {
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
    return this->graph.getDijkstraPathTo(to->getInfo());
}

void Map::viewPath(unsigned int idFrom, unsigned int idTo, bool api) {
    vector<Local *> *path;
    double weight;
    try {
        path = this->getPath(idFrom, idTo);
        weight = this->getWeight(idFrom, idTo);
    } catch (NonExistingVertex e) {
        throw e;
    }

    cout << "Path total weight: " << weight << endl;
    if (!api) {
        for (int i = 0; i < (*path).size()-1; i++) {
            cout << (*path)[i]->getId() << " -> ";
            if (i % 20 == 0 && i != 0) {
                cout << endl;
            }
        }
        cout << (*path).back()->getId() << endl;
    }


    if (api) {
        this->graphViewer->setVertexColor(idFrom, "white");
        this->graphViewer->setVertexColor(idTo, "white");
        for (int i = 0; i < this->numVertex(); i++) {
            this->graphViewer->clearVertexLabel(this->graph.getVertexSet()[i]->getInfo()->getId());
        }
        for (unsigned int & greenEdge : greenEdges) {
            this->graphViewer->clearEdgeColor(greenEdge);
            this->graphViewer->setEdgeThickness(greenEdge, 1);
        }
        this->viewGraph(PATH);

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
        for (auto it = (*path).begin(); it != (*path).end() - 1; it++) {
            unsigned idEdge = edgeIds[pair<Local *, Local *>(*it, *(it + 1))];
            graphViewer->setEdgeThickness(idEdge, 5);
            graphViewer->setEdgeColor(idEdge, "green");
            greenEdges.push_back(idEdge);
            if (!this->directed) {
                unsigned idEdgeBack = edgeIds[pair<Local *, Local *>(*(it + 1), *it)];
                graphViewer->setEdgeThickness(idEdgeBack, 5);
                graphViewer->setEdgeColor(idEdgeBack, "green");
                greenEdges.push_back(idEdgeBack);
            }
            if (it == (*path).begin()) continue;

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
    int distY = l1->getY() - l2->getY();
    int distX = l1->getX() - l2->getX();
    double distXPow = pow((double)distX, 2);
    double distYPow = pow((double)distY, 2);
    return sqrt(distXPow + distYPow);
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


bool Map::areStronglyConected(vector<Local *> &POIs) {
    if (!this->graph.isTarjanSolved()) this->graph.tarjanStronglyConnectedComponents();

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

bool Map::areStronglyConected(unsigned id1, unsigned id2) {
    if (!this->graph.isTarjanSolved()) this->graph.tarjanStronglyConnectedComponents();

    Vertex<Local *> * v1 = this->graph.findVertex(new Local(id1)), *v2 = this->graph.findVertex(new Local(id2));
    if (v1 == NULL) throw NonExistingVertex(id1);
    if (v2 == NULL) throw NonExistingVertex(id2);

    return v1->getSSC() == v2->getSSC();
}

void Map::testTarjanAlgorithm() {

    cout << "Tarjan Algorithm Testing" << endl;
    cout << "Pair Testing" << endl;
    cout << "(1, 5) Needs to be 0: " << this->areStronglyConected(1, 5) << endl;
    cout << "(8, 10) Needs to be 0: " << this->areStronglyConected(8, 10) << endl;
    cout << "(10, 12) Needs to be 1: " << this->areStronglyConected(10, 12) << endl;

    cout << "Vector Testing" << endl;
    vector<Local *> locs;
    locs.push_back(new Local(5)); locs.push_back(new Local(7)); locs.push_back(new Local(3));
    cout << "(5, 7, 3) Needs to be 1: " << this->areStronglyConected(locs) << endl;
    locs.push_back(new Local(8));
    cout << "(5, 7, 3, 8) Needs to be 0: " << this->areStronglyConected(locs) << endl;

}

double Map::convertXToAPI(double x) {
    if (abs(Local::maxX - Local::minX) <= 0.001)
        return -x * PRECISION;
    else
        return (x * PRECISION - Local::minX * PRECISION) / ((Local::maxX - Local::minX)  * PRECISION / (double)this->graphViewer->getWidth());
}

double Map::convertYToAPI(double y) {
    if (abs(Local::maxY - Local::minY) <= 0.001)
        return y;
    else
        return ((-y * PRECISION + Local::minY * PRECISION) + (Local::maxY -Local::minY) * PRECISION) / ((Local::maxY - Local::minY) * PRECISION / this->graphViewer->getHeight());
}

double Map::getWeight(unsigned int idFrom, unsigned int idTo) {
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
        return this->graph.getFloydWarshallWeight(from->getInfo(), to->getInfo());

    this->graph.dijkstraShortestPath(from->getInfo());
    return this->graph.getDijkstraWeightTo(to->getInfo());
}

void Map::viewGraphConectivity() {
    if (this->graph.isTarjanSolved()) this->graph.tarjanStronglyConnectedComponents();

    for (Vertex<Local *> *v : this->graph.getVertexSet()) {

    }

}

string giveColorToSSC(int ssc) {
    switch(ssc % 13) {
        case 0:
            return BLUE;
        case 1:
            return RED;
        case 2:
            return PINK;
        case 3:
            return MAGENTA;
        case 4:
            return ORANGE;
        case 5:
            return YELLOW;
        case 6:
            return GREEN;
        case 7:
            return CYAN;
        case 8:
            return GRAY;
        case 9:
            return LIGHT_GRAY;
        case 10:
            return DARK_GRAY;
        case 11:
            return BLACK;
        case 12:
            return WHITE;
    }
}
