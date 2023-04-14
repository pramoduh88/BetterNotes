#include "drawArea.h"
#include "UI.h"
#include <stdlib.h>

void drawAreaInitialize(void* c, Display* dis, Window win, GC gc, Colormap cmap){

    DrawArea* d = (DrawArea*) c;
    XDrawRectangle(dis, win, gc, d->x, d->y, d->width, d->height);
    XAllocColor(dis, cmap, &(d->color));

}

void drawAreaRedraw(void* c, Display* dis, Window win, GC gc){

    DrawArea* d = (DrawArea*) c;
    XDrawRectangle(dis, win, gc, d->x, d->y, d->width, d->height);

}

void drawAreaFocus(void* c, Display* dis, Window win, GC gc, int x, int y){
    DrawArea* d = (DrawArea*) c;
    d -> x_pos = x;
    d -> y_pos = y;
    d -> continued = 1;
    XSetForeground(dis, gc, (d->color).pixel);

}

void drawAreaDrag(void* c, Display* dis, Window win, GC gc, int x, int y){
    DrawArea* d = (DrawArea*) c;
    if(d-> continued){
        XDrawLine(dis, win, gc, d->x_pos, d->y_pos, x, y);
    }
    d -> continued = 1;
    d -> x_pos = x;
    d -> y_pos = y;
}

void drawAreaUnfocus(void* c, Display* dis, Window win, GC gc, int x, int y){
    DrawArea* d = (DrawArea*) c;
    d-> continued = 0;
}

void drawAreaButtonPress(void* c, Display* dis, Window win, GC gc, int x, int y){
    DrawArea* d = (DrawArea*) c;
    d -> x_pos = x;
    d -> y_pos = y;
    d -> continued = 1;    
}

Component* newDrawArea(char* name, int x, int y, int width, int height, int red, int green, int blue){

    DrawArea* d = (DrawArea*) malloc(sizeof(DrawArea));
    d -> x = x;
    d -> y = y;
    d -> height = height;
    d -> width = width;
    XColor xcolour;
    xcolour.red = red; 
    xcolour.green = green; 
    xcolour.blue = blue;
    xcolour.flags = DoRed | DoGreen | DoBlue;
    d -> color = xcolour;

    Component* c = newComponent((void*)d, x, y, x+width, y+height);
    c -> name = name;
    c -> initialize = drawAreaInitialize;
    c -> redraw = drawAreaRedraw;
    c -> focus = drawAreaFocus;
    c -> unfocus = drawAreaUnfocus;
    c -> drag = drawAreaDrag;
    c -> buttonPress = drawAreaButtonPress;
    return c;

}
