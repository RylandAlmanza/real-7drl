#include "reactions.h"
#include "interface.h"
#include "globals.h"

void flower_reaction(World *world, int self, int actor, Action action) {
    if (action == STEP_ON) {
        world->appearance[self].chr = '"';
        world->type[self] = FLOWER_CRUSHED;
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
