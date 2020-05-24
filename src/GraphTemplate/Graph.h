/*
 * GraphTemplate.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <sstream>
#include <stack>
#include "MutablePriorityQueue.h"
#include <algorithm>

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
    T info;						// content of the vertex
    vector<Edge<T> > adj;		// outgoing edges

    double dist = 0;
    Vertex<T> *path = NULL;
    int queueIndex = 0; 		// required by MutablePriorityQueue

    // used for dfs and bfs
    bool visited = false;
    bool processing = false;

    // used for tarjan algorithm
    unsigned tarjanId, tarjanLowlink;
    bool onStack;

    void addEdge(Vertex<T> *dest, double w);

public:
    Vertex(T in);
    T getInfo() const;
    double getDist() const;
    Vertex *getPath() const;
    unsigned getSSC() const;

    bool operator<(Vertex<T> & vertex) const;

    vector<Edge<T>> &getAdj();

    // // required by MutablePriorityQueue
    friend class Graph<T>;
    friend class MutablePriorityQueue<Vertex<T>>;
};

template<class T>
vector<Edge<T>> &Vertex<T>::getAdj() {
    return adj;
}

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
    adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
    return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
    Vertex<T> * dest;      // destination vertex
    double weight;         // edge weight
public:
    Edge(Vertex<T> *d, double w);

    Vertex<T> *getDest() const;

    friend class Graph<T>;
    friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}

template<class T>
Vertex<T> *Edge<T>::getDest() const {
    return dest;
}
/*************************** Graph  **************************/

template <class T>
class Graph {
    vector<Vertex<T> *> vertexSet;    // vertex set
    int edgeCounter = 0;

    void dfsVisit(Vertex<T> *v,  vector<T> & res) const;

    //Data for Dijkstra
    pair<bool, unsigned> dijkstraSolved = pair<bool, unsigned>(false, 0);

    // Data for Tarjan Algorithm
    std::stack<Vertex<T> *> stack;
    unsigned nextTarjanId;
    bool tarjanSolved = false;
    unsigned sccCount;

    // Data Structures for FloydWarshall Algorithm
    vector<vector<double>> dist;
    vector<vector<Vertex<T>*>> pred;
    bool floydWarshallSolved = false;

public:
    Vertex<T> *findVertex(const T &in) const;
    bool addVertex(const T &in);
    bool addVertex(Vertex<T> *in);
    bool addEdge(const T &sourc, const T &dest, double w);
    int getNumVertex() const;

    void setEdgeCounter(int edgeCounter);

    int getEdgeCounter() const;

    vector<Vertex<T> *> getVertexSet() const;
    vector<T> dfs() const;
    vector<T> bfs(const T &source) const;

    // Dijkstra
    void dijkstraShortestPath(const T &s);
    vector<T> *getSingleSourcePathTo(const T &dest) const;
    double getDijkstraWeightTo(const T &dest) const;

    void aStarShortestPath(const T &orig, const T &destiny);

    // FloydWarshall Functions
    void floydWarshallShortestPath();
    vector<T> *getfloydWarshallPath(const T &origin, const T &dest) const;
    double getFloydWarshallWeight(const T &origin, const T &dest) const;
    bool isFloydWarshallSolved() const;

    // Tarjan Algorithm / Strongly Connected Components "finder"
    void tarjanStronglyConnectedComponents();
    void tarjanDfs(Vertex<T> *at);
    bool isTarjanSolved() const;

    void dijkstraShortestPathAllPairs();
    void aStarShortestPathAllPairs();
};

template<class T>
unsigned Vertex<T>::getSSC() const {
    return this->tarjanLowlink;
}


template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template<class T>
int Graph<T>::getEdgeCounter() const {
    return edgeCounter;
}

template<class T>
void Graph<T>::setEdgeCounter(int edgeCounter) {
    Graph::edgeCounter = edgeCounter;
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet) {
        if (std::is_same<T, Local*>::value) {
            if (*(v->info) == *in)
                return v;
        }
        if (v->info == in)
            return v;
    }
    return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if ( findVertex(in) != NULL)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == NULL || v2 == NULL)
        return false;
    v1->addEdge(v2,w);
    return true;
}

/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
vector<T> Graph<T>::dfs() const {
    vector<T> res;
    auto it = this->vertexSet.begin();

    for (; it != this->vertexSet.end(); it++) {
        dfsVisit(*it, res);
    }

    for (auto v : this->vertexSet) {
        v->visited = false;
    }
    return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, vector<T> & res) const {
    if (v->visited) return;
    res.push_back(v->info);
    v->visited = true;

    Vertex<T> *cur;
    for (auto edge : v->adj) {
        cur = edge.dest;
        dfsVisit(cur, res);
    }
}

/*
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by bfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
vector<T> Graph<T>::bfs(const T & source) const {
    // HINT: Use the flag "visited" to mark newly discovered vertices.
    // HINT: Use the "queue<>" class to temporarily store the vertices.
    vector<T> res;
    queue<Vertex<T> *> auxQueue;
    if (this->vertexSet.empty()) return vector<T>();

    auxQueue.push(this->vertexSet[0]);
    Vertex<T> * cur;
    while (!auxQueue.empty()) {
        cur = auxQueue.front();
        if (!cur->visited) {
            cur->visited = true;
            for (auto edge : cur->adj) {
                auxQueue.push(edge.dest);
            }
            res.push_back(cur->info);
        }
        auxQueue.pop();
    }

    return res;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
    int originIndex = -1, i = 0;
    for (Vertex<T>* v : vertexSet) {
        if (origin == v->info) {
            originIndex = i;
        }
        i++;
    }
    if (originIndex == -1) {
        cerr << "This vertex doesn't exist!" << endl;
        return;
    }

    if (dijkstraSolved.first && dijkstraSolved.second == originIndex) return;

    for (auto v : vertexSet)
    {
        v->dist = INF;
        v->path = nullptr;
    }
    auto s = findVertex(origin);
    s->dist = 0;
    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);
    while(!q.empty())
    {
        auto v = q.extractMin();
        for(auto &e : v->adj)
        {
            auto od = e.dest->dist;
            auto w = e.dest;
            if(v->dist + e.weight < w->dist)
            {
                w->dist = v->dist + e.weight;
                w->path = v;
                if (od == INF)
                    q.insert(w);
                else
                    q.decreaseKey(w);
            }
        }
    }
    dijkstraSolved = pair<bool, unsigned>(true, originIndex);
}


template<class T>
vector<T> *Graph<T>::getSingleSourcePathTo(const T &dest) const{
    vector<T> *res = new vector<T>;

    auto v = findVertex(dest);

    if (v == nullptr || v->dist == INF)
        return res;

    for (; v != nullptr; v = v->path)
        res->push_back(v->info);

    reverse(res->begin(), res->end());

    return res;
}

template<class T>
double Graph<T>::getDijkstraWeightTo(const T &dest) const{
    Vertex<T> *v = findVertex(dest);

    if (v == nullptr || v->dist == INF)
        return INF;

    return v->dist;
}

template<class T>
void Graph<T>::aStarShortestPath(const T &orig, const T &destiny)
{
    if (std::is_same<T, Local*>::value) {
        Local *origin = (Local *) orig;
        Local *dest = (Local *) destiny;

        int originIndex = -1, i = 0;
        for (Vertex<T>* v : vertexSet) {
            if (origin == v->info) {
                originIndex = i;
            }
            i++;
        }
        if (originIndex == -1) {
            cerr << "This vertex doesn't exist!" << endl;
            return;
        }

        for (Vertex<T>* v : vertexSet) {
            v->path = nullptr;
            v->dist = INF;
        }

        Vertex<Local *> *s = findVertex(origin), *d = findVertex(dest);
        s->dist = origin->dist(dest);
        MutablePriorityQueue<Vertex<Local *>> q;
        q.insert(s);

        while (!q.empty()) {
            Vertex<Local *> *v = q.extractMin();

            if (v == d)
                break;

            for (auto &e : v->adj) {
                auto od = e.dest->dist;
                auto w = e.dest;
                auto l = v->info;

                //
                double h = v->dist - l->dist(dest) + e.weight + l->dist(dest);

                if (h < w->dist) {
                    w->dist = h;
                    w->path = v;

                    if (od == INF)
                        q.insert(w);
                    else
                        q.decreaseKey(w);
                }
            }
        }

        Vertex<Local *> *v = d;
        double l = 0;

        while (v->path != NULL)
        {
            for (auto e : v->path->getAdj())
            {
                if (e.dest->info->getId() == v->info->getId())
                {
                    l += e.weight;
                    break;
                }
            }
            v = v->path;
        }
    }
    else
        cout << "A* algorithm only works for Local objects" << endl;
}






/**************** All Pairs Shortest Path  ***************/

template<class T>
void Graph<T>::floydWarshallShortestPath() {
    if (this->floydWarshallSolved) return;

    dist.clear();
    dist = vector<vector<double>>(vertexSet.size(), vector<double>(vertexSet.size(), INT64_MAX));
    pred.clear();
    pred = vector<vector<Vertex<T>*>>(vertexSet.size(), vector<Vertex<T>*>(vertexSet.size(), NULL));

    // Build dist matrix
    int i = 0, j = 0;
    for (auto v1 : vertexSet) {
        for (auto v2 : vertexSet) {
            if (i == j)
                dist[i][j] = 0;
            else
                for (Edge<T> edge: v1->adj)
                    if (edge.dest->info == v2->info) {
                        dist[i][j] = edge.weight;
                        pred[i][j] = v1;
                    }
            j++;
        }
        i++;
        j = 0;
    }

    for (int k = 0; k < vertexSet.size(); k++) {
        cout << "Working... - " << k << "/" << this->vertexSet.size() << endl;
        int i = 0, j = 0;
        for (auto v1 : vertexSet) {
            for (auto v2 : vertexSet) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    pred[i][j] = pred[k][j];
                }
                j++;
            }
            i++;
            j = 0;
        }
    }
    floydWarshallSolved = true;
}

template<class T>
bool Graph<T>::isFloydWarshallSolved() const {
    return floydWarshallSolved;
}

template<class T>
vector<T> *Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
    vector<T> *res = new vector<T>();
    int srcIndex, destIndex;

    for (int i = 0; i < vertexSet.size(); i++) {
        if (vertexSet.at(i)->info == orig)
            srcIndex = i;
        else if (vertexSet.at(i)->info == dest)
            destIndex = i;
    }

    while (pred[srcIndex][destIndex] != vertexSet[srcIndex]) {
        res->emplace(res->begin(), pred[srcIndex][destIndex]->info);
        for (int i = 0; i < vertexSet.size(); i++) {
            if (vertexSet.at(i)->info == pred[srcIndex][destIndex]->info) {
                destIndex = i;
                break;
            }
        }
    }
    res->push_back(dest);
    res->insert(res->begin(), orig);

    return res;
}

template<class T>
void Graph<T>::dijkstraShortestPathAllPairs()
{
    dist.clear();
    dist = vector<vector<double>>(vertexSet.size(), vector<double>(vertexSet.size(), INT64_MAX));
    pred.clear();
    pred = vector<vector<Vertex<T>*>>(vertexSet.size(), vector<Vertex<T>*>(vertexSet.size(), NULL));

    int i = 0, j = 0;
    for (auto v1 : vertexSet)
    {
        for (auto v2 : vertexSet)
        {
            dist[i][j] = INT64_MAX;
            j++;
        }
        j = 0;
        i++;
    }

    i = 0, j = 0;
    for (auto v1 : vertexSet)
    {
        dijkstraShortestPath(v1->info);

        for (auto v2 : vertexSet)
        {
            if (dist[i][j] > v2->dist)
                dist[i][j] = v2->dist;

            j++;
        }
        i++;
    }
}

template<class T>
void Graph<T>::aStarShortestPathAllPairs()
{
    dist.clear();
    dist = vector<vector<double>>(vertexSet.size(), vector<double>(vertexSet.size(), INT64_MAX));
    pred.clear();
    pred = vector<vector<Vertex<T>*>>(vertexSet.size(), vector<Vertex<T>*>(vertexSet.size(), NULL));

    int i = 0, j = 0;
    for (auto v1 : vertexSet)
    {
        for (auto v2 : vertexSet)
        {
            aStarShortestPath(v1->info, v2->info);
            dist[i][j] = v2->dist;
            pred[i][j] = v2->path;

            j++;
        }
        j = 0;
        i++;
    }
}

template<class T>
bool Graph<T>::addVertex(Vertex<T> *in) {
    if ( findVertex(in->getInfo()) != NULL)
        return false;
    vertexSet.push_back(in);
    return true;
}


/** Conectivity Analisys **/
template <class T>
void Graph<T>::tarjanDfs(Vertex<T> *at) {
    stack.push(at);
    at->onStack = true;
    at->tarjanId = nextTarjanId;
    at->tarjanLowlink = nextTarjanId++;
    at->visited = true;

    for (Edge<T> edge : at->adj) {
        Vertex<T> *to = edge.getDest();
        if (!to->visited)
            tarjanDfs(to);
        if (to->onStack)
            at->tarjanLowlink = min(at->tarjanLowlink, to->tarjanLowlink);
    }

    if (at->tarjanId == at->tarjanLowlink) {
        while(true) {
            Vertex<T> *v = stack.top();
            v->onStack = false;
            v->tarjanLowlink = at->tarjanId;
            if (v->tarjanId == at->tarjanId) break;
            stack.pop();
        }
        this->sccCount++;
    }
}

template<class T>
void Graph<T>::tarjanStronglyConnectedComponents() {
    if (tarjanSolved) return;

    while (!stack.empty()) stack.pop();
    this->nextTarjanId = 0;
    this->sccCount = 0;

    for (Vertex<T> *v : this->vertexSet) {
        if (!v->visited) {
            tarjanDfs(v);
        }
    }

    while (!stack.empty()) stack.pop();

    tarjanSolved = true;
}

template<class T>
bool Graph<T>::isTarjanSolved() const {
    return tarjanSolved;
}

template<class T>
double Graph<T>::getFloydWarshallWeight(const T &origin, const T &dest) const {
    int srcIndex, destIndex;

    for (int i = 0; i < vertexSet.size(); i++) {
        if (vertexSet.at(i)->info == origin)
            srcIndex = i;
        else if (vertexSet.at(i)->info == dest)
            destIndex = i;
    }

    return dist[srcIndex][destIndex];
}

#endif /* GRAPH_H_ */