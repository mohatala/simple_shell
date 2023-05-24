#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define TOKEN_BUFFER_SIZE 128
#define TOKEN_DELIMITER " \t\r\n\a"


/**
 * struct shell_data - this struct contains all relevant data on runtime
 * @status: last status of the shell
 * @counter: lines counter
 * @pid: process ID of the shell
 * @av: ARGument Vector
 * @input: command line written by the user
 * @args: arguments of the command line
 * @_env: environment variable
 */
typedef struct shell_data
{
	int status;
	int counter;
	char *pid;
	char **av;
	char *input;
	char **args;
	char **_env;
} data_sh;

/**
 * struct separator_list_s - single linked list
 * @sep: ; | &
 * @next: next node
 */
typedef struct separator_list_s
{
	char sep;
	struct separator_list_s *next;
} sep_list_t;

/**
 * struct command_list_s - single linked list
 * @line: command line
 * @next: next node
 */
typedef struct command_list_s
{
	char *line;
	struct command_list_s *next;
} cmd_lst_t;

/**
 * struct replace_var - single linked list
 * @val: variable value
 * @len_var: variable length
 * @len_val: value length
 * @next: next node
 */
typedef struct replace_var
{
	char *val;
	int len_var;
	int len_val;
	struct replace_var *next;
} replace_var_t;

/**
 * struct builtin_s - struct for builtin command arguments.
 * @name: The name of the builtin command i.e ls, cd, echo
 * @f: data type pointer function.
 */
typedef struct builtin_s
{
	char *name;
	int (*f)(data_sh *data);
} builtin_t;

/* Pointer to an array of pointers to strings called the environment */
extern char **environ;

char **_reallocdp(char **ptr, unsigned int old_size, unsigned int new_size);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void _memcpy(void *newptr, const void *ptr, unsigned int size);


sep_list_t *add_separator_node_end(sep_list_t **head, char sep);
cmd_lst_t *add_command(cmd_lst_t **head, char *line);
void free_sep_list(sep_list_t **head);
void free_command_list(cmd_lst_t **head);


replace_var_t *add_replacement_variable_node(
	replace_var_t **head, int lvar, char *var, int lval);
void free_replacement_variable_list(replace_var_t **head);

int _strcmp(char *s1, char *s2);
int _strspn(char *s, char *s2);
char *_strchr(char *s, char c);
char *_strcpy(char *des, char *src);
char *_strcat(char *des, const char *src);

int _strlen(const char *s);
int _isdigit(const char *s);
int cmp_chars(char str[], const char *delim);
char *_str_del(char str[], const char *delim);
char *_strdup(const char *s);

void str_rev(char *s);

int check_first_char(char *input, int *i);
int check_rep(char *input, int i);
int check_err(char *input, int i, char last);
int check_syntax_err(data_sh *data, char *input);
void print_syntax_err(data_sh *data, char *input, int i, int bool);

char *rem_cmnt(char *in);
void run_shell_loop(data_sh *data);

/* split.c */
char **split_input(char *input);
char *swap_char(char *input, int bool);
int split_cmd_op(data_sh *data, char *input);
void des(sep_list_t **head_s, cmd_lst_t **head_l,
			   char *input);
void get_next(sep_list_t **list_s, cmd_lst_t **list_l,
			  data_sh *data);


char *replace_var(char *input, data_sh *data);
char *get_replaced_input(replace_var_t **head, char *input,
						 char *new_input, int nlen);
int replace_vars(replace_var_t **h, char *in, char *st,
					  data_sh *data);
void check_for_environment_variables(replace_var_t **h, char *in,
									 data_sh *data);


ssize_t get_input_line(char **lineptr, size_t *n, FILE *stream);
void read_input_line_from_buffer(char **lineptr, size_t *n, char *buff,
								 size_t j);


char *read_input_line(int *i_line);

int execute_input_line(data_sh *data);


char *find_cmd(char *cmd, char **_env);
int exec_cmmd(data_sh *data);
int _current_dir(char *path, int *i);
int cmd_exec(data_sh *data);
int check_cmd_err(char *dir, data_sh *data);

int print_env_var(data_sh *data);
char *get_env(const char *name, char **_env);


int _setenv(data_sh *data);
int unset_env(data_sh *data);
char *copy_info(char *name, char *value);
void set_env(char *name, char *value, data_sh *data);
int comp_env_varname(const char *nenv, const char *name);


void change_to_dir(data_sh *data);
void change_to_dot_dir(data_sh *data);
void change_to_home_dir(data_sh *data);
void change_to_ols_dir(data_sh *data);
int change_dir_sh(data_sh *data);


int get_len_int(int n);
int str_to_int(char *s);
char *int_to_string(int n);


char *error_message_get_cd(data_sh *data);
char *error_message_not_found(data_sh *data);
char *error_message_exit_shell(data_sh *data);
char *cd_error_message(data_sh *, char *, char *, char *);


char *error_message_env(data_sh *data);
char *error_message_path_126(data_sh *data);


void print_help_env(void);
void print_help_exit(void);
void print_help_setenv(void);
void print_help_general(void);
void print_help_unsetenv(void);


void print_help(void);
void print_help_cd(void);
void print_help_alias(void);


void handle_sigint(int sig);
int get_help(data_sh *data);
int get_error_code(data_sh *data, int eval);
int (*get_builtin_fun(char *cmd))(data_sh *data);


int exit_shell_program(data_sh *data);

#endif
