##
## ETNA PROJECT, 02/01/2023 by meritt_j
## Makefile
## File description:
##      Makefile for lib
##

NAME	=		libmy.a

SRC		=	properties.c			\
			logging.c				\
			external_lib/cJSON.c	\
			internal.c				\
			server.c 				\

OBJ		=		$(SRC:.c=.o)


all:		$(NAME)

$(NAME):	$(OBJ)
			ar -rc $(NAME) $(OBJ)

clean:
			rm -f $(OBJ)

fclean:		clean
			rm -f $(NAME)

re:		fclean all

.PHONY:	all clean fclean re
