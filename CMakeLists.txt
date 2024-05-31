cmake_minimum_required(VERSION 3.22)
project(DataStructureClassFinalAssignment)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(
#        -DOUTPUT
        #        -DDEBUG_INSTANT_SPEED
                -DDEBUG_TOTAL_TIME
        #        -DDEBUG_SINGLE_SEARCH
        #        -DDEBUG_GRAPH_NODE_BUFFER
        #        -DDEBUG_VEC_SET_LENGTH
        #        -DDEBUG_HNSW_BUILD_LAYERS
        #        -DDEBUG_HNSW_QUERY_LINKS
        #        -DDEBUG_READING_FILE
        #        -DDEBUG_LAYER_EMPLACE_NODE
        #        -DDEBUG_LAYER_SEARCH_NEAREST_STEP
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
add_executable(OtherHNSW lib/hnsw.cpp
        lib/hnsw.cpp)
add_executable(hnswlib
        lib/hnswlib/hnswlib.h
        lib/hnswlib/bruteforce.h
        lib/hnswlib/hnswalg.h
        lib/hnswlib/space_ip.h
        lib/hnswlib/space_l2.h
        lib/hnswlib/visited_list_pool.h
        lib/hnswlib/stop_condition.h
        LibTrial.cpp
)