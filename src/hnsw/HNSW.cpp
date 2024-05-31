//
// Created by azazo1 on 2024/5/29.
//

#include "HNSW.h"

namespace hnsw {
    HNSW::HNSW(const vec::Vec<point::Point> *ptVec): ptVec(ptVec) {
        buildLayers();
    }

    void HNSW::buildLayers() {
        const int len = ptVec->getLength();
        for (int i = 0; i < len; i++) {
            // 注意这里也要改变搜索批次
            layer3.changeSearchBatch();
            layer2.changeSearchBatch();
            layer1.changeSearchBatch();
            layer0.changeSearchBatch();
#ifdef DEBUG_HNSW_BUILD_LAYERS
            if (i % 3 == 0) {
                printf("Building...now: %d, tol: %d\n", i, len);
            }
#endif
            layer0.emplaceNode(*ptVec, i);
            layer1.emplaceNode(*ptVec, i);
            layer2.emplaceNode(*ptVec, i);
            layer3.emplaceNode(*ptVec, i);
        }
    }

    vec::Vec<int> HNSW::searchNearestTopK(const point::Point &target, const int k) const {
        layer3.changeSearchBatch();
        layer2.changeSearchBatch();
        layer1.changeSearchBatch();
        layer0.changeSearchBatch();

        // 先在最稀疏的 layer3 中搜索.
        int nearestPt = layer3.searchNearestNode(*ptVec, 0, target); // 只需要一个
        nearestPt = layer2.searchNearestNode(*ptVec, nearestPt, target);
        nearestPt = layer1.searchNearestNode(*ptVec, nearestPt, target);
        nearestPt = layer0.searchNearestNode(*ptVec, nearestPt, target);

        layer0.changeSearchBatch(); // inQueue 针对的队列发生变化
        return layer0.expandTopK(*ptVec, nearestPt, target, k); // 搜索到最接近的向量后拓展为 topk
    }
} // hnsw
