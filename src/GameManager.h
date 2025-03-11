#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Unit.h"
#include "Map.h"
#include <vector>

class GameManager {
public:
    std::vector<Unit> units;
    int selectedUnitIndex;

    GameManager();
    void AddWaypoint(int newX, int newY);
    void ConfirmPath(Map& map);
    void ClearPath();
    void AddUnit(int x, int y, float speed, bool enemy);
    void HandleClick(int mouseX, int mouseY, int tileSize);
    void Update(Map& map);
    void Draw(int tileSize);
    void CheckForCombat();
};

#endif
