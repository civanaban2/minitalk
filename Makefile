NAME_SERVER = server
NAME_CLIENT = client
LIB = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror


SRC_SERVER = src/server.c
SRC_CLIENT = src/client.c

OBJ_SERVER = $(SRC_SERVER:.c=.o)
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)

all: $(NAME_SERVER) $(NAME_CLIENT)

build:
	mkdir -p build

$(NAME_SERVER): $(OBJ_SERVER) $(LIB) build
	$(CC) $(CFLAGS) $(OBJ_SERVER) -Iinc -Linc -lft -o build/$(NAME_SERVER)

$(NAME_CLIENT): $(OBJ_CLIENT) $(LIB) build
	$(CC) $(CFLAGS) $(OBJ_CLIENT) -Iinc -Linc -lft -o build/$(NAME_CLIENT) 

$(LIB):
	make -C libft
	mv libft/$(LIB) inc

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I./inc

clean:
	rm -f $(OBJ_SERVER) $(OBJ_CLIENT)
	make -C libft clean
	rm -f inc/$(LIB)

fclean: clean
	rm -rf build

re: fclean all

.PHONY: all clean fclean re