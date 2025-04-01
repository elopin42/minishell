NAME=libft.a
FLAGS=-Wall -Wextra -Werror
CC=cc
HDRS=libft.h
SRCS=\
ft_atoi.c		ft_memchr.c      ft_split.c     ft_strncmp.c \
ft_bzero.c		ft_memcmp.c      ft_strchr.c    ft_strnstr.c \
ft_calloc.c		ft_memcpy.c      ft_strdup.c    ft_strrchr.c \
ft_isalnum.c	ft_memmove.c     ft_striteri.c  ft_strtrim.c \
ft_isalpha.c	ft_memset.c      ft_strjoin.c   ft_substr.c \
ft_isascii.c	ft_putchar_fd.c  ft_strlcat.c   ft_tolower.c \
ft_isdigit.c	ft_putendl_fd.c  ft_strlcpy.c   ft_toupper.c \
ft_isprint.c	ft_putnbr_fd.c   ft_strlen.c 	ft_itoa.c \
ft_putstr_fd.c	ft_strmapi.c
BONUS=\
ft_lstadd_back_bonus.c   ft_lstdelone_bonus.c  ft_lstmap_bonus.c \
ft_lstadd_front_bonus.c  ft_lstiter_bonus.c    ft_lstnew_bonus.c \
ft_lstclear_bonus.c      ft_lstlast_bonus.c    ft_lstsize_bonus.c
OBJS=$(SRCS:.c=.o)
BONUSOBJS=$(BONUS:.c=.o)

all: $(NAME)

bonus: $(OBJS) $(BONUSOBJS)
	ar rc $(NAME) $(OBJS) $(BONUSOBJS)
	ranlib $(NAME)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

%.o: %.c $(HDRS)
	$(CC) $(FLAGS) -c $<

clean:
	rm -rf $(OBJS)
	rm -rf $(BONUSOBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re