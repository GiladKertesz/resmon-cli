CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC = main.c args.c read_utils.c
OBJ = $(SRC:.c=.o)
EXEC = resmon-cli

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC)
