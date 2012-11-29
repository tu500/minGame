#include <string.h>

#include <ChunkedMap.h>

#include "Player.h"


int randMinerals[] = {1000, 100, 50, 4, 1};

static inline int genChunkSeed(int seed, int x, int y)
{
	return ((x << 16) | (y & 0xffff))^seed;
}

void calcAdditional(Chunk *c, int index)
{
    for (int count = 0; count < MINERALCOUNT; count++)
    {
        int rX = rand() % 3 - 1;
        int rY = rand() % 3 - 1;

        int i = index;
        i += rY * CHUNKSIZE + rX;

        if(i >= CHUNKSIZE * CHUNKSIZE)
            continue;

        c->tiles[i] = c->tiles[index];
        if (rand() % 1000 >= randMinerals[c->tiles[i]])
          return;
    }
}

void calcMin(Chunk *c)
{
    for(int i = 0; i < CHUNKSIZE * CHUNKSIZE; i++)
    {
        int type = rand() % (MINERALCOUNT - 1) + 1;
        
        if(rand() % 1000 < randMinerals[type])
        {
            c->tiles[i] = type;
            calcAdditional(c, i);
        }
    }
}

void genNewChunk(Chunk *c, int x, int y, int seed)
{
    srand(genChunkSeed(seed, x, y));
    c->x = x;
    c->y = y;
    memset(c->tiles, MIN_EARTH, CHUNKSIZE * CHUNKSIZE);
    calcMin(c);
}
