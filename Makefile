# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nrabarij <nrabarij@student.42antanana>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/30 18:27:41 by nrabarij          #+#    #+#              #
#    Updated: 2024/06/17 13:15:08 by nrabarij         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	pipex

PROGBIN			=	libpipex.a

CC				=	gcc

CFLAG			=	-Wall -Wextra -Werror

BIN				=	libft.a

DIRBIN			=	./libft

BINPROG			=	-L./ -lpipex

LIBFT			=	$(addprefix $(DIRBIN)/, $(BIN))

RM				=	rm -rf

DIRSRC			=	./src

SRC				=	ppx_init.c ppx_exec.c ppx_split.c

SRCS			=	$(addprefix $(DIRSRC)/, $(SRC))

OBJS			=	$(SRCS:%.c=%.o)

MAIN			=	$(DIRSRC)/pipex.c

%.o				:	%.c
					$(CC) $(CFLAG) -c $< -o $@

$(NAME)			:	$(PROGBIN) $(LIBFT) $(SRCS) $(MAIN)
					$(CC) $(CFLAG) $(MAIN) $(BINPROG) -o $(NAME)

$(PROGBIN)		:	$(LIBFT) $(OBJS)
					cp $(LIBFT) $(PROGBIN)
					ar rcs $(PROGBIN) $(OBJS)

$(LIBFT)		:
					make -C $(DIRBIN) all

all				:	$(NAME)

clean			:
					make -C $(DIRBIN) clean
					$(RM) $(OBJS)

fclean			:	clean
					make -C $(DIRBIN) fclean
					$(RM) $(PROGBIN)
					$(RM) $(NAME)

re				:	fclean all

### TEST RULES #################################################################
VG			=	valgrind
VGF1		=	--leak-check=full --show-leak-kinds=all --track-origins=yes
VGF2		=	--track-fds=yes --trace-children=yes
VALGRIND	=	$(VG) $(VGF1) $(VGF2)
IPF			=	Makefile
OPF			=	test.txt
CMD1		=	"cat -e"
CMD2		=	"cat -e"
PPX_ARG		=	$(IPF) $(CMD1) $(CMD2) $(OPF)
leaks		:	$(NAME)
				$(VALGRIND) ./$(NAME) $(PPX_ARG)
################################################################################

.PHONY			:	all clean fclean re
