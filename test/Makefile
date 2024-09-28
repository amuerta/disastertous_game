CC=clang
DEBUG=-g #"-fsanitize=address" // cant init window 
LIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11 

collision_bin:
	$(CC) collision.c -o collision $(LIBS) $(DEBUG)
