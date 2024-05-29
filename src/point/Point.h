//
// Created by azazo1 on 2024/5/29.
//

#ifndef END_HOMEWORK_POINT_H
#define END_HOMEWORK_POINT_H

namespace point {
    /**
     * 定义了向量的维度数.
     * @note 此值只能在第一次输入的时候更改, 即只能在所有向量都没有创建之前更改.
     */
    inline int N_FEATURE = -1;

    /**
     * 表示题目中的一个向量, 为了防止名称重复, 我选择命名为 Point.
     */
    class Point {
        /**
         * 一个向量的各个维度的值, 此数组的大小由全局变量 N_FEATURE 定义.
         */
        double *features = nullptr;

    public:
        Point();

        Point(const Point &other);

        Point &operator=(const Point &other);

        ~Point();

        /**
         * 从标准输入中读取向量的 features, 有效的输入是 N_FEATURE 个浮点数.
         * @note 标准输入的值由调用者保证.
         */
        void readFromInput();

        /**
         * 获取此向量的 features, 以便进行读取和修改值操作.
         * @note 不要在 Point 类以外对 features 进行销毁操作.
         */
        double *getFeatures();

        double distanceTo(const Point &other) const;

        bool operator==(const Point &other) const;

        double operator[](int idx) const;
    };
}


#endif //END_HOMEWORK_POINT_H
