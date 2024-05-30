cmake_minimum_required(VERSION 3.22)
project(DataStructureClassFinalAssignment)

set(CMAKE_CXX_STANDARD 17)

add_executable(DataStructureClassFinalAssignment
        main.cpp
        BaselineTrial.cpp
        src/point/Point.cpp
        src/vec/Vec.h
        src/vec/Vec.tpp
        src/hashset/HashSet.cpp
        src/hashset/HashSet.h
        src/linkedlist/LinkedList.tpp
        src/linkedlist/LinkedList.h
        src/hnsw/HNSW.cpp
        src/hnsw/HNSW.h
        src/hnsw/GraphNode.cpp
        src/hnsw/GraphNode.h
        src/hnsw/Layer.cpp
        src/hnsw/Layer.h
        src/pq/PriorityQueue.h
)

add_executable(MergedResult MergedResult.cpp)
