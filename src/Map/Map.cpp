//
// Created by joao on 16/05/20.
//

#include "Map.h"
#include "Utilities/utils.h"

void Map::viewGraph(ViewGraph type) {

    graphViewer->closeWindow();
    if (type == PATH) {
        for (int i = 0; i < this->numVertex(); i++) {
            this->graphViewer->clearVertexLabel(this->graph.getVertexSet()[i]->getInfo()->getId());
        }
        for (unsigned int &greenEdge : greenEdges) {
            this->graphViewer->clearEdgeColor(greenEdge);
            this->graphViewer->setEdgeThickness(greenEdge, 1);
        }
    }
    if (type == CONECTIVITY)
        this->graphViewer->defineVertexSize(10);

    graphViewer->createWindow(graphViewer->getWidth(), graphViewer->getHeight());
    graphViewer->defineEdgeCurved(false);

    graphViewer->defineVertexColor("white");

    for (auto vertex : graph.getVertexSet()) {
        graphViewer->addNode(vertex->getInfo()->getId(), convertXToAPI(vertex->getInfo()->getX()), convertYToAPI(vertex->getInfo()->getY()));
        if (type != CONECTIVITY)
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
    if (!this->graph.isTarjanSolved())
        this->graph.tarjanStronglyConnectedComponents();

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

double Map::getTotalWeight(vector<unsigned> &poi_ids) {
    double weight = 0;

    int origin_id = poi_ids[0];

    for (int i = 0; i < poi_ids.size(); i++) {
        if (i == poi_ids.size() - 1)
            weight += getWeight(poi_ids[i], origin_id);

        else
            weight += getWeight(poi_ids[i], poi_ids[i + 1]);
    }

    return weight;
}

pair<vector<Local *>, double> Map::minimumWeightTour(vector<unsigned> *poi_ids)
{
    vector<unsigned> res;

    sort(poi_ids->begin(), poi_ids->end());

    double cost, minCost = INT64_MAX;

    do {
        cost = getTotalWeight(*poi_ids);

        if (cost < minCost)
        {
            minCost = cost;

            res = *poi_ids;
        }
    } while (next_permutation(poi_ids->begin(), poi_ids->end()));

    vector<Local *> path;
    vector<Local *> *twoPointPath;
    twoPointPath = this->getPath(res[0], res[1]);
    path = *twoPointPath;
    for (auto it = res.begin() + 1; it != res.end()-1; it++) {
        twoPointPath = this->getPath(*it, *(it+1));
        path.insert(path.end(), twoPointPath->begin() + 1, twoPointPath->end());
    }
    twoPointPath = this->getPath(res.back(), res[0]);
    path.insert(path.end(), twoPointPath->begin() + 1, twoPointPath->end());

    return pair<vector<Local *>, double>(path, minCost);
}

void Map::viewGraphConectivity() {
    if (this->graph.isTarjanSolved()) this->graph.tarjanStronglyConnectedComponents();
    this->viewGraph(CONECTIVITY);
    for (Vertex<Local *> *v : this->graph.getVertexSet()) {
        this->graphViewer->setVertexColor(v->getInfo()->getId(), giveColorToSSC(v->getSSC()));
    }
    this->graphViewer->rearrange();

}

void Map::viewTour(vector<Local *> path, double weight, vector<Local *> pois, bool api) {
    cout << "Tour total weight: " << weight << endl;
    if (!api) {
        for (int i = 0; i < (path).size()-1; i++) {
            cout << (path)[i]->getId() << " -> ";
            if (i % 20 == 0 && i != 0) {
                cout << endl;
            }
        }
        cout << (path).back()->getId() << endl;
    }

    if (api) {
        this->viewGraph(PATH);

        switch (this->locs[(path)[0]->getId()]) {
            case Tag::DEFAULT:
                graphViewer->setVertexLabel((path)[0]->getId(), "Start");
                break;
            case Tag::PRISON:
                graphViewer->setVertexLabel((path)[0]->getId(), "Start/Prison");
                break;
            case Tag::POLICE:
                graphViewer->setVertexLabel((path)[0]->getId(), "Start/Police");
                break;
            case Tag::COURT:
                graphViewer->setVertexLabel((path)[0]->getId(), "Start/Court");
                break;
            case Tag::HQ:
                graphViewer->setVertexLabel((path)[0]->getId(), "Start/HQ");
                break;
        }
        for (auto it = (path).begin(); it != (path).end() - 1; it++) {
            unsigned idEdge = edgeIds[pair<Local *, Local *>((*it), (*(it +1)))];
            graphViewer->setEdgeThickness(idEdge, 5);
            graphViewer->setEdgeColor(idEdge, "green");
            greenEdges.push_back(idEdge);
            if (!this->directed) {
                unsigned idEdgeBack = edgeIds[pair<Local *, Local *>((*it + 1), (*(it)))];
                if (idEdgeBack == 0) continue;
                graphViewer->setEdgeThickness(idEdgeBack, 5);
                graphViewer->setEdgeColor(idEdgeBack, "green");
                greenEdges.push_back(idEdgeBack);
            }
            if (it == (path).begin()) continue;

            graphViewer->setVertexSize((*it)->getId(), 10);
            switch (this->locs[(*it)->getId()]) {
                case Tag::DEFAULT:
                    break;
                case Tag::HQ:
                    graphViewer->setVertexSize((*it)->getId(), 20);
                    break;
                default:
                    graphViewer->setVertexSize((*it)->getId(), 15);
            }
        }

        for (auto poiID : pois) {
            graphViewer->setVertexColor(poiID->getId(), CYAN);
            graphViewer->setVertexSize(poiID->getId(), 20);
            if (poiID == pois[0]) continue;
            switch (this->locs[poiID->getId()]) {
                case Tag::DEFAULT:
                    graphViewer->setVertexLabel(poiID->getId(), "POI");
                    break;
                case Tag::PRISON:
                    graphViewer->setVertexLabel(poiID->getId(), "POI/Prison");
                    break;
                case Tag::POLICE:
                    graphViewer->setVertexLabel(poiID->getId(), "POI/Police");
                    break;
                case Tag::COURT:
                    graphViewer->setVertexLabel(poiID->getId(), "POI/Court");
                    break;
                case Tag::HQ:
                    graphViewer->setVertexLabel(poiID->getId(), "POI/HQ");
                    break;
            }
        }
        graphViewer->rearrange();
    }
}

string Map::giveColorToSSC(int ssc) {
    if (!sscToColor[ssc].empty()) return sscToColor[ssc];
    int a = sscToColor.size();
    switch(a % 13) {
        case 0:
            sscToColor[ssc] = DARK_GRAY;
            return sscToColor[ssc];
        case 1:
            sscToColor[ssc] = RED;
            return sscToColor[ssc];
        case 2:
            sscToColor[ssc] = CYAN;
            return sscToColor[ssc];
        case 3:
            sscToColor[ssc] = MAGENTA;
            return sscToColor[ssc];
        case 4:
            sscToColor[ssc] = BLUE;
            return sscToColor[ssc];
        case 5:
            sscToColor[ssc] = YELLOW;
            return sscToColor[ssc];
        case 6:
            sscToColor[ssc] = GREEN;
            return sscToColor[ssc];
        case 7:
            sscToColor[ssc] = ORANGE;
            return sscToColor[ssc];
        case 8:
            sscToColor[ssc] = GRAY;
            return sscToColor[ssc];
        case 9:
            sscToColor[ssc] = PINK;
            return sscToColor[ssc];
        case 10:
            sscToColor[ssc] = BLACK;
            return sscToColor[ssc];
        case 11:
            sscToColor[ssc] = WHITE;
            return sscToColor[ssc];
        case 12:
            sscToColor[ssc] = LIGHT_GRAY;
            return sscToColor[ssc];
    }
}
