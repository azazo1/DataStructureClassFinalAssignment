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
    const HNSW hnsw(&vec);
    scanf("%d", &nq);
    int cnt = 0;
    timeb start{};
    ftime(&start);
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
        printf("\r%d, time(s): %lld, time(ms): %hu, avg: %f Hz",
               cnt,
               now.time - start.time,
               now.millitm - start.millitm,
               1.0 * cnt / ((now.time + now.millitm * 1.0 / 1000) -
                            (start.time + start.millitm * 1.0 / 1000)));
        fflush(stdout);
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
#ifdef DEBUG_TOTAL_TIME
    timeb now{};
    ftime(&now);
    printf("time(s): %lld, time(ms): %hu, avg: %f Hz",
           now.time - start.time,
           now.millitm - start.millitm,
           1.0 * cnt / ((now.time + now.millitm * 1.0 / 1000) -
                        (start.time + start.millitm * 1.0 / 1000)));
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
