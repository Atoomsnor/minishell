NAME = minishell

SRC = main.c string_split.c pipex.c pathfinding.c
SRC_DIR = source

SRC_COPE	= $(SRC:%=$(SRC_DIR)/%)
OBJ			= $(SRC:%.c=$(OBJ_DIR)/%.o)
OBJ_DIR		= obj

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -pthread

LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJ)
	$(CC) $(OBJ) -L. $(LIBFT_LIB) -lreadline $(CFLAGS) -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	
re: fclean all

.PHONY: all clean fclean re