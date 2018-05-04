# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    c.Makefile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pribault <pribault@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/16 14:01:15 by pribault          #+#    #+#              #
#    Updated: 2018/04/20 13:46:12 by pribault         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libsocket.a
NAMESO = $(NAME:%.a=%.so)
CC = clang
SRC =	socket_new.c socket_delete.c\
		socket_bind.c socket_unbind.c\
		socket_set_callback.c socket_connect.c\
		socket_poll_events.c\
		socket_add_incoming_client.c\
		socket_get_incoming_message.c\
		socket_manage_incoming_messages.c\
		socket_manage_write_requests.c\
		socket_find_client_by_address.c\
		socket_remove_client.c\
		socket_enqueue_write.c\
		socket_enqueue_write_by_fd.c\
		socket_add_client_by_fd.c\
		socket_attach_data.c\
		socket_get_data.c\
		socket_set_timeout.c\
		socket_get_timeout.c\
		socket_set_read_buffer_size.c\
		socket_get_read_buffer_size.c\
		client_attach_data.c\
		client_get_data.c\
		client_get_address.c\
		client_get_fd.c
DIR = c
SRC_DIR = src
OBJ_DIR = .obj
OBJ =	$(sort $(patsubst %.c, $(OBJ_DIR)/$(DIR)/%.o, $(SRC)))
FLAGS = -Wall -Wextra -Werror
INCLUDES =	libsocket.h\
			libsocket_defines.h\
			libsocket_enums.h\
			libsocket_structures.h
INCLUDE =	$(sort \
			$(patsubst %.h, include/%.h,\
			$(INCLUDES)))
N =	0
MAX =	$(words $(OBJ))
COMPILED =	false
LIBFT =			libft
LIBFT_INC_DIR =	$(LIBFT)/include
LIBFT_INCLUDES =	libft.h\
					ft_printf.h\
					ft_joinf.h\
					malloc.h\
					prototypes.h\
					structs.h
DEPENDENCIES =	$(sort \
				$(patsubst %.h, $(LIBFT_INC_DIR)/%.h,\
				$(LIBFT_INCLUDES)))
PREFIX_NAME =	libsocket_C

.PHONY: clean fclean all re norme

.SILENT:

all: $(NAME)
	@if [ $(COMPILED) = true ]; then echo "\033[0m\033[38;5;166m[$(PREFIX_NAME) ∎∎∎∎] \033[0m🐹  \033[38;5;214m$(NAME) done\033[0m"; else echo "\033[0m\033[38;5;166m[$(PREFIX_NAME) ∎∎∎∎] \033[0m🐹  \033[38;5;214mnothing to be done\033[0m"; fi

$(OBJ_DIR):
	@mkdir $@

$(OBJ_DIR)/$(DIR): | $(OBJ_DIR)
	@mkdir $@

$(OBJ_DIR)/$(DIR)/%.o: $(SRC_DIR)/$(DIR)/%.c $(INCLUDE) $(DEPENDENCIES) | $(OBJ_DIR)/$(DIR)
	@$(CC) $(FLAGS) -fPIC -I include -I $(LIBFT_INC_DIR) -o $@ -c $<
	@$(eval N=$(shell echo $$(($(N)+1))))
	@$(eval PERCENT=$(shell echo $$((($(N)*100)/$(MAX)))))
	@printf "\033[0m\033[38;5;166m[$(PREFIX_NAME) \033[38;5;7m%3u%%\033[38;5;166m] \033[0m🌶  \033[38;5;226m$(@:$(OBJ_DIR)/$(DIR)/%.o=%.o) done\033[0m\n" $(PERCENT)

$(NAME): $(OBJ)
	@ar rc $(NAME) $(OBJ)
	@ranlib $(NAME)
	@$(eval COMPILED=true)

clean:
	@rm -rf $(OBJ_DIR)/$(DIR)
	@echo "\033[0m\033[38;5;166m[$(PREFIX_NAME) ∎∎∎∎] \033[0m\033[38;5;45mobject files removed\033[0m"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(NAMESO)
	@echo "\033[0m\033[38;5;166m[$(PREFIX_NAME) ∎∎∎∎] \033[0m\033[38;5;87m$(NAME) and $(NAMESO) removed\033[0m"

norme:
	@norminette $(OBJ:$(OBJ_DIR)/%.o=$(SRC_DIR)/%.c) $(INCLUDE)

re: fclean all
