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
        mutable int searchBatch = 0;


        /**
         * 为一个节点生成距离缓存, 会设置 node 的 batch 和 distance 字段, 将 inQueue 字段设置为 false.
         * @note 由于 node 存放索引值, 所以需要 ptVec 参数.
         * @param ptVec 向量容器
         * @param node 要计算距离缓存的节点
         * @param target 目标向量, 计算节点所代表的向量和其的距离.
         */
        void generateBufferForNode(const Vec<point::Point> &ptVec,
                                   const GraphNode &node,
                                   const point::Point &target) const;

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
         * 把 nearest 向量拓展为 topk.
         * @param ptVec 向量容器
         * @param startPtIdx 拓展的起始向量在向量容器中的索引值, 如果此索引值不在此 Layer 中, 返回空数组.
         * @param target 目标向量
         * @param k topK 中的 K, 即查找的最近向量数量.
         * @return 返回最接近向量在向量容器中的索引值, 元素有 k 个, 另一种情况见 startPtIdx.
         */
        Vec<int> expandTopK(const Vec<point::Point> &ptVec,
                            int startPtIdx,
                            const point::Point &target,
                            int k) const;

        /**
         * 查找层内和目标向量最接近的向量.
         * @param ptVec 向量容器
         * @param startPtIdx 搜索的起始向量在向量容器中的索引值, 如果此索引值不在此 Layer 中, 返回 -1.
         * @param target 目标向量
         * @return 返回最接近向量在向量容器中的索引值, 另一种情况见 startPtIdx.
         */
        int searchNearestNode(const Vec<point::Point> &ptVec,
                              int startPtIdx,
                              const point::Point &target) const;

        /**
         * 改变搜索批次, 使所有节点的距离缓冲都失效.
         * @note 需要在目标搜索向量发生改变 或 inQueue 字段针对的队列发生变化 的时候调用.
         */
        void changeSearchBatch() const;
    };
} // hnsw

#endif //LAYER_H
