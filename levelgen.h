#ifndef LEVELGEN_H
#define LEVELGEN_H

#include "world.h"

void save_area(World *world);
void load_area(World *world);
int create_player(int x, int y, World *world);

void generate_nature (World *world);
void generate_village (World *world);

#endif
