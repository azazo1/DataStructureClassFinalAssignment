//
// Created by azazo1 on 2024/5/29.
//

#include "Point.h"
#include <cstdio>

namespace point {
    Point::Point(): features(new double[N_FEATURE]) {
    }

    Point::Point(const Point &other): Point() {
        for (int i = 0; i < N_FEATURE; i++) {
            features[i] = other.features[i];
        }
    }

    Point &Point::operator=(const Point &other) {
        if (this == &other) {
            return *this;
        }
        for (int i = 0; i < N_FEATURE; i++) {
            features[i] = other.features[i];
        }
        return *this;
    }

    Point::~Point() {
        delete[] features;
    }

    void Point::readFromInput() {
        for (int i = 0; i < N_FEATURE; i++) {
            if (scanf("%lf", features + i) != 1) {
                throw "Input Error";
            }
        }
    }

    double *Point::getFeatures() {
        return features;
    }

    double Point::distanceTo(const Point &other) const {
        double sum = 0;
        for (int i = 0; i < N_FEATURE; i++) {
            const double diff = features[i] - other.features[i];
            sum += diff * diff;
        }
        return sum;
    }

    bool Point::operator==(const Point &other) const {
        if (this == &other) {
            return true;
        }
        for (int i = 0; i < N_FEATURE; i++) {
            if (features[i] != other.features[i]) {
                return false;
            }
        }
        return true;
    }

    double Point::operator[](const int idx) const {
        return features[idx];
    }
}
