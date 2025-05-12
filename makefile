all: main
main: main.c
	gcc main.c -o main -I$(HOME)/.local/include -L$(HOME)./local/lib -lSDL3 -lSDL3_ttf
