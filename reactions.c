#include "reactions.h"
#include "interface.h"
#include "globals.h"
#include "tiles.h"

void flower_reaction(World *world, int self, int actor, Action action) {
    if (action == STEP_ON) {
        Position pos = world->position[self];
        create_tile(pos.x, pos.y, FLOWER_CRUSHED, world);
        if (actor == player) {
            interface_write(0, 14, "A flower is crushed beneath your feet.");
            interface_flush();
        }
    }
}

void water_reaction(World *world, int self, int actor, Action action) {
    if (action == STEP_ON) {
        if (actor == player) {
            interface_write(0, 14, "You step through the shallow water.");
            interface_flush();
        }
    }
}

void tree_reaction(World *world, int self, int actor, Action action) {
    if (action == BUMP) {
        if (actor == player) {
            interface_write(0, 14, "You carelessly walk into a tree.");
            interface_flush();
        }
    }
}

void villager_reaction(World *world, int self, int actor, Action action) {
    if (action == GREET) {
        if (actor == player) {
            interface_write(0, 14, "You greet the villager. He responds with a friendly \"Heyo!\"");
            interface_flush();
        }
    }
}
