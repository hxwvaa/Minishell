NAME = minishell

EXEC = execution
PAR	 = parsing

SRC = minislime.c $(PAR)/tokenize.c $(PAR)/tokenize_utils.c builtins/our_echo.c\
		builtins/our_env.c builtins/our_pwd.c builtins/our_unset.c\
		builtins/our_export.c builtins/our_cd.c builtins/our_exit.c\
		$(PAR)/pre_execute.c free_arr_list.c $(PAR)/expand.c mini_utils.c\
		$(PAR)/toklist_cmdlist.c $(PAR)/cmdlist_utils.c $(PAR)/list_funcs.c\
		gnl/get_next_line.c $(EXEC)/execution.c $(EXEC)/process_hd.c\
		$(EXEC)/process_redir.c $(EXEC)/builtin_execute.c\
		$(EXEC)/execute_child.c $(EXEC)/execute_parent.c\
		$(EXEC)/exec_utils.c $(EXEC)/findpath.c $(EXEC)/process_hd_child.c\
		$(EXEC)/hd_expansion.c $(EXEC)/hd_ex_utils.c $(EXEC)/wait_loop.c\
		$(PAR)/quotes_expand.c $(PAR)/handle_quotes.c $(PAR)/expansion.c\
		$(PAR)/unclosed_quotes.c builtins/updating_pwds.c


OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Werror -Wextra

DEL = rm -rf

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Colors
GR	= \033[32;1m
RE	= \033[31;1m
YE	= \033[33;1m
CY	= \033[36;1m
RC	= \033[0m

all: $(LIBFT) $(NAME)

$(LIBFT):
	@echo "$(CY)Building libft$(RC)"
	@make bonus -C $(LIBFT_DIR)

%.o: %.c minishell.h
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@echo "$(CY)Generating minishell$(RC)"
	@$(CC) -o $@ $^ -lreadline -L/opt/vagrant/embedded/lib/ -Iopt/vagrant/embedded/include/readline $(LIBFT)

clean:
	@echo "$(YE)Cleaning all object files$(RC)\n"
	@make clean -C $(LIBFT_DIR)
	@$(DEL) $(OBJ)

fclean: clean
	@echo "$(YE)Cleaning all additional objects and libraries$(RC)\n"
	@make fclean -C $(LIBFT_DIR)
	@$(DEL) $(NAME)

re: fclean all

valgrind: $(NAME)
	valgrind --trace-children=yes -s --suppressions=ignore_readline_leaks.txt  --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes ./minishell

.PHONY: all clean fclean re
