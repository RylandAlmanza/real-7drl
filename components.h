#ifndef COMPONENTS_H
#define COMPONENTS_H

typedef enum ComponentEnum Component;

enum ComponentEnum {
    COMPONENT_NONE = 0,
    COMPONENT_POSITION = 1 << 0,
    COMPONENT_APPEARANCE = 1 << 1,
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
};

#endif
