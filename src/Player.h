// Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Unit.h"

class Player {
public:
    Player(bool id);
    int GetID() const;

private:
    bool playerID;
};

#endif // PLAYER_H
