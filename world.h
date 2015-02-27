#ifndef WORLD_H
#define WORLD_H

#include "components.h"

#define MAX_ENTITIES 1024

typedef struct WorldStruct World;

struct WorldStruct {
    int count;
    int mask[MAX_ENTITIES];
    Position position[MAX_ENTITIES];
    Appearance appearance[MAX_ENTITIES];
};

int create_entity(World *world);
void destroy_entity(World *world, int entity);

#endif
