#include <iostream>

#include "lib/hnswlib/hnswlib.h"


int main() {
    int dim = 16; // Dimension of the elements
    int max_elements = 10000; // Maximum number of elements, should be known beforehand
    int M = 16; // Tightly connected with internal dimensionality of the data
    // strongly affects the memory consumption
    int ef_construction = 200; // Controls index search speed/build speed tradeoff
    int k;
    std::cin >> max_elements >> dim >> k;

    // Initing index
    hnswlib::L2Space space(dim);
    hnswlib::HierarchicalNSW<float> *alg_hnsw = new hnswlib::HierarchicalNSW<float>(
        &space, max_elements, M, ef_construction);

    // Generate random data
    // std::mt19937 rng;
    // rng.seed(47);
    // std::uniform_real_distribution<> distrib_real;
    float *data = new float[dim * max_elements];
    // for (int i = 0; i < dim * max_elements; i++) {
    // data[i] = distrib_real(rng);
    // }
    // 读取输入
    for (int i = 0; i < dim * max_elements; i++) {
        std::cin >> data[i];
    }

    // Add data to index
    for (int i = 0; i < max_elements; i++) {
        alg_hnsw->addPoint(data + i * dim, i);
    }

    // Query the elements for themselves and measure recall
    float correct = 0;
    int nq;
    std::cin >> nq;
    for (int i = 0; i < nq; i++) {
        float queryData[dim];
        for (int j = 0; j < dim; ++j) {
            std::cin >> queryData[j];
        }
        std::priority_queue<std::pair<float, hnswlib::labeltype> > result = alg_hnsw->searchKnn(
            data + i * dim, 1);
        while (!result.empty()) {
            hnswlib::labeltype label = result.top().second;
            std::cout << label << " ";
        }
        std::cout << std::endl;
    }

    delete[] data;
    delete alg_hnsw;
    return 0;
}
