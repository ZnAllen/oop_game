#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include <vector>

enum class TileType { EMPTY, MOUNTAIN, WATER, GRASS, CASTLE, OBSTACLE };

struct Tile {
    TileType type;
    bool isVisible;  // 是否已探索
    bool isDiscovered; // 草地專用：是否已被發現
};

class Map {
private:
    int width, height;           
    std::vector<std::vector<Tile>> grid;  

public:
    int tileSize;
    Map(int width, int height, int tileSize);
    void Draw();  
    void RevealArea(int x, int y, int radius, TileType fromType);  
    bool IsWalkable(int x, int y, bool hasWaterTool) const;  
    TileType GetTileType(int x, int y) const;
};

#endif // MAP_H
