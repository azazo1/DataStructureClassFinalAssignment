//
// Created by azazo1 on 2024/5/29.
//

#ifndef LAYER_H
#define LAYER_H
#include "GraphNode.h"
#include "../vec/Vec.h"
#include "../point/Point.h"

namespace hnsw {
    class Layer : public vec::Vec<GraphNode> {
        int dk;

    public:
        /**
         * 创建一个各个节点相互连接的向量节点层.
         * 相距系数是指在向量容器中每 dk 个向量就有一个向量被添加到磁层中.
         * @param dk 相距系数
         */
        explicit Layer(int dk);

        /**
         * 尝试在层中添加一个节点.
         * @note 参数不是直接传进 GraphNode, 而是在内部创建, 所以叫 emplace.
         * @note Layer 会自己决定是否要把此向量添加到层中, 因素是 dk.
         * @param ptVec 向量容器
         * @param pointIdx 要添加的节点在向量容器中的索引
         */
        void emplaceNode(const Vec<point::Point> &ptVec, int pointIdx);

        /**
         * 查找和目标向量最接近的向量.
         * @param point 目标向量
         * @param ptVec 向量容器
         * @param k topK 中的 K, 即最近向量数量.
         * @return 返回最接近向量在向量容器中的索引值, 元素有 k 个.
         */
        Vec<int> searchNearestNode(const point::Point &point,
                                   const Vec<point::Point> &ptVec,
                                   int k) const;
    };
} // hnsw

#endif //LAYER_H
