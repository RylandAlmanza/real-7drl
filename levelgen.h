#ifndef LEVELGEN_H
#define LEVELGEN_H

#include "world.h"

int create_player (int x, int y, World *world);
int create_grass (int x, int y, World *world);
int create_tree (int x, int y, World *world);
int create_water (int x, int y, World *world);
int create_flower (int x, int y, World *world);
int create_wall (int x, int y, World *world);
int create_path (int x, int y, World *world);

void generate_nature (World *world);
void generate_village (World *world);

#endif
