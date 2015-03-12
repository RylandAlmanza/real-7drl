#include <stdlib.h>
#include <stdio.h>
#include "beings.h"
#include "levelgen.h"
#include "reactions.h"
#include "interface.h"
#include "actions.h"
#include "globals.h"
#include "utilities.h"
#include "tiles.h"

typedef struct EntitySaveStruct EntitySave;

struct EntitySaveStruct {
    int x;
    int y;
    EntityType type;
};

void save_area (World *world) {
    int i, x, y;
    FILE *area_file;
    FILE *entity_file;
    FILE *count_file;
    char filename[255];
    sprintf(filename, "world/%d-%d.area", area_x, area_y);
    area_file = fopen(filename, "wb");
    for (y = 0; y < area_height; y++) {
        for (x = 0; x < area_width; x++) {
            fwrite(&world->type[y * area_width + x], sizeof(EntityType), 1, area_file);
        }
    }

    fclose(area_file);
    sprintf(filename, "world/%d-%d.ents", area_x, area_y);
    entity_file = fopen(filename, "wb");

    EntitySave entity_save;
    int entity_count = 0;
    for (i = area_width * area_height; i < MAX_ENTITIES; i++) {
        if (world->mask[i] != COMPONENT_NONE) {
            entity_save = (EntitySave) {
                .x = world->position[i].x,
                .y = world->position[i].y,
                .type = world->type[i]
            };
            fwrite(&entity_save, sizeof(EntitySave), 1, entity_file);
            entity_count++;
        }
    }

    fclose(entity_file);
    sprintf(filename, "world/%d-%d.coun", area_x, area_y);
    count_file = fopen(filename, "wb");

    fwrite(&entity_count, sizeof(int), 1, count_file);

    fclose(count_file);
}

void load_area(World *world) {
    int i, x, y;
    FILE *area_file;
    FILE *entity_file;
    FILE *count_file;
    char filename[255];
    sprintf(filename, "world/%d-%d.area", area_x, area_y);
    area_file = fopen(filename, "rb");
    EntityType entity_type;
    for (y = 0; y < area_height; y++) {
        for (x = 0; x < area_width; x++) {
            fread(&entity_type, sizeof(EntityType), 1, area_file);
            create_tile(x, y, entity_type, world);
        }
    }

    fclose(area_file);
    sprintf(filename, "world/%d-%d.coun", area_x, area_y);
    count_file = fopen(filename, "rb");
    int entity_count;
    fread(&entity_count, sizeof(int), 1, count_file);
    fclose(count_file);

    sprintf(filename, "world/%d-%d.ents", area_x, area_y);
    entity_file = fopen(filename, "rb");

    EntitySave entity_save;
    for (i = 0; i < entity_count; i++) {
        fread(&entity_save, sizeof(EntitySave), 1, entity_file);
        int being = create_being(entity_save.x, entity_save.y, entity_save.type, world);
        if (entity_save.type == PLAYER) player = being;
    }
    fclose(entity_file);
}

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

void generate_nature (World *world) {
    int y = 0;
    int x = 0;
    for (y = 0; y < area_height; y++) {
        for (x = 0; x < area_width; x++) {
            create_tile(x, y, GRASS, world);
        }
    }
    for (y = 0; y < area_height; y++) {
        for (x = 0; x < area_width; x++) {
            if (rand() % 15 == 0) {
                create_tile(x, y, TREE, world);
            }
        }
    }
    for (y = 0; y < area_height; y++) {
        for (x = 0; x < area_width; x++) {
            if (rand() % 20 == 0) {
                create_tile(x, y, FLOWER, world);
            }
        }
    }
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 24; x++) {
            if (rand() % 50 == 0) {
                for (int j = 0; j < 3; j++) {
                    for (int i = 0; i < 3; i++) {
                        if (rand() % 2 == 0) {
                            create_tile(x + i, y + j, WATER, world);
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
                create_tile(x + i, y + j, FLOOR, world);
            }
            if (chr == '#') {
                create_tile(x + i, y + j, WOOD_WALL, world);
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
    create_tile(x + door_x, y + door_y, DOOR, world);
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
                create_tile(x + i, y + j, STONE_WALL, world);
            }
            if (chr == '~') {
                create_tile(x + i, y + j, WATER, world);
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
            create_tile(x, y, GRASS, world);
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
        create_being(world->position[tile].x, world->position[tile].y, VILLAGER, world);
    }
}
