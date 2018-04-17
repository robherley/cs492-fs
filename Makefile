# Name of your program:
NAME=VMsimulator

# List of all .cpp source code files included in your program (separated by spaces):
SRC=$(wildcard src/*.cpp)

SRCPATH=./
OBJ=$(addprefix $(SRCPATH), $(SRC:.cpp=.o))

RM=rm -f
INCPATH=include
CPPFLAGS= -I $(INCPATH) -std=c++11 -Wall -Werror -pedantic-errors -fmessage-length=0


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
	./$(NAME) plist ptrace 2 FIFO +

