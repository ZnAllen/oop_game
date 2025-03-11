#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>

struct Node {
    int x, y;
    int gCost, hCost, fCost;
    Node* parent;

    Node(int _x, int _y, int _gCost, int _hCost, Node* _parent = nullptr) 
        : x(_x), y(_y), gCost(_gCost), hCost(_hCost), parent(_parent) {
        fCost = gCost + hCost;
    }
};

// 比較函式，用於優先隊列
struct CompareNode {
    bool operator()(const Node* a, const Node* b) {
        return a->fCost > b->fCost;
    }
};

// A* 導航函式
std::vector<std::pair<int, int>> AStar(int startX, int startY, int goalX, int goalY, const class Map& map);

#endif // PATHFINDING_H
