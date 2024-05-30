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
        const int nearest = searchNearestNode(ptVec, 0, ptVec[pointIdx]);
        Vec<int> nearestTopK = expandTopK(ptVec, nearest, ptVec[pointIdx], N_LINK);
        for (const int i: nearestTopK) {
            // 建立双向连接
            (*this)[i].link(nodeIdx);
            (*this)[nodeIdx].link(nodeIdx);
        }
    }

    vec::Vec<int> Layer::expandTopK(const Vec<point::Point> &ptVec, const int startPtIdx,
                                    const point::Point &target, const int k) const {
        if (startPtIdx % dk != 0) {
            return Vec<int>();
        }
        if (length <= k) {
            // Layer 节点数不够, 不拓展了, 直接返回
            // 排序
            pq::PriorityQueue<GraphNode *> pq(length, compareNode);
            for (GraphNode &node: *this) {
                if (node.batch != searchBatch) {
                    generateBufferForNode(ptVec, node, target);
                }
                pq.add(&node);
            }
            // 倒序提取
            Vec<int> rst(length);
            rst.setLength(length);
            for (int i = length - 1; i >= 0; i--) {
                rst[i] = pq.pop()->pointIdx;
            }
            return rst;
        }
        // Layer 节点数足够, 开始拓展节点, 起始这里有点像广度优先搜索.
        pq::PriorityQueue<GraphNode *> rstQueue(k, compareNode); // 为了保持顺序, 用的是优先级队列, 而不是链表.
        mylinklist::LinkedList<GraphNode *> queue;
        GraphNode *node = arr + startPtIdx / dk;

        if (node->batch != searchBatch) {
            generateBufferForNode(ptVec, *node, target);
        }
        node->inQueue = true; // 此处 inQueue 字段表示此节点是否已经被 queue 记录.
        queue.push_back(node);

        while (rstQueue.getSize() + queue.size() < k) {
            node = queue.pop_head(); // 节点都已经缓存过.
            rstQueue.add(node);

            // 遍历所有邻居.
            const int size = node->links.size();
            for (int i = 0; i < size; i++) {
                // todo 优化, 使用迭代器
                int neighborIdx;
                node->links.retrieve(i, neighborIdx);
                GraphNode *neighbor = arr + neighborIdx;

                // 只有邻居没被搜索过时才加入.
                if (neighbor->batch != searchBatch) {
                    generateBufferForNode(ptVec, *neighbor, target);
                }
                if (!neighbor->inQueue) {
                    neighbor->inQueue = true;
                    queue.push_back(neighbor); // 插入到 queue 的节点都要保证已经缓存过.
                }
                if (queue.size() + rstQueue.getSize() == k) {
                    break;
                }
            }
        }
        while (!queue.empty()) {
            rstQueue.add(queue.pop_head());
        }
        // 拓展完毕, rstQueue 中的节点数一定是 k.
        Vec<int> rst(k);
        rst.setLength(k);
        for (int i = k - 1; i >= 0; i--) {
            rst[i] = rstQueue.pop()->pointIdx;
        }
        return rst;
    }

    void Layer::generateBufferForNode(const Vec<point::Point> &ptVec, GraphNode &node,
                                      const point::Point &target) const {
        node.batch = searchBatch;
        node.distance = ptVec[node.pointIdx].distanceTo(target);
        node.inQueue = false;
    }

    int Layer::searchNearestNode(const Vec<point::Point> &ptVec, const int startPtIdx,
                                 const point::Point &target) const {
        // 起始向量不在此层.
        if (startPtIdx % dk != 0) {
            return -1;
        }
        GraphNode *node = arr + startPtIdx / dk;
        GraphNode *newNode = node;
        while (newNode != nullptr) {
            node = newNode;
            newNode = nullptr;
            const int neighborCnt = node->links.size();
            for (int i = 0; i < neighborCnt; i++) {
                // 遍历每个邻居
                int neighborIdx;
                node->links.retrieve(i, neighborIdx);
                GraphNode &neighbor = arr[neighborIdx];
                if (neighbor.batch != searchBatch) {
                    generateBufferForNode(ptVec, neighbor, target);
                }
                // 找到 distance 更小的邻居
                if (neighbor.distance < node->distance) {
                    // 找到 distance 最小的邻居
                    if (newNode == nullptr || neighbor.distance < newNode->distance) {
                        newNode = &neighbor;
                    }
                }
            }
        }
        return node->pointIdx;
    }

    void Layer::changeSearchBatch() const {
        searchBatch++;
    }
} // hnsw
