#ifndef __UI__
#define __UI__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

struct ComponentStructure{
    void* object;
    char* name;
    int x1;
    int x2;
    int y1;
    int y2;
    void (*initialize)(void*, Display*, Window, GC, Colormap);
    void (*redraw)(void*, Display*, Window, GC);
    void (*focus)(void*, Display*, Window, GC, int, int);
    void (*unfocus)(void*, Display*, Window, GC, int, int);
    void (*buttonPress)(void*, Display*, Window, GC, int, int);
    void (*drag)(void*, Display*, Window, GC, int, int);
};

typedef struct ComponentStructure Component;

void initialize(int, int, int, int, int, char*, char*);
Component* newComponent(void*, int, int, int, int);
void registerComponent(Component*);
void run();
#endif