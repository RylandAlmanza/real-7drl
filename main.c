#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "world.h"
#include "interface.h"
#include "globals.h"
#include "actions.h"
#include "reactions.h"

int create_player (int x, int y, World *world) {
    int player = create_entity(world);
    world->mask[player] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_ACTION | COMPONENT_TYPE;
    world->position[player] = (Position) {.x = x, .y = y};
    world->appearance[player] = (Appearance) {
        .chr = '@',
        .fg = WHITE,
        .bg = BLACK,
        .attrs = 0
    };
    world->action[player] = &player_action;
    world->type[player] = PLAYER;
    return player;
}

int create_grass (int x, int y, World *world) {
    int tile;
    if (world->mask[(y * 27) + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = (y * 27) + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
    world->position[tile] = (Position) {.x = x, .y = y};
    world->appearance[tile] = (Appearance) {
        .chr = '.',
        .fg = GREEN,
        .bg = BLACK,
        .attrs = 0
    };
    world->type[tile] = GRASS;
    return tile;
}

int create_tree (int x, int y, World *world) {
    int tile;
    if (world->mask[(y * 27) + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = (y * 27) + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_SOLID;
    world->position[tile] = (Position) {.x = x, .y = y};
    world->appearance[tile] = (Appearance) {
        .chr = 'T',
        .fg = GREEN,
        .bg = BLACK,
        .attrs = 0
    };
    world->type[tile] = TREE;
    return tile;
}

int create_water (int x, int y, World *world) {
    int tile;
    if (world->mask[(y * 27) + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = (y * 27) + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
    world->position[tile] = (Position) {.x = x, .y = y};
    world->appearance[tile] = (Appearance) {
        .chr = '~',
        .fg = BLUE,
        .bg = BLACK,
        .attrs = 0
    };
    world->type[tile] = WATER;
    return tile;
}

int create_flower (int x, int y, World *world) {
    int tile;
    if (world->mask[(y * 27) + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = (y * 27) + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_REACTION;
    world->position[tile] = (Position) {.x = x, .y = y};
    world->appearance[tile] = (Appearance) {
        .chr = '*',
        .fg = YELLOW,
        .bg = BLACK,
        .attrs = 0
    };
    world->type[tile] = FLOWER;
    world->reaction[tile] = &flower_reaction;
    return tile;
}

void generate_map (World *world) {
    int y = 0;
    int x = 0;
    for (y = 0; y < 12; y++) {
        for (x = 0; x < 27; x++) {
            create_grass(x, y, world);
        }
    }
    for (y = 0; y < 12; y++) {
        for (x = 0; x < 27; x++) {
            if (rand() % 15 == 0) {
                create_tree(x, y, world);
            }
        }
    }
    for (y = 0; y < 12; y++) {
        for (x = 0; x < 27; x++) {
            if (rand() % 20 == 0) {
                create_flower(x, y, world);
            }
        }
    }
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 24; x++) {
            if (rand() % 50 == 0) {
                for (int j = 0; j < 3; j++) {
                    for (int i = 0; i < 3; i++) {
                        if (rand() % 2 == 0) {
                            create_water(x + i, y + j, world);
                        }
                    }
                }
            }
        }
    }
}


void draw_world(World *world) {
    int i = 0;
    while (world->mask[i] != COMPONENT_NONE) {
        int x = world->position[i].x;
        int y = world->position[i].y;
        char chr = world->appearance[i].chr;
        int fg = world->appearance[i].fg;
        int bg = world->appearance[i].bg;
        int attrs = world->appearance[i].attrs;
        interface_draw(x, y, chr, fg, bg, attrs);
        i++;
    }
}

void draw_border() {
    int attrs = A_BOLD | A_REVERSE;
    int fg = BLACK;
    int bg = BLACK;
    for (int x = -1; x < 28; x++) {
        interface_draw(x, -1, ' ', fg, bg, attrs);
        interface_draw(x, 12, ' ', fg, bg, attrs);
    }
    for (int y = -1; y < 13; y++) {
        interface_draw(-1, y, ' ', fg, bg, attrs);
        interface_draw(27, y, ' ', fg, bg, attrs);
    }
}

int main() {
    srand(time(NULL));
    interface_init();
    World world;
    generate_map(&world);
    player = create_player(0, 0, &world);
    while (key_pressed != 'q') {
        interface_wipe();
        draw_border();
        draw_world(&world);
        interface_flush();

        key_pressed = getch();
        for (int i = 0; i < MAX_ENTITIES; i++) {
            if ((world.mask[i] & COMPONENT_ACTION) == COMPONENT_ACTION) {
                world.action[i](&world, i);
            }
        }
    }
    interface_uninit();
    return 0;
}
