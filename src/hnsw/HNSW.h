//
// Created by azazo1 on 2024/5/29.
//

#ifndef HNSW_H
#define HNSW_H
#include "Layer.h"
#include "../vec/Vec.h"

#define DK_LAYER1 5
#define DK_LAYER2 50
#define DK_LAYER3 300

namespace hnsw {
    class HNSW {
        const vec::Vec<point::Point> *ptVec;
        /**
         * 每个向量都要在 layer0 中创建节点, layer1/2/3 中是每 DK_LAYER1/2/3 个向量放一个节点.
         * @note 靠后的 layer 中的节点必须要在前面的节点中出现, 以便不同层次间的切换, 此需要靠 DK_LAYER1/2/3 的值来保证
         *      此时后面层的节点如果代表 pointIdx 向量索引位置的节点, 则可以通过 pointIdx / DK_LAYER1 * DK_LAYER2
         *      (假设后面层是 layer2, 前面的层是 layer1) 得到前一层的表示 pointIdx 的节点的索引值.
         */
        Layer layer0 = Layer(1);
        /**
         * 见 @code layer0
         */
        Layer layer1 = Layer(DK_LAYER1);
        /**
         * 见 @code layer0
         */
        Layer layer2 = Layer(DK_LAYER2);
        /**
         * 见 @code layer0
         */
        Layer layer3 = Layer(DK_LAYER3);

        /**
         * 用向量容器中的值来构建所有的层.
         */
        void buildLayers();

    public:
        /**
         * @param ptVec 所有向量的容器
         */
        HNSW(const vec::Vec<point::Point> *ptVec);

        /**
         * 找到和目标向量最接近的向量.
         * @param target 目标向量
         * @return 最接近的向量在向量容器中的索引值, 元素数有 k 个.
         */
        vec::Vec<int> searchNearestTopK(const point::Point &target, int k) const;
    };
} // hnsw

#endif //HNSW_H
