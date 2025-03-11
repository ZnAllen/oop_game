#include "Pathfinding.h"
#include "Map.h"
#include <algorithm>
#include <unordered_set>

std::vector<std::pair<int, int>> AStar(int startX, int startY, int goalX, int goalY, const Map& map) {
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet;
    std::unordered_map<int, Node*> allNodes;
    std::unordered_set<int> closedSet;

    auto getKey = [](int x, int y) { return x * 1000 + y; }; // 簡單哈希函數

    Node* startNode = new Node(startX, startY, 0, std::abs(goalX - startX) + std::abs(goalY - startY));
    openSet.push(startNode);
    allNodes[getKey(startX, startY)] = startNode;

    std::vector<std::pair<int, int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->x == goalX && current->y == goalY) {
            std::vector<std::pair<int, int>> path;
            while (current) {
                path.push_back({current->x, current->y});
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet.insert(getKey(current->x, current->y));

        for (auto& dir : directions) {
            int nx = current->x + dir.first;
            int ny = current->y + dir.second;

            if (!map.IsWalkable(nx, ny, false) || closedSet.count(getKey(nx, ny))) continue;

            int newGCost = current->gCost + 1;
            int newHCost = std::abs(goalX - nx) + std::abs(goalY - ny);
            int newFCost = newGCost + newHCost;

            if (allNodes.count(getKey(nx, ny)) && allNodes[getKey(nx, ny)]->gCost <= newGCost)
                continue;

            Node* neighbor = new Node(nx, ny, newGCost, newHCost, current);
            openSet.push(neighbor);
            allNodes[getKey(nx, ny)] = neighbor;
        }
    }

    return {}; // 沒有找到路徑
}
