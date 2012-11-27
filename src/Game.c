#include <game/Game.h>
#include <Debug.h>
#include <game/Font.h>

#include <stdio.h>
#include <string.h>

#include "Player.h"
#include "Sprites.h"
#include "Menu.h"

#include <TiledMap.h>
#include <ChunkedMap.h>

void loadChunk(Chunk *c, int x, int y)
{
  *c = (Chunk) {x,y};
  c->tiles[15] = 1;
}
void saveChunk(Chunk *c)
{}

//TiledMap *map;
ChunkedMap *map;
TileInfo tileInfo[3];
MapObject player;

void testinit()
{
  tileInfo[0] = (TileInfo) {sprt_empty, COLLISION_NONE};
  tileInfo[1] = (TileInfo) {sprt_earth, COLLISION_BB};
  tileInfo[2] = (TileInfo) {sprt_diamonds, COLLISION_BB};
  map = ChunkedMap_init(16, tileInfo, loadChunk, saveChunk);
  //map = TiledMap_init(20, 20, 16, tileInfo);
  //map->tiles[88] = 1;
  //memset(&map->tiles[0], 1, map->sizeX * map->sizeY * sizeof(Tile));

  player = (MapObject) {
    0, 0,
    sprt_diamonds,
    16 * PIXEL_RESOLUTION, 16 * PIXEL_RESOLUTION,
    COLLISION_BB
  };
  listInsert(&map->objects, &player);
}






void Init(struct Gamestate*);
void OnEnter(struct Gamestate* state);
void OnLeave(struct Gamestate* state);
void Update(uint32_t);
void Draw(Bitmap *);
void isAlive(Bitmap *);



Gamestate InitState = { Init, OnEnter, OnLeave, Update, Draw };
Game* TheGame = &(Game) {&InitState};


static int ticks = 0;

static int tempr = 0;
static int tempb = 0;
static int tempg = 0;



Player p1;


void Init(struct Gamestate* state)
{
  testinit();
  initPlayer(&p1);
}

void OnEnter(struct Gamestate* state)
{
}

void OnLeave(struct Gamestate* state)
{
}

void Update(uint32_t a)
{
  //int oldPos[] = { player.x, player.y };
  //      if (GetControllerState1().buttons.Up){
  //              player.y -= PIXEL_RESOLUTION;
  //      }
  //      if (GetControllerState1().buttons.Down){
  //              player.y += PIXEL_RESOLUTION;
  //      }
  //      if (GetControllerState1().buttons.Right){
  //              player.x += PIXEL_RESOLUTION;
  //      }
  //      if (GetControllerState1().buttons.Left){	
  //              player.x -= PIXEL_RESOLUTION;
  //      }
  //if (MObj_collisionMap(map, &player))
  //{
  //  // Reset position on collision
  //  player.x = oldPos[0];
  //  player.y = oldPos[1];
  //}

  //TiledMap_update(map, a);
  ChunkedMap_update(map, a);
  if (!player.moving && canPlayerMove(&p1))
  {
        if (GetControllerState1().buttons.Up){
          MObj_moveTo(&player, player.x, player.y - (PIXEL_RESOLUTION * map->tileSize), PIXEL_RESOLUTION, true);
        }
        if (GetControllerState1().buttons.Down){
          MObj_moveTo(&player, player.x, player.y + (PIXEL_RESOLUTION * map->tileSize), PIXEL_RESOLUTION, true);
        }
        if (GetControllerState1().buttons.Right){
          MObj_moveTo(&player, player.x + (PIXEL_RESOLUTION * map->tileSize), player.y, PIXEL_RESOLUTION, true);
        }
        if (GetControllerState1().buttons.Left){
          MObj_moveTo(&player, player.x - (PIXEL_RESOLUTION * map->tileSize), player.y, PIXEL_RESOLUTION, true);
        }
  }






	ticks += a;
	ticks %= 255;

	if (GetControllerState1().buttons.Up){
		tempb += 20;
	}
	if (GetControllerState1().buttons.Down){
		tempb -= 20;
	}
	if (GetControllerState1().buttons.Right){
		tempg += 20;
	}
	if (GetControllerState1().buttons.Left){	
		tempg -= 20;
	}
	if (GetControllerState1().buttons.L)	tempr -= 20;
	if (GetControllerState1().buttons.R)	tempr += 20;
  if (GetControllerState1().buttons.Start) ChangeState(&Menu);
}

void Draw(Bitmap *b)
{

	ClearBitmap(b);
        //TiledMap_draw(b, map, 0, 0);
        ChunkedMap_draw(b, map,
            divRD(-player.x, PIXEL_RESOLUTION) + (SCREEN_X / 2 - 8),
            divRD(-player.y, PIXEL_RESOLUTION) + (SCREEN_Y / 2 - 8));/**/
        //ChunkedMap_draw(b, map, 0, 0);
  setFont(fontwhite8);
  char *highscoreString;
  asprintf(&highscoreString, "%d, %d", player.x, player.y);
  DrawText(b, highscoreString, 0, 8);
  free(highscoreString);
  //Warn if player is overloaded
  if(!canPlayerMove(&p1)){
    setFont(fontblack16);
    char *message;
    asprintf(&message, "OVERLOAD!");
    DrawFilledRectangle(b, 100, 91, 140, 17, RGB(255, 0, 0));
    DrawText(b, message, 84, 92);
    free(message); 
    setFont(fontwhite8);   
  }

	isAlive(b);
}

void isAlive(Bitmap *b){
	DrawFilledRectangle(b, 310,0, 10, 10,RGB(tempr,tempg,tempb));
	DrawFilledRectangle(b, 313,2, 4, 4,RGB(ticks, ticks, ticks));
	if(tempr > 255) tempr = 255;
	if(tempg > 255) tempg = 255;
	if(tempb > 255) tempb = 255;

	if(tempr < 0) tempr = 0;
	if(tempg < 0) tempg = 0;
	if(tempb < 0) tempb = 0;
}



