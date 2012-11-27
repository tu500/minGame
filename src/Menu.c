#include <game/Game.h>
#include <game/Font.h>

#include "Player.h"
#include "Menu.h"

#include <stdio.h>
#include <string.h>



void Menu_Init(struct Gamestate*);
void Menu_OnEnter(struct Gamestate*);
void Menu_Update(uint32_t a);
void Menu_Draw(Bitmap* b);

Gamestate Menu = {Menu_Init, Menu_OnEnter, NULL, Menu_Update, Menu_Draw};


int menuSelectedItem = 0;
static const int itemCount = 13;
static const int borderX = 16;
static const int borderY = 16;
static const int rectBorder = 2;
static const int rowHeight = 10;
static const int colWidth = 16;

int listEdgeX = 16 + 2 * 2 + 1;   //borderX + rectBorder * 2 + 1;
int listEdgeY = 16 + 2 * 2 + 10 + 2;  //borderY + rectBorder * 2 + 1 + rowHeight + 2;
int maxListRows = 10;
int itemIndex = 0;


void Menu_Init(struct Gamestate* state){
	setFont(fontwhite8);
	menuSelectedItem = 0;
}

void Menu_Update(uint32_t a){

	if (GetControllerState1().buttons.Up){
		if(menuSelectedItem > 0) menuSelectedItem--;
	}
	if (GetControllerState1().buttons.Down){
		if(menuSelectedItem < itemCount) menuSelectedItem++;
	}
	if (GetControllerState1().buttons.Start){
		//ExitState();
	}

}

void Menu_OnEnter(struct Gamestate* gamestate){

}

void Menu_Draw(Bitmap* b){

	//Create Background
	DrawFilledRectangle(b, borderX, borderY, SCREEN_X - 2 * borderX, SCREEN_Y - 2 * borderY, RGB(0,0,0));
	//Inner red Border
	DrawRectangle(b, borderX + rectBorder, borderY + rectBorder ,
	 (SCREEN_X - 2 * borderX) - 2 * rectBorder , (SCREEN_Y - 2 * borderY ) - 2 * rectBorder, RGB(193,0,0));

	//Column Header
	char *itemHeader;
			asprintf(&itemHeader, "Itemname    SPD  SPW   INV    PRZ");
			DrawText(b, itemHeader, listEdgeX + 0 * colWidth, listEdgeY - rowHeight -2);
			free(itemHeader);

	//Items
	for(int i = 0 ; i < maxListRows ; i++){
		itemIndex = i + menuSelectedItem - 4;
		if (itemIndex >= 0 && itemIndex < itemCount){
			//Item name
			char *itemName;
			asprintf(&itemName, itemList[itemIndex]->name);
			DrawText(b, itemName, listEdgeX + 0 * colWidth, listEdgeY + rowHeight * i);
			free(itemName);

			//Item Dig-Speed
			char *itemSPD;
			asprintf(&itemSPD, "% 3d", itemList[itemIndex]->speedDigMod);
			DrawText(b, itemSPD, listEdgeX + 6 * colWidth, listEdgeY + rowHeight * i);
			free(itemSPD);

			//Item Walk-Speed
			char *itemSPW;
			asprintf(&itemSPW, "% 2d", itemList[itemIndex]->speedWalkMod);
			DrawText(b, itemSPW, listEdgeX + 9 * colWidth, listEdgeY + rowHeight * i);
			free(itemSPW);	

			//Item Inventory size
			char *itemINV;
			asprintf(&itemINV, "% 4d", itemList[itemIndex]->inventorySizeTaken);
			DrawText(b, itemINV, listEdgeX + 11 * colWidth, listEdgeY + rowHeight * i);
			free(itemINV);		


			char *itemPRZ;
			asprintf(&itemPRZ, "% 5d", itemList[itemIndex]->prize);
			DrawText(b, itemPRZ, listEdgeX + 14 * colWidth, listEdgeY + rowHeight * i);
			free(itemPRZ);	
		}else{
			// dunno
		}
	}
	//Draw Border around current selection
	DrawRectangle(b, listEdgeX , listEdgeY + 4 * rowHeight -2, SCREEN_X - (2 * borderX + 2 * 2 * rectBorder + 2), rowHeight + 2 * 1, RGB(128, 0, 0));

	//Separate ItemList from Character Infos
	DrawHorizontalLine(b, listEdgeX , listEdgeY + 10 * rowHeight + 2 , SCREEN_X - (2 * 2 * rectBorder + 2 * borderX + 2), RGB(255,255,255));

	setFont(fontwhite16);
	char *playerCash;
	asprintf(&playerCash, "$:% 3d Inv:% 3d", p1.money, getFreeInvSpace(&p1));
	DrawText(b, playerCash, listEdgeX , listEdgeY + rowHeight * 11);
	free(playerCash);

	setFont(fontwhite8);
	char *playerMinerals;
	asprintf(&playerMinerals, "C:% 3d  I:% 3d  G:% 3d  D:% 3d", 20, 40, 60, 90);
	DrawText(b, playerMinerals, listEdgeX + 0 * colWidth, listEdgeY + rowHeight * 14);
	free(playerMinerals);	


}
