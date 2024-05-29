//
// Created by azazo1 on 2024/5/29.
//

#include "GraphNode.h"

namespace hnsw {
    GraphNode::GraphNode(const int pointIdx): pointIdx(pointIdx) {
    }

    void GraphNode::link(const int otherIdx) {
        links.push_back_unique(otherIdx);
    }
} // hnsw
