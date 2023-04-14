#include "UI.h"

struct DrawAreaStructure{
    int x;
    int y;
    int x_pos;
    int y_pos;
    int continued;
    int height;
    int width;
    XColor color;
};

typedef struct DrawAreaStructure DrawArea;

Component* newDrawArea(char*, int, int, int, int, int, int, int);