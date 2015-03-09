#include <stdbool.h>
#include <stdlib.h>
#include "actions.h"
#include "globals.h"

void player_action(World *world, int self) {
    Position new_pos = world->position[player];
    int can_move = true;
    if (key_pressed == 'k') {
        new_pos.y--;
    }
    if (key_pressed == 'l') {
        new_pos.x++;
    }
    if (key_pressed == 'j') {
        new_pos.y++;
    }
    if (key_pressed == 'h') {
        new_pos.x--;
    }

    if (new_pos.x < 0 || new_pos.x >= 27 ||
        new_pos.y < 0 || new_pos.y >= 12) {
        can_move = false;
    }

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (i == self) continue;
        if ((world->mask[i] & COMPONENT_SOLID) == COMPONENT_SOLID) {
            int bx = world->position[i].x;
            int by = world->position[i].y;
            if (bx == new_pos.x && by == new_pos.y) {
                can_move = false;
                if (world->type[i] == VILLAGER) {
                    world->reaction[i](world, i, player, GREET);
                }
            }
        }
    }

    /*int tile = (new_pos.y * world_width) + new_pos.x;
    if ((world->mask[tile] & COMPONENT_SOLID) == COMPONENT_SOLID) {
        can_move = false;
    }
    if (world->type[tile] == FLOWER) {
        world->reaction[tile](world, tile, player, STEP_ON);
    }
    if (world->type[tile] == WATER) {
        world->reaction[tile](world, tile, player, STEP_ON);
    }
    if (world->type[tile] == TREE) {
        world->reaction[tile](world, tile, player, BUMP);
    }*/
    if (can_move) {
        world->position[player] = new_pos;
    }
}

void villager_action(World *world, int self) {
    Position new_pos = world->position[self];
    int can_move = true;
    int direction = (rand() % 5);
    if (direction == 1) {
        new_pos.y--;
    }
    if (direction == 2) {
        new_pos.x++;
    }
    if (direction == 3) {
        new_pos.y++;
    }
    if (direction == 4) {
        new_pos.x--;
    }

    if (new_pos.x < 0 || new_pos.x >= 27 ||
        new_pos.y < 0 || new_pos.y >= 12) {
        can_move = false;
    }

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (i == self) continue;
        if ((world->mask[i] & COMPONENT_SOLID) == COMPONENT_SOLID) {
            int bx = world->position[i].x;
            int by = world->position[i].y;
            if (bx == new_pos.x && by == new_pos.y) {
                can_move = false;
            }
        }
    }

    if (can_move) {
        world->position[self] = new_pos;
    }
}
