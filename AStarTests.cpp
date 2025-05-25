#include <iostream>
#include <cassert>
#include "Astar.h"


#if TESTS

void test_pathExists() {
    int width = 3, height = 3;
    Astar astar(width, height, heuristicType::manhattan, { 0, 0 }, { 2, 2 });

    auto path = astar.findPath();
    assert(!path.empty());
    assert(path.front() == std::make_pair(0, 0));
    assert(path.back() == std::make_pair(2, 2));
    std::cout << "test_pathExists  passed.\n";
}

void test_sameStartEnd_throws() {
    try {
        Astar astar(3, 3, heuristicType::manhattan, { 1, 1 }, { 1, 1 });
        assert(false);
    }
    catch (const std::invalid_argument& e) {
        assert(std::string(e.what()) == "Start and end points are the same");
        std::cout << "test_sameStartEnd_throws passed.\n";
    }
}

void test_noPath_throws() {
    int width = 2, height = 2;
    Astar astar(width, height, heuristicType::manhattan, { 0, 0 }, { 1, 1 });
    astar.setObstacle(0, 1);
    astar.setObstacle(1, 0);

    try {
        astar.findPath();
        assert(false); 
    }
    catch (const std::invalid_argument& e) {
        std::cout << "test_noPath_throws passed.\n";
    }
}

void test_correctPathLength() {
    int width = 2, height = 2;
    Astar astar(width, height, heuristicType::manhattan, { 0, 0 }, { 1, 1 });
    auto path = astar.findPath();
    assert(static_cast<int>(path.size()) == 3); // (0,0) → (0,1) → (1,1)
    std::cout << "test_correctPathLength passed.\n";
}

int main() {
    test_pathExists();
    test_sameStartEnd_throws();
    test_noPath_throws();
    test_correctPathLength();

    std::cout << "All tests passed.\n";
    return 0;
}

#endif
