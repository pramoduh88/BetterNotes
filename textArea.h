#include "UI.h"

struct TextAreaStructure{
    int x;
    int y;
    char* text;
    int height;
    int width;
    XColor color;
};

typedef struct TextAreaStructure DrawArea;

Component* newTextArea(char*, int, int, int, int, int, int, int);