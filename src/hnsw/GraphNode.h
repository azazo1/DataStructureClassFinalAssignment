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
 */
#define N_LINK 3

namespace hnsw {
    struct GraphNode {
        /**
         * 向量在向量容器中的索引.
         */
        int pointIdx = -1;

        /**
         * 此节点和其他节点的连接.
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
    };
} // hnsw

#endif //GRAPHNODE_H
