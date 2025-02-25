NAME_SERVER = server
NAME_CLIENT = client
NAME_LIB = libft.a

DIR_BUILD = build
DIR_SRC = src
DIR_INC = inc
DIR_LIB = libft

INCLUDES_H = -I$(DIR_INC) -I$(DIR_LIB)
INCLUDES_LIB = $(DIR_LIB)/$(NAME_LIB)

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_SERVER = $(DIR_SRC)/server.c
SRC_CLIENT = $(DIR_SRC)/client.c

OBJ_SERVER = $(SRC_SERVER:.c=.o)
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)

all: $(DIR_BUILD) $(DIR_BUILD)/$(NAME_SERVER) $(DIR_BUILD)/$(NAME_CLIENT) 

$(DIR_BUILD):
	mkdir -p build

$(DIR_BUILD)/$(NAME_SERVER): $(INCLUDES_LIB) $(OBJ_SERVER)
	$(CC) $(CFLAGS) $(INCLUDES_H) $(INCLUDES_LIB) $(OBJ_SERVER) -o $(DIR_BUILD)/$(NAME_SERVER)

$(DIR_BUILD)/$(NAME_CLIENT): $(INCLUDES_LIB) $(OBJ_CLIENT)
	$(CC) $(CFLAGS) $(INCLUDES_H) $(INCLUDES_LIB) $(OBJ_SERVER) -o $(DIR_BUILD)/$(NAME_CLIENT) 

$(OBJ_SERVER): %.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES_H) -c $< -o $@ 

$(OBJ_CLIENT): %.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES_H) -c $< -o $@ 

$(INCLUDES_LIB):
	make -C libft

clean:
	rm -f $(OBJ_SERVER) $(OBJ_CLIENT)
	make -C libft fclean

fclean: clean
	rm -rf build

re: fclean all

.PHONY: all clean fclean re