set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11 -I../../src/ExpandRand -fPIC")
add_library (ExpandRand ../../src/ExpandRand/expandRand.cpp)
