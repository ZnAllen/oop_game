#include "raylib.h"
#include "Map.h"
#include "GameManager.h"
#include "Unit.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <iostream>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    const int tileSize = 40;

    InitWindow(screenWidth + 220, screenHeight, "Drag to Set Waypoints");
    SetTargetFPS(60);
    Map gameMap(screenWidth / tileSize, screenHeight / tileSize, tileSize);
    //Unit playerUnit(5, 5, 2.0f);

    GameManager gameManager;
    gameManager.AddUnit(2, 2, 10.0f, 100);
    gameManager.AddUnit(5, 5, 10.0f, 100);
    gameManager.AddEnemy(2, 4, 10.0f, 100);
    gameManager.AddEnemy(5, 3, 10.0f, 100);

    Vector2 lastMousePos = {-1, -1};

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        int gridX = mousePos.x / tileSize;
        int gridY = mousePos.y / tileSize;

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            gameManager.HandleClick(mousePos.x,mousePos.y,tileSize);
            if (((int)mousePos.x != (int)lastMousePos.x || (int)mousePos.y != (int)lastMousePos.y) && gameManager.units[gameManager.selectedUnitIndex].action == Action::MOVE) {
                if (gameMap.IsWalkable(gridX, gridY, false)) {
                    gameManager.AddWaypoint(gridX, gridY);
                    lastMousePos = mousePos;
                }
            }
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            lastMousePos = {-1, -1}; // 重置滑鼠位置
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            gameManager.ClearPath(); // 清除路線
        }

        gameManager.Update(gameMap);
        //gameManager.CheckForCombat();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        gameMap.Draw();
        gameManager.Draw(tileSize, gameMap);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
