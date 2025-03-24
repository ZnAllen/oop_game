#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Unit.h"
#include "Map.h"
#include <vector>

class GameManager {
public:
    std::vector<Unit> units;
    std::vector<Unit> unitsEnemy;
    int selectedUnitIndex;
    int Soldiers;
    float selectedSoldiers;
    float maxSoldiers;   

    GameManager();
    void AddWaypoint(int newX, int newY);
    void AddEnemy(int x, int y, float speed, int soldier);
    void ConfirmPath(Map& map);
    void ClearPath();
    void AddUnit(int x, int y, float speed, int soldier);
    void HandleClick(int mouseX, int mouseY, int tileSize);
    void Update(Map& map);
    void Draw(int tileSize, Map& map);
    void DrawUI(Map& map);  // 新增 UI 繪製函數
    void deploy(Map& map);
    void merge();
};

#endif
