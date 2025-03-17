#include "Unit.h"
#include <iostream>
#include <cstdio>

Unit::Unit(int startX, int startY, float speed, bool enemy) 
    : x(startX), y(startY), step_x(startX), step_y(startY), moveSpeed(speed), isEnemy(enemy), 
      hp(100), attackRange(1), attackDamage(20), isSelected(false), moving(false), pathIndex(0), nextStepTimer(0), spendTime(2), attackCooldownTime(2),
      action(Action::NONE){}

void Unit::AddWaypoint(int newX, int newY) {
    if (waypoints.empty() || waypoints.back() != std::make_pair(newX, newY)) {
        waypoints.push_back({newX, newY});
    }
}

void Unit::ConfirmPath(Map& map) {
    std::vector<std::pair<int, int>> newPath;
    int lastX = x, lastY = y;
    
    for (auto& wp : waypoints) {
        std::vector<std::pair<int, int>> path = AStar(lastX, lastY, wp.first, wp.second, map);
        
        if (!path.empty()) {
            newPath.insert(newPath.end(), path.begin(), path.end());
            lastX = wp.first;
            lastY = wp.second;
        }
    }

    if (!newPath.empty()) {
        paths.push_back(newPath);
    }

    waypoints.clear();
    moving = !paths.empty();
    pathIndex = 0;
}


void Unit::ClearPath() {
    step_x = x;
    step_y = y;
    nextStepTimer = 0;
    waypoints.clear();
    paths.clear();
    moving = false;
    pathIndex = 0;
}

void Unit::Update(Map& map) {
    if (moving && !paths.empty()) {
        auto& currentPath = paths.front();
        if (pathIndex < currentPath.size()) {
            if(nextStepTimer > 0){
                if(step_x > currentPath[pathIndex].first) step_x -= (1.0/spendTime)*GetFrameTime();
                else if(step_x < currentPath[pathIndex].first) step_x += (1.0/spendTime)*GetFrameTime();
                else if(step_y > currentPath[pathIndex].second) step_y -= (1.0/spendTime)*GetFrameTime();
                else if(step_y < currentPath[pathIndex].second) step_y += (1.0/spendTime)*GetFrameTime();
                nextStepTimer -= GetFrameTime();
            }else{
                nextStepTimer = spendTime;
                x = currentPath[pathIndex].first;
                y = currentPath[pathIndex].second;
                step_x = x;
                step_y = y;
                pathIndex ++;
                if(currentPath[pathIndex - 1].first == currentPath[pathIndex].first && currentPath[pathIndex - 1].second == currentPath[pathIndex].second) pathIndex++;
                int vision = (map.GetTileType(x, y) == TileType::MOUNTAIN) ? 2 : 1;
                map.RevealArea(x, y, vision, map.GetTileType(x, y));
            }
        }
        if (pathIndex >= currentPath.size()) {
            pathIndex = 0;
            nextStepTimer = 0;
            paths.erase(paths.begin()); // 刪除已完成的路線
        }

        if (paths.empty()) {
            moving = false;
        }
    }
}


void Unit::Draw(int TILE_SIZE) {
// 畫出單位

    DrawCircle(step_x * TILE_SIZE + TILE_SIZE / 2, step_y * TILE_SIZE + TILE_SIZE / 2, TILE_SIZE / 3, unitColor);
    // 繪製規劃中的路線（白色）
    if (!waypoints.empty()) {
        int lastX = x, lastY = y;
        for (auto& wp : waypoints) {
            int worldX1 = lastX * TILE_SIZE + TILE_SIZE / 2;
            int worldY1 = lastY * TILE_SIZE + TILE_SIZE / 2;
            int worldX2 = wp.first * TILE_SIZE + TILE_SIZE / 2;
            int worldY2 = wp.second * TILE_SIZE + TILE_SIZE / 2;

            DrawLine(worldX1, worldY1, worldX2, worldY2, WHITE);
            lastX = wp.first;
            lastY = wp.second;
        }
    }

    // 繪製已確認的路線（藍色）
    for (auto& path : paths) {
        for (size_t i = 1; i < path.size(); i++) {
            int worldX1 = path[i - 1].first * TILE_SIZE + TILE_SIZE / 2;
            int worldY1 = path[i - 1].second * TILE_SIZE + TILE_SIZE / 2;
            int worldX2 = path[i].first * TILE_SIZE + TILE_SIZE / 2;
            int worldY2 = path[i].second * TILE_SIZE + TILE_SIZE / 2;

            DrawLine(worldX1, worldY1, worldX2, worldY2, BLUE);
        }
    }
    int barWidth = TILE_SIZE - 4;
    int barHeight = 5;
    int barX = x * TILE_SIZE + 2;
    int barY = y * TILE_SIZE - 10;
    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    DrawRectangle(barX, barY, barWidth * hp / 100, barHeight, RED);
}

void Unit::Attack(Unit& target) {
    target.hp -= attackDamage;
    if (target.hp <= 0) {
        target.hp = 0; // 避免負數
    }
}
