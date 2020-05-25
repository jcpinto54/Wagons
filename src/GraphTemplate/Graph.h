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
    vector<Vertex<T> *> vertexSet;    // vertex set is not oredered
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
    bool allPairsSolved = false;

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

    // Single Source Paths Algorithms
    void dijkstraShortestPath(const T &s);
    void aStarShortestPath(const T &orig, const T &destiny);
    vector<T> *getSingleSourcePathTo(const T &dest) const;
    double getSingleSourceWeightTo(const T &dest) const;

    // All Pairs Paths Functions
    void floydWarshallShortestPath();
    vector<T> *getAllPairsPath(const T &origin, const T &dest) const;
    double getAllPairsWeight(const T &origin, const T &dest) const;
    bool isAllPairsSolved() const;
    void dijkstraShortestPathAllPairs();
    void aStarShortestPathAllPairs();

    // Tarjan Algorithm / Strongly Connected Components "finder"
    void tarjanStronglyConnectedComponents();
    void tarjanDfs(Vertex<T> *at);
    bool isTarjanSolved() const;

    void resetAllPairsSolved();
    void resetTarjanSolved();
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


template<class T>
bool Graph<T>::addVertex(Vertex<T> *in) {
    if ( findVertex(in->getInfo()) != NULL)
        return false;
    vertexSet.push_back(in);
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
    // Find index of "origin" in this->vertexSet to see if this dijkstra has been solved
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

    // If this Dijkstra Algorithm has been solved do not solve it again
    if (dijkstraSolved.first && dijkstraSolved.second == originIndex) return;

    // Clear previous Single Source Paths calculations
    for (auto v : vertexSet)
    {
        v->dist = INF;
        v->path = nullptr;
    }

    // Initialize algorithm -> start processing origin vertex
    Vertex<T> *s = findVertex(origin);
    s->dist = 0;
    // Priority Queue is ordered by the minimum distance to the origin vertex
    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);

    // Similar to bfs. Adds vertices that needs processing to the priority queue (in bfs we add to a vector)
    // Cycle terminates when there are no more vertices to process
    while(!q.empty())
    {
        // Get next vertex that is "closest" to the origin
        Vertex<T> *v = q.extractMin();
        for(Edge<T> &edge : v->adj)
        {
            // Current distance to origin
            double distToOrigin = edge.dest->dist;
            // Edge "edge" destination vertex
            Vertex<T> *dest = edge.dest;
            // If the current distance to the origin + distance to the next point is less
            //  than the next vertex distance to the origin
            if(v->dist + edge.weight < dest->dist)
            {
                // Update next vertex with new best distance
                dest->dist = v->dist + edge.weight;
                dest->path = v;
                // If next vertex has not been visited yet, store it for later proccessing.
                if (distToOrigin == INF)
                    q.insert(dest);
                else
                    q.decreaseKey(dest);
            }
        }
    }
    dijkstraSolved = pair<bool, unsigned>(true, originIndex);
}

// An alternative to Dijkstra
template<class T>
void Graph<T>::aStarShortestPath(const T &orig, const T &destiny)
{
    // A* algorithm only works for Local* objects because we need to know each vertex coordinates.
    if (std::is_same<T, Local*>::value) {
        // Cast to Local *
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

        //Clear vertexSet for algorithm
        for (Vertex<T>* v : vertexSet) {
            v->path = nullptr;
            v->dist = INF;
        }

        //Find Vertexes relative to passed arguments
        Vertex<Local *> *s = findVertex(origin), *d = findVertex(dest);

        //Setting origins distance as euclidian distance to destiny
        s->dist = origin->dist(dest);

        //Create minimum priority queue to store the vertices to process
        MutablePriorityQueue<Vertex<Local *>> q;
        q.insert(s);

        while (!q.empty()) { //Only ends when queue is empty if destiny vertex not found
            // Extract minimum vertex in queue
            Vertex<Local *> *v = q.extractMin();

            // Ends when destiny vertex end
            if (v == d)
                break;

            for (auto &e : v->adj) { //Iterate through adjacent edges
                auto w = e.dest; //Vertex for edge destiny
                auto od = w->dist; //Old distance
                auto l = v->info; //Local (info of vertex)

                // h is the distance calculated with euclidean distance
                double h = v->dist - l->dist(dest) + e.weight + w->info->dist(dest);

                if (h < w->dist) { //if calculated distance (weight + euclidean distance) is smaller than current distance, replace it and path
                    w->dist = h;
                    w->path = v;

                    if (od == INF) //if old distance still same as setup, put vertex in the queue
                        q.insert(w);
                    else //if not, decrease key
                        q.decreaseKey(w);
                }
            }
        }

        Vertex<Local *> *v = d; //vertex variable intialized with destiny vertex
        double l = 0; //variable for total weight

        while (v->path != NULL) //sum the total weight of path of destiny
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
        cout << "A* algorithm only works for 'Local *' objects" << endl;
}

// Retrieves paths calculated with Dijkstra and A* algorithms
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

// Retrieves weight of the path calculated with Dijkstra and A* algorithms
template<class T>
double Graph<T>::getSingleSourceWeightTo(const T &dest) const{
    Vertex<T> *v = findVertex(dest);

    if (v == nullptr || v->dist == INF)
        return INF;

    return v->dist;
}



/**************** All Pairs Shortest Path  ***************/

// Calculates all possible paths inside a graph
template<class T>
void Graph<T>::floydWarshallShortestPath() {
    if (this->allPairsSolved) return;

    // Matrix with all pairs of distances. To access to distance from vertices i to j, simply dist[i][j]
    dist.clear();
    dist = vector<vector<double>>(vertexSet.size(), vector<double>(vertexSet.size(), INT64_MAX));
    // Matrix that stores vertices predecessors
    pred.clear();
    pred = vector<vector<Vertex<T>*>>(vertexSet.size(), vector<Vertex<T>*>(vertexSet.size(), NULL));

    // Initialize dist and pred matrix
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
                // If a path from vertices i to j that goes through k is shorter than just going from i to j directly
                //  then update dist and pred Matrices
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
    allPairsSolved = true;
}

// Alternative to Floyd Warshall Algorithm. Calculate A* algorithm for all pairs of vertices
template<class T>
void Graph<T>::aStarShortestPathAllPairs()
{
    if (this->allPairsSolved) return;

    // Clear dist and pred matrices. Explained in Floyd Warshall
    dist.clear();
    dist = vector<vector<double>>(vertexSet.size(), vector<double>(vertexSet.size(), INT64_MAX));
    pred.clear();
    pred = vector<vector<Vertex<T>*>>(vertexSet.size(), vector<Vertex<T>*>(vertexSet.size(), NULL));

    int i = 0, j = 0;
    for (auto v1 : vertexSet)
    {
        cout << "Working... - " << i << "/" << this->vertexSet.size() << endl;
        for (auto v2 : vertexSet)
        {
            // For each pair of vertices compute A* algorithm
            aStarShortestPath(v1->info, v2->info);
            // Store results
            dist[i][j] = v2->dist;
            pred[i][j] = v2->path;

            j++;
        }
        j = 0;
        i++;
    }

    allPairsSolved = true;
}

// This function is not working properly
template<class T>
void Graph<T>::dijkstraShortestPathAllPairs()
{
    if (this->allPairsSolved) return;

    // Clear dist and pred matrices. Explained in Floyd Warshall
    dist.clear();
    dist = vector<vector<double>>(vertexSet.size(), vector<double>(vertexSet.size(), INT64_MAX));
    pred.clear();
    pred = vector<vector<Vertex<T>*>>(vertexSet.size(), vector<Vertex<T>*>(vertexSet.size(), NULL));


    int i = 0, j = 0;
    for (auto v1 : vertexSet)
    {
        // Compute dijkstra one time per line of matrices
        dijkstraShortestPath(v1->info);
        for (auto v2 : vertexSet)
        {
            // Store results
            if (dist[i][j] > v2->dist)
            {
                dist[i][j] = v2->dist;
                pred[i][j] = v2->path;
            }
            j++;
        }
        i++;
    }

    allPairsSolved = true;
}


// After preprocessing the graph with an "All Pairs" algorithm call this function to get a path
template<class T>
vector<T> *Graph<T>::getAllPairsPath(const T &orig, const T &dest) const{
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

// After preprocessing the graph with an "All Pairs" algorithm call this function to get a weight of a path
template<class T>
double Graph<T>::getAllPairsWeight(const T &origin, const T &dest) const {
    int srcIndex, destIndex;

    for (int i = 0; i < vertexSet.size(); i++) {
        if (vertexSet.at(i)->info == origin)
            srcIndex = i;
        else if (vertexSet.at(i)->info == dest)
            destIndex = i;
    }

    return dist[srcIndex][destIndex];
}

template<class T>
bool Graph<T>::isAllPairsSolved() const {
    return allPairsSolved;
}

template<class T>
void Graph<T>::resetAllPairsSolved() {
    this->allPairsSolved = false;
}

/******************** Conectivity Analisys ********************/

// Tarjan Algortihm. Detects strongly connected components
template<class T>
void Graph<T>::tarjanStronglyConnectedComponents() {
    if (tarjanSolved) return;

    // Clear stack (just to be sure) and initializes vairables
    // stack is used to keep track of our graph processing
    while (!stack.empty()) stack.pop();
    // nextTarjanId is used to give an independent ID from the other algorithms to the vertices
    this->nextTarjanId = 0;
    // sscCount is the number of strongly connected components
    this->sccCount = 0;

    // Execute tarjanDfs for all (unvisited) vertices. Since tarjanDfs uses recursion, all vertices will be processed.
    for (Vertex<T> *v : this->vertexSet) {
        if (!v->visited) {
            tarjanDfs(v);
        }
    }

    // Clear stack again
    while (!stack.empty()) stack.pop();

    tarjanSolved = true;
}

// Auxiliary function to tarjan algorithm
template <class T>
void Graph<T>::tarjanDfs(Vertex<T> *at) {
    // Initialize processing
    stack.push(at);
    at->onStack = true;
    // Give independent ID
    at->tarjanId = nextTarjanId;
    // tarjanLowLink will be the smallest ID of a vertex that this vertex can reach
    //  This will be used to tell what SCC a vertex is part of.
    at->tarjanLowlink = nextTarjanId++;
    at->visited = true;

    // For each edge
    for (Edge<T> edge : at->adj) {
        // Get its Destination
        Vertex<T> *to = edge.getDest();
        // If it hasn't been visited, then visit it
        if (!to->visited)
            tarjanDfs(to);
        // On backtrack or if simply the vertex has been visited
        if (to->onStack)
            // See if you can lower your lowlink
            at->tarjanLowlink = min(at->tarjanLowlink, to->tarjanLowlink);
    }

    // When you processed all nodes of a strongly connected component
    if (at->tarjanId == at->tarjanLowlink) {
        // Clear stack until you reach the node you are processing and update lowlinks
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
bool Graph<T>::isTarjanSolved() const {
    return tarjanSolved;
}

template<class T>
void Graph<T>::resetTarjanSolved() {
    this->tarjanSolved = false;

}

#endif /* GRAPH_H_ */