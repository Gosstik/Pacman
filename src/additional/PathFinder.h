//#pragma once
//
//#include <iostream>
//#include <queue>
//#include <stack>
//#include "Map.h"
//#include "Globals.h"
//
//
// namespace my {
//
//    struct NumDir {
//        std::pair<int, int> Right = {1, 0};
//        std::pair<int, int> Left = {-1, 0};
//        std::pair<int, int> Up = {0, -1};
//        std::pair<int, int> Down = {0, 1};
//    };
//
//    struct Cell {
//        int row, col; // current position
//        int GValue; // total distance already travelled to reach the node
//        int FValue;  // FValue = GValue + remaining distance estimate (smaller
//        FValue gets priority)
//
//        Cell(int r, int c, int g = -1, int f = -1);
//        bool isValid() const;
//        bool isBlocked(const Map& map) const;
//        bool coincide(const Cell& dest) const;
//        int getGValue() const;
//        int getFValue() const;
//        void calculateFValue(const Cell& dest);
//        void calculateGValue();
//        int getHValue(const Cell& dest) const; // HValue == guess distance
//        between current cell and destination friend bool operator<(const Cell
//        &a, const Cell &b); // Determine order in the priority_queue
//    };
//
//    void updateDirections(std::vector<std::vector<Dir>>& directions,
//    std::pair<int, int>& currDir, NumDir& numDir, Cell& successor);
//
//    std::stack<Dir> tracePath(const std::vector<std::vector<Dir>>& directions,
//    const Cell& src, const Cell& dest);
//
///// A-star algorithm.
//// The path returned is a stack of direction. Starting from source and moving
///according stack.top() / we can reach destination
//    std::stack<Dir> pathFind(const Map& map, const Cell& src, const Cell& dest
//    = {aboveCageCenterMapY, rightAboveCageCenterMapX});
//
////// A Utility Function to check correctness of the algorithm
////void printDir(const std::vector<std::vector<Dir>>& directions)
//}