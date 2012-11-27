#ifndef __MENU_H__
#define __MENU_H__

#include <string.h>

extern Gamestate Menu;
extern Item* itemList[];

void Menu_Init(struct Gamestate*);
void Menu_OnEnter(struct Gamestate*);
void Menu_Update(uint32_t a);
void Menu_Draw(Bitmap* b);

void sellAllMinerals(Player *p);



#endif