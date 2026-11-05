CC      = gcc
CFLAGS  = -Wall -Wextra -g -IC:/msys64/ucrt64/include/SDL2
LDFLAGS = -lmingw32 -lSDL2main -lSDL2

SRC    = main.c cpu.c bus.c window.c
OBJ    = $(SRC:.c=.o)
TARGET = nes.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

%.o: %.c cpu.h bus.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all cleanNo