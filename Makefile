NAME_SERVER = server
NAME_CLIENT = client
NAME_LIB = ft

DIR_BUILD = build
DIR_SRC = src
DIR_INC = inc
DIR_LIB = libft

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_SERVER = $(DIR_SRC)/server.c
SRC_CLIENT = $(DIR_SRC)/client.c

INCLUDES_H = -I$(DIR_INC) -I$(DIR_LIB)
INCLUDES_LIB = -L$(DIR_LIB) -l$(NAME_LIB)

OBJ_SERVER = $(SRC_SERVER:.c=.o)
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)

BUILD_SERVER = $(DIR_BUILD)/$(NAME_SERVER)
BUILD_CLIENT = $(DIR_BUILD)/$(NAME_CLIENT)
BUILD_LIB = $(DIR_LIB)/lib$(NAME_LIB).a

all: $(BUILD_SERVER) $(BUILD_CLIENT)

$(BUILD_SERVER): $(BUILD_LIB) $(OBJ_SERVER)
	mkdir -p $(DIR_BUILD)
	$(CC) $(OBJ_SERVER) $(INCLUDES_LIB) -o $(BUILD_SERVER)

$(BUILD_CLIENT): $(BUILD_LIB) $(OBJ_CLIENT)
	mkdir -p $(DIR_BUILD)
	$(CC) $(OBJ_CLIENT) $(INCLUDES_LIB) -o $(BUILD_CLIENT)

$(BUILD_LIB):
	make -C $(DIR_LIB)

$(OBJ_SERVER): %.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES_H) -c $< -o $@ 

$(OBJ_CLIENT): %.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES_H) -c $< -o $@ 

clean:
	rm -f $(OBJ_SERVER) $(OBJ_CLIENT)
	make -C $(DIR_LIB) fclean

fclean: clean
	rm -rf $(DIR_BUILD)

re: fclean all

.PHONY: all clean fclean re