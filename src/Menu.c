#include <game/Game.h>
#include <game/Font.h>

#include "Player.h"
#include "Menu.h"
#include "Sprites.h"

#include <stdio.h>
#include <string.h>

Gamestate Menu = {Menu_Init, Menu_OnEnter, Menu_OnLeave, Menu_Update, Menu_Draw};


int menuSelectedItem = 0;
static const int itemCount = 12;
static const int borderX = 16;
static const int borderY = 16;
static const int rectBorder = 2;
static const int rowHeight = 10;
static const int colWidth = 16;
static const int circleRadius = 2;
static const int menuTickSpeed = 20;

static int lastMenuUpdated = 0;

int listEdgeX = 16 + 2 * 2 + 1;   //borderX + rectBorder * 2 + 1;
int listEdgeY = 16 + 2 * 2 + 10 + 2;  //borderY + rectBorder * 2 + 1 + rowHeight + 2;
int maxListRows = 10;
int itemIndex = 0;


void Menu_Init(struct Gamestate* state){
	setFont(fontwhite8);
	menuSelectedItem = 0;
}

void Menu_Update(uint32_t a){

	if (SysTickCounter-lastMenuUpdated > menuTickSpeed){
		if (GetControllerState1().buttons.Up){
			if(menuSelectedItem > 0) menuSelectedItem--;
			lastMenuUpdated = SysTickCounter;
		}
		if (GetControllerState1().buttons.Down){
			if(menuSelectedItem < itemCount) menuSelectedItem++;
			lastMenuUpdated = SysTickCounter;
		}
		//Return to Game
		if (GetControllerState1().buttons.Start){
			ExitState();
			lastMenuUpdated = SysTickCounter;
		}
		//Sell Item
		if (GetControllerState1().buttons.B){
			itemSell(&p1, (Item*)itemList[menuSelectedItem]);
			lastMenuUpdated = SysTickCounter;
		}
		//Buy Item
		if (GetControllerState1().buttons.A){
			itemBuy(&p1, (Item*)itemList[menuSelectedItem]);
			lastMenuUpdated = SysTickCounter;
		}
		if (GetControllerState1().buttons.X){
			sellAllMinerals(&p1);
			lastMenuUpdated = SysTickCounter;
		}
		if (GetControllerState1().buttons.Y){
			mineralPickup(&p1, MIN_DIAMOND);
			lastMenuUpdated = SysTickCounter;
		}
	}

}

void Menu_OnEnter(struct Gamestate* gamestate)
{
	lastMenuUpdated = SysTickCounter;
}

void Menu_OnLeave(struct Gamestate* state)
{
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
		if (itemIndex >= 0 && itemIndex <= itemCount){
			//Mark Items you can't afford
			if(itemList[itemIndex]->prize > p1.money){
				DrawFilledCircle(b, listEdgeX +circleRadius , listEdgeY + rowHeight * i + circleRadius + 1, circleRadius ,RGB(255,0,0));
			}	
			//Item Owned
			if(listCount(&p1.inventory, (Item*)itemList[itemIndex]) >= 1){
				DrawFilledCircle(b, listEdgeX +circleRadius , listEdgeY + rowHeight * i + circleRadius + 1, circleRadius ,RGB(0,255,0));
			}
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

			//Item Prize
			char *itemPRZ;
			asprintf(&itemPRZ, "% 5d", itemList[itemIndex]->prize);
			DrawText(b, itemPRZ, listEdgeX + 14 * colWidth, listEdgeY + rowHeight * i);
			free(itemPRZ);
			setFont(fontwhite8);
		}else{
			// dunno
		}
	}
	//Draw Border around current selection
	DrawRectangle(b, listEdgeX , listEdgeY + 4 * rowHeight -2, SCREEN_X - (2 * borderX + 2 * 2 * rectBorder + 2), rowHeight + 2 * 1, RGB(128, 0, 0));

	//Separate ItemList from Character Infos
	DrawHorizontalLine(b, listEdgeX , listEdgeY + 10 * rowHeight + 2 , SCREEN_X - (2 * 2 * rectBorder + 2 * borderX + 2), RGB(255,255,255));

	//Draw Player Info (Cash / Free Space)
	setFont(fontwhite16);
	char *playerCash;
	asprintf(&playerCash, "$:%3d Inv:%3d", p1.money, getFreeInvSpace(&p1));
	DrawText(b, playerCash, listEdgeX , listEdgeY + rowHeight * 11);
	free(playerCash);
	setFont(fontwhite8);	


	//Draw Players Minerals
	//Coal
	DrawRLEBitmap(b,sprt_coal,listEdgeX + 1 * colWidth,listEdgeY + rowHeight * 13);
	//Iron
	DrawRLEBitmap(b,sprt_treasure1,listEdgeX + 5 * colWidth,listEdgeY + rowHeight * 13);
	//Gold
	DrawRLEBitmap(b,sprt_gold,listEdgeX + 9 * colWidth,listEdgeY + rowHeight * 13);
	//Diamonds
	DrawRLEBitmap(b,sprt_diam_red,listEdgeX + 13 * colWidth,listEdgeY + rowHeight * 13);

	char *playerMinerals;
	asprintf(&playerMinerals, "     % 3d     % 3d     % 3d     % 3d",
		getMineralAmount(&p1, MIN_COAL),
		getMineralAmount(&p1, MIN_IRON),
		getMineralAmount(&p1, MIN_GOLD),
		getMineralAmount(&p1, MIN_DIAMOND));
	DrawText(b, playerMinerals, listEdgeX - colWidth / 2, listEdgeY + rowHeight * 13.5);
	free(playerMinerals);	


}


void sellAllMinerals(Player *p){
	for(int i = 0 ; i < MINERALCOUNT; i++){
		mineralSell(p, i, getMineralAmount(p, i));
	}
}
