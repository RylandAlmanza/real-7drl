#include <time.h>
#include <stdlib.h>
#include "world.h"
#include "interface.h"

int player;
int key_pressed;

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

    int tile = (new_pos.y * 27) + new_pos.x;
    if ((world->mask[tile] & COMPONENT_SOLID) == COMPONENT_SOLID) {
        can_move = false;
    }

    for (int i = (27 * 12); i < MAX_ENTITIES; i++) {
        if ((world->mask[i] & COMPONENT_SOLID) == COMPONENT_SOLID) {
            can_move = false;
        }
    }

    if (can_move) {
        world->position[player] = new_pos;
    }
}

void flower_reaction(World *world, int self) {
    world->appearance[self].chr = '.';
}

int create_player (int x, int y, World *world) {
    int player = create_entity(world);
    world->mask[player] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_ACTION;
    world->position[player] = (Position) {.x = x, .y = y};
    world->appearance[player] = (Appearance) {
        .chr = '@',
        .fg = WHITE,
        .bg = BLACK,
        .attrs = 0
    };
    world->action[player] = &player_action;
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
