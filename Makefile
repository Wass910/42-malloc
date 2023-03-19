ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC			= @gcc
CFLAGS		= -Wall -Wextra -Werror -g -fPIC
LIGHTFLAGS	= -Wall -Wextra -Werror -g -fsanitize=address

NAME		= libft_malloc_$(HOSTTYPE).so
CHECK		= check_malloc

SRCS		= srcs/malloc.c srcs/realloc.c srcs/free.c \
				srcs/utils.c 

SRCS_TEST	= srcs/main.c
INCLUDES	= inc/libft_malloc.h

OBJ			= $(SRCS:.c=.o)


all: $(NAME)


$(NAME): $(OBJ) $(INCLUDES)
	@$(CC) -shared -o $(NAME) $(OBJ)
	ln -fs $(NAME) libft_malloc.so

test:
	@$(CC) $(LIGHTFLAGS) $(SRCS_TEST) -L. -lft_malloc -o $(CHECK)

clean:
	@rm -rf $(OBJ) 

fclean: clean
	@rm -rf $(NAME)
	@rm -rf libft_malloc.so
	@rm -rf check_malloc

re: fclean all

.PHONY: all clean fclean re test