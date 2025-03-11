#include "GameManager.h"
#include <iostream>
#include <algorithm>
#include "raylib.h"

GameManager::GameManager() : selectedUnitIndex(-1) {}

void GameManager::AddUnit(int x, int y, float speed, bool enemy) {
    units.emplace_back(x, y, speed, enemy);
}

void GameManager::HandleClick(int mouseX, int mouseY, int tileSize) {
    if(selectedUnitIndex != -1 && !units[selectedUnitIndex].waypoints.empty()) return;
    int gridX = mouseX / tileSize;
    int gridY = mouseY / tileSize;
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

