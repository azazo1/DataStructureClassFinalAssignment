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
        const int nodeIdx = length; // 此处 length 应该等于 pointIdx / dk
#ifdef DEBUG_LAYER_EMPLACE_NODE
        printf("DK: %d, Emplacing GraphNode: %d\n", dk, pointIdx);
#endif
        push_back(node); // 此处发生了拷贝, 因此下方以左值使用 node 将会是错误的,
        // ↑ 此处直接添加到 layer 里可以防止 searchNearestNode 遇到空的情况, 同时此节点又与其他节点没有连接, 不会使 nearest 和 nodeIdx 相同.
        const int nearest = searchNearestNode(ptVec, 0, ptVec[pointIdx]);
        changeSearchBatch();
        Vec<int> nearestTopK = searchKNN(ptVec, nearest, ptVec[pointIdx], N_LINK);
        for (const int i: nearestTopK) {
            // 建立双向连接
            if (i / dk != nodeIdx) {
                // 不与自己建立连接
                (*this)[i / dk].link(nodeIdx);
                (*this)[nodeIdx].link(i / dk);
#ifdef DEBUG_LAYER_EMPLACE_NODE
                printf("GraphNode Link: %d-%d, Distance: %.2f\n",
                       i,
                       nodeIdx,
                       ptVec[i].distanceTo(ptVec[pointIdx]));
#endif
            }
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
            pq::PriorityQueue<const GraphNode *> pq(length, compareNode);
            for (int i = 0; i < length; i++) {
                const GraphNode &node = arr[i];
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
            for (mylinklist::Iterator<int> i = node->links.getIter(); i.hasNext();) {
                const int neighborIdx = i.next();
                // const int size = node->links.size();
                // for (int i = 0; i < size; i++) {
                // int neighborIdx;
                // node->links.retrieve(i, neighborIdx);
                GraphNode *neighbor = arr + neighborIdx;
                // 只有邻居没被搜索过时才加入.
                if (neighbor->batch != searchBatch) {
                    generateBufferForNode(ptVec, *neighbor, target);
                }
                if (!neighbor->inQueue) {
                    neighbor->inQueue = true;
                    queue.push_back(neighbor); // 插入到 queue 的节点都要保证已经缓存过.
                }
            }
        }
        while (!queue.empty()) {
            rstQueue.add(queue.pop_head());
        }
        while (rstQueue.getSize() > k) {
            rstQueue.pop();
        }
        // 拓展完毕, rstQueue 中的节点数一定是 k.
        Vec<int> rst(k);
        rst.setLength(k);
        for (int i = k - 1; i >= 0; i--) {
            rst[i] = rstQueue.pop()->pointIdx;
        }
        return rst;
    }

    vec::Vec<int> Layer::searchKNN(const Vec<point::Point> &ptVec, const int startPtIdx,
                                   const point::Point &target, const int k) const {
        if (startPtIdx % dk != 0) {
            return Vec<int>();
        }
        if (length <= k) {
            // Layer 节点数不够, 不搜索了, 直接返回
            // 排序
            pq::PriorityQueue<const GraphNode *> pq(length, compareNodeInverse);
            // get ↑ 方法取出的是距离最近的节点.
            for (int i = 0; i < length; i++) {
                const GraphNode &node = arr[i];
                if (node.batch != searchBatch) {
                    generateBufferForNode(ptVec, node, target);
                }
                pq.add(&node);
            }
            Vec<int> rst(length);
            while (pq.getSize() > 0) {
                rst.push_back(pq.pop()->pointIdx);
            }
            return rst;
        }
        pq::PriorityQueue<const GraphNode *> topQueue(2 * k, compareNode);
        pq::PriorityQueue<const GraphNode *> candidateQueue(2 * k, compareNodeInverse);
        const GraphNode *node = arr + startPtIdx / dk;
        if (node->batch != searchBatch) {
            generateBufferForNode(ptVec, *node, target);
        }
        double lowerBound = node->distance;
        topQueue.add(node);
        candidateQueue.add(node);
        node->inQueue = true; // 此处 inQueue 表示是否被添加过到 topQueue(结果队列) 中, 即是否被访问过(visited).
        while (candidateQueue.getSize() != 0) {
            node = candidateQueue.pop(); // 取得一个距离最近的候选人.
            if (node->distance > lowerBound) {
                // 最近的候选人节点都比 topQueue 中最远的节点要远, 故结束循环.
                break;
            }
            // 遍历每个邻居.
            for (mylinklist::Iterator<int> i = node->links.getIter(); i.hasNext();) {
                const int neighborIdx = i.next();
                const GraphNode &neighbor = arr[neighborIdx];
                if (neighbor.batch != searchBatch) {
                    generateBufferForNode(ptVec, neighbor, target);
                }
                if (!neighbor.inQueue) {
                    neighbor.inQueue = true;
                    // 判断邻居是否要添加进 topQueue.
                    if (topQueue.getSize() < k || neighbor.distance < lowerBound) {
                        topQueue.add(&neighbor);
                        candidateQueue.add(&neighbor);

                        // 添加了节点, 所以要限制 topQueue 数量.
                        while (topQueue.getSize() > k) {
                            topQueue.pop(); // 删除距离最远的节点.
                        }
                        lowerBound = topQueue.get()->distance;
                    }
                }
            }
        }
        Vec<int> rst(k);
        rst.setLength(k);
        for (int i = k - 1; i >= 0; i--) {
            rst[i] = topQueue.pop()->pointIdx;
        }
        return rst;
    }

    void Layer::generateBufferForNode(const Vec<point::Point> &ptVec, const GraphNode &node,
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
        if (node->batch != searchBatch) {
            generateBufferForNode(ptVec, *node, target);
        }
        GraphNode *newNode = node;
#ifdef DEBUG_LAYER_SEARCH_NEAREST_STEP
        printf("Search Step:");
#endif
        while (newNode != nullptr) {
#ifdef DEBUG_LAYER_SEARCH_NEAREST_STEP
            printf(" %5d(%.2f)", newNode->pointIdx, newNode->distance);
#endif
            node = newNode;
            newNode = nullptr;
            // 遍历每个邻居
            for (mylinklist::Iterator<int> i = node->links.getIter(); i.hasNext();) {
                const int neighborIdx = i.next();
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
#ifdef DEBUG_LAYER_SEARCH_NEAREST_STEP
        printf("\n");
#endif
        return node->pointIdx;
    }

    void Layer::changeSearchBatch() const {
        searchBatch++;
    }
} // hnsw
