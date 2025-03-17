#ifndef UNIT_H
#define UNIT_H

#include "raylib.h"
#include "Map.h"
#include "Pathfinding.h"
#include <vector>

enum class Action { NONE, MOVE, ATTACK };
class Unit {
    public:
        int x, y;
        float step_x, step_y;
        Color unitColor = BLUE;
        Action action; //紀錄當前動作
        float moveSpeed;
        bool moving;
        bool isSelected;
        bool isEnemy; // 是否是敵人
        int hp; // 生命值
        int attackRange; // 攻擊範圍（單位：格）
        int attackDamage; // 攻擊力
        float attackCooldownTime; // 攻擊冷卻時間
        float attackTimer; //紀錄下次還需要多久才能攻擊
        float nextStepTimer; // 記錄下一步移動時間
        float spendTime; // 紀錄行走一格所需時間
        std::vector<std::pair<int, int>> waypoints;
        std::vector<std::vector<std::pair<int, int>>> paths;
        int pathIndex;
    
        Unit(int startX, int startY, float speed,bool enemy);
        void AddWaypoint(int newX, int newY);
        void ConfirmPath(Map& map);
        void ClearPath();
        void Update(Map& map);
        void Attack(Unit& target);
        void Draw(int TILE_SIZE);
    };    

#endif // UNIT_H
