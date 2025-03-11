#include "Map.h"

Map::Map(int w, int h, int tSize) : width(w), height(h), tileSize(tSize) {
    grid.resize(height, std::vector<Tile>(width, { TileType::EMPTY, false, false }));

    // 測試地形：障礙物、城堡、山、海、草地
    grid[3][3].type = TileType::OBSTACLE;
    grid[5][5].type = TileType::CASTLE;
    grid[7][7].type = TileType::MOUNTAIN;
    grid[2][6].type = TileType::WATER;
    grid[4][4].type = TileType::GRASS;
}

void Map::Draw() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color color;
            switch (grid[y][x].type) {
                case TileType::EMPTY:    color = GREEN; break;
                case TileType::MOUNTAIN: color = DARKBROWN; break;
                case TileType::WATER:    color = BLUE; break;
                case TileType::GRASS:    color = DARKGREEN; break;
                case TileType::CASTLE:   color = BLUE; break;
                case TileType::OBSTACLE: color = DARKGRAY; break;
            }

            if (!grid[y][x].isVisible) {
                color = BLACK;  // 迷霧未探索區
            }
            
            // 草地額外處理
            if (grid[y][x].type == TileType::GRASS && !grid[y][x].isDiscovered) {
                color = DARKGREEN;  // 草地未被發現
            }

            DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, color);
            DrawRectangleLines(x * tileSize, y * tileSize, tileSize, tileSize, WHITE);
        }
    }
}

// 更新視野，山地會遮擋較矮的格子
void Map::RevealArea(int x, int y, int radius, TileType fromType) {
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                // 視野受山脈影響
                if (grid[ny][nx].type == TileType::MOUNTAIN && fromType != TileType::MOUNTAIN) {
                    continue;
                }
                // 如果該格是草地且未發現過，標記為已發現
                if (grid[ny][nx].type == TileType::GRASS && !grid[ny][nx].isDiscovered) {
                    grid[ny][nx].isDiscovered = true; // 進入草地才會更新視野
                }
                grid[ny][nx].isVisible = true;
            }
        }
    }
}

// 移動檢測：水域需要特殊工具
bool Map::IsWalkable(int x, int y, bool hasWaterTool) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    if (grid[y][x].type == TileType::OBSTACLE) return false;
    if (grid[y][x].type == TileType::WATER && !hasWaterTool) return false;
    return true;
}

TileType Map::GetTileType(int x, int y) const {
    return grid[y][x].type;
}
