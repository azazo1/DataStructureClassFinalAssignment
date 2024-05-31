//
// Created by azazo1 on 2024/5/30.
//
#include <cstdio>
#include <cmath>
#include <exception>
#include <sys/timeb.h>

#define DK_LAYER1 100
#define DK_LAYER2 300
#define DK_LAYER3 600

namespace pq {
    class EmptyPQException : public std::exception {
        const char *what() const noexcept override {
            return "Emtpy LinkedList";
        }
    };

    /**
     * 采用线性存储
     */
    template<class T>
    class PriorityQueue {
    public:
        PriorityQueue(bool (*cmp)(T, T));

        PriorityQueue(size_t initialCapacity, bool (*cmp)(T, T));

        PriorityQueue(const PriorityQueue &other);

        PriorityQueue<T> &operator=(const PriorityQueue &other);

        ~PriorityQueue();

        /**
         * 讲一个元素添加到优先级队列.
         */
        void add(T ele);

        /**
         * 返回优先级队列顶部的元素.
         */
        T get() const;

        /**
         * 增大内部数组容量, 此函数不会缩减容量.
         * @param newCapacity 新的容量大小
         */
        void reserve(size_t newCapacity);

        /**
         * 弹出优先级队列的顶部元素.
         */
        T pop();

        size_t getHeight() const;

        size_t getSize() const;

    private:
        /**
         * 将位于 index 中的元素下降到它应该到的地方.
         */
        void swim(int index);

        /**
         * 将位于 index 中的元素提升到它应该到的地方.
         */
        void sink(int index);

        /**
         * 用于两个元素的比较, 如果第一个参数小于第二个参数, 应该返回 true.
         * @note 此方法中最大的元素将会在堆顶.
         */
        bool (*cmp)(T, T);

        T *arr;
        size_t capacity;
        size_t eleCnt;
    };
}

namespace pq {
    template<class T>
    size_t PriorityQueue<T>::getHeight() const {
        return static_cast<int>(log2(eleCnt));
    }

    template<class T>
    size_t PriorityQueue<T>::getSize() const {
        return eleCnt;
    }

    template<class T>
    PriorityQueue<T>::PriorityQueue(bool (*cmp)(T, T)): PriorityQueue(3, cmp) {
    }

    template<class T>
    PriorityQueue<T>::PriorityQueue(size_t initialCapacity, bool (*cmp)(T, T))
        : cmp(cmp),
          arr(new T[initialCapacity]),
          capacity(initialCapacity),
          eleCnt(0) {
    }

    template<class T>
    PriorityQueue<T>::PriorityQueue(const PriorityQueue &other)
        : cmp(nullptr),
          arr(new T[other.capacity]),
          capacity(other.capacity),
          eleCnt(other.eleCnt) {
        cmp = other.cmp;
        for (int i = 0; i < eleCnt; i++) {
            arr[i] = other.arr[i];
        }
    }

    template<class T>
    PriorityQueue<T> &PriorityQueue<T>::operator=(const PriorityQueue &other) {
        if (this == &other) {
            return *this;
        }
        cmp = other.cmp;
        capacity = other.capacity;
        eleCnt = other.eleCnt;
        delete[] arr; // 注意旧内存的回收
        arr = new T[capacity];
        for (int i = 0; i < eleCnt; i++) {
            arr[i] = other.arr[i];
        }
        return *this;
    }

    template<class T>
    PriorityQueue<T>::~PriorityQueue() {
        delete[] arr;
        eleCnt = 0;
        capacity = 0;
    }

    template<class T>
    void PriorityQueue<T>::reserve(size_t newCapacity) {
        if (capacity >= newCapacity) {
            return;
        }
        T *newArr = new T[newCapacity];
        for (int i = 0; i < eleCnt; i++) {
            newArr[i] = arr[i];
        }
        capacity = newCapacity;
        delete[] arr;
        arr = newArr;
    }

    template<class T>
    T PriorityQueue<T>::get() const {
        return arr[0];
    }

    template<class T>
    void PriorityQueue<T>::add(T ele) {
        if (eleCnt == capacity) {
            //        capacity *= 2;
            //        reserve(capacity);
            reserve(capacity * 2); // 前面那两行是错的, 不应该提前更改capacity大小, 否则会导致reserve内的误判
        }
        arr[eleCnt] = ele;
        swim(eleCnt++);
    }

    template<class T>
    void PriorityQueue<T>::swim(int index) {
        T moved = arr[index];
        int parent = (index - 1) / 2;
        while (parent != index && cmp(arr[parent], moved)) {
            arr[index] = arr[parent];
            index = parent;
            parent = (index - 1) / 2;
        }
        arr[index] = moved;
    }

    template<class T>
    void PriorityQueue<T>::sink(int index) {
        T moved = arr[index];
        int son = index * 2 + 1;
        while (son < eleCnt) {
            if (son < eleCnt - 1 && cmp(arr[son], arr[son + 1])) {
                son++;
            }
            if (cmp(moved, arr[son])) {
                arr[index] = arr[son];
                index = son;
                son = index * 2 + 1;
            } else {
                break;
            }
        }
        arr[index] = moved;
    }

    template<class T>
    T PriorityQueue<T>::pop() {
        if (eleCnt == 0) {
            throw EmptyPQException();
        }
        eleCnt--;
        T rst = arr[0];
        arr[0] = arr[eleCnt];
        sink(0);
        return rst;
    }
}

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


namespace vec {
    template<class T>
    Vec<T>::Vec(const int initialSize)
        : arr(new T[initialSize]),
          capacity(initialSize),
          length(0) {
    }

    template<class T>
    T &Vec<T>::operator[](int idx) {
        return arr[idx];
    }

    template<class T>
    const T &Vec<T>::operator[](int idx) const {
        return arr[idx];
    }

    template<class T>
    void Vec<T>::push_back(const T &val) {
        if (length == capacity) {
            reallocate(capacity * 2);
        }
        arr[length++] = val;
    }

    template<class T>
    void Vec<T>::reallocate(const int newCapacity) {
        if (newCapacity <= capacity) {
            return;
        }
        capacity = newCapacity;
        T *newArr = new T[newCapacity];
        for (int i = 0; i < length; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }

    template<class T>
    int Vec<T>::getCapacity() const {
        return capacity;
    }

    template<class T>
    int Vec<T>::getLength() const {
        return length;
    }

    template<class T>
    Vec<T>::~Vec() {
        delete[] arr;
        length = 0;
        capacity = 0;
    }

    template<class T>
    bool Vec<T>::contains(const T &target) const {
        for (int i = 0; i < length; i++) {
            if (target == arr[i]) {
                return true;
            }
        }
        return false;
    }

    template<class T>
    Vec<T>::Vec(const Vec &other): Vec(other.capacity) {
        length = other.length;
        for (int i = 0; i < length; i++) {
            arr[i] = other.arr[i];
        }
    }

    template<class T>
    Vec<T> &Vec<T>::operator=(const Vec &other) {
        if (this == &other) {
            return *this;
        }
        reallocate(other.capacity);
        length = other.length;
        for (int i = 0; i < other.length; i++) {
            arr[i] = other.arr[i];
        }
        return *this;
    }

    template<class T>
    T *Vec<T>::begin() {
        return arr;
    }

    template<class T>
    T *Vec<T>::end() {
        return arr + length;
    }

    template<class T>
    void Vec<T>::setLength(int length) {
#ifdef DEBUG_VEC_SET_LENGTH
        if (length > capacity) {
            std::cerr << "invalid length" << std::endl;
            exit(1001);
        }
#endif
        this->length = length;
    }
}

namespace point {
    class InputException : public std::exception {
        const char *what() const noexcept override {
            return "Input Exception";
        }
    };

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
                throw InputException();
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

namespace mylinklist {
    class EmptyException : public std::exception {
        const char *what() const noexcept override {
            return "Emtpy LinkedList";
        }
    };

    template<class T>
    struct Node {
        T val;
        Node *next;

        Node();

        Node(const Node &other) = delete;

        explicit Node(const T &val, Node<T> *link = nullptr);

        ~Node();
    };

    enum ErrorCode {
        SUC, EMPTY, RANGE_ERROR, DUPLICATE
    };

    template<class T>
    class Iterator {
        const Node<T> *startNode;

    public:
        Iterator(const Node<T> *startNode);

        bool hasNext() const;

        T next();
    };

    template<class T>
    class LinkedList {
    public:
        LinkedList();

        LinkedList(const LinkedList &other);

        LinkedList<T> &operator=(const LinkedList<T> &other);

        bool full() const;

        int size() const;

        bool empty() const;

        void clear();

        void traverse(void (*fn)(T &));

        ErrorCode retrieve(int position, T &val) const;

        ErrorCode replace(int position, const T &val);

        ErrorCode remove(int position, T &disposed);

        ErrorCode insert(int position, const T &val);

        ErrorCode push_back(const T &val);

        T pop_head();

        /**
         * 在列表末尾添加元素, 如果元素已经在列表中出现则不添加.
         * @param val 要添加的元素
         * @return 错误代码, 成功添加为 SUC, 重复了则为 DUPLICATE.
         */
        ErrorCode push_back_unique(const T &val);

        bool contains(const T &target);

        ~LinkedList();

        Iterator<T> getIter() const;

    protected:
        int count = 0;
        Node<T> *head = nullptr;
        Node<T> *tail = nullptr;

        Node<T> *getNode(int position) const;
    };
} // mylinklist

namespace mylinklist {
    template<class T>
    Node<T>::Node(): val(), next(nullptr) {
    }

    template<class T>
    Node<T>::Node(const T &val, Node<T> *link): val(val), next(link) {
    }

    template<class T>
    Node<T>::~Node() {
        // 实际上这并不是一个很好的设计, 使其在拷贝 Node 时出现异常.
        delete next;
    }

    template<class T>
    bool LinkedList<T>::full() const {
        auto *cur = new Node<T>();
        return cur == nullptr;
    }

    template<class T>
    LinkedList<T>::LinkedList(): head(nullptr) {
    }

    template<class T>
    LinkedList<T>::LinkedList(const LinkedList &other): head(nullptr) {
        *this = other;
    }

    template<class T>
    LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &other) {
        if (this == &other) {
            return *this;
        }
        clear();
        count = other.count;
        Node<T> *oNode = other.head;
        while (oNode != nullptr) {
            if (head == nullptr) {
                head = new Node<T>(oNode->val);
                tail = head;
            } else {
                tail->next = new Node<T>(oNode->val);
                tail = tail->next;
            }
            oNode = oNode->next;
        }
        return *this;
    }

    template<class T>
    int LinkedList<T>::size() const {
        return count;
    }

    template<class T>
    bool LinkedList<T>::empty() const {
        return count == 0;
    }

    template<class T>
    void LinkedList<T>::clear() {
        delete head;
        head = nullptr;
        count = 0;
    }

    template<class T>
    void LinkedList<T>::traverse(void (*fn)(T &)) {
        Node<T> *cur = head;
        while (cur != nullptr) {
            fn(cur->val);
            cur = cur->next;
        }
    }

    template<class T>
    ErrorCode LinkedList<T>::retrieve(int position, T &val) const {
        if (count == 0) {
            return EMPTY;
        }
        if (position < 0 || position >= count) {
            return RANGE_ERROR;
        }
        Node<T> *cur = getNode(position);
        val = cur->val;
        return SUC;
    }

    template<class T>
    ErrorCode LinkedList<T>::replace(int position, const T &val) {
        if (count == 0) {
            return EMPTY;
        }
        if (position < 0 || position >= count) {
            return RANGE_ERROR;
        }
        Node<T> *cur = getNode(position);
        cur->val = val;
        return SUC;
    }

    template<class T>
    ErrorCode LinkedList<T>::remove(int position, T &disposed) {
        if (count == 0) {
            return EMPTY;
        }
        if (position < 0 || position >= count) {
            return RANGE_ERROR;
        }
        if (position == 0) {
            Node<T> *n = head->next;
            head->next = nullptr; // in case free the later node (see ~Node)
            disposed = head->val;
            delete head;
            head = n;
            if (count == 1) {
                tail = nullptr;
            }
        } else {
            Node<T> *prev = getNode(position - 1);
            Node<T> *cur = prev->next;
            disposed = cur->val;
            prev->next = cur->next;
            cur->next = nullptr;
            delete cur;
            if (position == count - 1) {
                tail = prev;
            }
        }
        count--;
        return SUC;
    }

    template<class T>
    ErrorCode LinkedList<T>::insert(const int position, const T &val) {
        if (position < 0 || position > count) {
            return RANGE_ERROR;
        }
        if (position == 0) {
            Node<T> *h = head;
            head = new Node<T>(val);
            head->next = h;
            if (count == 0) {
                tail = head;
            }
        } else {
            Node<T> *prev = getNode(position - 1);
            auto *cur = new Node<T>(val);
            cur->next = prev->next;
            prev->next = cur;
            if (position == count) {
                tail = cur;
            }
        }
        count++;
        return SUC;
    }

    template<class T>
    Node<T> *LinkedList<T>::getNode(int position) const {
        Node<T> *cur = head;
        for (int i = 0; i < position; i++) {
            cur = cur->next;
        }
        return cur;
    }

    template<class T>
    LinkedList<T>::~LinkedList() {
        count = 0;
        delete head;
        head = nullptr;
    }

    template<class T>
    bool LinkedList<T>::contains(const T &target) {
        Node<T> *cur = head;
        while (cur != nullptr) {
            if (cur->val == target) {
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    template<class T>
    ErrorCode LinkedList<T>::push_back(const T &val) {
        if (tail == nullptr) {
            head = tail = new Node<T>(val);
        } else {
            tail->next = new Node<T>(val);
            tail = tail->next;
        }
        count++;
        return SUC;
    }

    template<class T>
    ErrorCode LinkedList<T>::push_back_unique(const T &val) {
        Node<T> *cur = head;
        if (cur == nullptr) {
            head = new Node<T>(val);
        } else {
            while (cur->next != nullptr) {
                if (cur->val == val) {
                    return DUPLICATE;
                }
                cur = cur->next;
            }
            cur->next = new Node<T>(val);
        }
        count++;
        return SUC;
    }

    template<class T>
    T LinkedList<T>::pop_head() {
        if (head == nullptr) {
            throw EmptyException();
        }
        T rst;
        remove(0, rst);
        return rst;
    }

    template<class T>
    Iterator<T> LinkedList<T>::getIter() const {
        return head;
    }

    template<class T>
    Iterator<T>::Iterator(const Node<T> *startNode): startNode(startNode) {
    }

    template<class T>
    T Iterator<T>::next() {
        T rst = startNode->val;
        startNode = startNode->next;
        return rst;
    }

    template<class T>
    bool Iterator<T>::hasNext() const {
        return startNode != nullptr;
    }
} // mylinklist

/**
 * 每个节点的最大主动连接数, 在节点加入到 Graph 中, 每个节点查找与其最近的 N 个节点,
 * 并与其建立连接.
 * @note 此值不代表 links 列表中元素个数最大值, 在后续节点加入 Graph 时, 节点可能被动拓展连接.
 */
#define N_LINK 10

namespace hnsw {
    struct GraphNode {
        /**
         * 用于记录缓存的距离所属的搜索批次.
         */
        mutable int batch = -1;
        /**
         * 用于记录缓存的距离.
         */
        mutable double distance = -1;
        /**
         * 用来标志此节点是否在队列中.
         */
        mutable bool inQueue = false;
        /**
         * 向量在向量容器中的索引.
         */
        int pointIdx = -1;

        /**
         * 此节点和其他节点的连接, 储存的是其他节点在 Layer 中的索引.
         */
        mylinklist::LinkedList<int> links;

        /**
         * 和另外一个节点建立连接, 不会创建重复的连接.
         * @note 此方法只会修改自身的 links.
         * @note 正常情况下此方法只会被 emplaceNode 调用,
         *      emplaceNode 须保证 other 节点那一半的连接正常建立.
         *      这么做的原因是我认为底层的代码不应该持有高层代码的引用.
         * @param otherIdx 另外一个节点在层中的索引值
         */
        void link(int otherIdx);

        explicit GraphNode(int pointIdx);

        GraphNode() = default;
    };

    /**
     * 判断第一个节点是否比第二个节点的距离近.
     * @note 距离值得是 distance 字段, 本方法不会对 batch 和 distance 进行检查, 请使用时自行检查.
     *      在宏 DEBUG_GRAPH_NODE_BUFFER 被定义时, 此函数会检查 n1 和 n2 的 distance 和 batch.
     *      如果检查到 distance 有为 -1 或者 batch 两者不相等, 则报错.
     */
    bool compareNode(const GraphNode *n1, const GraphNode *n2);

    /**
     * 上一个方法的非 const 版本.
     */
    bool compareNode(GraphNode *n1, GraphNode *n2);

    /**
     * 反向比较版本.
     */
    bool compareNodeInverse(const GraphNode *n1, const GraphNode *n2);

    bool compareNodeInverse(GraphNode *n1, GraphNode *n2);
} // hnsw

namespace hnsw {
    GraphNode::GraphNode(const int pointIdx): pointIdx(pointIdx) {
    }

    void GraphNode::link(const int otherIdx) {
        links.push_back_unique(otherIdx);
    }

    bool compareNode(const GraphNode *n1, const GraphNode *n2) {
#ifdef DEBUG_GRAPH_NODE_BUFFER
        if (n1->distance == -1 || n2->distance == -1 || n1->batch != n2->batch) {
            std::cerr << "Node Empty Buffer" << std::endl;
            exit(1000);
        }
#endif
        return n1->distance < n2->distance;
    }

    bool compareNode(GraphNode *n1, GraphNode *n2) {
#ifdef DEBUG_GRAPH_NODE_BUFFER
        if (n1->distance == -1 || n2->distance == -1 || n1->batch != n2->batch) {
            std::cerr << "Node Empty Buffer" << std::endl;
            exit(1000);
        }
#endif
        return n1->distance < n2->distance;
    }

    bool compareNodeInverse(const GraphNode *n1, const GraphNode *n2) {
#ifdef DEBUG_GRAPH_NODE_BUFFER
        if (n1->distance == -1 || n2->distance == -1 || n1->batch != n2->batch) {
            std::cerr << "Node Empty Buffer" << std::endl;
            exit(1000);
        }
#endif
        return n1->distance > n2->distance;
    }

    bool compareNodeInverse(GraphNode *n1, GraphNode *n2) {
#ifdef DEBUG_GRAPH_NODE_BUFFER
        if (n1->distance == -1 || n2->distance == -1 || n1->batch != n2->batch) {
            std::cerr << "Node Empty Buffer" << std::endl;
            exit(1000);
        }
#endif
        return n1->distance > n2->distance;
    }
} // hnsw

namespace hnsw {
    class Layer : public vec::Vec<GraphNode> {
        int dk;
        mutable int searchBatch = 0;


        /**
         * 为一个节点生成距离缓存, 会设置 node 的 batch 和 distance 字段, 将 inQueue 字段设置为 false.
         * @note 由于 node 存放索引值, 所以需要 ptVec 参数.
         * @param ptVec 向量容器
         * @param node 要计算距离缓存的节点
         * @param target 目标向量, 计算节点所代表的向量和其的距离.
         */
        void generateBufferForNode(const Vec<point::Point> &ptVec,
                                   const GraphNode &node,
                                   const point::Point &target) const;

    public:
        /**
         * 创建一个各个节点相互连接的向量节点层.
         * 相距系数是指在向量容器中每 dk 个向量就有一个向量被添加到磁层中.
         * @param dk 相距系数
         */
        explicit Layer(int dk);

        /**
         * 尝试在层中添加一个节点.
         * @note 参数不是直接传进 GraphNode, 而是在内部创建, 所以叫 emplace.
         * @note Layer 会自己决定是否要把此向量添加到层中, 因素是 dk.
         * @param ptVec 向量容器
         * @param pointIdx 要添加的节点在向量容器中的索引
         */
        void emplaceNode(const Vec<point::Point> &ptVec, int pointIdx);

        /**
         * 把 nearest 向量拓展为 topk.
         * @param ptVec 向量容器
         * @param startPtIdx 拓展的起始向量在向量容器中的索引值, 如果此索引值不在此 Layer 中, 返回空数组.
         * @param target 目标向量
         * @param k topK 中的 K, 即查找的最近向量数量.
         * @return 返回最接近向量在向量容器中的索引值, 元素有 k 个, 另一种情况见 startPtIdx.
         * @deprecated
         */
        Vec<int> expandTopK(const Vec<point::Point> &ptVec,
                            int startPtIdx,
                            const point::Point &target,
                            int k) const;

        /**
         * 从给定节点开始, 搜索距离目标向量的最近的 k 个向量.
         * @param ptVec 向量容器
         * @param startPtIdx 拓展的起始向量在向量容器中的索引值, 如果此索引值不在此 Layer 中, 返回空数组.
         * @param target 目标向量
         * @param k topK 中的 K, 即查找的最近向量数量.
         * @return 返回最接近向量在向量容器中的索引值, 元素有 k 个, 另一种情况见 startPtIdx.
         */
        Vec<int> searchKNN(const Vec<point::Point> &ptVec,
                           int startPtIdx,
                           const point::Point &target,
                           int k) const;

        /**
         * 查找层内和目标向量最接近的向量.
         * @param ptVec 向量容器
         * @param startPtIdx 搜索的起始向量在向量容器中的索引值, 如果此索引值不在此 Layer 中, 返回 -1.
         * @param target 目标向量
         * @return 返回最接近向量在向量容器中的索引值, 另一种情况见 startPtIdx.
         */
        int searchNearestNode(const Vec<point::Point> &ptVec,
                              int startPtIdx,
                              const point::Point &target) const;

        /**
         * 改变搜索批次, 使所有节点的距离缓冲都失效.
         * @note 需要在目标搜索向量发生改变 或 inQueue 字段针对的队列发生变化 的时候调用.
         */
        void changeSearchBatch() const;
    };
} // hnsw

namespace hnsw {
    Layer::Layer(const int dk): dk(dk) {
    }

    void Layer::emplaceNode(const Vec<point::Point> &ptVec, const int pointIdx) {
        if (pointIdx % dk != 0) {
            return;
        }
        const GraphNode node(pointIdx);
        const int nodeIdx = length; // 此处 length 应该等于 pointIdx / dk
#ifdef DEBUG_LAYER_EMPLACE_NODE
        printf("DK: %d, Emplacing GraphNode: %d\n", dk, pointIdx);
#endif
        push_back(node); // 此处发生了拷贝, 因此下方以左值使用 node 将会是错误的,
        // ↑ 此处直接添加到 layer 里可以防止 searchNearestNode 遇到空的情况, 同时此节点又与其他节点没有连接, 不会使 nearest 和 nodeIdx 相同.
        const int nearest = searchNearestNode(ptVec, 0, ptVec[pointIdx]);
        changeSearchBatch();
        Vec<int> nearestTopK = searchKNN(ptVec, nearest, ptVec[pointIdx], N_LINK);
        for (const int i: nearestTopK) {
            // 建立双向连接
            if (i / dk != nodeIdx) {
                // 不与自己建立连接
                (*this)[i / dk].link(nodeIdx);
                (*this)[nodeIdx].link(i / dk);
#ifdef DEBUG_LAYER_EMPLACE_NODE
                printf("GraphNode Link: %d-%d, Distance: %.2f\n",
                       i,
                       nodeIdx,
                       ptVec[i].distanceTo(ptVec[pointIdx]));
#endif
            }
        }
    }

    vec::Vec<int> Layer::expandTopK(const Vec<point::Point> &ptVec, const int startPtIdx,
                                    const point::Point &target, const int k) const {
        if (startPtIdx % dk != 0) {
            return Vec<int>();
        }
        if (length <= k) {
            // Layer 节点数不够, 不拓展了, 直接返回
            // 排序
            pq::PriorityQueue<const GraphNode *> pq(length, compareNode);
            for (int i = 0; i < length; i++) {
                const GraphNode &node = arr[i];
                if (node.batch != searchBatch) {
                    generateBufferForNode(ptVec, node, target);
                }
                pq.add(&node);
            }
            // 倒序提取
            Vec<int> rst(length);
            rst.setLength(length);
            for (int i = length - 1; i >= 0; i--) {
                rst[i] = pq.pop()->pointIdx;
            }
            return rst;
        }
        // Layer 节点数足够, 开始拓展节点, 起始这里有点像广度优先搜索.
        pq::PriorityQueue<GraphNode *> rstQueue(k, compareNode); // 为了保持顺序, 用的是优先级队列, 而不是链表.
        mylinklist::LinkedList<GraphNode *> queue;
        GraphNode *node = arr + startPtIdx / dk;

        if (node->batch != searchBatch) {
            generateBufferForNode(ptVec, *node, target);
        }
        node->inQueue = true; // 此处 inQueue 字段表示此节点是否已经被 queue 记录.
        queue.push_back(node);

        while (rstQueue.getSize() + queue.size() < k) {
            node = queue.pop_head(); // 节点都已经缓存过.
            rstQueue.add(node);

            // 遍历所有邻居.
            for (mylinklist::Iterator<int> i = node->links.getIter(); i.hasNext();) {
                const int neighborIdx = i.next();
                // const int size = node->links.size();
                // for (int i = 0; i < size; i++) {
                // int neighborIdx;
                // node->links.retrieve(i, neighborIdx);
                GraphNode *neighbor = arr + neighborIdx;
                // 只有邻居没被搜索过时才加入.
                if (neighbor->batch != searchBatch) {
                    generateBufferForNode(ptVec, *neighbor, target);
                }
                if (!neighbor->inQueue) {
                    neighbor->inQueue = true;
                    queue.push_back(neighbor); // 插入到 queue 的节点都要保证已经缓存过.
                }
            }
        }
        while (!queue.empty()) {
            rstQueue.add(queue.pop_head());
        }
        while (rstQueue.getSize() > k) {
            rstQueue.pop();
        }
        // 拓展完毕, rstQueue 中的节点数一定是 k.
        Vec<int> rst(k);
        rst.setLength(k);
        for (int i = k - 1; i >= 0; i--) {
            rst[i] = rstQueue.pop()->pointIdx;
        }
        return rst;
    }

    vec::Vec<int> Layer::searchKNN(const Vec<point::Point> &ptVec, const int startPtIdx,
                                   const point::Point &target, const int k) const {
        if (startPtIdx % dk != 0) {
            return Vec<int>();
        }
        if (length <= k) {
            // Layer 节点数不够, 不拓展了, 直接返回
            // 排序
            pq::PriorityQueue<const GraphNode *> pq(length, compareNodeInverse);
            // get ↑ 方法取出的是距离最近的节点.
            for (int i = 0; i < length; i++) {
                const GraphNode &node = arr[i];
                if (node.batch != searchBatch) {
                    generateBufferForNode(ptVec, node, target);
                }
                pq.add(&node);
            }
            Vec<int> rst(length);
            while (pq.getSize() > 0) {
                rst.push_back(pq.pop()->pointIdx);
            }
            return rst;
        }
        pq::PriorityQueue<const GraphNode *> topQueue(2 * k, compareNode);
        pq::PriorityQueue<const GraphNode *> candidateQueue(2 * k, compareNodeInverse);
        const GraphNode *node = arr + startPtIdx / dk;
        if (node->batch != searchBatch) {
            generateBufferForNode(ptVec, *node, target);
        }
        double lowerBound = node->distance;
        topQueue.add(node);
        candidateQueue.add(node);
        node->inQueue = true; // 此处 inQueue 表示是否被添加过到 topQueue(结果队列) 中, 即是否被访问过(visited).
        while (candidateQueue.getSize() != 0) {
            node = candidateQueue.pop(); // 取得一个距离最近的候选人.
            if (node->distance > lowerBound) {
                // 最近的候选人节点都比 topQueue 中最远的节点要远, 故结束循环.
                break;
            }
            // 遍历每个邻居.
            for (mylinklist::Iterator<int> i = node->links.getIter(); i.hasNext();) {
                const int neighborIdx = i.next();
                const GraphNode &neighbor = arr[neighborIdx];
                if (neighbor.batch != searchBatch) {
                    generateBufferForNode(ptVec, neighbor, target);
                }
                if (!neighbor.inQueue) {
                    neighbor.inQueue = true;
                    // 判断邻居是否要添加进 topQueue.
                    if (topQueue.getSize() < k || neighbor.distance < lowerBound) {
                        topQueue.add(&neighbor);
                        candidateQueue.add(&neighbor);

                        // 添加了节点, 所以要限制 topQueue 数量.
                        while (topQueue.getSize() > k) {
                            topQueue.pop(); // 删除距离最远的节点.
                        }
                        lowerBound = topQueue.get()->distance;
                    }
                }
            }
        }
        Vec<int> rst(k);
        rst.setLength(k);
        for (int i = k - 1; i >= 0; i--) {
            rst[i] = topQueue.pop()->pointIdx;
        }
        return rst;
    }

    void Layer::generateBufferForNode(const Vec<point::Point> &ptVec, const GraphNode &node,
                                      const point::Point &target) const {
        node.batch = searchBatch;
        node.distance = ptVec[node.pointIdx].distanceTo(target);
        node.inQueue = false;
    }

    int Layer::searchNearestNode(const Vec<point::Point> &ptVec, const int startPtIdx,
                                 const point::Point &target) const {
        // 起始向量不在此层.
        if (startPtIdx % dk != 0) {
            return -1;
        }
        GraphNode *node = arr + startPtIdx / dk;
        if (node->batch != searchBatch) {
            generateBufferForNode(ptVec, *node, target);
        }
        GraphNode *newNode = node;
#ifdef DEBUG_LAYER_SEARCH_NEAREST_STEP
        printf("Search Step:");
#endif
        while (newNode != nullptr) {
#ifdef DEBUG_LAYER_SEARCH_NEAREST_STEP
            printf(" %5d(%.2f)", newNode->pointIdx, newNode->distance);
#endif
            node = newNode;
            newNode = nullptr;
            // const int neighborCnt = node->links.size();
            // for (int i = 0; i < neighborCnt; i++) {
            // 遍历每个邻居
            // int neighborIdx;
            // node->links.retrieve(i, neighborIdx);
            for (mylinklist::Iterator<int> i = node->links.getIter(); i.hasNext();) {
                const int neighborIdx = i.next();
                GraphNode &neighbor = arr[neighborIdx];
                if (neighbor.batch != searchBatch) {
                    generateBufferForNode(ptVec, neighbor, target);
                }
                // 找到 distance 更小的邻居
                if (neighbor.distance < node->distance) {
                    // 找到 distance 最小的邻居
                    if (newNode == nullptr || neighbor.distance < newNode->distance) {
                        newNode = &neighbor;
                    }
                }
            }
        }
#ifdef DEBUG_LAYER_SEARCH_NEAREST_STEP
        printf("\n");
#endif
        return node->pointIdx;
    }

    void Layer::changeSearchBatch() const {
        searchBatch++;
    }
} // hnsw

namespace hnsw {
    /**
     * 见 queryLinks
     */
    class NotDebuggingException final : public std::exception {
    public:
        const char *what() const noexcept override {
            return "Program is not debugging";
        }
    };

    class HNSW {
        const vec::Vec<point::Point> *ptVec;
        /**
         * 每个向量都要在 layer0 中创建节点, layer1/2/3 中是每 DK_LAYER1/2/3 个向量放一个节点.
         * @note 靠后的 layer 中的节点必须要在前面的节点中出现, 以便不同层次间的切换, 此需要靠 DK_LAYER1/2/3 的值来保证
         *      此时后面层的节点如果代表 pointIdx 向量索引位置的节点, 则可以通过 pointIdx / DK_LAYER1 * DK_LAYER2
         *      (假设后面层是 layer2, 前面的层是 layer1) 得到前一层的表示 pointIdx 的节点的索引值.
         */
        Layer layer0 = Layer(1);
        /**
         * 见 @code layer0
         */
        Layer layer1 = Layer(DK_LAYER1);
        /**
         * 见 @code layer0
         */
        Layer layer2 = Layer(DK_LAYER2);
        /**
         * 见 @code layer0
         */
        Layer layer3 = Layer(DK_LAYER3);

        /**
         * 用向量容器中的值来构建所有的层.
         */
        void buildLayers();

    public:
        /**
         * @param ptVec 所有向量的容器
         */
        HNSW(const vec::Vec<point::Point> *ptVec);

        /**
         * 找到和目标向量最接近的向量.
         * @param target 目标向量
         * @param k 最近向量数量
         * @return 最接近的向量在向量容器中的索引值, 元素数有 k 个.
         */
        vec::Vec<int> searchNearestTopK(const point::Point &target, int k) const;

        /**
         * 此方法从标准输入中读取向量在向量容器中的索引, 然后输出 layer0 中的对应节点所拥有的 links.
         * @param distanceReference 显示每个节点和此向量的距离, 如果提供 nullptr 则距离代表节点和查询节点的距离.
         * @note 此方法仅做测试用, 在宏 DEBUG_HNSW_QUERY_LINKS 没被定义时调用会抛出 NotDebuggingException 异常.
         */
        void queryLinks(const point::Point *distanceReference = nullptr) const;
    };
} // hnsw

namespace hnsw {
    HNSW::HNSW(const vec::Vec<point::Point> *ptVec): ptVec(ptVec) {
        buildLayers();
    }

    void HNSW::buildLayers() {
        const int len = ptVec->getLength();
        for (int i = 0; i < len; i++) {
            // 注意这里也要改变搜索批次
            layer3.changeSearchBatch();
            layer2.changeSearchBatch();
            layer1.changeSearchBatch();
            layer0.changeSearchBatch();
#ifdef DEBUG_HNSW_BUILD_LAYERS
            if (i % 3 == 0) {
                printf("Building...now: %d, tol: %d\n", i, len);
            }
#endif
            layer0.emplaceNode(*ptVec, i);
            layer1.emplaceNode(*ptVec, i);
            layer2.emplaceNode(*ptVec, i);
            layer3.emplaceNode(*ptVec, i);
        }
    }

    vec::Vec<int> HNSW::searchNearestTopK(const point::Point &target, const int k) const {
        layer3.changeSearchBatch();
        layer2.changeSearchBatch();
        layer1.changeSearchBatch();
        layer0.changeSearchBatch();

        // 先在最稀疏的 layer3 中搜索.
        int nearestPt = layer3.searchNearestNode(*ptVec, 0, target); // 只需要一个
        nearestPt = layer2.searchNearestNode(*ptVec, nearestPt, target);
        nearestPt = layer1.searchNearestNode(*ptVec, nearestPt, target);
        nearestPt = layer0.searchNearestNode(*ptVec, nearestPt, target);

        layer0.changeSearchBatch(); // inQueue 针对的队列发生变化
        return layer0.searchKNN(*ptVec, nearestPt, target, k); // 搜索到最接近的向量后拓展为 topk
    }

    void HNSW::queryLinks(const point::Point *distanceReference) const {
#ifndef DEBUG_HNSW_QUERY_LINKS
        throw NotDebuggingException();
#endif
        int ptIdx;
        while (scanf("%d", &ptIdx) == 1) {
            double distance;
            if (distanceReference != nullptr) {
                distance = distanceReference->distanceTo(ptVec->operator[](ptIdx));
            } else {
                distance = 0;
            }
            printf("%5d(%.2f) links(distance): ", ptIdx, distance);
            for (mylinklist::Iterator<int> i = layer0[ptIdx].links.getIter(); i.hasNext();) {
                const int neighborIdx = i.next();
                printf("%5d(%.2f) ", neighborIdx,
                       ptVec->operator[](neighborIdx).distanceTo(
                           distanceReference != nullptr
                               ? *distanceReference
                               : ptVec->operator[](ptIdx)));
            }
            printf("\n");
        }
    }
} // hnsw

using namespace point;
using namespace vec;
using namespace hnsw;

extern vec::Vec<int> baselineSearch(const vec::Vec<point::Point> &vec,
                                    const point::Point &query, const int k);

#define OUTPUT

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

int main() {
    main1();
}
