//
// Created by azazo1 on 2024/5/30.
//
#include <cstdio>
#include <ctime>

#include "src/hnsw/HNSW.h"
#include "src/point/Point.h"
#include "src/vec/Vec.h"
using namespace point;
using namespace vec;
using namespace hnsw;

#define DEBUG
// #define DEBUG_SINGLE_SEARCH
#define DEBUG_GRAPH_NODE_BUFFER
#define DEBUG_VEC_SET_LENGTH

int main() {
    int n, d, k, nq;
    scanf("%d %d %d", &n, &d, &k);
    N_FEATURE = d;
    Vec<Point> vec;
    for (int i = 0; i < n; i++) {
        Point pt;
        pt.readFromInput();
        vec.push_back(pt);
    }
    HNSW hnsw(&vec);
    scanf("%d", &nq);
    int cnt = 0;
    time_t start = time(NULL);
    for (int i = 0; i < nq; i++) {
        Point pt;
        pt.readFromInput();
        // auto vecIdx = baselineSearch(vec, pt, k);
        auto vecIdx = hnsw.searchNearestTopK(pt, k);
        for (int j: vecIdx) {
#ifndef DEBUG
#ifndef DEBUG_SINGLE_SEARCH
            printf("%d ", j);
#endif
#endif

#ifdef DEBUG_SINGLE_SEARCH
            printf("\r%d, time: %lld, avg: %f",
                   cnt++,
                   time(NULL),
                   1.0 * cnt / (time(NULL) - start));
            fflush(stdout);
#endif
        }
#ifdef DEBUG
#ifndef DEBUG_SINGLE_SEARCH
        printf("\r%d, time: %lld, avg: %f",
               cnt++,
               time(NULL),
               1.0 * cnt / (time(NULL) - start));
        fflush(stdout);
#endif
#endif

#ifndef DEBUG
#ifndef DEBUG_SINGLE_SEARCH
        printf("\n");
#endif
#endif
    }
}
