NAME = ircserv

SRCS = $(wildcard src/*.cpp src/server/*.cpp src/cmd/*.cpp)
OBJS = $(SRCS:.cpp=.o)

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
INCLUDES = -I ./includes

RM = rm -rf


.cpp.o:
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $(<:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "\n"
	@echo "Compiling IRC server..."
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@echo "Done!"

clean:
	@echo "Removing binaries..."
	@$(RM) $(OBJS)
	@echo "Done!"

fclean: clean
	@echo "Removing executable..."
	@$(RM) $(NAME)
	@echo "Done!"

re: fclean all

.PHONY: all clean, fclean, re