#include "UI.h"
#include "drawArea.h"

#include <stdlib.h>


int main(){



	Component* d1 = newDrawArea("draw area 1", 10, 10, 485, 580,65535, 0, 0);
	Component* d2 = newDrawArea("draw area 2", 505, 10, 485, 580, 0, 0, 65535);
	registerComponent(d1);
	registerComponent(d2);
	initialize(0, 0, 1000, 600, 5, "Better Notes!", "BetterNotes");
	run();

	return 0;
}



