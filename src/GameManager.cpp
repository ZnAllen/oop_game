#include "GameManager.h"
#include <iostream>
#include <algorithm>
#include "raylib.h"

GameManager::GameManager() : selectedUnitIndex(-1), selectedAction(Action::NONE){}

void GameManager::AddUnit(int x, int y, float speed, bool enemy) {
    units.emplace_back(x, y, speed, enemy);
}

void GameManager::HandleClick(int mouseX, int mouseY, int tileSize) {
    if(selectedUnitIndex != -1 && !units[selectedUnitIndex].waypoints.empty()) return;
    int gridX = mouseX / tileSize;
    int gridY = mouseY / tileSize;

    int panelWidth = 200;
    int panelX = GetScreenWidth() - panelWidth - 10;
    int panelY = 10;
    int buttonY = panelY + 100;

    // 偵測 UI 按鈕點擊
    if (mouseX >= panelX + 10 && mouseX <= panelX + 90 && mouseY >= buttonY && mouseY <= buttonY + 30) {
        selectedAction = Action::MOVE;
        return;
    }
    if (mouseX >= panelX + 110 && mouseX <= panelX + 190 && mouseY >= buttonY && mouseY <= buttonY + 30) {
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

void GameManager::Draw(int tileSize) {
    for (auto& unit : units) {
        unit.Draw(tileSize);
    }
    DrawUI(); // 顯示 UI 面板
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

void GameManager::DrawUI() {
    if (selectedUnitIndex < 0 || selectedUnitIndex >= (int)units.size()) return;

    Unit& unit = units[selectedUnitIndex];

    int panelWidth = 220;
    int panelHeight = 150;
    int panelX = GetScreenWidth() - panelWidth;  // UI 放最右邊
    int panelY = 0;

    // 繪製背景面板
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, DARKGRAY);
    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, WHITE);

    // 顯示單位資訊
    DrawText("Unit Info", panelX + 10, panelY + 10, 20, WHITE);
    DrawText(TextFormat("X: %d, Y: %d", unit.x, unit.y), panelX + 10, panelY + 30, 16, WHITE);
    DrawText(TextFormat("HP: %d", unit.hp), panelX + 10, panelY + 50, 16, WHITE);
    DrawText(TextFormat("Moving: %s", unit.moving ? "Yes" : "No"), panelX + 10, panelY + 70, 16, WHITE);

    // 按鈕區域
    int buttonY = panelY + 100;
    
    // 移動按鈕
    DrawRectangle(panelX + 10, buttonY, 80, 30, selectedAction == Action::MOVE ? LIGHTGRAY : GRAY);
    DrawRectangleLines(panelX + 10, buttonY, 80, 30, WHITE);
    DrawText("Move", panelX + 30, buttonY + 5, 16, WHITE);

    // 攻擊按鈕
    DrawRectangle(panelX + 110, buttonY, 80, 30, selectedAction == Action::ATTACK ? LIGHTGRAY : GRAY);
    DrawRectangleLines(panelX + 110, buttonY, 80, 30, WHITE);
    DrawText("Attack", panelX + 130, buttonY + 5, 16, WHITE);
}