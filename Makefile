# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pribault <pribault@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/04 14:45:06 by pribault          #+#    #+#              #
#    Updated: 2018/05/04 14:56:04 by pribault         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	ft_ping
FLAGS =	-Wall -Wextra -Werror
CC = clang

OBJ_DIR =	.obj
SRC_DIR =	src
INC_DIR =	include

SRC =	ft_ping.c
OBJ =	$(sort $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC)))
INC =	ft_ping.h

LIBFT =	libft
LIBFT_INC_DIR =	$(LIBFT)/include
LIBFT_INC =	libft.h ft_printf.h ft_joinf.h\
			malloc.h prototypes.h structs.h

INCLUDES =	$(sort \
			$(patsubst %.h, $(INC_DIR)/%.h, $(INC))\
			$(patsubst %.h, $(LIBFT_INC_DIR)/%.h, $(LIBFT_INC)))

.PHONY: all clean fclean re

all: $(NAME)

$(LIBFT)/libft.a:
	make -C $(LIBFT)

$(NAME): $(OBJ)
	echo $(OBJ)
	echo $(INCLUDES)
	$(CC) -o $@ $(FLAGS) $(OBJ) -L $(LIBFT) -lft

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDES) $(LIBFT)/libft.a | $(OBJ_DIR)
	$(CC) -o $@ $(FLAGS) -I $(INC_DIR) -I $(LIBFT_INC_DIR) -c $<

clean:
	make -C $(LIBFT) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	make -C $(LIBFT) fclean
	rm -f $(NAME)

re: fclean all