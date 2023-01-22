SRC	=	src/main.c

BIN	=	top

OBJ	=	$(SRC:.c=.o)

CFLAGS	+=	-Wall -Werror -Wextra -lncurses

CC	=	gcc

RM	=	rm -rf

all:	$(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(BIN)

re: fclean all

.PHONY: all clean fclean re
