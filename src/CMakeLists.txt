cmake_minimum_required(VERSION 3.0)

project(rrhash)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=gnu++11 -pthread")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(ExpandRand)

add_library(${PROJECT_NAME} MODULE
            ${CMAKE_CURRENT_SOURCE_DIR}/0000_AlgoSet.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/1100_HeapSort.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/1200_QuickSort.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/1300_MergeSort.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/1400_PrioQueue.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/1500_BinomialHeap.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/1600_FibonacciHeap.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/1700_DijkstraShortestPath.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/1800_PrimMST.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/1900_KruskalMST.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/2000_HuffmanCoding.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/2100_JobSequencing.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/2200_IncreasingConsective.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/2300_BinarySearch.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/2400_KthSmallest.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/2500_KnapSack01.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/2600_LeastSQ.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/2700_CuckooHash.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/2800_BloomFilter.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/2900_SuffixArray.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3000_SuffixTrie.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3100_AVLTree.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3200_BinarySearchTree.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3300_AutomataPatternSearch.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3400_BTree.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3500_KMPstringmatch.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3600_MatrixInverse.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3700_QuadTree.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3800_RedBlackTree.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/3900_ReversePolishNotation.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/4000_RobinKarp.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/4100_Viterbi.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/4200_ZArrayPatternSearch.cpp)
ADD_DEPENDENCIES(${PROJECT_NAME} ExpandRand)

target_link_libraries(${PROJECT_NAME} ExpandRand stdc++ pthread)

add_executable(rrtest ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)

IF (WIN32)
	target_link_libraries(rrtest dl psapi)
ELSE ()
	target_link_libraries(rrtest dl)
ENDIF ()

ADD_DEPENDENCIES(rrtest ${PROJECT_NAME})