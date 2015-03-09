#include <stdlib.h>
#include "levelgen.h"
#include "reactions.h"
#include "interface.h"
#include "actions.h"
#include "globals.h"
#include "utilities.h"

#define area_width 27
#define area_height 12

int random_empty_tile(World *world) {
    while (true) {
        int x = rand() % area_width;
        int y = rand() % area_height;
        int tile = y * area_width + x;
        if ((world->mask[y * area_width + x] & COMPONENT_SOLID) != COMPONENT_SOLID) {
            return tile;
        }
    }
}

int create_player (int x, int y, World *world) {
    int player = create_entity(world);
    world->mask[player] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_ACTION | COMPONENT_TYPE | COMPONENT_SOLID;
    world->position[player] = (Position) {.x = x,
                                          .y = y};
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

int create_villager(int x, int y, World *world) {
    int tile;
    int villager = create_entity(world);
    world->mask[villager] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_ACTION |
                            COMPONENT_TYPE | COMPONENT_SOLID | COMPONENT_REACTION;
    world->position[villager] = (Position) {.x = x,
                                            .y = y};
    world->appearance[villager] = (Appearance) {
        .chr = '@',
        .fg = YELLOW,
        .bg = BLACK,
        .attrs = 0
    };
    world->action[villager] = &villager_action;
    world->reaction[villager] = &villager_reaction;
    world->type[villager] = VILLAGER;
    return villager;
}

int create_grass (int x, int y, World *world) {
    int tile;
    if (world->mask[y * area_width + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = y * area_width + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
    world->position[tile] = (Position) {.x = x,
                                        .y = y};
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
    if (world->mask[(y * area_width) + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = (y * area_width) + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_SOLID | COMPONENT_REACTION;
    world->position[tile] = (Position) {.x = x,
                                        .y = y};
    world->appearance[tile] = (Appearance) {
        .chr = 'T',
        .fg = GREEN,
        .bg = BLACK,
        .attrs = 0
    };
    world->type[tile] = TREE;
    world->reaction[tile] = &tree_reaction;
    return tile;
}

int create_water (int x, int y, World *world) {
    int tile;
    if (world->mask[(y * area_width) + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = (y * area_width) + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_REACTION;
    world->position[tile] = (Position) {.x = x,
                                        .y = y};
    world->appearance[tile] = (Appearance) {
        .chr = '~',
        .fg = BLUE,
        .bg = BLACK,
        .attrs = 0
    };
    world->type[tile] = WATER;
    world->reaction[tile] = &water_reaction;
    return tile;
}

int create_flower (int x, int y, World *world) {
    int tile;
    if (world->mask[(y * area_width) + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = (y * area_width) + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_REACTION;
    world->position[tile] = (Position) {.x = x,
                                        .y = y};
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

int create_wood_wall (int x, int y, World *world) {
    int tile;
    if (world->mask[(y * area_width) + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = (y * area_width) + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_SOLID;
    world->position[tile] = (Position) {.x = x,
                                        .y = y};
    world->appearance[tile] = (Appearance) {
        .chr = ' ',
        .fg = YELLOW,
        .bg = BLACK,
        .attrs = A_DIM | A_REVERSE
    };
    world->type[tile] = WALL;
    return tile;
}

int create_stone_wall (int x, int y, World *world) {
    int tile;
    if (world->mask[(y * area_width) + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = (y * area_width) + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_SOLID;
    world->position[tile] = (Position) {.x = x,
                                        .y = y};
    world->appearance[tile] = (Appearance) {
        .chr = ' ',
        .fg = BLACK,
        .bg = BLACK,
        .attrs = A_BOLD | A_REVERSE
    };
    world->type[tile] = WALL;
    return tile;
}

int create_floor (int x, int y, World *world) {
    int tile;
    if (world->mask[(y * area_width) + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = (y * area_width) + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
    world->position[tile] = (Position) {.x = x,
                                        .y = y};
    world->appearance[tile] = (Appearance) {
        .chr = '.',
        .fg = YELLOW,
        .bg = BLACK,
        .attrs = A_DIM
    };
    world->type[tile] = PATH;
    return tile;
}

int create_door (int x, int y, World *world) {
    int tile;
    if (world->mask[(y * area_width) + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = (y * area_width) + x;
    }
    world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
    world->position[tile] = (Position) {.x = x,
                                        .y = y};
    world->appearance[tile] = (Appearance) {
        .chr = '+',
        .fg = YELLOW,
        .bg = BLACK,
        .attrs = A_DIM
    };
    world->type[tile] = DOOR;
    return tile;
}

void generate_nature (World *world) {
    int y = 0;
    int x = 0;
    for (y = 0; y < area_height; y++) {
        for (x = 0; x < area_width; x++) {
            create_grass(x, y, world);
        }
    }
    for (y = 0; y < area_height; y++) {
        for (x = 0; x < area_width; x++) {
            if (rand() % 15 == 0) {
                create_tree(x, y, world);
            }
        }
    }
    for (y = 0; y < area_height; y++) {
        for (x = 0; x < area_width; x++) {
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

void create_building(int x, int y, World *world) {
    int building_width = 5;
    int building_height = 4;
    int door_x;
    int door_y;
    bool door_placed = false;
    char *building_ascii = "#####"
                           "#...#"
                           "#...#"
                           "#####";
    for (int j = 0; j < building_height; j++) {
        for (int i = 0; i < building_width; i++) {
            char chr = building_ascii[j * building_width + i];
            if (chr == '.') {
                create_floor(x + i, y + j, world);
            }
            if (chr == '#') {
                create_wood_wall(x + i, y + j, world);
            }
        }
    }
    if (rand() % 2 == 0) {
        door_x = 1 + (rand() % 3);
        door_y = (rand() % 2) == 0 ? 0 : 3;
    } else {
        door_y = 1 + (rand() % 2);
        door_x = (rand() % 2) == 0 ? 0 : 4;
    }
    create_door(x + door_x, y + door_y, world);
}

void create_fountain(int x, int y, World *world) {
    int fountain_width = 3;
    int fountain_height = 3;
    char *fountain_ascii = " # "
                           "#~#"
                           " # ";
    for (int j = 0; j < fountain_height; j++) {
        for (int i = 0; i < fountain_width; i++) {
            char chr = fountain_ascii[j * fountain_width + i];
            if (chr == '#') {
                create_stone_wall(x + i, y + j, world);
            }
            if (chr == '~') {
                create_water(x + i, y + j, world);
            }
        }
    }
}

void generate_village(World *world) {
    int x, y;
    int fountain_x = rand() % 3;
    int fountain_y = rand() % 2;
    int building_count = 0;
    for (y = 0; y < area_height; y++) {
        for (x = 0; x < area_width; x++) {
            create_grass(x, y, world);
        }
    }
    for (y = 0; y < 2; y++) {
        for (x = 0; x < 3; x++) {
            if (x == fountain_x && y == fountain_y) {
                continue;
            }
            if (rand() % 2 == 0) {
                create_building(x * 9 + 2, y * 6 + 1, world);
                building_count++;
            }
        }
    }
    if (rand() % 3 == 0) {
        int x = rand() % 3;
        int y = rand() % 2;
        create_fountain(fountain_x * 9 + 3, fountain_y * 6 + 1 + (rand() % 2), world);
    }
    for (int i = 0; i < building_count; i++) {
        int tile = random_empty_tile(world);
        create_villager(world->position[tile].x, world->position[tile].y, world);
    }
}
