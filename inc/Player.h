
#ifndef PLAYER_H
#define PLAYER_H

#include <list.h>

typedef enum MineralTypes{

    MIN_EARTH,
    MIN_COAL,
    MIN_IRON,
    MIN_GOLD,
    MIN_DIAMOND,
    MINERALCOUNT

}MineralTypes;

typedef struct Mineral{

    int value;
    int weight;
    int hp;
    int prob;
    int upper;
    int lower;
    const char* name;
    const char* desc;

} Mineral;

typedef struct Item{

    int inventorySizeTaken;
    int speedDigMod;
    int speedWalkMod;
    int maxDepthMod;
    int inventorySizeMod;
    int prize;

    const char* name;
    const char* desc;
} Item;

typedef struct Player{

    int speedDig;
    int speedWalk;
    int maxDepth;
    int inventorySize;
    int money;

    list inventory;
    int minerals[MINERALCOUNT];

} Player;

void initPlayer(Player *p);
bool itemBuy(Player *p, Item *i);
void validatePlayerValues(Player *p);
void updateItems(Player *p);
int getFreeInvSpace(Player *p);
//Returns amount of specified mineral
int getMineralAmount(Player *p, MineralTypes m);
extern Player p1;

#endif


