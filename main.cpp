#include <cstdio>
#include <ctime>

#include "src/point/Point.h"
#include "src/vec/Vec.h"

#define DEBUG
// #define DEBUG_SINGLE_SEARCH

using namespace vec;
using namespace point;

extern Vec<int> baselineSearch(const Vec<Point> &, const Point &, int);

extern Vec<int> baselineSearchWithHashSet(const Vec<Point> &, const Point &, int);

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
    scanf("%d", &nq);
    int cnt = 0;
    time_t start = time(NULL);
    for (int i = 0; i < nq; i++) {
        Point pt;
        pt.readFromInput();
        // auto vecIdx = baselineSearch(vec, pt, k);
        auto vecIdx = baselineSearchWithHashSet(vec, pt, k);
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
