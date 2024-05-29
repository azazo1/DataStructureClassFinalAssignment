//
// Created by azazo1 on 2024/5/29.
//

#include "Layer.h"

namespace hnsw {
    Layer::Layer(int dk): dk(dk) {
    }

    void Layer::emplaceNode(const Vec<point::Point> &ptVec, const int pointIdx) {
        if (pointIdx % dk != 0) {
            return;
        }
        const GraphNode node(pointIdx);
        push_back(node); // 此处发生了拷贝, 因此下方以左值使用 node 将会是错误的.
        const int nodeIdx = length;
        Vec<int> nearest = searchNearestNode(ptVec[pointIdx], ptVec, N_LINK);
        for (const int i: nearest) {
            // 建立双向连接
            (*this)[i].link(nodeIdx);
            (*this)[nodeIdx].link(nodeIdx);
        }
    }
} // hnsw
