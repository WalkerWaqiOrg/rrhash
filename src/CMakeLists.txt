cmake_minimum_required (VERSION 2.8)

add_compile_options(-O2 -I../src/ -std=c++11 -std=gnu++11)

project (build)

add_subdirectory(ExpandRand)

add_library(build MODULE ../src/0000_AlgoSet.cpp ../src/1100_HeapSort.cpp ../src/1200_QuickSort.cpp ../src/1300_MergeSort.cpp ../src/1400_PrioQueue.cpp ../src/1500_BinomialHeap.cpp ../src/1600_FibonacciHeap.cpp ../src/1700_DijkstraShortestPath.cpp ../src/1800_PrimMST.cpp ../src/1900_KruskalMST.cpp ../src/2000_HuffmanCoding.cpp ../src/2100_JobSequencing.cpp ../src/2200_IncreasingConsective.cpp ../src/2300_BinarySearch.cpp ../src/2400_KthSmallest.cpp ../src/2500_KnapSack01.cpp ../src/2600_LeastSQ.cpp ../src/2700_CuckooHash.cpp ../src/2800_BloomFilter.cpp ../src/2900_SuffixArray.cpp ../src/3000_SuffixTrie.cpp ../src/3100_AVLTree.cpp ../src/3200_BinarySearchTree.cpp ../src/3300_AutomataPatternSearch.cpp ../src/3400_BTree.cpp ../src/3500_KMPstringmatch.cpp ../src/3600_MatrixInverse.cpp ../src/3700_QuadTree.cpp ../src/3800_RedBlackTree.cpp ../src/3900_ReversePolishNotation.cpp ../src/4000_RobinKarp.cpp ../src/4100_Viterbi.cpp ../src/4200_ZArrayPatternSearch.cpp)

target_link_libraries(build ExpandRand stdc++)

