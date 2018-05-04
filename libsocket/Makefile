# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pribault <pribault@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/16 11:40:05 by pribault          #+#    #+#              #
#    Updated: 2018/04/18 15:59:17 by pribault         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libsocket.a
NAMESO = $(NAME:%.a=%.so)
MAKE_DIR = .Makefiles
OBJ_DIR = .obj
EXT = Makefile
CMAKEFILE = $(MAKE_DIR)/c.$(EXT)
CPPMAKEFILE = $(MAKE_DIR)/cpp.$(EXT)
TESTMAKEFILE = $(MAKE_DIR)/test.$(EXT)

.PHONY: clean fclean all re norme $(NAME) $(NAMESO) $(CMAKEFILE) $(CPPMAKEFILE)

.SILENT:

$(NAME): $(CMAKEFILE) $(CPPMAKEFILE)

$(NAMESO): $(NAME)
	@$(eval OBJ=$(shell ar t $(NAME)))
	@ar x $(NAME)
	@$(CC) -o $@ -shared $(OBJ)
	@rm $(OBJ)
	@echo "\033[0m\033[38;5;166m[$@ ∎∎∎∎] \033[0m🐹  \033[38;5;214mdone\033[0m"

$(CMAKEFILE):
	@make -f $(CMAKEFILE)

$(CPPMAKEFILE):
	@make -f $(CPPMAKEFILE)

test: $(NAME)
	@make -f $(TESTMAKEFILE)

all: $(NAME)

clean:
	@make -f $(CMAKEFILE) clean
	@make -f $(CPPMAKEFILE) clean

fclean:
	@make -f $(CMAKEFILE) fclean
	@make -f $(CPPMAKEFILE) fclean
	@make -f $(TESTMAKEFILE) fclean
	@rm -f $(NAMESO)

norme:
	@make -f $(CMAKEFILE) norme
	@make -f $(CPPMAKEFILE) norme

re:
	@make -f $(CMAKEFILE) re
	@make -f $(CPPMAKEFILE) re
