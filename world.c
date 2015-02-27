#include <stdio.h>
#include "world.h"

int create_entity(World *world) {
    int entity;
    for (entity = 0; entity < MAX_ENTITIES; entity++) {
        if (world->mask[entity] == COMPONENT_NONE) {
            return entity;
        }
    }
    printf("Error! No more entities left!\n");
    return MAX_ENTITIES;
}

void destroy_entity(World *world, int entity) {
    world->mask[entity] = COMPONENT_NONE;
}
