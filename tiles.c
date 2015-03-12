#include "tiles.h"
#include "interface.h"
#include "world.h"
#include "reactions.h"
#include "globals.h"

int create_tile (int x, int y, EntityType entity_type, World *world) {
    int tile;
    if (world->mask[y * area_width + x] == COMPONENT_NONE) {
        tile = create_entity(world);
    } else {
        tile = y * area_width + x;
    }
    world->position[tile] = (Position) {.x = x,
                                        .y = y};
    world->type[tile] = entity_type;

    switch (entity_type) {
        case GRASS:
            world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
            world->appearance[tile] = (Appearance) {
                .chr = '.',
                .fg = GREEN,
                .bg = BLACK,
                .attrs = 0
            };
            break;
        case TREE:
            world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_SOLID | COMPONENT_REACTION;
            world->appearance[tile] = (Appearance) {
                .chr = 'T',
                .fg = GREEN,
                .bg = BLACK,
                .attrs = 0
            };
            world->reaction[tile] = &tree_reaction;
            break;
        case WATER:
            world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_REACTION;
            world->appearance[tile] = (Appearance) {
                .chr = '~',
                .fg = BLUE,
                .bg = BLACK,
                .attrs = 0
            };
            world->reaction[tile] = &water_reaction;
            break;
        case FLOWER:
            world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_REACTION;
            world->appearance[tile] = (Appearance) {
                .chr = '*',
                .fg = YELLOW,
                .bg = BLACK,
                .attrs = 0
            };
            world->reaction[tile] = &flower_reaction;
            break;
        case FLOWER_CRUSHED:
            world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
            world->appearance[tile] = (Appearance) {
                .chr = '"',
                .fg = YELLOW,
                .bg = BLACK,
                .attrs = 0
            };
            break;
        case WOOD_WALL:
            world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_SOLID;
            world->appearance[tile] = (Appearance) {
                .chr = ' ',
                .fg = YELLOW,
                .bg = BLACK,
                .attrs = A_DIM | A_REVERSE
            };
            break;
        case STONE_WALL:
            world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_SOLID;
            world->appearance[tile] = (Appearance) {
                .chr = ' ',
                .fg = BLACK,
                .bg = BLACK,
                .attrs = A_BOLD | A_REVERSE
            };
            break;
        case FLOOR:
            world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
            world->appearance[tile] = (Appearance) {
                .chr = '.',
                .fg = YELLOW,
                .bg = BLACK,
                .attrs = A_DIM
            };
            break;
        case DOOR:
            world->mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
            world->appearance[tile] = (Appearance) {
                .chr = '+',
                .fg = YELLOW,
                .bg = BLACK,
                .attrs = A_DIM
            };
            break;
    }
    return tile;
}
