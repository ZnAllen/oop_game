#include "GameManager.h"
#include <iostream>
#include <algorithm>
#include "raylib.h"
#include  "raygui.h"

GameManager::GameManager() : selectedUnitIndex(-1), selectedSoldiers(1), maxSoldiers(1000){}

void GameManager::AddUnit(int x, int y, float speed, int soldier) {
    units.emplace_back(x, y, speed, soldier);
}
void GameManager::AddEnemy(int x, int y, float speed, int soldier){
    unitsEnemy.emplace_back(x, y, speed, soldier);
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
        units[selectedUnitIndex].action = Action::MOVE;
        return;
    }
    if (GuiButton({GetScreenWidth() - panelWidth + 110, 0, 80, 30}, "Click Me")) {
        units[selectedUnitIndex].action = Action::ATTACK;
        return;
    }
    for (size_t i = 0; i < units.size(); i++) {
        if (units[i].x == gridX && units[i].y == gridY) {
            units[selectedUnitIndex].unitColor = BLUE;
            units[selectedUnitIndex].isSelected  = false;
            selectedUnitIndex = i;
            units[selectedUnitIndex].unitColor = RED;
            units[selectedUnitIndex].isSelected = true;
            maxSoldiers = units[selectedUnitIndex].SoldierNumber;
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
    units.erase(std::remove_if(units.begin(), units.end(), [](const Unit& u) { return u.SoldierNumber <= 0; }), units.end());
    merge();
    for(int i = 0; i < units.size(); i++){
        if(units[i].isSelected) selectedUnitIndex = i;
    }
}

void GameManager::Draw(int tileSize, Map& map) {
    for (auto& unit : units) {
        unit.Draw(tileSize);
        for(auto& Enemy : unitsEnemy){
            if(abs(unit.x - Enemy.x) <= 1 && abs(unit.y - Enemy.y) <= 1) Enemy.Draw(tileSize);
        }
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

void GameManager::DrawUI(Map& map) {
    if (selectedUnitIndex < 0 || selectedUnitIndex >= (int)units.size()) return;

    Unit& unit = units[selectedUnitIndex];

    int panelWidth = 220;
    int panelHeight = 200;
    int panelX = GetScreenWidth() - panelWidth;  // UI 放最右邊
    int panelY = 0;


    // 繪製背景面板
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, DARKGRAY);
    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, WHITE);

    // 顯示單位資訊
    DrawText("Unit Info", panelX + 10, panelY + 10, 20, WHITE);
    DrawText(TextFormat("X: %d, Y: %d", unit.x, unit.y), panelX + 10, panelY + 30, 16, WHITE);
    DrawText(TextFormat("HP: %d", unit.SoldierNumber), panelX + 10, panelY + 50, 16, WHITE);
    DrawText(TextFormat("Moving: %s", unit.moving ? "Yes" : "No"), panelX + 10, panelY + 70, 16, WHITE);


    DrawText("Soldiers:", panelX + 10, panelY + 70, 16, WHITE);
    
    // 繪製滑動條
    /*selectedSoldiers =*/GuiSlider(
        (Rectangle){panelX + 10, panelY + 70, 180, 20}, // 滑動條位置
        NULL, NULL, &selectedSoldiers, 1, maxSoldiers // 滑動範圍
    );

    Soldiers = (int)selectedSoldiers;

    // 顯示當前選擇的數量
    char numText[10];
    sprintf(numText, "%d", Soldiers);
    DrawText(numText, panelX + 160, panelY + 50, 16, WHITE);

    // 繪製出征按鈕
    if (GuiButton((Rectangle){panelX + 10, panelY + 110, 180, 30}, "Deploy")) {
        deploy(map);  // 按下後執行出征邏輯
    }


    // 按鈕區域
    int buttonY = panelY + 160;

    // 移動按鈕（使用 Raygui）
    if (GuiButton((Rectangle){panelX + 10, buttonY, 80, 30}, "Move")) {
        units[selectedUnitIndex].action = Action::MOVE;
    }

    // 攻擊按鈕（使用 Raygui）
    if (GuiButton((Rectangle){panelX + 110, buttonY, 80, 30}, "Attack")) {
        units[selectedUnitIndex].action = Action::ATTACK;
    }
}

void GameManager::deploy(Map& map){
    if(selectedUnitIndex >= 0 && selectedUnitIndex < (int)units.size()) units[selectedUnitIndex].SoldierNumber -= (int)selectedSoldiers;
    AddUnit(units[selectedUnitIndex].x, units[selectedUnitIndex].y, 10.0f, (int)selectedSoldiers);
    units.back().waypoints = units[selectedUnitIndex].waypoints;
    units[selectedUnitIndex].waypoints.clear();
    units.back().ConfirmPath(map);
}

void GameManager::merge(){
    for(int i = 0; i < units.size(); i++){
        for(int j = 0; j < units.size(); j++){
            if(i != j && units[i].x == units[j].x && units[i].y == units[j].y && !units[i].moving && !units[j].moving){
                units[i].SoldierNumber += units[j].SoldierNumber;
                units[j].SoldierNumber = 0;
            }
        }
    }
}