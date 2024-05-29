//
// Created by azazo1 on 2024/5/23.
//
#include <cstdio>
#include <cfloat>

#include "src/hashset/HashSet.h"
#include "src/point/Point.h"
#include "src/vec/Vec.h"

/**
 * 基准向量查找
 * @param vec 存放要被查找的向量
 * @param query 要查找的目标
 * @param k 要查找的最近向量个数
 * @return 查找结果在 vec 中的坐标集合.
 *
 * @note 在本人的电脑上, 占用 cpu 约 15%, 在测试集 8000 中能做到每秒 105 次查询.
 */
vec::Vec<int> baselineSearch(const vec::Vec<point::Point> &vec,
                                               const point::Point &query, int k) {
    if (vec.getLength() == 0) {
        return vec::Vec<int>();
    }
    vec::Vec<int> vecIdx;
    for (int i = 0; i < k; i++) {
        int nearestIdx = 0;
        double shortestDistance = DBL_MAX;
        // 跳转到第一个没被记录的向量, 此时 nearestIdx 不会超过 vec 的尺寸, 因为有 k 的限制.
        while (vecIdx.contains(nearestIdx)) nearestIdx++;

        for (int j = nearestIdx; j < vec.getLength(); ++j) {
            if (vecIdx.contains(j)) {
                continue;
            }
            if (const double distance = vec[j].distanceTo(query);
                distance < shortestDistance) {
                shortestDistance = distance;
                nearestIdx = j;
            }
        }
        vecIdx.push_back(nearestIdx);
    }
    return vecIdx;
}

/**
 * 基准向量查找, 但是储存索引的容器变成散列表
 * @param vec 存放要被查找的向量
 * @param query 要查找的目标
 * @param k 要查找的最近向量个数
 * @return 查找结果在 vec 中的坐标集合.
 *
 * @note 在本人的电脑上, 占用 cpu 约 15%, 在测试集 8000 中 hashSize = 3 时能做到每秒 130 次查询.
 *      在 hashSzie >= 100 时 能做到每秒 170 次查询.
 */
vec::Vec<int>
baselineSearchWithHashSet(const vec::Vec<point::Point> &vec,
                          const point::Point &query, int k) {
    if (vec.getLength() == 0) {
        return vec::Vec<int>();
    }
    myhashset::HashSet idxSet(1000);
    for (int i = 0; i < k; i++) {
        int nearestIdx = 0;
        double shortestDistance = DBL_MAX;
        // 跳转到第一个没被记录的向量, 此时 nearestIdx 不会超过 vec 的尺寸, 因为有 k 的限制.
        while (idxSet.contains(nearestIdx)) nearestIdx++;

        for (int j = nearestIdx; j < vec.getLength(); ++j) {
            if (idxSet.contains(j)) {
                continue;
            }
            if (const double distance = vec[j].distanceTo(query);
                distance < shortestDistance) {
                shortestDistance = distance;
                nearestIdx = j;
            }
        }
        idxSet.put(nearestIdx);
    }
    return idxSet.flatten();
}
