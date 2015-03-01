#include "reactions.h"

void flower_reaction(World *world, int self, int actor, Action action) {
    if (action == STEP_ON) {
        world->appearance[self].chr = '"';
        world->type[self] = FLOWER_CRUSHED;
    }
}

