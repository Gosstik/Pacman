//#include "PathFinder.h"
//
//
// namespace my {
//    Cell::Cell(int r, int c, int g, int f) : row(r), col(c), GValue(g),
//    FValue(f) {}
//
//    int Cell::getGValue() const {
//        return GValue;
//    }
//
//    int Cell::getFValue() const {
//        return FValue;
//    }
//
//    bool Cell::isValid() const {
//        return row >= 0 && row < mapHeightCell && col >= 0 && col <
//        mapWidthCell;
//    }
//
//    bool Cell::isBlocked(const Map& map) const {
//        return map[row][col] == 'W';
//    }
//
//    bool Cell::coincide(const Cell& dest) const {
//        return row == dest.row && col == dest.col;
//    }
//
//    void Cell::calculateFValue(const Cell& dest) {
//        FValue = GValue + getHValue(dest);
//    }
//
//    void Cell::calculateGValue() {
//        GValue += 1;
//    }
//
//    int Cell::getHValue(const Cell& dest) const {
//        int res = abs(dest.row - row) + abs(dest.col - col); // Manhattan
//        Distance (only for 4 directions) return res;
//    }
//
//    bool operator<(const Cell &a, const Cell &b) {
//        return a.getFValue() > b.getFValue();
//    }
//
//    void updateDirections(std::vector<std::vector<Dir>>& directions,
//    std::pair<int, int>& currDir, NumDir& numDir, Cell& successor) {
//        if (currDir == numDir.Right)
//            directions[successor.row][successor.col] = Dir::Left;
//        else if (currDir == numDir.Left)
//            directions[successor.row][successor.col] = Dir::Right;
//        else if (currDir == numDir.Up)
//            directions[successor.row][successor.col] = Dir::Down;
//        else if (currDir == numDir.Down)
//            directions[successor.row][successor.col] = Dir::Up;
//    }
//
//    std::stack<Dir> tracePath(const std::vector<std::vector<Dir>>& directions,
//    const Cell& src, const Cell& dest) {
//        Cell currLoc = dest;
//        std::stack<Dir> path;
//        // Start from destination cell and move according data in
//        directions[][].
//        // On each step we invert direction (if Left => push Right, ...)
//        because we want
//        // returned stack to contain directions starting from source cell.
//        while (!currLoc.coincide(src)) {
//            if (directions[currLoc.row][currLoc.col] == Dir::Right) {
//                ++currLoc.col;
//                path.push(Dir::Left);
//            }
//            else if (directions[currLoc.row][currLoc.col] == Dir::Left) {
//                --currLoc.col;
//                path.push(Dir::Right);
//            }
//            else if (directions[currLoc.row][currLoc.col] == Dir::Up) {
//                --currLoc.row;
//                path.push(Dir::Down);
//            }
//            else if (directions[currLoc.row][currLoc.col] == Dir::Down) {
//                ++currLoc.row;
//                path.push(Dir::Up);
//            }
//        }
//        return path;
//    }
//
//// A-star algorithm.
//// The path returned is a stack  of direction digits.
// std::stack<Dir> pathFind(const Map& map, const Cell& src, const Cell& dest) {
//     // check trivial cases
//     if (!src.isValid()) {
//         std::cerr << "Source is invalid\n";
//         exit(7);
//     }
//     else if (!dest.isValid()) {
//         std::cerr << "Destination is invalid\n";
//         exit(7);
//     }
//     else if (src.isBlocked(map)) {
//         std::cerr << "Source is blocked\n";
//         exit(7);
//     }
//     else if (dest.isBlocked(map)) {
//         std::cerr << "Destination is blocked\n";
//         exit(7);
//     }
//     else if (src.coincide(dest)) {
//         return {};
//     }
//
//     // closedNodes - 2D boolean vector that shows what nodes have already
//     been in the priority_queue
//     // (true => this cell has been deleted from priority_queue
//     std::vector<std::vector<bool>> closedNodes(mapHeightCell,
//     std::vector<bool>(mapWidthCell, false));
//     // directions - 2D vector which contains directions pointing to the node
//     from which we got to the current one std::vector<std::vector<Dir>>
//     directions(mapHeightCell, std::vector<Dir>(mapWidthCell, Dir::Stop));
//     // FValue - 2D vector of FValues (-1 means that node has not been handled
//     yet std::vector<std::vector<int>> FValue(mapHeightCell,
//     std::vector<int>(mapWidthCell, -1));
//     // fourDir - 2D vector of all possible directions (Down, Up, Left, Right)
//     std::vector<std::pair<int, int>> fourDir {{0, 1}, {0, -1}, {-1, 0}, {1,
//     0}}; NumDir numDir;
//
//     std::priority_queue<Cell> queue;
//     queue.emplace(src);
//
//     while (!queue.empty()) {
//         // extracting parent to handle its successors
//         Cell parent = queue.top();
//         queue.pop();
//
//         closedNodes[parent.row][parent.col] = true;
//
//         // Generating all 4 successors of current parent (Down, Up, Left,
//         Right) and handle them for (auto& currDir : fourDir) {
//             Cell successor(parent.row + currDir.second, parent.col +
//             currDir.first, parent.getGValue()); if (successor.isValid()) {
//                 // If the destination cell is the same as the current
//                 successor if (successor.coincide(dest)) {
////                    std::cout << "4\n";
//                    updateDirections(directions, currDir, numDir, successor);
//                    return tracePath(directions, src, dest);
//                }
//                    // If the successor is already on the
//                    // closed list or if it is blocked, then
//                    // ignore it. Else do the following
//                else if (!closedNodes[successor.row][successor.col] &&
//                !successor.isBlocked(map)) {
//                    successor.calculateGValue();
//                    //std::cout << "successor.getGValue() = " <<
//                    successor.getGValue() << std::endl;
//                    successor.calculateFValue(dest);
//                    if (FValue[successor.row][successor.col] == -1 ||
//                    FValue[successor.row][successor.col] >
//                    successor.getFValue()) {
//                        //std::cout << "emplace successor: " <<
//                        successor.getLocation().row << " " <<
//                        successor.getLocation().col << std::endl;
//                        queue.emplace(successor);
//                        FValue[successor.row][successor.col] =
//                        successor.getFValue(); updateDirections(directions,
//                        currDir, numDir, successor);
//                    }
//                }
//            }
//        }
//    }
//    // When the destination cell is not found and the open
//    // list is empty, then we conclude that we failed to
//    // reach the destination cell. This may happen when the
//    // there is no way to destination cell (due to
//    // blockages)
//    std::cerr << "Failed to find the Destination Cell\n";
//    exit(7);
//}
//
////// A Utility Function to check correctness of the algorithm
////void printDir(const std::vector<std::vector<Dir>>& directions) {
////    for (auto& line : directions) {
////        for (auto&& elem : line) {
////            if (elem == Dir::Right)
////                std::cout << "Right ";
////            else if (elem == Dir::Left)
////                std::cout << "Left  ";
////            else if (elem == Dir::Up)
////                std::cout << "Up    ";
////            else if (elem == Dir::Down)
////                std::cout << "Down  ";
////            else if (elem == Dir::Stop)
////                std::cout << "..... ";
////        }
////        std::cout << std::endl;
////    }
////}
//}