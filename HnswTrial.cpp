//
// Created by azazo1 on 2024/5/30.
//
#include <cstdio>
#include "src/hnsw/HNSW.h"
#include "src/point/Point.h"
#include "src/vec/Vec.h"
#include <sys/timeb.h>
using namespace point;
using namespace vec;
using namespace hnsw;

extern Vec<int> baselineSearch(const Vec<Point> &vec,
                               const Point &query, int k);


void main1() {
    int n, d, k, nq;
    scanf("%d %d %d", &n, &d, &k);
    N_FEATURE = d;
    Vec<Point> vec;
    for (int i = 0; i < n; i++) {
        Point pt;
        pt.readFromInput();
        vec.push_back(pt);
#ifdef DEBUG_READING_FILE
        if (i % 10000 == 0) {
            printf("Reading...now: %d, tol: %d\n", i, n);
        }
#endif
    }
    timeb startBuilding{};
    ftime(&startBuilding);
    const HNSW hnsw(&vec);
    scanf("%d", &nq);
    int cnt = 0;
    timeb startSearching{};
    ftime(&startSearching);
    for (int i = 0; i < nq; i++) {
        Point pt;
        pt.readFromInput();
        auto vecIdx = hnsw.searchNearestTopK(pt, k);
        for (int j: vecIdx) {
#ifndef DEBUG_INSTANT_SPEED
#ifdef OUTPUT
            printf("%d ", j);
#endif
#endif
        }
        cnt++;
#ifdef DEBUG_INSTANT_SPEED
        timeb now{};
        ftime(&now);
        const time_t searchingTime = (now.time - startSearching.time) * 1000
                                     + now.millitm - startSearching.millitm;
        printf("Building Result { cnt: %d, time(ms): %lld, avg: %f Hz }\n",
               n,
               searchingTime,
               n / static_cast<double>(searchingTime) * 1000);
#endif

#ifndef DEBUG_INSTANT_SPEED
#ifdef OUTPUT
        printf("\n");
#endif
#endif
    }
#ifdef DEBUG_INSTANT_SPEED
    printf("\n");
#endif
#ifdef DEBUG_SUMMARY
    timeb now{};
    ftime(&now);
    const time_t buildingTime = (startSearching.time - startBuilding.time) * 1000
                                + startSearching.millitm - startBuilding.millitm;
    const time_t searchingTime = (now.time - startSearching.time) * 1000
                                 + now.millitm - startSearching.millitm;
    printf("\nParameters: { DK_LAYER1: %d, DK_LAYER2: %d, DK_LAYER3: %d, N_LINK: %d }\n",
           DK_LAYER1, DK_LAYER2, DK_LAYER3, N_LINK);
    printf("Building Result { cnt: %d, time(ms): %lld, avg: %f Hz }\n",
           n,
           buildingTime,
           n / static_cast<double>(buildingTime) * 1000);
    printf("Searching Result { cnt: %d, time(ms) : %lld, avg: %f Hz }\n",
           cnt,
           searchingTime,
           cnt / static_cast<double>(searchingTime) * 1000);
#endif
}

/**
 * 测试准确性.
 */
void main2() {
    int n, d, k, nq;
    scanf("%d %d %d", &n, &d, &k);
    N_FEATURE = d;
    Vec<Point> vec;
    for (int i = 0; i < n; i++) {
        Point pt;
        pt.readFromInput();
        vec.push_back(pt);
#ifdef DEBUG_READING_FILE
        if (i % 10000 == 0) {
            printf("Reading...now: %d, tol: %d\n", i, n);
        }
#endif
    }
    const HNSW hnsw(&vec);
    scanf("%d", &nq);
    for (int j = 0; j < nq; j++) {
        Point pt;
        pt.readFromInput();
        auto baseline = baselineSearch(vec, pt, k);
        auto vecIdx = hnsw.searchNearestTopK(pt, k);
        for (int i = 0; i < k; i++) {
            printf("Node: %10d:%10d, \tDistance: %f, %f\n",
                   vecIdx[i],
                   baseline[i],
                   pt.distanceTo(vec[vecIdx[i]]),
                   pt.distanceTo(vec[baseline[i]]));
        }
        printf("\n");
    }
    hnsw.queryLinks(&vec[79]);
}

int main() {
    main1();
}
