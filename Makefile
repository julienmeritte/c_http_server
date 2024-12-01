##
## ETNA PROJECT, 02/01/2023 by meritt_j
## Makefile
## File description:
##      Makefile for lib
##

NAME    =   httpserver

NAME_TEST = units_test

SRC     =   ./main.c
 
SRC_TEST = ./lib/logging_test.c \
			./lib/server_test.c \

CFLAGS= -Wall -Wextra -std=c11


CPPFLAGS    =	-I./include/

CFLAGS_TEST = -I./include/ -Wall -Wextra -lcriterion --coverage

all:    $(NAME)

$(NAME):
			make -C lib/
			gcc -o $(NAME) $(SRC) $(CFLAGS) $(CPPFLAGS) -L./lib -lmy

clean:
			make clean -C lib/

fclean:     clean
			make fclean -C lib/
			rm -f $(NAME)
			rm -f $(NAME_TEST)
			find -name "*.gcda" -delete -o -name "*.gcno" -delete

test:
			make -C lib/
			gcc -o $(NAME_TEST) $(SRC_TEST) $(CFLAGS_TEST) -L./lib -lmy
			./$(NAME_TEST)

re:         fclean all

reTest:		fclean test

.PHONY:     all test clean fclean re test reTest
