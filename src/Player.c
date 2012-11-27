#include "Player.h"

#include <string.h>
#include <stdbool.h>

////////////////////////////////////
///////// ITEM DEFINITIONS /////////


//                       INVTA, SPD, SPW, MDM,   INV,    PRZ,   NAME,          DESC
Item shoesStart =       {  100,   0,   0,   0,     0,      0,   "Begn Shoe",   "The Shoes you start with."};
Item pickaxeStart =     {  100,   2,  -1,   0,     0,      0,   "Begn Axe",    "The Pickace you start with."};
Item backpackStart =    {    0,  -1,  -1,   0,   300,      0,   "Begn Pack",   "The Backpack you start with."};

Item shoesSlow =        {  200,   1,   3,   1,     0,    300,   "Slow Shoe",   "Fairly slow shoes. They speed you up somewhat."};
Item shoesFast =        {  400,   2,   5,   2,     0,    700,   "Fast Shoe",   "Quite good shoes you might want to take for a walk."};
Item shoesTurbo =       {  700,   3,   9,   3,     0,   1100,   "Turb Shoe",   "Very good shoes that make you feel incredibly fast."};

Item pickaxeSlow =      {  300,   4,  -1,   2,     0,    400,   "Slow Axe",    "A weak Pickaxe. There are better ones."};
Item pickaxeFast =      {  500,   7,  -2,   4,     0,    700,   "Fast Axe",    "Quite good pickaxe. It gets the job done well."};
Item pickaxeTurbo =     {  900,  11,  -3,   7,     0,   1300,   "Turb Axe",    "Awesomely fast pickaxe. You're blazing through rock."};

Item backpackSmall =    {    0,  -1,  -2,   1,   800,    500,   "Smal Pack",   "It can barely hold more than your equiptment."};
Item backpackMedium =   {    0,  -2,  -3,   3,  1600,    900,   "Med  Pack",    "It should get you through a few minutes of mining."};
Item backpackBig =      {    0,  -3,  -4,   6,  2600,   1200,   "Big  Pack",    "This backpack should hold all you'd ever want to carry."};
Item backpackHuge =     {    0,  -4,  -5,  10,  3300,   1900,   "Huge Pack",   "This backpack can probably hold more minerals than our world has."};

Item* itemList[] =  {
    &shoesStart,    &pickaxeStart,      &backpackStart,
    &shoesSlow,     &shoesFast,         &shoesTurbo,
    &pickaxeSlow,   &pickaxeFast,       &pickaxeTurbo,
    &backpackSmall, &backpackMedium,    &backpackBig,       &backpackHuge
};

///////////// MINERALS /////////////

Mineral mineralInfo[MINERALCOUNT] = {
    //VAL ,WGHT, HP ,PROB,UPPR,LOWR,NAME, DESC 
    {    0,   0,   2,1000,   0, 255,"Earth", "Cmon, you dont need a description."},
    {    5,   3,   6, 100,   0,  12,"Coal", "Nice coal."},
    {   35,   7,   9,  70,   5,  18,"Iron", "Some iron. Sells good."},
    {   60,  11,  20,  40,   9,  24,"Gold", "Shiny gold. Worth quite a lot."},
    {  300, 400,  50,  10,  22,  30,"Diamond", "Precious diamonds. Aren't they beautiful?"}
};

//////// Starting Values ///////////

static const int startSpeedDig = 3;
static const int startSpeedWalk = 10;
static const int startMaxDepth = 10;
static const int startInventorySize = 100;
static const int startMoney = 1337;


////////////////////////////////////

//Initializes Player using starting Values
void initPlayer(Player *p){
    *p=(Player){
        startSpeedDig,
        startSpeedWalk,
        startMaxDepth,
        startInventorySize,
        startMoney
    };
    listInsert(&p->inventory, &shoesStart);
    listInsert(&p->inventory, &pickaxeStart);
    listInsert(&p->inventory, &backpackStart);

    //Initialize Mineral-Array
    memset(p->minerals, 0, sizeof(p->minerals));
}

//Buys an Item, returns true if successful
bool itemBuy(Player *p, Item *i){
    //Check if Item is already owned
    if(listCount(&p->inventory, i) >= 1 ) return false;
    int newmoney = p->money -= i->prize;
    int newspace = getFreeInvSpace(p) - i->inventorySizeTaken;
    if( newmoney >= 0 && newspace >= 0){
        p->money = newmoney;
        listInsert(&p->inventory, i);
        updateItems(p);
        return true;
    }
    return false;
}

//Sells an Item, returns true if successful
bool itemSell(Player *p, Item *i){
    if(listSearch(&p->inventory, i)){
        listRemoveByValue(&p->inventory, i);
        p->money += i->prize*(2/3);
        updateItems(p);
        return true;
    }
    return false;
}

//Picks up a mineral, returns true if successful
bool mineralPickup(Player *p, MineralTypes m){
    //Mineral is of Type Earth. We Ignore
    if(m == 0) return true;
    int newspace = getFreeInvSpace(p) - mineralInfo[m].weight;
    if(newspace >= 0){
        p->minerals[m]++;
        return true;
    }
    return false;
}

//Sells a mineral, returns true if successful
bool mineralSell(Player *p, MineralTypes m, int amount){
    //Mineral is of Type Earth. We ignore
    if(m == 0) return true;
    if(p->minerals[m] >= amount){
        p->minerals[m] -= amount;
        p->money += amount * mineralInfo[m].value;
        updateItems(p);
        return true;
    }
    return false;
}

//Returns amount of specified mineral
int getMineralAmount(Player *p, MineralTypes m){
    return p->minerals[m];
}

//Makes sure no important values are negative
void validatePlayerValues(Player *p){
    if(p->speedDig <= 0) p->speedDig = 1;
    if(p->speedWalk <= 0) p->speedWalk = 1;
    if(p->maxDepth <= 0) p->maxDepth = 1;
    if(p->inventorySize <= 0) p->inventorySize = 1;    
}

//Updates Player stats based on items
void updateItems(Player *p){
    p->speedDig = startSpeedDig;
    p->speedWalk = startSpeedWalk;
    p->maxDepth = startMaxDepth;
    p->inventorySize = startInventorySize;

    for(list_el *i = p->inventory.head; i != NULL; i = i->next){
        p->speedDig += ((Item*)i->val)->speedDigMod;
        p->speedWalk += ((Item*)i->val)->speedWalkMod;
        p->maxDepth += ((Item*)i->val)->maxDepthMod;
        p->inventorySize += ((Item*)i->val)->inventorySizeMod;
    }
    validatePlayerValues(p);
}

//Returns free space of inventory
int getFreeInvSpace(Player *p){
    int space = p->inventorySize;
    for(list_el *i = p->inventory.head; i != NULL; i = i->next){
        space -= ((Item*)i->val)->inventorySizeTaken;
        space += ((Item*)i->val)->inventorySizeMod;
    }
    for(int i = 0; i < MINERALCOUNT ; i++){
        space -= p->minerals[i] * mineralInfo[i].weight;
    }
    return space;
}

//Returns if player is allowed to move (Player might be overloaded)
bool canPlayerMove(Player *p){
    return (getFreeInvSpace(p) > 0);
}

//Returns Players walking speed
int playerSpeedWalk(Player *p){
    return p->speedWalk;
}

//Returns Players Digging speed
int playerSpeedDig(Player *p){
    return p->speedDig;
}

//Returns Players Max Depth
int playerMaxDepth(Player *p){
    return p->maxDepth;
}
