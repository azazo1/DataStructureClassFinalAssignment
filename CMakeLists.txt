cmake_minimum_required(VERSION 3.22)
project(DataStructureClassFinalAssignment)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(
#        -DDEBUG
#        -DDEBUG_SINGLE_SEARCH
#        -DDEBUG_GRAPH_NODE_BUFFER
#        -DDEBUG_VEC_SET_LENGTH
#        -DDEBUG_HNSW_BUILD_LAYERS
#        -DDEBUG_READING_FILE
#        -DDEBUG_LAYER_EMPLACE_NODE
)

add_executable(BaselineTrial
        BaselineTrial.cpp
        src/point/Point.cpp
        src/vec/Vec.h
        src/vec/Vec.tpp
        src/hashset/HashSet.cpp
        src/hashset/HashSet.h
        src/linkedlist/LinkedList.tpp
        src/linkedlist/LinkedList.h
)

add_executable(HnswTrial
        HnswTrial.cpp
        src/hnsw/GraphNode.cpp
        src/hnsw/GraphNode.h
        src/hnsw/HNSW.cpp
        src/hnsw/HNSW.h
        src/hnsw/Layer.cpp
        src/hnsw/Layer.h
        src/linkedlist/LinkedList.h
        src/linkedlist/LinkedList.tpp
        src/vec/Vec.tpp
        src/vec/Vec.h
        src/point/Point.cpp
        src/point/Point.h
        src/pq/PriorityQueue.h
        src/pq/PriorityQueue.tpp

        BaselineTrial.cpp
        src/hashset/HashSet.cpp
        src/hashset/HashSet.h
)

add_executable(MergedResult MergedResult.cpp)
