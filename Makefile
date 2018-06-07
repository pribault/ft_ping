# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pribault <pribault@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/04 14:45:06 by pribault          #+#    #+#              #
#    Updated: 2018/06/06 23:55:04 by pribault         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	ft_ping
FLAGS =	-Wall -Wextra -Werror
CC = clang

OBJ_DIR =	.obj
SRC_DIR =	src
INC_DIR =	include

SRC =	ft_ping.c client_callback.c message_callback.c\
		flags.c endian.c ip.c icmp.c ping.c icmp_2.c signal.c\
		flags_2.c timeout.c
OBJ =	$(sort $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC)))
INC =	ft_ping.h ip.h icmp.h

LIBFT =	libft
LIBFT_INC_DIR =	$(LIBFT)/include
LIBFT_INC =	libft.h ft_printf.h ft_joinf.h\
			malloc.h prototypes.h structs.h

LIBSOCKET =	libsocket
LIBSOCKET_INC_DIR =	$(LIBSOCKET)/include
LIBSOCKET_INC =		libsocket.h libsocket_defines.h libsocket_enums.h\
					libsocket_structures.h

INCLUDES =	$(sort \
			$(patsubst %.h, $(INC_DIR)/%.h, $(INC))\
			$(patsubst %.h, $(LIBFT_INC_DIR)/%.h, $(LIBFT_INC))\
			$(patsubst %.h, $(LIBSOCKET_INC_DIR)/%.h, $(LIBSOCKET_INC)))

.PHONY: all clean fclean re $(LIBFT)/libft.a $(LIBSOCKET)/libsocket.a

all: $(NAME)

$(LIBFT)/libft.a:
	make -C $(LIBFT)

$(LIBSOCKET)/libsocket.a:
	make -C $(LIBSOCKET)

$(NAME): $(OBJ) $(LIBFT)/libft.a $(LIBSOCKET)/libsocket.a
	$(CC) -o $@ $(FLAGS) $(OBJ) -L $(LIBSOCKET) -lsocket -L $(LIBFT) -lft -lm
	sudo chown root:root $(NAME)
	sudo chmod 4755 $(NAME)

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDES) Makefile | $(OBJ_DIR)
	$(CC) -o $@ $(FLAGS) -I $(INC_DIR) -I $(LIBFT_INC_DIR) -I $(LIBSOCKET_INC_DIR) -c $<

clean:
	make -C $(LIBFT) clean
	make -C $(LIBSOCKET) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	make -C $(LIBFT) fclean
	make -C $(LIBSOCKET) fclean
	rm -f $(NAME)

re: fclean all
