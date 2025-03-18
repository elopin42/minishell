NAME=minishell
FLAGS=-Wall -Wextra -Werror -I includes/ -I libft/includes -I fd_printf/includes -g3
LIBFT=libft/libft.a
FD_PRINTF=fd_printf/libfdprintf.a
LIBSFLAGS=-L libft/ -lft -L fd_printf/ -lfdprintf -lreadline
SRCSF=main.c \
	  get_tokens.c \
	  debug_tools.c \
	  signal.c		\
	  exec.c		\
	  get_ast.c		\
	  free_tools.c	\
	  get_list_tokens.c \
	  ft_tokens_list_tools.c \
	  built-in.c \
	  get_next_line.c \
	  get_next_line_utils.c
OBJSF=$(SRCSF:.c=.o)
SRCDIR=srcs/
OBJDIR=objs/
SRCS=$(addprefix srcs/, $(SRCSF))
OBJS=$(addprefix objs/, $(OBJSF))

all: $(LIBFT) $(FD_PRINTF) $(NAME)

$(LIBFT):
	@make -C libft/ bonus

$(FD_PRINTF):
	@make -C fd_printf/

$(NAME): $(LIBFT) $(FD_PRINTF) $(OBJDIR) $(OBJS)
	@echo "\033[0m\033[1;35m|\033[0m"
	@cc $(LIBFT) $(FD_PRINTF) $(FLAGS) -o $(NAME) $(OBJS) $(LIBSFLAGS)
	@echo "\033[1;32mminishell ready ✓\033[0m"

$(OBJDIR)%.o:$(SRCDIR)%.c
	@echo " \c"
	@cc $(FLAGS) -c $^ -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@echo "\033[1;35mminishell compiling... |\033[0m\033[45m\c"

clean:
	@rm -rf $(OBJDIR)
	@echo "\033[1;31mminishell objects deleted\033[0m"
	@make -C libft clean
	@echo "\033[1;31mlibft objects deleted\033[0m"
	@make -C fd_printf clean
	@echo "\033[1;31mfd_printf objects deleted\033[0m"


fclean: clean
	@rm -rf $(NAME)
	@echo "\033[1;31mminishell binary file deleted\033[0m"
	@make -C libft/ fclean
	@make -C fd_printf fclean

re: fclean all

.PHONY: all clean fclean re
