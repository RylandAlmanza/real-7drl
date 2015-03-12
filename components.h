#ifndef COMPONENTS_H
#define COMPONENTS_H

typedef enum ComponentEnum Component;

enum ComponentEnum {
    COMPONENT_NONE = 0,
    COMPONENT_POSITION = 1 << 0,
    COMPONENT_APPEARANCE = 1 << 1,
    COMPONENT_ACTION = 1 << 2,
    COMPONENT_REACTION = 1 << 4,
    COMPONENT_SOLID = 1 << 8,
    COMPONENT_TYPE = 1 << 16,
};

typedef struct PositionStruct Position;

struct PositionStruct {
    int x;
    int y;
};

typedef struct AppearanceStruct Appearance;

struct AppearanceStruct {
    char chr;
    int fg;
    int bg;
    int attrs;
};

typedef enum ActionEnum Action;

enum ActionEnum {
    ATTACK,
    STEP_ON,
    BUMP,
    GREET
};

typedef enum EntityTypeEnum EntityType;

enum EntityTypeEnum {
    PLAYER,
    GRASS,
    TREE,
    FLOWER,
    WATER,
    FLOWER_CRUSHED,
    FLOOR,
    DOOR,
    VILLAGER,
    WOOD_WALL,
    STONE_WALL
};

#endif
