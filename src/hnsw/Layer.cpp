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
        Vec<int> nearest = expandTopK(ptVec, 0, ptVec[pointIdx], N_LINK);
        for (const int i: nearest) {
            // 建立双向连接
            (*this)[i].link(nodeIdx);
            (*this)[nodeIdx].link(nodeIdx);
        }
    }

    vec::Vec<int> Layer::expandTopK(const Vec<point::Point> &ptVec, int startPtIdx,
                                    const point::Point &target, int k) const {
        // todo 突然发现不对, 如果 k == 1, 假设极端情况 所有点连成一条线, 那么搜索会提前结束, 并不能找到最近的点.
        // todo 按照注释重写此方法.
        searchBatch++; // 更新批次计数
        if (length <= k) {
            //  Layer 节点数不够, 不搜索了, 直接返回
            // 排序
            pq::PriorityQueue<GraphNode *> pq(length, compareNode);
            for (GraphNode &node: *this) {
                pq.add(&node);
            }
            // 倒序提取
            Vec<int> rst(length);
            for (int i = length - 1; i >= 0; i--) {
                rst[i] = pq.pop()->pointIdx;
            }
            return rst;
        }
        // Layer 节点数足够, 取前 k 个节点直接放进 pq 然后开始搜索.
        pq::PriorityQueue<GraphNode *> pq(compareNode);
        for (int i = 0; i < k; i++) {
            generateBufferForNode(ptVec, arr[i], target); // 生成计算缓冲.
            arr[i].inQueue = true;
            pq.add(arr + i); // 以第 0 个节点作为开始节点.
        }
        bool added = true;
        // 没有新节点被添加进 pq 时, 说明搜索完毕.
        while (pq.getSize() != 0 && added) {
            GraphNode *node = pq.pop();
            node->inQueue = false;
            const int size = node->links.size();
            added = false;
            for (int i = 0; i < size; i++) {
                // 遍历每个邻居
                int neighborIdx;
                node->links.retrieve(i, neighborIdx); // todo 优化这个, 造个迭代器
                GraphNode &neighbor = arr[neighborIdx];
                // 接下来, 如果 neighbor 不在 pq 中, 对 neighbor 生成缓冲, 进行判断:
                // 只有 neighbor 的 distance 小于 node 时才把 neighbor 添加进 pq.
                if (neighbor.batch != searchBatch) {
                    // 此次搜索没有涉及过此邻居, 邻居中的缓冲值都是无效的.
                    generateBufferForNode(ptVec, neighbor, target);
                }
                if (!neighbor.inQueue && neighbor.distance < node->distance) {
                    neighbor.inQueue = true;
                    pq.add(&neighbor);
                    added = true;
                }
            }
        }
        // 取 distance 最小的 k 个, 倒序存放在 Vec 中, 忽略前面的节点.
        while (pq.getSize() > k) {
            pq.pop();
        }
        Vec<int> rst(k);
        for (int i = 0; i < k; i++) {
            rst[i] = pq.pop()->pointIdx;
        }
        return rst;
    }

    void Layer::generateBufferForNode(const Vec<point::Point> &ptVec, GraphNode &node,
                                      const point::Point &target) const {
        node.batch = searchBatch;
        node.distance = ptVec[node.pointIdx].distanceTo(target);
        node.inQueue = false;
    }
} // hnsw
