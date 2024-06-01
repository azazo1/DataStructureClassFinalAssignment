//
// Created by azazo1 on 2024/5/29.
//

#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include "../linkedlist/LinkedList.h"

/**
 * 每个节点的最大主动连接数, 在节点加入到 Graph 中, 每个节点查找与其最近的 N 个节点,
 * 并与其建立连接.
 * @note 此值不代表 links 列表中元素个数最大值, 在后续节点加入 Graph 时, 节点可能被动拓展连接.
 * @note 个人大致测试了一下, 觉得 10 比较好.
 */
#define N_LINK 10

namespace hnsw {
    struct GraphNode {
        /**
         * 用于记录缓存的距离所属的搜索批次.
         */
        mutable int batch = -1;
        /**
         * 用于记录缓存的距离.
         */
        mutable double distance = -1;
        /**
         * 用来标志此节点是否在队列中, 在后来此值的意义改变,
         * 变成了是否在搜索过程中探索了此节点, 也就是说此变量可以和 batch 融合,
         * batch == searchBatch 时说明此次搜索探索过此节点, 但我懒得改了.
         */
        mutable bool inQueue = false;
        /**
         * 向量在向量容器中的索引.
         */
        int pointIdx = -1;

        /**
         * 此节点和其他节点的连接, 储存的是其他节点在 Layer 中的索引.
         */
        mylinklist::LinkedList<int> links;

        /**
         * 和另外一个节点建立连接, 不会创建重复的连接.
         * @note 此方法只会修改自身的 links.
         * @note 正常情况下此方法只会被 emplaceNode 调用,
         *      emplaceNode 须保证 other 节点那一半的连接正常建立.
         *      这么做的原因是我认为底层的代码不应该持有高层代码的引用.
         * @param otherIdx 另外一个节点在层中的索引值
         */
        void link(int otherIdx);

        explicit GraphNode(int pointIdx);

        GraphNode() = default;
    };

    /**
     * 判断第一个节点是否比第二个节点的距离近.
     * @note 距离值得是 distance 字段, 本方法不会对 batch 和 distance 进行检查, 请使用时自行检查.
     *      在宏 DEBUG_GRAPH_NODE_BUFFER 被定义时, 此函数会检查 n1 和 n2 的 distance 和 batch.
     *      如果检查到 distance 有为 -1 或者 batch 两者不相等, 则报错.
     */
    bool compareNode(const GraphNode *n1, const GraphNode *n2);

    /**
     * 上一个方法的非 const 版本.
     */
    bool compareNode(GraphNode *n1, GraphNode *n2);

    /**
     * 反向比较版本.
     */
    bool compareNodeInverse(const GraphNode *n1, const GraphNode *n2);

    bool compareNodeInverse(GraphNode *n1, GraphNode *n2);
} // hnsw

#endif //GRAPHNODE_H
