//
// Created by azazo1 on 2024/5/29.
//

#ifndef END_HOMEWORK_VECCONTAINER_H
#define END_HOMEWORK_VECCONTAINER_H

namespace vec {
    /**
     * 此类是一个线性存储容器, 为了防止 Vector 和向量的名称重复, 我将其命名为
     * Vec
     */
    template<class T>
    class Vec {
    protected:
        T *arr = nullptr;
        int capacity;
        int length;

        /**
         * 重新分配容量, 此方法不支持缩短容量.
         * @note请不要在此方法之外修改 capacity 字段.
         * @param newCapacity 新的容量.
         */
        void reallocate(int newCapacity);

    public:
        explicit Vec(int initialSize = 10);

        Vec(const Vec &other);

        Vec &operator=(const Vec &other);

        ~Vec();

        T &operator[](int idx);

        const T &operator[](int idx) const;

        void push_back(const T &val);

        /**
         * 修改此 Vec 的尺寸.
         * 此方法不会对元素进行新建或者修改, 只会改变 length 字段的值,
         * 且参数 length 需要小于等于 capacity 字段.
         * @note 如果宏 DEBUG_VEC_SET_LENGTH 被定义, length 参数不符合上述要求时, 会报错.
         *       如果宏没被定义, 不会检查 length 参数和 capacity 字段的关系, 调用者需要自己保证 length <= capacity.
         * @param length 期望的尺寸
         */
        void setLength(int length);

        int getCapacity() const;

        int getLength() const;

        bool contains(const T &target) const;

        T *end();

        T *begin();
    };
}

#include "Vec.tpp" // 似乎 C++ 中类模版的声明和定义只能在一个翻译单元中, 于是我把此类模版的方法实现放在此处

#endif //END_HOMEWORK_VECCONTAINER_H
