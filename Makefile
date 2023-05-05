SRC = ./boggle.c \
	./grid.c

OBJ = $(SRC:.c=.o)
CFLAGS += -O2 -Wall -Wextra -Werror
NAME = boggle

$(NAME): $(OBJ)
	gcc -o $(NAME) $(OBJ)

all:     $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:     fclean all

.PHONY: all clean fclean re
