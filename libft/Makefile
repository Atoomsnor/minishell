# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nhendrik <nhendrik@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/22 15:49:19 by roversch          #+#    #+#              #
#    Updated: 2025/07/10 15:06:54 by nhendrik         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRC = ft_atoi.c ft_atol.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c \
ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c \
ft_putnbr.c ft_putnbr_fd.c ft_putstr.c ft_putstr_fd.c ft_split.c \
ft_strchr.c ft_striteri.c ft_strdup.c ft_strjoin.c ft_strlcat.c \
ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c \
ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
ft_iswhitespace.c

OBJ_DIR = obj
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I ./include -I printf/include -I get_next_line/include

# Directories
PRINTF_DIR = printf/source
GNL_DIR = get_next_line/source

# Source files for printf & get_next_line
PRINTF_SRC = ft_printf.c printf_base10.c printf_base16.c printf_chars.c
GNL_SRC = get_next_line.c get_next_line_utils.c

# Object files for printf & get_next_line (fixed)
PRINTF_OBJ = $(PRINTF_SRC:%.c=$(OBJ_DIR)/%.o)
GNL_OBJ = $(GNL_SRC:%.c=$(OBJ_DIR)/%.o)

# All object files
ALL_OBJ = $(OBJ) $(PRINTF_OBJ) $(GNL_OBJ)

all: $(NAME)

$(NAME): $(ALL_OBJ)
	ar rc $(NAME) $(ALL_OBJ)

# Compile libft objects
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -o $@ $< $(INCLUDE)

# Compile printf objects into obj
$(OBJ_DIR)/%.o: $(PRINTF_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -o $@ $< $(INCLUDE)

# Compile gnl objects into obj
$(OBJ_DIR)/%.o: $(GNL_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -o $@ $< $(INCLUDE)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re