//
// Created by azazo1 on 2024/5/29.
//

#include "GraphNode.h"

#ifdef DEBUG_GRAPH_NODE_BUFFER
#include <iostream>
#endif

namespace hnsw {
    GraphNode::GraphNode(const int pointIdx): pointIdx(pointIdx) {
    }

    void GraphNode::link(const int otherIdx) {
        links.push_back_unique(otherIdx);
    }

    bool compareNode(const GraphNode *n1, const GraphNode *n2) {
#ifdef DEBUG_GRAPH_NODE_BUFFER
        if (n1->distance == -1 || n2->distance == -1 || n1->batch != n2->batch) {
            std::cerr << "Node Empty Buffer" << std::endl;
            exit(1000);
        }
#endif
        return n1->distance < n2->distance;
    }

    bool compareNode(GraphNode *n1, GraphNode *n2) {
#ifdef DEBUG_GRAPH_NODE_BUFFER
        if (n1->distance == -1 || n2->distance == -1 || n1->batch != n2->batch) {
            std::cerr << "Node Empty Buffer" << std::endl;
            exit(1000);
        }
#endif
        return n1->distance < n2->distance;
    }
    bool compareNodeInverse(const GraphNode *n1, const GraphNode *n2) {
#ifdef DEBUG_GRAPH_NODE_BUFFER
        if (n1->distance == -1 || n2->distance == -1 || n1->batch != n2->batch) {
            std::cerr << "Node Empty Buffer" << std::endl;
            exit(1000);
        }
#endif
        return n1->distance > n2->distance;
    }

    bool compareNodeInverse(GraphNode *n1, GraphNode *n2) {
#ifdef DEBUG_GRAPH_NODE_BUFFER
        if (n1->distance == -1 || n2->distance == -1 || n1->batch != n2->batch) {
            std::cerr << "Node Empty Buffer" << std::endl;
            exit(1000);
        }
#endif
        return n1->distance > n2->distance;
    }
} // hnsw
