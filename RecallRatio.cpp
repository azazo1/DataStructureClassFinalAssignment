//
// Created by azazo1 on 2024/6/1.
//
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

bool in(const int *arr, const int k, const int target) {
    for (int i = 0; i < k; i++) {
        if (arr[i] == target) {
            return true;
        }
    }
    return false;
}

int main() {
    ifstream ifreal(
        R"(D:\Program_Projects\Cpp_Projects\DataStructureClassFinalAssignment\dataset\b8000_q45000_k30_d8_1.out)");
    ifstream ifcheck(
        R"(D:\Program_Projects\Cpp_Projects\DataStructureClassFinalAssignment\dataset\testdataset.hnsw.out)");
    if (!ifreal || !ifcheck) {
        cerr << "无法打开输入文件" << endl;
        return 1;
    }
    int k, nq;
    ifreal >> k >> nq;
    size_t correct = 0;
    int batch[k]; // 待检查的输出数据
    for (int j = 0; j < nq; j++) {
        for (int i = 0; i < k; i++) {
            ifcheck >> batch[i];
        }
        for (int i = 0; i < k; i++) {
            int target;
            ifreal >> target;
            correct += in(batch, k, target);
        }
    }
    cout << "Total: " << fixed << k * nq << ", Correct: " << fixed << correct
            << ", Recall: " << fixed << setprecision(2) << static_cast<double>(correct) / k / nq *
            100 << '%' << endl;
}
