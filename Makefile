# Name of your program:
NAME=fs

# List of all .cpp source code files included in your program (separated by spaces):
SRC=$(wildcard src/*.cpp)

SRCPATH=./
OBJ=$(addprefix $(SRCPATH), $(SRC:.cpp=.o))

RM=rm -f
INCPATH=include
CPPFLAGS= -I $(INCPATH) -std=c++11 -Wall -Werror -pedantic-errors -fmessage-length=0 -g


all: $(OBJ)
	g++ $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	-$(RM) *~
	-$(RM) *#*
	-$(RM) *swp
	-$(RM) *.core
	-$(RM) *.stackdump
	-$(RM) $(SRCPATH)/src/*.o
	-$(RM) $(SRCPATH)/src/*.obj
	-$(RM) $(SRCPATH)/src/*~
	-$(RM) $(SRCPATH)/src/*#*
	-$(RM) $(SRCPATH)/src/*swp
	-$(RM) $(SRCPATH)/src/*.core
	-$(RM) $(SRCPATH)/src/*.stackdump

fclean: clean
	-$(RM) $(NAME)

re: fclean all

test: all
	./fs -f file_list.txt -d dir_list.txt -s 512 -b 2