run: program
	./main

program: main.c
	gcc -o main main.c drawArea.c UI.c drawArea.h UI.h -lX11

clean:
	rm -f main