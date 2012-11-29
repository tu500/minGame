
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

static const int TILE_EMPTY = MINERALCOUNT;

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

//Sells an Item, returns true if successful
bool itemSell(Player *p, Item *i);
void validatePlayerValues(Player *p);
void updateItems(Player *p);
int getFreeInvSpace(Player *p);

//Returns amount of specified mineral
int getMineralAmount(Player *p, MineralTypes m);

//Picks up a mineral, returns true if successful
bool mineralPickup(Player *p, MineralTypes m);

//Sells a mineral, returns true if successful
bool mineralSell(Player *p, MineralTypes m, int amount);

//Returns if player is allowed to move (Player might be overloaded)
bool canPlayerMove(Player *p);

//Returns Players walking speed
int playerSpeedWalk(Player *p);

//Returns Players Digging speed
int playerSpeedDig(Player *p);

//Returns Players Max Depth
int playerMaxDepth(Player *p);
extern Player p1;


extern Mineral mineralInfo[MINERALCOUNT];

#endif


