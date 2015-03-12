#include "beings.h"
#include "actions.h"
#include "reactions.h"
#include "interface.h"

int create_being(int x, int y, EntityType entity_type, World *world) {
    int being = create_entity(world);
    world->position[being] = (Position) {.x = x,
                                         .y = y};
    world->type[being] = entity_type;
    switch (entity_type) {
        case PLAYER:
            world->mask[being] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_ACTION | COMPONENT_TYPE | COMPONENT_SOLID;
            world->appearance[being] = (Appearance) {
                .chr = '@',
                .fg = WHITE,
                .bg = BLACK,
                .attrs = 0
            };
            world->action[being] = &player_action;
            break;
        case VILLAGER:
            world->mask[being] = COMPONENT_POSITION | COMPONENT_APPEARANCE | COMPONENT_ACTION |
                                 COMPONENT_TYPE | COMPONENT_SOLID | COMPONENT_REACTION;
            world->appearance[being] = (Appearance) {
                .chr = '@',
                .fg = YELLOW,
                .bg = BLACK,
                .attrs = 0
            };
            world->action[being] = &villager_action;
            world->reaction[being] = &villager_reaction;
            break;
    }
            
    return being;
}
