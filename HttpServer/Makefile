NAME	=  ./HttpServer
RM		=	rm -f
CC		=   gcc -g -Wall -Wextra -Werror -std=c17 -lpthread
GLOBAL  =  ./manageRequest.c \
			./Route/route.c \
			./Route/utils.c \
			./iniParser/iniParser.c \
			./server.c

TESTNAME=  ./test

SRC=$(wildcard ./Route/Controller/*.c)

OBJ		=	$(GLOBAL:%.c=%.o)
OBJCONTROLLER = $(SRC:%.c=%.o)

all:    ourLib

ourLib:	$(OBJ) $(OBJCONTROLLER)
		$(CC) -pthread ./main.c $(OBJ) $(OBJCONTROLLER) -o $(NAME)

clean:
		-$(RM) $(OBJ) $(OBJCONTROLLER)

fClean:  clean
		-$(RM) $(NAME)

re:  	fClean all

test:
	gcc -lcriterion --coverage ./Tests/test.c $(GLOBAL) $(SRC) -o $(TESTNAME)

testCleaner:
	-$(RM) ./*.gcda
	-$(RM) ./*.gcno


.PHONY: all clean fClean ourlib test testCleaner re

