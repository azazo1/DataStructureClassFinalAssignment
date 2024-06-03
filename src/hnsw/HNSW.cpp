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
            int searchStartIdx = layer3.emplaceNode(*ptVec, i);
            searchStartIdx = layer2.emplaceNode(*ptVec, i, searchStartIdx);
            searchStartIdx = layer1.emplaceNode(*ptVec, i, searchStartIdx);
            layer0.emplaceNode(*ptVec, i, searchStartIdx);
        }
    }

    vec::Vec<int> HNSW::searchNearestTopK(const point::Point &target, const int k) const {
        layer3.changeSearchBatch();
        layer2.changeSearchBatch();
        layer1.changeSearchBatch();
        layer0.changeSearchBatch();

        // 先在最稀疏的 layer3 中搜索.
        int nearestPt = layer3.searchNearestNode(*ptVec, 0, target);
        nearestPt = layer2.searchNearestNode(*ptVec, nearestPt, target);
        nearestPt = layer1.searchNearestNode(*ptVec, nearestPt, target);
        nearestPt = layer0.searchNearestNode(*ptVec, nearestPt, target);

        // 与上面四行做对比实验, 发现在 8000 数据集下对速度的影响不大.
        // int nearestPt = layer0.searchNearestNode(*ptVec, 0, target);

        layer0.changeSearchBatch(); // inQueue 针对的队列发生变化
        return layer0.searchKNN(*ptVec, nearestPt, target, k); // 搜索到最接近的向量后拓展为 topk
    }

    void HNSW::queryLinks(const point::Point *distanceReference) const {
#ifndef DEBUG_HNSW_QUERY_LINKS
        throw NotDebuggingException();
#endif
        int ptIdx;
        while (scanf("%d", &ptIdx) == 1) {
            double distance;
            if (distanceReference != nullptr) {
                distance = distanceReference->distanceTo(ptVec->operator[](ptIdx));
            } else {
                distance = 0;
            }
            printf("%5d(%.2f) links(distance): ", ptIdx, distance);
            for (mylinklist::Iterator<int> i = layer0[ptIdx].links.getIter(); i.hasNext();) {
                const int neighborIdx = i.next();
                printf("%5d(%.2f) ", neighborIdx,
                       ptVec->operator[](neighborIdx).distanceTo(
                           distanceReference != nullptr
                               ? *distanceReference
                               : ptVec->operator[](ptIdx)));
            }
            printf("\n");
        }
    }
} // hnsw
