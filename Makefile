
TARGET=main.c
EXECNAME=game


SRC=./src
GAME=$(SRC)/$(TARGET)
CC=clang
DEBUG=-g #"-fsanitize=address" // cant init window 
LIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11 
EXEC=./bin/$(EXECNAME)

build:
	$(CC) $(GAME) -o $(EXEC) $(LIBS) $(DEBUG)

run: build 	
	$(EXEC)
	
