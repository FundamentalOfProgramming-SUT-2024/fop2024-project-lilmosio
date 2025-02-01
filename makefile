CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lncurses

SRC = main.c game.c menu.c map.c player.c save.c
OBJ = $(SRC:.c=.o)
TARGET = rogue_game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)