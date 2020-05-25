//
// Created by joao on 16/05/20.
//

#include "Map.h"

#if !(defined(__linux__) || defined(__APPLE__))
#define MAXFLOAT 9e300
#endif
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
    this->graphViewer->defineVertexSize(10);
    if (type == CONECTIVITY)
        this->graphViewer->defineVertexSize(20);
    graphViewer->createWindow(graphViewer->getWidth(), graphViewer->getHeight());
    graphViewer->defineEdgeCurved(false);

    graphViewer->defineVertexColor("white");

    for (auto vertex : graph.getVertexSet()) {
        graphViewer->addNode(vertex->getInfo()->getId(), (vertex->getInfo()->getX()), (vertex->getInfo()->getY()));
        if (type != CONECTIVITY)
            switch (vertex->getInfo()->getTag()) {
                case Tag::COURT:
                    graphViewer->setVertexColor(vertex->getInfo()->getId(), "orange");
                    graphViewer->setVertexLabel(vertex->getInfo()->getId(), "Court");
                    graphViewer->setVertexSize(vertex->getInfo()->getId(), 25);
                    break;
                case Tag::POLICE:
                    graphViewer->setVertexColor(vertex->getInfo()->getId(), "blue");
                    graphViewer->setVertexSize(vertex->getInfo()->getId(), 25);
                    graphViewer->setVertexLabel(vertex->getInfo()->getId(), "Police");
                    break;
                case Tag::PRISON:
                    graphViewer->setVertexColor(vertex->getInfo()->getId(), "black");
                    graphViewer->setVertexSize(vertex->getInfo()->getId(), 25);
                    graphViewer->setVertexLabel(vertex->getInfo()->getId(), "Prison");
                    break;
                case Tag::HQ:
                    graphViewer->setVertexColor(vertex->getInfo()->getId(), "cyan");
                    graphViewer->setVertexSize(vertex->getInfo()->getId(), 50);
                    graphViewer->setVertexLabel(vertex->getInfo()->getId(), "HQ");
                    break;
            }
        graphViewer->setVertexLabel(vertex->getInfo()->getId(), to_string(vertex->getInfo()->getId()));
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

vector<Local *> *Map::getPath(unsigned int idFrom, unsigned int idTo, int algo) {
    Vertex<Local *> *from, *to;
    from = this->graph.findVertex(new Local(idFrom));
    to = this->graph.findVertex(new Local(idTo));
    if (from == NULL) {
        throw NonExistingVertex(idFrom);
    }
    if (to == NULL) {
        throw NonExistingVertex(idTo);
    }
    switch (algo) {
        case 2:
            this->graph.aStarShortestPath(from->getInfo(), to->getInfo());
            return this->graph.getSingleSourcePathTo(to->getInfo());
        case 1:
            if (this->graph.isAllPairsSolved())
                return this->graph.getAllPairsPath(from->getInfo(), to->getInfo());
            else
                cout << "Floyd-Warshall not calculated! (Default is Dijkstra)" << endl;
        case 0:
            this->graph.dijkstraShortestPath(from->getInfo());
            return this->graph.getSingleSourcePathTo(to->getInfo());

    }
    return new vector<Local *>();
}

void Map::viewPath(unsigned int idFrom, unsigned int idTo, bool api, int &algo) {
    vector<Local *> *path = new vector<Local*>();
    double weight;
    try {
        path = this->getPath(idFrom, idTo, algo);
        weight = this->getWeight(idFrom, idTo, algo);
    } catch (NonExistingVertex e) {
        throw e;
    }
    if (path->empty()) {
        cout << "There is no path between these points!" << endl;
        return;
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
        graphViewer->setVertexSize(idFrom, 40);
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
        graphViewer->setVertexSize(idTo, 40);
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

            switch (this->locs[(*it)->getId()]) {
                case Tag::DEFAULT:
                    graphViewer->setVertexSize((*it)->getId(), 15);
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

void Map::applyAllPairs(int algo) {

    switch (algo)
    {
        case 0:
            this->graph.floydWarshallShortestPath();
            break;

        case 1:
            this->graph.aStarShortestPathAllPairs();
            break;

        default:
            break;
    }
}

void Map::applyDijkstra(Local* const &origin) {
    this->graph.dijkstraShortestPath(origin);
}

void Map::applySingleSource(Local* const &destiny) {
    this->graph.getSingleSourcePathTo(destiny);
}

void Map::applyAStar(Local* const &origin, Local* const &destiny) {
    this->graph.aStarShortestPath(origin, destiny);
}

void Map::applyTarjan() {
    this->graph.tarjanStronglyConnectedComponents();
}

void Map::applyDfs() {
    this->graph.dfs();
}

void Map::applyBfs(Local* const &source) {
    this->graph.bfs(source);
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

bool Map::areStronglyConected(vector<POI *> &POIs) {
    if (!this->graph.isTarjanSolved())
        this->graph.tarjanStronglyConnectedComponents();

    bool connected = true;
    bool firstTime = true;
    unsigned lowlink;

    for (auto poi : POIs) {
        Local * loc = poi->getLoc();
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

double Map::getWeight(unsigned int idFrom, unsigned int idTo, int algo) {
    Vertex<Local *> *from, *to;
    from = this->graph.findVertex(new Local(idFrom));
    to = this->graph.findVertex(new Local(idTo));
    if (from == NULL) {
        throw NonExistingVertex(idFrom);
    }
    if (to == NULL) {
        throw NonExistingVertex(idTo);
    }

    switch (algo) {
        case 2:
            this->graph.aStarShortestPath(from->getInfo(), to->getInfo());
            return this->graph.getSingleSourceWeightTo(to->getInfo());
        case 1:
            if (this->graph.isAllPairsSolved())
                return this->graph.getAllPairsWeight(from->getInfo(), to->getInfo());
            else
                cout << "Floyd-Warshall not calculated! (Default is Dijkstra)" << endl;
        case 0:
            this->graph.dijkstraShortestPath(from->getInfo());
            return this->graph.getSingleSourceWeightTo(to->getInfo());

    }
    return 1.0;
}

double Map::getTotalWeight(vector<POI *> &poi_ids, int algo) {
    vector<double> weights = getPartedWeights(poi_ids, algo);
    double weight = 0;
    for (auto w : weights) {
        weight += w;
    }
    return weight;
}

// This is a TSP solution
Util::triplet<vector<Local *>, double, pair<Time, unsigned>> Map::minimumWeightTour(vector<POI *> *pois, Wagon * wagon, int algo) {
    // Sort is needed to make next_permutation() work. (See cycle condition)
    sort(pois->begin() + 1, pois->end() - 1);
    double cost, minCost = MAXFLOAT;

    // start is used to see if a tour doesn't make the wagon be late
    DateTime start = (*pois)[0]->getDt();
    vector<POI *> *tempRes = nullptr;
    vector<unsigned> res;

    // Add a POI at the end equal to the first one, but with "unlimited time" to reach it. So the path returns to the original place.
    POI *end = new POI(pois->at(0)->getLoc(), pois->at(0)->getDt().date + 1, pois->at(0)->getDt().time);
    pois->push_back(end);

    do {
        bool inTime = true;
        // weights is a vector of the weights between all POIs in vector (*pois). weights[0] = distance(pois->at(0), pois->at(1))
        vector<double> weights = getPartedWeights(*pois, algo);

        // Arrival at the POIs. Never goes down
        DateTime arrivaltime = start;
        for (int i = 1; i < weights.size(); i++) {
            // Time to travel between 2 POIs
            pair<Time, unsigned> takes = wagon->distToTime(weights[i]);
            arrivaltime = arrivaltime + takes.first;
            arrivaltime.date += takes.second;
            // If we arrive after the desired time, then we are late
            if ((*pois)[i]->getDt()< arrivaltime) {
                inTime = false;
                break;
            }
        }
        // We don't want a path that makes us get late
        if (!inTime) continue;

        cost = getTotalWeight(*pois, algo);

        // If we found a new best path, then remember it
        if (cost < minCost)
        {
            minCost = cost;
            tempRes = pois;
        }
            // Permutate the order of the POIs
    } while (next_permutation(pois->begin() + 1, pois->end() - 1));

    // If we didn't find a path that reaches all POIs in time
    if (tempRes == nullptr) {
        return Util::triplet<vector<Local *>, double, pair<Time, unsigned>>(vector<Local *>(), -1.0, pair<Time,unsigned>(Time(), 0));
    }

    // Convert vector of POIs to vector of POI ids
    for (auto p : *tempRes) {
        res.push_back(p->getLoc()->getId());
    }

    vector<Local *> path;
    vector<Local *> *twoPointPath;
    // Next 6 lines of code:
    //  Transform the vector of POI ids into a vector of Local *, that is the sequence of points of the result path.
    twoPointPath = this->getPath(res[0], res[1], algo);
    path = *twoPointPath;
    for (auto it = res.begin() + 1; it != res.end()-1; it++) {
        twoPointPath = this->getPath(*it, *(it+1), algo);
        path.insert(path.end(), twoPointPath->begin() + 1, twoPointPath->end());
    }

    return Util::triplet<vector<Local *>, double, pair<Time, unsigned>>(path, minCost, wagon->distToTime(minCost));
}

void Map::viewGraphConectivity() {
    if (this->graph.isTarjanSolved()) this->graph.tarjanStronglyConnectedComponents();
    this->viewGraph(CONECTIVITY);
    for (Vertex<Local *> *v : this->graph.getVertexSet()) {
        this->graphViewer->setVertexColor(v->getInfo()->getId(), giveColorToSCC(v->getSSC()));
    }
    this->graphViewer->rearrange();

}

void Map::viewTour(vector<Local *> path, double weight, pair<Time, unsigned> time, vector<POI *> pois, bool api) {
    cout << "Tour total weight: " << weight << endl;
    cout << "Tour duration: " << time.second << " days and " << time.first << endl;
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

            switch (this->locs[(*it)->getId()]) {
                case Tag::DEFAULT:
                    graphViewer->setVertexSize((*it)->getId(), 15);
                    break;
                case Tag::HQ:
                    graphViewer->setVertexSize((*it)->getId(), 25);
                    break;
                default:
                    graphViewer->setVertexSize((*it)->getId(), 20);
            }
        }

        for (auto poiID : pois) {
            graphViewer->setVertexColor(poiID->getLoc()->getId(), GREEN);
            graphViewer->setVertexSize(poiID->getLoc()->getId(), 30);
            if (poiID == pois[0]) continue;
            switch (this->locs[poiID->getLoc()->getId()]) {
                case Tag::DEFAULT:
                    graphViewer->setVertexLabel(poiID->getLoc()->getId(), "POI");
                    break;
                case Tag::PRISON:
                    graphViewer->setVertexLabel(poiID->getLoc()->getId(), "POI/Prison");
                    break;
                case Tag::POLICE:
                    graphViewer->setVertexLabel(poiID->getLoc()->getId(), "POI/Police");
                    break;
                case Tag::COURT:
                    graphViewer->setVertexLabel(poiID->getLoc()->getId(), "POI/Court");
                    break;
                case Tag::HQ:
                    graphViewer->setVertexLabel(poiID->getLoc()->getId(), "POI/HQ");
                    break;
            }
        }
        graphViewer->rearrange();
    }
}

string Map::giveColorToSCC(int scc) {
    if (!sccToColor[scc].empty()) return sccToColor[scc];
    int a = sccToColor.size();
    switch(a % 13) {
        case 0:
            sccToColor[scc] = DARK_GRAY;
            return sccToColor[scc];
        case 1:
            sccToColor[scc] = RED;
            return sccToColor[scc];
        case 2:
            sccToColor[scc] = CYAN;
            return sccToColor[scc];
        case 3:
            sccToColor[scc] = MAGENTA;
            return sccToColor[scc];
        case 4:
            sccToColor[scc] = BLUE;
            return sccToColor[scc];
        case 5:
            sccToColor[scc] = YELLOW;
            return sccToColor[scc];
        case 6:
            sccToColor[scc] = GREEN;
            return sccToColor[scc];
        case 7:
            sccToColor[scc] = ORANGE;
            return sccToColor[scc];
        case 8:
            sccToColor[scc] = GRAY;
            return sccToColor[scc];
        case 9:
            sccToColor[scc] = PINK;
            return sccToColor[scc];
        case 10:
            sccToColor[scc] = BLACK;
            return sccToColor[scc];
        case 11:
            sccToColor[scc] = WHITE;
            return sccToColor[scc];
        case 12:
            sccToColor[scc] = LIGHT_GRAY;
            return sccToColor[scc];
    }
    return PINK;
}

vector<double> Map::getPartedWeights(vector<POI *> &poi_ids, int algo) {
    vector<double> weights;

    for (int i = 0; i < poi_ids.size() - 1; i++) {
            weights.push_back(getWeight(poi_ids[i]->getLoc()->getId(), poi_ids[i + 1]->getLoc()->getId(), algo));
    }

    return weights;
}

Tag Map::getTag(unsigned int id) {
    return locs[id];
}

// Check if a start POI is compatible with a vector of POIs that the wagon (*w) will travel through.
//  algo is used to know what path algorithm to use
//  poi is "trying" to get inside pois.
bool Map::isStartPoiCompatible(POI * poi, vector<POI *> pois, Wagon *w, int algo) {
    for (int i = 0; i < pois.size(); i++) {
        // Start POIs haven't got odd indices. This is an assumption for this algorithm
        if (i%2 == 1) continue;
        // Checks if the poi date is on the same day as all of POIs inside pois
        if (poi->getDt().date - pois[i]->getDt().date > 0) return false;
        // Two start transport times cannot differ by more than 15 minutes
        if (Time(00,15,01) < poi->getDt().time - pois[i]->getDt().time) return false;
        double weight = this->getWeight(poi->getLoc()->getId(), pois[i]->getLoc()->getId(), algo);
        pair<Time, unsigned> timeTaken = w->distToTime(weight);
        // If trip between poi and a POI in pois takes more than a day, then return false
        if (timeTaken.second > 0) return false;
        // Checks if trip between poi and any of the POIs causes the wagon to be late
        if (poi->getDt().time - pois[i]->getDt().time < timeTaken.first) return false;
    }
    return true;
}

// Check if a end POI is compatible with a vector of POIs that the wagon (*w) will travel through.
//  algo is used to know what path algorithm to use
//  poi is "trying" to get inside pois.
bool Map::isEndPoiCompatible(POI * poi, vector<POI *> pois, Wagon *w, int algo) {
    for (int i = 0; i < pois.size(); i++) {
        // End POIs only got odd indices. This is an assumption for this algorithm
        if (i % 2 == 0) continue;
        // Checks if the poi date is on the same day as all of POIs inside pois
        if (poi->getDt().date - pois[i]->getDt().date > 0) return false;
        double weight = this->getWeight(poi->getLoc()->getId(), pois[i]->getLoc()->getId(), algo);
        pair<Time, unsigned> timeTaken = w->distToTime(weight);
        // If trip between poi and a POI in pois takes more than a day, then return false
        if (timeTaken.second > 0) return false;
        // Checks if trip between poi and any of the POIs causes the wagon to be late
        if (poi->getDt().time - pois[i]->getDt().time < timeTaken.first) return false;
    }
    return true;
}

void Map::resetAllPairsSolved() {
    this->graph.resetAllPairsSolved();
}

void Map::resetTarjanSolved() {
    this->graph.resetTarjanSolved();
}

void Map::resetDijkstraSolved() {
    this->graph.resetDijkstraSolved();
}

