#include "GameManager.h"
#include <iostream>
#include <algorithm>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include  "raygui.h"

GameManager::GameManager() : selectedUnitIndex(-1), selectedAction(Action::NONE){}

void GameManager::AddUnit(int x, int y, float speed, bool enemy) {
    units.emplace_back(x, y, speed, enemy);
}

void GameManager::HandleClick(int mouseX, int mouseY, int tileSize) {
    if(selectedUnitIndex != -1 && !units[selectedUnitIndex].waypoints.empty()) return;
    int gridX = mouseX / tileSize;
    int gridY = mouseY / tileSize;

    float panelWidth = 200;
    float panelX = GetScreenWidth() - panelWidth - 10;
    float panelY = 10;
    float buttonY = panelY + 100;

    // 偵測 UI 按鈕點擊
    if (GuiButton({GetScreenWidth() - panelWidth + 10, 0, 80, 30}, "Click Me")) {
        selectedAction = Action::MOVE;
        return;
    }
    if (GuiButton({GetScreenWidth() - panelWidth + 110, 0, 80, 30}, "Click Me")) {
        selectedAction = Action::ATTACK;
        return;
    }

    for (size_t i = 0; i < units.size(); i++) {
        if (units[i].x == gridX && units[i].y == gridY) {
            units[selectedUnitIndex].unitColor = BLUE;
            units[selectedUnitIndex].isSelected  = false;
            selectedUnitIndex = i;
            units[selectedUnitIndex].unitColor = RED;
            units[selectedUnitIndex].isSelected = true;
            return;
        }
    }

    /*if (selectedUnitIndex >= 0 && selectedUnitIndex < (int)units.size()) {
        Unit& unit = units[selectedUnitIndex];

        if (selectedAction == Action::MOVE) {
            unit.ClearPath();
            unit.AddWaypoint(gridX, gridY);
        }
    }*/
}

void GameManager::Update(Map& map) {
    if (selectedUnitIndex >= 0 && selectedUnitIndex < (int)units.size()) {
        for(auto& unit : units){
            unit.Update(map);
        }
    }
    units.erase(std::remove_if(units.begin(), units.end(), [](const Unit& u) { return u.hp <= 0; }), units.end());
    for(int i = 0; i < units.size(); i++){
        if(units[i].isSelected) selectedUnitIndex = i;
    }
}

void GameManager::Draw(int tileSize, Map& map) {
    for (auto& unit : units) {
        unit.Draw(tileSize);
    }
    DrawUI(map); // 顯示 UI 面板
}

void GameManager::AddWaypoint(int newX, int newY){
    if(!units[selectedUnitIndex].moving){
        units[selectedUnitIndex].AddWaypoint(newX,newY);
    }
}

void GameManager::ClearPath(){
    units[selectedUnitIndex].ClearPath();
}

void GameManager::ConfirmPath(Map& map){
    if(!units[selectedUnitIndex].moving){
        units[selectedUnitIndex].ConfirmPath(map);
    }
}

void GameManager::CheckForCombat() {
    for (auto& attacker : units) {
        // 先減少 attackTimer，每幀只減少一次
        if (attacker.attackTimer > 0) {
            attacker.attackTimer -= GetFrameTime();
            continue;  // **如果還在冷卻，直接跳過攻擊檢查**
        }

        for (auto& target : units) {
            if (target.isEnemy != attacker.isEnemy) {
                int dx = std::abs(target.x - attacker.x);
                int dy = std::abs(target.y - attacker.y);
                if (dx + dy <= attacker.attackRange) {
                    attacker.Attack(target);
                    attacker.attackTimer = attacker.attackCooldownTime;
                    break;  // **確保每次攻擊後，不再繼續檢查其他敵人**
                }
            }
        }
    }
}

void GameManager::DrawUI(Map& map) {
    if (selectedUnitIndex < 0 || selectedUnitIndex >= (int)units.size()) return;

    Unit& unit = units[selectedUnitIndex];

    int panelWidth = 220;
    int panelHeight = 200;
    int panelX = GetScreenWidth() - panelWidth;  // UI 放最右邊
    int panelY = 0;

    float selectedSoldiers = 1;  // 預設最少出征 1 名士兵
    float maxSoldiers = 10;      // 這裡可以用玩家擁有的士兵數量來更新


    // 繪製背景面板
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, DARKGRAY);
    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, WHITE);

    // 顯示單位資訊
    DrawText("Unit Info", panelX + 10, panelY + 10, 20, WHITE);
    DrawText(TextFormat("X: %d, Y: %d", unit.x, unit.y), panelX + 10, panelY + 30, 16, WHITE);
    DrawText(TextFormat("HP: %d", unit.hp), panelX + 10, panelY + 50, 16, WHITE);
    DrawText(TextFormat("Moving: %s", unit.moving ? "Yes" : "No"), panelX + 10, panelY + 70, 16, WHITE);


    DrawText("Soldiers:", panelX + 10, panelY + 70, 16, WHITE);
    
    // 繪製滑動條
    selectedSoldiers = GuiSliderBar(
        (Rectangle){panelX + 10, panelY + 70, 180, 20}, // 滑動條位置
        NULL, NULL, NULL, selectedSoldiers, 100 // 滑動範圍
    );

    // 顯示當前選擇的數量
    char numText[10];
    sprintf(numText, "%d", selectedSoldiers);
    DrawText(numText, panelX + 160, panelY + 50, 16, WHITE);

    // 繪製出征按鈕
    if (GuiButton((Rectangle){panelX + 10, panelY + 110, 180, 30}, "Deploy")) {
        ConfirmPath(map);  // 按下後執行出征邏輯
    }


    // 按鈕區域
    int buttonY = panelY + 160;

    // 移動按鈕（使用 Raygui）
    if (GuiButton((Rectangle){panelX + 10, buttonY, 80, 30}, "Move")) {
        selectedAction = Action::MOVE;
    }

    // 攻擊按鈕（使用 Raygui）
    if (GuiButton((Rectangle){panelX + 110, buttonY, 80, 30}, "Attack")) {
        selectedAction = Action::ATTACK;
    }
}