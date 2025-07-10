/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:12:09 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/13 00:24:42 by tbeauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../fd_printf/fd_printf.h"
# include "../libft/libft.h"
# include "get_next_line.h"
# include "typedefs.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <unistd.h>
# define CHILD 0
# define HISTORY_FILE ".minishell_history"

# define MALLOC_ERROR 1
# define SYNTAX_ERROR 2
# define CMD_NOT_FOUND_ERROR 127

extern volatile sig_atomic_t	g_heredoc_interrupted;

void							get_tokens(t_env *ms);
void							cut_chain_for_redir(t_parser *p, t_env *ms);
void							recursive_call_for_redir(t_parser *p,
									t_env *ms);
void							append_arguments(t_parser *p, t_env *ms);
t_tokens						*find_close(t_tokens *tokens, t_env *ms);
t_token_part					*new_token_part(char *content,
									t_quote_type quote, bool expand);
int								is_redirection_operator(char *token);
void							append_token_part(t_token_part **head,
									t_token_part *new_part);
void							trunc_node_cmd(t_parser *p);
void							cut_chain(t_pparser *p);
void							set_node_type(t_parser *p, t_env *ms);
void							isolate_op(t_pparser *p);
void							set_type(t_pparser *p);
bool							is_valid_expand_char(char c);
void							expand_exit_code(t_tokens **tokens, t_env *ms,
									int si, char **tmp);
void							expand_zero(t_tokens **tokens, t_env *ms,
									int si, char **tmp);
void							expand_digit_to_empty(t_tokens **tokens,
									t_env *ms, int si, char **tmp);
char							*expand_token(char *tmp, char *var, int si,
									int stop);
int								ft_seek_index(char *s, char c);
void							save_history(void);
void							print_logo(void);
void							find_next_op(t_pparser *p);
void							handle_no_pipe_case(t_parser *p, t_env *ms);
void							handle_pipe_case(t_parser *p, t_env *ms);
int								process_tmp(char *tmp,
									t_token_part **parts_head,
									bool *has_quotes);
int								add_token_no_quotes(t_tokens **tokens,
									char *tmp);
int								add_token_multi(t_tokens **tokens,
									t_token_part *parts_head);
int								finalize_tokens(t_tokens **tokens, char *tmp,
									t_token_part *parts_head, bool has_quotes);
int								add_no_quotes_to_tokens(t_tokens **tokens,
									char *tmp);
int								add_plain_part(t_token_part_args *args);
int								add_quoted_part(t_token_part_args *args);
t_token_part					*create_token_part(char *content,
									t_quote_type quote, bool expand);
void							add_part_to_list(t_token_part **head,
									t_token_part *new_part);
int								handle_quote(t_quote_args *args, int *i);
int								process_plain_part(t_token_part **parts_head,
									char *str, int start, int len);
int								process_quoted_part(t_quote_args *a, int start,
									int len, char quote_char);
void							print_error(int err_flag);
void							special_ninja(t_parser *p, t_env *ms);
void							print_tab(char **tab);
void							search_and_destroy(t_tokens **tokens,
									t_env *ms);
void							setup_readline_signals(void);
void							ptit_clean_du_cul(char ***argscmd, char **path,
									t_env *ms, int code);
void							handle_sigint_readline(int sig);
int								get_user_input(t_env *ms);
void							parse_command(t_env *ms);
void							handle_parse_error(t_env *ms);
void							cleanup_command(t_env *ms);
void							execute_or_error(t_env *ms);
void							process_command(t_env *ms);
void							handler(int sig);
void							setup_signals(void);
int								apply_redirections(t_ast *ast, t_env *ms);
bool							choose_command(t_ast *ast, t_env *ms);
void							handle_heredoc_sigint(int sig);
int								is_valid_identifier(const char *s);
int								heredoc_loop(t_ast *ast, int pipe_fd[2]);
t_ast							*get_final_cmd(t_ast *ast);
void							handle_and(t_ast *ast, t_env *ms);
void							free_tab(char **tab);
void							execute_cmd(t_tokens *cmd, t_env *ms);
void							cleanup(t_env *ms);
void							exit_clean(t_env *ms, int exit_code);
t_ast							*get_ast(t_tokens **tokens, t_env *ms);
void							print_ast(t_ast *node, int depth);
void							ft_tokens_add_back(t_tokens **lst,
									t_tokens *new);
void							ft_tokens_add_front(t_tokens **lst,
									t_tokens *new);
void							ft_clear_tokens(t_tokens **lst,
									void (*del)(void *));
void							ft_del_token(t_tokens *lst,
									void (*del)(void *));
t_tokens						*ft_new_token(void const *content,
									t_token_type type, t_token_part *parts);
int								ft_lst_tokens_size(t_tokens *lst);
void							print_tokens(t_tokens *tokens);
void							get_list_tokens(t_env *ms);
int								count_tokens(char *line);
int								string_to_tokens(char *line, int save_count);
bool							has_dquotes(char *tmp);
bool							has_squotes(char *tmp);
int								is_in_set(char c, char const *set);
int								count_chars_to_remove(char const *s1,
									char const *set);
char							*ft_strtrimv2(char const *s1, char const *set);
void							delete_ast(t_ast **node);
void							search_and_execute_cmd(t_env *ms);
void							consume_tree(t_ast *ast, t_env *ms);
bool							found_builtin(t_tokens *cmd, t_env *ms);
t_tokens						*dup_tokens(t_tokens *og);
void							save_history(void);
void							handle_or(t_ast *ast, t_env *ms);
int								handle_append_out(t_ast *ast, t_env *ms);
int								handle_redir_out(t_ast *ast, t_env *ms);
int								handle_redir_in(t_ast *ast, t_env *ms);
void							handle_pipe(t_ast *ast, t_env *ms);
void							consume_tree(t_ast *ast, t_env *ms);
void							expand_tokens(t_ast *ast, t_env *ms);
bool							expand_command(t_tokens **cmd, t_env *ms);
void							fork_and_execute_cmd(t_tokens *cmd, t_env *ms);
bool							ft_export_extra(t_tokens *t, t_env *ms, int i);
t_ast							*init_node(void);
t_ast							*handle_parenthesis(t_tokens *tokens,
									t_tokens **og_tokens, t_env *ms);
void							ft_clear_right_tokens(t_tokens **lst,
									void (*del)(void *));
void							ft_clear_left_tokens(t_tokens **lst,
									void (*del)(void *));
bool							set_error(t_env *ms, int err_code);
bool							set_parse_error(t_env *ms, int err_code);

void							ft_old_and_pwd(t_env *ms);
void							close_all_fds(void);

void							read_nl_bs(char **next, char **tmp, t_env *ms,
									int *i);
void							read_nl_quotes(char **next, char **tmp,
									t_env *ms, int *i);

int								is_operand1(char *str);
int								is_operand2(char *str);
int								is_operand(char *str);
bool							is_dquotes(char *tmp);
bool							is_squotes(char *tmp);

int								compute_end_operand_index(char *str, int i,
									int *start);
int								is_in_set(char c, char const *set);
int								count_chars_to_remove(char const *s1,
									char const *set);
char							*ft_strtrimv2(char const *s1, char const *set);

bool							add_to_tokens(t_tokens **tokens, char *tmp,
									char *to_trim, t_token_type type);
bool							token_stops(char *line, int i, bool in_dquotes,
									bool in_quotes);
void							handle_specials(char *line, int i,
									t_tknz_bools *t, t_env *ms);
int								check_quotes_ended(t_tknz_bools t, t_env *ms);

t_ast							*init_node(void);
bool							is_not_redir(t_tokens *token);
void							cut_chain_and_recursive_call(t_parser *p,
									t_env *ms);
void							cut_chain_for_redir(t_parser *p, t_env *ms);
void							recursive_call_for_redir(t_parser *p,
									t_env *ms);

bool							ft_check_egal(t_tokens *t);
bool							check_edges(char *cmd_line);

int								find_index(char *str, char c);
int								find_key(char **env, char *key);
void							update_export(t_env *ms, char *var, int h);
void							creat_export(t_env *ms, char *var);
char							*add_guillemet(char *envp);
int								no_find_key(char *env);
void							ft_env(t_env *ms);
char							*print_path(int a);
void							ft_unset_co(char *str, t_env *ms);
void							ft_cd(char *path, t_env *ms);
char							*remove_quotes(char *str);
bool							has_quotes(char *str);
bool							check_builtin(t_tokens *token, char *builtin,
									t_env *ms);
char							*reconstruct_from_parts(t_token_part *parts);
char							*expand_parts(t_token_part *parts, t_env *ms);
bool							found_builtin(t_tokens *cmd, t_env *ms);
bool							ft_export_extra(t_tokens *t, t_env *ms, int i);
bool							ft_cd_sys(t_tokens *t, t_env *ms);
bool							ft_echo(t_tokens *cmd, t_tokens *t, t_env *ms);
void							ft_print_echo(char *s1, char *s2, int i);
void							ft_trie_env(t_env *ms);
void							print_export(t_env *ms);
int								ft_export(t_env *ms, char *var);
bool							is_valid_export_identifier(const char *s);
int								ft_shlvl(t_env *ms);
void							ft_old_and_pwd(t_env *ms);

void							handle_pipe(t_ast *ast, t_env *ms);
void							create_pipe(int *pipe_fd);
void							fork_or_die(pid_t *pid);
void							wait_and_restore_stdin(t_env *ms, int input_fd,
									int last_pid);

char							*join_path(char *cmd, char **paths, t_env *ms);
char							*get_path(char *cmd, char **envp, t_env *ms);

int								add_quotes_to_tokens(t_tokens **tokens,
									char *tmp, char *to_trim,
									t_token_type type);
int								add_no_quotes_to_tokens(t_tokens **tokens,
									char *tmp);
char							*add_quotes_utils(char *str, char *envp);
void							ft_exit(t_tokens *t, t_env *ms);

t_token_part					*split_token_by_quotes(const char *str);

void							free_token_parts(t_token_part **head);
void							print_parts(t_token_part *parts);
t_token_part					*dup_parts(t_token_part *parts);
bool							is_one_dquotes(char *tmp);
bool							is_one_squotes(char *tmp);
void							free_token(t_tokens **tok);
t_tokens						*dup_token(t_tokens *token);
int								is_logic_operator(char *str);
int								is_redirection(char *str);
void							handle_heredoc_sigint(int sig);
int								heredoc_loop(t_ast *ast, int pipe_fd[2]);

#endif
