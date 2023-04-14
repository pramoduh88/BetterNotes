#include "UI.h"
#include <stdlib.h>

int __COMPONENTCOUNT__;
Component** __COMPONENTLIST__;
struct ComponentNode* __TEMPCOMPONENTLIST__;
struct ComponentNode* __CURRENTTEMPNODE__;

Display *dis;
int __SCREEN__;
Window win;
GC gc;
Atom wmDeleteMessage;
Colormap cmap; 

void redraw();
void close_x();

struct ComponentNode {
    Component* component;
    struct ComponentNode* next;
};

void registerComponent(Component* c){

    struct ComponentNode* newNode = (struct ComponentNode*) malloc(sizeof(struct ComponentNode));
    newNode -> component = c;
    newNode -> next = NULL;
    if(!__COMPONENTCOUNT__){
        __TEMPCOMPONENTLIST__ =  newNode;     
    }else{
        __CURRENTTEMPNODE__ -> next = newNode;
    }
    __CURRENTTEMPNODE__ = newNode;
    __COMPONENTCOUNT__++;
    return;
}

void initialize(int x, int y, int width, int height, int borderWidth, char* windowName, char* iconName){

	unsigned long black,white;
	dis=XOpenDisplay((char *)0);
   	__SCREEN__=DefaultScreen(dis);
	black=BlackPixel(dis,__SCREEN__),
	white=WhitePixel(dis,__SCREEN__);
   	win=XCreateSimpleWindow(dis,DefaultRootWindow(dis),x,y, width, height, borderWidth,black, white);
    cmap= XDefaultColormap(dis,__SCREEN__);
	XSetStandardProperties(dis,win,windowName,iconName,None,NULL,0,NULL);
	XSelectInput(dis, win, ExposureMask|ButtonPressMask| ButtonReleaseMask|Button1MotionMask);
	gc=XCreateGC(dis, win, 0,0);        
	XSetBackground(dis,gc,white);
	XSetForeground(dis,gc,black);
	XClearWindow(dis, win);
	XMapRaised(dis, win);

	wmDeleteMessage = XInternAtom(dis, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dis, win, &wmDeleteMessage, 1);

    if(__COMPONENTCOUNT__>0){
        __COMPONENTLIST__ = (Component**) malloc(sizeof(Component*)*__COMPONENTCOUNT__);
    }

    int i;
    for(i=0; i<__COMPONENTCOUNT__; i++){
        __COMPONENTLIST__[i] = __TEMPCOMPONENTLIST__->component;      
        __CURRENTTEMPNODE__ = __TEMPCOMPONENTLIST__;
        __TEMPCOMPONENTLIST__ = __TEMPCOMPONENTLIST__ -> next;
        free(__CURRENTTEMPNODE__);
        
    }
    for(i=0; i<__COMPONENTCOUNT__; i++){
        __COMPONENTLIST__[i]->initialize(__COMPONENTLIST__[i]->object,dis,win,gc, cmap);
    }

}

void redraw(){

    XClearWindow(dis, win);
    int i;
    for(i=0; i<__COMPONENTCOUNT__; i++){
        __COMPONENTLIST__[i]->redraw(__COMPONENTLIST__[i]->object, dis, win, gc);
    }

}

Component* getComponent(int x, int y){
    int i;
    for(i=0; i<__COMPONENTCOUNT__; i++){
        if(__COMPONENTLIST__[i]->x1 <= x && __COMPONENTLIST__[i]->x2 >= x && __COMPONENTLIST__[i]->y1 <= y && __COMPONENTLIST__[i]->y2 >= y){
            return __COMPONENTLIST__[i];
        }
    }
    return NULL;
}

void run(){

        XEvent event;
        Component* currentComponent = NULL;
        Component* oldComponent = NULL;
        int x;
        int y;

    	while(1){
		
		XNextEvent(dis, &event);

		if (event.type==Expose && event.xexpose.count==0) {
			redraw();
		}

        if (event.type == ClientMessage && event.xclient.data.l[0] == wmDeleteMessage){
			close_x(dis, win, gc);
        }
		if (event.type==ButtonPress) {
            x = event.xbutton.x;
            y = event.xbutton.y;
            currentComponent = getComponent(x, y);
            if (currentComponent){
                if(!oldComponent){
                    currentComponent -> focus(currentComponent->object, dis, win, gc, x, y);
                }else{
                    if(currentComponent != oldComponent){
                        oldComponent -> unfocus(oldComponent->object, dis, win, gc, x, y);
                        currentComponent -> focus(currentComponent->object, dis, win, gc, x, y);
                    }
                }

                oldComponent = currentComponent;      
                currentComponent ->buttonPress(currentComponent->object, dis, win, gc, x, y);
            }
		}

		if (event.type == MotionNotify){
            x = event.xbutton.x;
            y = event.xbutton.y;            
            currentComponent = getComponent(x, y);
            if(currentComponent){
                if(oldComponent && currentComponent != oldComponent){
                    oldComponent -> unfocus(oldComponent->object, dis, win, gc, x, y);
                    currentComponent -> focus(currentComponent->object, dis, win, gc, x, y);
                }
                oldComponent = currentComponent;                  
                currentComponent ->drag(currentComponent->object, dis, win, gc, x, y);
            }
		}						

	}

}

void close_x() {
	XFreeGC(dis, gc);
	XDestroyWindow(dis, win);
	XCloseDisplay(dis);	
	exit(0);				
}

void mockInitialize(void* obj, Display* dip, Window win, GC gc, Colormap cmap){
    return;
}

void mockRedraw(void* obj, Display* dip, Window win, GC gc){
    return;
}

void mockAction(void* obj, Display* dis, Window win, GC gc, int x, int y){
    return;
}


Component* newComponent(void* obj, int x1, int y1, int x2, int y2){

    Component* c = (Component*) malloc(sizeof(Component));
    c -> object = obj;
    if (x2 >= x1){
        c -> x1 = x1;
        c -> x2 = x2;
    }else{
        c -> x1 = x2;
        c -> x2 = x1;
    }
    if (y2 >= y1){
        c -> y1 = y1;
        c -> y2 = y2;
    }else{
        c -> y1 = y2;
        c -> y2 = y1;
    }
    c -> initialize = mockInitialize;
    c -> redraw = mockRedraw;
    c -> focus = mockAction;
    c -> unfocus = mockAction;
    c -> drag = mockAction;    

    return c;
}