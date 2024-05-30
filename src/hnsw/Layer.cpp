//
// Created by azazo1 on 2024/5/29.
//

#include "Layer.h"

#include "../pq/PriorityQueue.h"

namespace hnsw {
    Layer::Layer(const int dk): dk(dk) {
    }

    void Layer::emplaceNode(const Vec<point::Point> &ptVec, const int pointIdx) {
        if (pointIdx % dk != 0) {
            return;
        }
        const GraphNode node(pointIdx);
        push_back(node); // 此处发生了拷贝, 因此下方以左值使用 node 将会是错误的.
        const int nodeIdx = length;
        Vec<int> nearest = searchNearestNodeTopK(ptVec, ptVec[pointIdx], N_LINK);
        for (const int i: nearest) {
            // 建立双向连接
            (*this)[i].link(nodeIdx);
            (*this)[nodeIdx].link(nodeIdx);
        }
    }

    vec::Vec<int> Layer::searchNearestNodeTopK(
        const Vec<point::Point> &ptVec,
        const point::Point &target, int k) const {
        if (length == 0) {
            throw "empty";
        }
        pq::PriorityQueue<GraphNode *> pq(compareNode);
        generateBufferForNode(ptVec, arr[0], target); // 生成计算缓冲.
        pq.add(arr + 0); // 以第 0 个节点作为开始节点.
        while (pq.getSize() != 0) {
            GraphNode *node;
            // /*
            //  * 我发现一个问题, 如果 pq 大小小于 k, 那么虽然直接把 neighbor 添加进来了, node 却出去了,
            //  * 如果 node 是最接近的节点, 那么就会有点问题.
            //  * 于是在 pq 大小小于 k 时, 不弹出 node, 给一点前期积累的机会.
            //  */
            // if (pq.getSize() < k) {
                // node = pq.get(); // 不行, 这样子的话, 第二次获取可能还是这个节点.
            // } else {
                // node = pq.pop();
            // } // 以上思路是错的
            const int size = node->links.size();
            for (int i = 0; i < size; i++) {
                // 遍历每个邻居
                int neighborIdx;
                node->links.retrieve(i, neighborIdx);
                GraphNode &neighbor = arr[neighborIdx];
                // 接下来, 如果 neighbor 不在 pq 中, 对 neighbor 生成缓冲, 进行判断:
                // 如果 pq 的大小小于 k, 直接把 neighbor 添加进 pq.
                // 否则如果 pq 大小大于等于 k, 则只有 neighbor 的 distance 小于 node 时才把 neighbor 添加进 pq.
            }
        }
        searchBatch++; // 更新批次计数
        // todo 取最靠近的 k 个, 倒序存放在 Vec 中, 忽略前面的节点.
    }

    void Layer::generateBufferForNode(const Vec<point::Point> &ptVec, GraphNode &node,
                                      const point::Point &target) const {
        node.batch = searchBatch;
        node.distance = ptVec[node.pointIdx].distanceTo(target);
        node.inQueue = false;
    }
} // hnsw
