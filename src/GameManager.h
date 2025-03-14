#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Unit.h"
#include "Map.h"
#include <vector>

enum class Action { NONE, MOVE, ATTACK };
class GameManager {
public:
    std::vector<Unit> units;
    int selectedUnitIndex;
    Action selectedAction;  // 當前選擇的行動

    GameManager();
    void AddWaypoint(int newX, int newY);
    void ConfirmPath(Map& map);
    void ClearPath();
    void AddUnit(int x, int y, float speed, bool enemy);
    void HandleClick(int mouseX, int mouseY, int tileSize);
    void Update(Map& map);
    void Draw(int tileSize);
    void CheckForCombat();
    void DrawUI();  // 新增 UI 繪製函數
};

#endif
