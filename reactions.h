#ifndef REACTIONS_H
#define REACTIONS_H

#include "world.h"

void flower_reaction(World *world, int self, int actor, Action action);
void water_reaction(World *world, int self, int actor, Action action);
void tree_reaction(World *world, int self, int actor, Action action);
void villager_reaction(World *world, int self, int actor, Action action);

#endif
