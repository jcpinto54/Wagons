//
// Created by joao on 16/05/20.
//

#include "Map.h"
#include "Utilities/utils.h"


Map::Map(unordered_map<unsigned int, Vertex<Local *> *> &map) {
    for (auto & pair : map) {
        this->graph.addVertex(pair.second);
    }

    int width = (Local::getMaxX() - Local::getMinX()) + 50;
    int height = (Local::getMaxY() - Local::getMinY()) + 50;

    this->graphViewer = new GraphViewer(width, height, false);
}

void Map::viewGraph() {
    graphViewer->createWindow(graphViewer->getWidth(), graphViewer->getHeight());

    for (auto vertex : graph.getVertexSet()) {
        graphViewer->addNode(vertex->getInfo()->getId(), vertex->getInfo()->getX(), vertex->getInfo()->getY());
        switch (vertex->getInfo()->getTag()) {
            case Tag::COURT:
                graphViewer->setVertexIcon(vertex->getInfo()->getId(), "../data/court.png");
                break;
            case Tag::POLICE:
                graphViewer->setVertexIcon(vertex->getInfo()->getId(), "../data/police.png");
                break;
            case Tag::PRISON:
                graphViewer->setVertexIcon(vertex->getInfo()->getId(), "../data/prison.png");
                break;
            case Tag::HQ:
                graphViewer->setVertexIcon(vertex->getInfo()->getId(), "../data/high.png");
                break;
        }
    }
    int i = 0;
    for (auto vertex : graph.getVertexSet()) {
        for (auto edge : vertex->getAdj()) {
            graphViewer->addEdge(i, vertex->getInfo()->getId(), edge.getDest()->getInfo()->getId(), EdgeType::UNDIRECTED);
            i++;
        }
    }

    graphViewer->rearrange();
}

Map::Map() {}
