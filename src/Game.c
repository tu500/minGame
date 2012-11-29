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

void genNewChunk(Chunk *c, int x, int y, int seed);
void loadChunk(Chunk *c, int x, int y)
{
  *c = (Chunk) {x,y};
  genNewChunk(c, x, y, 0x13370042);
  //c->tiles[15] = 1;
}
void saveChunk(Chunk *c)
{}

//TiledMap *map;
ChunkedMap *map;
TileInfo tileInfo[MINERALCOUNT + 1];
MapObject player;



Animation playerAnimR;
Animation playerAnimL;
Animation playerAnimRFast;
Animation playerAnimLFast;
bool moveDir = false;

void testinit()
{
  playerAnimR =  (Animation){50, 2, ANIM_REPEAT, &sprt_playerR[0]};
  playerAnimL =  (Animation){50, 2, ANIM_REPEAT, &sprt_playerL[0]};
  playerAnimRFast =  (Animation){10, 2, ANIM_REPEAT, &sprt_playerR[0]};
  playerAnimLFast =  (Animation){10, 2, ANIM_REPEAT, &sprt_playerL[0]};
  tileInfo[MIN_EARTH  ] =(TileInfo) {sprt_earth, COLLISION_BB};
  tileInfo[MIN_COAL   ] = (TileInfo) {sprt_coal, COLLISION_BB};
  tileInfo[MIN_IRON   ] = (TileInfo) {sprt_diam_green, COLLISION_BB};
  tileInfo[MIN_GOLD   ] = (TileInfo) {sprt_gold, COLLISION_BB};
  tileInfo[MIN_DIAMOND] = (TileInfo) {sprt_diamonds, COLLISION_BB};
  tileInfo[TILE_EMPTY ] = (TileInfo) {sprt_empty, COLLISION_NONE};
  map = ChunkedMap_init(16, tileInfo, loadChunk, saveChunk);
  ChunkedMap_setTile(map, 0, 0, TILE_EMPTY);

  player = (MapObject) {
    0, 0,
    NULL,
    16 * PIXEL_RESOLUTION, 16 * PIXEL_RESOLUTION,
    COLLISION_BB,
    .animation=&playerAnimR
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
static int lastMenuUpdated = 0;
static const int menuTickSpeed = 20;


static int ticks = 0;

static int tempr = 0;
static int tempb = 0;
static int tempg = 0;



Player p1;
int mineTicker = 0;


void Init(struct Gamestate* state)
{
  testinit();
  initPlayer(&p1);
}

void OnEnter(struct Gamestate* state)
{
  lastMenuUpdated = SysTickCounter;
}

void OnLeave(struct Gamestate* state)
{
}



// callback for player movement
static bool resetAnimation(MapObject *o, MapObject *o2);
static bool resetAnimation2(MapObject *o);
static void resetAnimation3(MapObject *o);

static inline void update_movePlayer(uint32_t a)
{
  if (!player.moving && canPlayerMove(&p1))
  {
        int tileSize = map->tileSize * PIXEL_RESOLUTION;
        int tx = divRD(player.x, tileSize);
        int ty = divRD(player.y, tileSize);

        if (GetControllerState1().buttons.Up)
        {
          ty--;
        }
        else if (GetControllerState1().buttons.Down)
        {
          ty++;
        }
        else if (GetControllerState1().buttons.Right)
        {
          tx++;
          moveDir = true;
        }
        else if (GetControllerState1().buttons.Left)
        {
          tx--;
          moveDir = false;
        }
        else
        {
          return;
        }


        Tile tile = ChunkedMap_getTile(map, tx, ty);
        if (tile == TILE_EMPTY)
        {
          mineTicker = 0;
          MObj_moveTo(&player, tx * tileSize, ty * tileSize, (PIXEL_RESOLUTION / 8) * playerSpeedWalk(&p1), true);
          player.moving->onObjCollision = resetAnimation;
          player.moving->onMapCollision = resetAnimation2;
          player.moving->onTargetReached = resetAnimation3;
          if (moveDir)
            player.animation = &playerAnimRFast;
          else
            player.animation = &playerAnimLFast;
        }

        else if(true) //TODO check for inv space
        {
          mineTicker += a * playerSpeedDig(&p1);
          if (mineTicker >= mineralInfo[tile].hp)
          {
            ChunkedMap_setTile(map, tx, ty, TILE_EMPTY);
            if (tile < MINERALCOUNT && tile != MIN_EARTH)
              p1.minerals[tile]++;
          }
        }
  }
}

void Update(uint32_t a)
{
  ChunkedMap_update(map, a);
  update_movePlayer(a);


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

  if (SysTickCounter-lastMenuUpdated > menuTickSpeed)
    if (GetControllerState1().buttons.Start)
      ChangeState(&Menu);
}

void Draw(Bitmap *b)
{

	ClearBitmap(b);
        ChunkedMap_draw(b, map,
            divRD(-player.x, PIXEL_RESOLUTION) + (SCREEN_X / 2 - 8),
            divRD(-player.y, PIXEL_RESOLUTION) + (SCREEN_Y / 2 - 8));

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




// callbacks for player movement
static bool resetAnimation(MapObject *o, MapObject *o2)
{
  if (moveDir)
    player.animation = &playerAnimR;
  else
    player.animation = &playerAnimL;
  MObj_cancelMovement(&player);
  return false;
}
static bool resetAnimation2(MapObject *o)
{
  if (moveDir)
    player.animation = &playerAnimR;
  else
    player.animation = &playerAnimL;
  MObj_cancelMovement(&player);
  return false;
}
static void resetAnimation3(MapObject *o)
{
  if (moveDir)
    player.animation = &playerAnimR;
  else
    player.animation = &playerAnimL;
  MObj_cancelMovement(&player);
}
