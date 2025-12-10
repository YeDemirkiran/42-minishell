NAME = test
CC = cc
FLAGS = -Wall -Wextra -Werror

INC_DIR = ../include/
LIBFT = ../libft.a

SRC = *.c

all: $(NAME)

$(NAME): $(SRC) $(LIBFT)
	$(CC) $(FLAGS) -o $(NAME) -I$(INC_DIR) $(SRC) $(LIBFT)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re