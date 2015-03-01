#include <stdbool.h>
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

    for (int i = (27 * 12); i < MAX_ENTITIES; i++) {
        if ((world->mask[i] & COMPONENT_SOLID) == COMPONENT_SOLID) {
            can_move = false;
        }
    }

    int tile = (new_pos.y * 27) + new_pos.x;
    if ((world->mask[tile] & COMPONENT_SOLID) == COMPONENT_SOLID) {
        can_move = false;
    }
    if (world->type[tile] == FLOWER) {
        world->reaction[tile](world, tile, player, STEP_ON);
    }
    if (can_move) {
        world->position[player] = new_pos;
    }
}
