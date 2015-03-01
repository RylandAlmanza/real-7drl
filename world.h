#ifndef WORLD_H
#define WORLD_H

#include "components.h"

#define MAX_ENTITIES 1024

typedef struct WorldStruct World;

struct WorldStruct {
    int mask[MAX_ENTITIES];
    Position position[MAX_ENTITIES];
    Appearance appearance[MAX_ENTITIES];
    void (*action[MAX_ENTITIES])(World *world, int self);
    void (*reaction[MAX_ENTITIES])(World *world, int self, int actor);
};

int create_entity(World *world);
void destroy_entity(World *world, int entity);

#endif
