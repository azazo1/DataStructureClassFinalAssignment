// //
// // Created by azazo1 on 2024/5/29.
// // 存放单文件版的代码
//
// #include <cstdio>
// #include <cfloat>
// /**
//  * 定义了向量的维度数.
//  * @note 此值只能在第一次输入的时候更改, 即只能在所有向量都没有创建之前更改.
//  */
// inline int N_FEATURE = -1;
//
// /**
//  * 表示题目中的一个向量, 为了防止名称重复, 我选择命名为 Point.
//  */
// class Point {
//     /**
//      * 一个向量的各个维度的值, 此数组的大小由全局变量 N_FEATURE 定义.
//      */
//     double *features = nullptr;
//
// public:
//     Point();
//
//     Point(const Point &other);
//
//     Point &operator=(const Point &other);
//
//     ~Point();
//
//     /**
//      * 从标准输入中读取向量的 features, 有效的输入是 N_FEATURE 个浮点数.
//      * @note 标准输入的值由调用者保证.
//      */
//     void readFromInput();
//
//     /**
//      * 获取此向量的 features, 以便进行读取和修改值操作.
//      * @note 不要在 Point 类以外对 features 进行销毁操作.
//      */
//     double *getFeatures();
//
//     double distanceTo(const Point &other) const;
//
//     bool operator==(const Point &other) const;
//
//     double operator[](int idx) const;
// };
//
// Point::Point(): features(new double[N_FEATURE]) {
// }
//
// Point::Point(const Point &other): Point() {
//     for (int i = 0; i < N_FEATURE; i++) {
//         features[i] = other.features[i];
//     }
// }
//
// Point &Point::operator=(const Point &other) {
//     if (this == &other) {
//         return *this;
//     }
//     for (int i = 0; i < N_FEATURE; i++) {
//         features[i] = other.features[i];
//     }
//     return *this;
// }
//
// Point::~Point() {
//     delete[] features;
// }
//
// void Point::readFromInput() {
//     for (int i = 0; i < N_FEATURE; i++) {
//         if (scanf("%lf", features + i) != 1) {
//             throw "Input Error";
//         }
//     }
// }
//
// double *Point::getFeatures() {
//     return features;
// }
//
// double Point::distanceTo(const Point &other) const {
//     double sum = 0;
//     for (int i = 0; i < N_FEATURE; i++) {
//         const double diff = features[i] - other.features[i];
//         sum += diff * diff;
//     }
//     return sum;
// }
//
// bool Point::operator==(const Point &other) const {
//     if (this == &other) {
//         return true;
//     }
//     for (int i = 0; i < N_FEATURE; i++) {
//         if (features[i] != other.features[i]) {
//             return false;
//         }
//     }
//     return true;
// }
//
// double Point::operator[](const int idx) const {
//     return features[idx];
// }
//
//
// /**
//  * 此类是一个线性存储容器, 为了防止 Vector 和向量的名称重复, 我将其命名为
//  * VecContainer
//  */
// template<class T>
// class VecContainer {
//     T *arr = nullptr;
//     int capacity;
//     int length;
//
//     /**
//      * 重新分配容量, 此方法不支持缩短容量.
//      * @note请不要在此方法之外修改 capacity 字段.
//      * @param newCapacity 新的容量.
//      */
//     void reallocate(int newCapacity);
//
// public:
//     explicit VecContainer(int initialSize = 1);
//
//     VecContainer(const VecContainer &other);
//
//     VecContainer &operator=(const VecContainer &other);
//
//     ~VecContainer();
//
//     T &operator[](int idx);
//
//     const T &operator[](int idx) const;
//
//     void push_back(const T &val);
//
//     int getCapacity() const;
//
//     int getLength() const;
//
//     bool contains(const T &target) const;
//
//     T *end();
//
//     T *begin();
// };
//
//
// template<class T>
// VecContainer<T>::VecContainer(const int initialSize)
//     : arr(new T[initialSize]),
//       capacity(initialSize),
//       length(0) {
// }
//
// template<class T>
// T &VecContainer<T>::operator[](int idx) {
//     return arr[idx];
// }
//
// template<class T>
// const T &VecContainer<T>::operator[](int idx) const {
//     return arr[idx];
// }
//
// template<class T>
// void VecContainer<T>::push_back(const T &val) {
//     if (length == capacity) {
//         reallocate(capacity * 2);
//     }
//     arr[length++] = val;
// }
//
// template<class T>
// void VecContainer<T>::reallocate(const int newCapacity) {
//     if (newCapacity <= capacity) {
//         return;
//     }
//     capacity = newCapacity;
//     T *newArr = new T[newCapacity];
//     for (int i = 0; i < length; i++) {
//         newArr[i] = arr[i];
//     }
//     delete[] arr;
//     arr = newArr;
// }
//
// template<class T>
// int VecContainer<T>::getCapacity() const {
//     return capacity;
// }
//
// template<class T>
// int VecContainer<T>::getLength() const {
//     return length;
// }
//
// template<class T>
// VecContainer<T>::~VecContainer() {
//     delete[] arr;
//     length = 0;
//     capacity = 0;
// }
//
// template<class T>
// bool VecContainer<T>::contains(const T &target) const {
//     for (int i = 0; i < length; i++) {
//         if (target == arr[i]) {
//             return true;
//         }
//     }
//     return false;
// }
//
// template<class T>
// VecContainer<T>::VecContainer(const VecContainer &other): VecContainer(other.capacity) {
//     length = other.length;
//     for (int i = 0; i < length; i++) {
//         arr[i] = other.arr[i];
//     }
// }
//
// template<class T>
// VecContainer<T> &VecContainer<T>::operator=(const VecContainer &other) {
//     if (this == &other) {
//         return *this;
//     }
//     reallocate(other.capacity);
//     length = other.length;
//     for (int i = 0; i < other.length; i++) {
//         arr[i] = other.arr[i];
//     }
//     return *this;
// }
//
// template<class T>
// T *VecContainer<T>::begin() {
//     return arr;
// }
//
// template<class T>
// T *VecContainer<T>::end() {
//     return arr + length;
// }
//
// /**
//  * 基准向量查找
//  * @param vec 存放要被查找的向量
//  * @param query 要查找的目标
//  * @param k 要查找的最近向量个数
//  * @return 查找结果在 vec 中的坐标集合.
//  */
// VecContainer<int> baselineSearch(const VecContainer<Point> &vec, const Point &query, int k) {
//     if (vec.getLength() == 0) {
//         return VecContainer<int>();
//     }
//     VecContainer<int> vecIdx;
//     for (int i = 0; i < k; i++) {
//         int nearestIdx = 0;
//         double shortestDistance = DBL_MAX;
//         // 跳转到第一个没被记录的向量, 此时 nearestIdx 不会超过 vec 的尺寸, 因为有 k 的限制.
//         while (vecIdx.contains(nearestIdx)) nearestIdx++;
//
//         for (int j = nearestIdx; j < vec.getLength(); ++j) {
//             if (vecIdx.contains(j)) {
//                 continue;
//             }
//             if (const double distance = vec[j].distanceTo(query);
//                 distance < shortestDistance) {
//                 shortestDistance = distance;
//                 nearestIdx = j;
//             }
//         }
//         vecIdx.push_back(nearestIdx);
//     }
//     return vecIdx;
// }
//
// int main() {
//     int n, d, k, nq;
//     scanf("%d %d %d", &n, &d, &k);
//     N_FEATURE = d;
//     VecContainer<Point> vec;
//     for (int i = 0; i < n; i++) {
//         Point pt;
//         pt.readFromInput();
//         vec.push_back(pt);
//     }
//     scanf("%d", &nq);
//     for (int i = 0; i < nq; i++) {
//         Point pt;
//         pt.readFromInput();
//         auto vecIdx = baselineSearch(vec, pt, k);
//         for (int j: vecIdx) {
//             // printf("%f %f %f\n", vec[j][0], vec[j][1], vec[j][2]);
//             printf("%d ", j);
//         }
//         printf("\n");
//     }
// }
