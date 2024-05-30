//
// Created by azazo1 on 2024/5/29.
//

#include "HNSW.h"

#include <algorithm>

namespace hnsw {
    HNSW::HNSW(const vec::Vec<point::Point> *ptVec): ptVec(ptVec) {
        buildLayers();
    }

    void HNSW::buildLayers() {
        for (int i = 0; i < ptVec->getLength(); i++) {
            layer0.emplaceNode(*ptVec, i);
            layer1.emplaceNode(*ptVec, i);
            layer2.emplaceNode(*ptVec, i);
            layer3.emplaceNode(*ptVec, i);
        }
    }

    vec::Vec<int> HNSW::searchNearestTopK(const point::Point &target, int k) {
        // 先在最稀疏的 layer3 中搜索.
        int nearestPt = layer3.searchNearestNode(*ptVec, 0, target); // 只需要一个
        nearestPt = layer2.searchNearestNode(*ptVec, nearestPt, target);
        nearestPt = layer1.searchNearestNode(*ptVec, nearestPt, target);
        nearestPt = layer0.searchNearestNode(*ptVec, nearestPt, target);
        return layer0.expandTopK(*ptVec, nearestPt, target, k); // 搜索到最接近的向量后拓展为 topk
    }
} // hnsw
