//
// Created by azazo1 on 2024/5/31.
//
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <queue>
#include <set>

const int MAX_LEVEL = 10; // 最大层数
const int EF_CONSTRUCTION = 500; // 构建时的探索因子
const float DEFAULT_EPS = 1e-4; // 默认的精度

class HnswNode {
public:
    std::vector<float> vec;
    std::vector<HnswNode*> neighbors;
    int level;

    HnswNode(std::vector<float>& v, int lvl) : vec(v), level(lvl) {
        neighbors.assign(MAX_LEVEL, nullptr);
    }
};

class HnswIndex {
private:
    std::vector<HnswNode*> nodes;
    int maxLevel;
    int efConstruction;
    float defaultEps;

public:
    HnswIndex(int mLevel, int efConstr, float eps) : maxLevel(mLevel), efConstruction(efConstr), defaultEps(eps) {}

    void addPoint(std::vector<float>& point) {
        int level = getRandomLevel();
        HnswNode* newNode = new HnswNode(point, level);

        if (level > maxLevel) {
            maxLevel = level;
        }

        nodes.push_back(newNode);

        if (nodes.size() == 1) {
            return;
        }

        addToSimpleSet(newNode, 0);

        std::vector<std::pair<HnswNode*, float>> nearestNodes = getNeighbors(newNode, efConstruction, defaultEps);
        for (auto nn : nearestNodes) {
            addToSimpleSet(newNode, nn.first->level);
            addNeighborsNearPoint(newNode, nn.first, nn.second);
            addNeighborsNearPoint(nn.first, newNode, nn.second);
        }
    }

    std::vector<int> searchKnn(std::vector<float>& query, int k) {
        std::priority_queue<std::pair<float, HnswNode*>> result;
        std::vector<float> distByLevel(maxLevel + 1, 0);
        std::vector<bool> isVisited(nodes.size(), false);

        for (int i = 0; i <= maxLevel; i++) {
            distByLevel[i] = std::numeric_limits<float>::max();
        }

        for (HnswNode* node : nodes) {
            float dist = l2Dist(query, node->vec);
            result.push({ dist, node });
        }

        while (result.size() > k) {
            result.pop();
        }

        while (!result.empty()) {
            std::pair<float, HnswNode*> current = result.top();
            result.pop();

            float lowerBound = current.first;

            for (int level = maxLevel; level >= 0; level--) {
                for (HnswNode* neighbor : current.second->neighbors[level]) {
                    if (!isVisited[neighbor - nodes[0]]) {
                        isVisited[neighbor - nodes[0]] = true;
                        float dist = l2Dist(query, neighbor->vec);
                        float totalDist = dist + distByLevel[level];
                        if (totalDist < lowerBound) {
                            result.push({ totalDist, neighbor });
                            while (result.size() > k) {
                                result.pop();
                            }
                            lowerBound = result.top().first;
                        }
                        if (totalDist - distByLevel[level] <= lowerBound) {
                            distByLevel[level] = totalDist - dist;
                        }
                    }
                }
            }
        }

        std::vector<int> knn;

        while (!result.empty()) {
            knn.push_back(result.top().second - nodes[0]);
            result.pop();
        }

        std::reverse(knn.begin(), knn.end());

        return knn;
    }

private:
    void addToSimpleSet(HnswNode* node, int level) {
        int curLevel = (node->level > level) ? level : node->level;
        while (node->neighbors[curLevel] != nullptr) {
            node = node->neighbors[curLevel];
        }
        node->neighbors[curLevel] = node;
    }

    void addNeighborsNearPoint(HnswNode* node, HnswNode* neighbor, float dist) {
        for (int level = neighbor->level; level >= 0; level--) {
            while (node->neighbors[level] != nullptr && l2Dist(node->neighbors[level]->vec, node->vec) > l2Dist(node->vec, neighbor->vec)) {
                node = node->neighbors[level];
            }
            if (level == neighbor->level || node->neighbors[level] == nullptr) {
                neighbor->neighbors[level] = node->neighbors[level];
                node->neighbors[level] = neighbor;
            }
        }
    }

    std::vector<std::pair<HnswNode*, float>> getNeighbors(HnswNode* node, int ef, float eps) {
        std::set<std::pair<float, HnswNode*>> candidates;
        std::vector<float> distByLevel(maxLevel + 1, 0);
        std::vector<bool> isVisited(nodes.size(), false);

        for (int i = 0; i <= maxLevel; i++) {
            distByLevel[i] = std::numeric_limits<float>::max();
        }

        candidates.insert({ 0, nodes[0] });

        while (candidates.size() < ef) {
            float lastDist = candidates.rbegin()->first;
            float newDist = lastDist + eps;

            candidates.insert({ newDist, nodes[rand() % nodes.size()] });
        }

        std::vector<std::pair<HnswNode*, float>> nearestNodes;

        while (!candidates.empty()) {
            std::pair<float, HnswNode*> current = *candidates.begin();
            candidates.erase(candidates.begin());

            float upperBound = current.first;

            if (upperBound > distByLevel[current.second->level]) {
                distByLevel[current.second->level] = upperBound;
            }

            nearestNodes.push_back({ current.second, upperBound });

            for (int level = current.second->level; level >= 0; level--) {
                for (HnswNode* neighbor : current.second->neighbors[level]) {
                    if (!isVisited[neighbor - nodes[0]]) {
                        isVisited[neighbor - nodes[0]] = true;
                        float dist = l2Dist(current.second->vec, neighbor->vec);
                        float totalDist = dist + distByLevel[level];
                        if (totalDist < upperBound) {
                            candidates.insert({ totalDist, neighbor });
                        }
                    }
                }
            }
        }

        return nearestNodes;
    }

    float l2Dist(std::vector<float>& v1, std::vector<float>& v2) {
        float dist = 0;
        for (int i = 0; i < v1.size(); i++) {
            float diff = v1[i] - v2[i];
            dist += diff * diff;
        }
        return std::sqrt(dist);
    }

    int getRandomLevel() {
        static std::default_random_engine generator;
        static std::uniform_real_distribution<float> distribution(0.0, 1.0);

        int level = 0;
        while (distribution(generator) < 0.5 && level < MAX_LEVEL - 1) {
            level++;
        }

        return level;
    }
};

int main() {
    std::vector<std::vector<float>> data = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15} };

    HnswIndex index(MAX_LEVEL, EF_CONSTRUCTION, DEFAULT_EPS);

    for (auto point : data) {
        index.addPoint(point);
    }

    std::vector<int> knn = index.searchKnn(data[0], 3);

    for (auto i : knn) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}