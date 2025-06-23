/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:23:29 by mhuthmay          #+#    #+#             */
/*   Updated: 2025/06/23 13:12:24 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* -------------------------- System Header Includes ------------------------- */

#include <stdlib.h>                                 // For memory allocation and standard utilities
#include <stddef.h>	                                // For size_t type
#include <unistd.h>                                 // For POSIX system calls (e.g., write, read)
#include <dirent.h>                                 // For directory handling
#include <errno.h>                                  // For error number definitions
#include <readline/readline.h>                      // For readline functionality
#include <readline/history.h>                       // For command history management
#include <stdio.h>                                  // For standard I/O operations
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdbool.h>

/* ---------------------------- Error Message Macros ------------------------- */

/* Macros for standard error messages used throughout the Minishell project */

# define NO_SUCH_FILE_OR_DIRECTORY "No such file or directory\n"
# define PERMISSION_DENIED "Permission denied\n"
# define NOT_A_DIRECTORY "Not a directory\n"
# define TOO_MANY_ARGUMENTS "too many arguments\n"
# define ARGUMENT_LIST_TOO_LONG "Argument list too long\n"
# define TOO_MANY_LEVELS_OF_SYMB_LINK "Too many levels of symbolic links\n"
# define FILE_NAME_TOO_LONG "File name too long\n"
# define COMMAND_NOT_FOUND "command not found\n"
# define NUMERIC_ARGUMENT_REQUIRED "numeric argument required\n"
# define NOT_A_VALID_IDENTIFIER "not a valid identifier\n"


/* ---------- Environment Structure ---------- */

typedef struct s_env {
    char                *key;
    char                *value;
    struct s_env        *prev;
    struct s_env        *next;
} t_env;

/* ---------- Command Structure ---------- */

typedef struct s_command {
    char                *cmdpath;
    char                **args;
    char                **infiles;
    char                **outfiles;
    char                **heredoc_delimiters;
    int                 append;
    bool                was_quoted;
	int				    is_heredoc;
    int                 heredoc_fd;
    struct s_command    *next;
} t_command;

/* ---------- Master Structure ---------- */

typedef struct s_master {
    struct s_command    *cmds;
    struct s_env        *env;
	pid_t				*pids;
    int                 errorcode;
} t_master;

/* ---------- Tokentype ENUMS ---------- */

typedef enum{
    WORD,
    INPUT,
    OUTPUT,
    HEREDOC,
    APPEND,
    PIPE
} t_tokentype;

/* ---------- Token Structure ---------- */

typedef struct s_tokens {
    char                *token;
    t_tokentype         type;
    bool                was_quoted;
    struct s_tokens     *next;
    struct s_tokens     *prev;
} t_tokens;

typedef struct s_expand_state
{
    int i;
    bool in_double_quotes;
} t_expand_state;

typedef struct s_token_state
{
    t_tokens *newtmp;
    t_tokens *oldtmp;
    int i;
} t_token_state;

/* Struct to manage position tracking */
typedef struct s_token_positions
{
	int		inpos;
	int		outpos;
	int		hdpos;
	int		argpos;
}			t_token_positions;



/* ---------------------------- Function Prototypes -------------------------- */

/* ---------- General Helper Functions ---------- */

/* Comparising Functions */
int         ft_strcmp(char *str1, char *str2);
int         ft_strncmp(char *str1, char *str2, int n);
int         ft_isalpha(char c);
int         ft_isnum(char c);
int         ft_isspace(char c);

/* Split String Function */
char        **ft_split(char *tosplit, char seperator);

/* Numeric Operation Functions */
char        *ft_itoa(int num);
long        ft_atol(char *str, int *overflow);

/* String Allocation Functions */
char        *ft_strdup(char *str);
char        *ft_strndup(const char *str, int n);
char        *ft_strjoin3(char *str1, char *str2, char *str3);

/* String Operation Functions */
void        ft_putstr_fd(char *str, int fd);
int         ft_strchr(char *str, char c);
int         ft_strlen(char *str);
void        ft_strncpy(char *src, char *dst, int n);

/* ---------- Execution Functions ---------- */

/* Executor Functions */
void	    ft_exec(t_master *master);
void	    ft_execpipe(t_master *master);
int 	    ft_execute_builtin(t_master *master, t_command *currentcmd);
int		    ft_openinfiles(t_command *currentcmd);
int ft_openoutfiles(t_command *currentcmd);
void	    ft_checkcmdpath(t_master *master, t_command *currentcmd);
bool		ft_isbuiltin(t_command *command);
void	    ft_checkforcmdpath(t_master *master, t_command *currentcmd);
char	    **ft_getpathsarr(t_master master);
int 	    ft_heredoc(t_command *currentcmd, t_master *master);
void        ft_execute_commands(t_master *master);
int ft_is_directory(char *path);

/* ---------- Built-in Functions ---------- */

int         ft_env(t_command *currentcmd, t_env *env);
int         ft_export(t_command *currentcmd, t_env *env);
void        ft_print_export_line(t_env *current_var);
int         ft_check_varkey(char *variable);
int         ft_update_if_exist(t_env *env, char *key, char *variable);
int         ft_unset(t_command *currentcmd, t_env **env);
int         ft_set_export_var(t_env **env, char *variable);
int         ft_cd(t_command *currentcmd, t_env *env);
int         ft_echo(t_command *currentcmd);
int         ft_exit(t_command *currentcmd);
int         ft_pwd(void);

/* ---------- Free Functions ---------- */

void        ft_free_and_exit(t_master *master, unsigned char exitcode);
void        ft_free_str_array(char **array);
void        ft_free_variable(t_env *var);
void        ft_free_env(t_env *env);
void        ft_free_commands(t_command *commands);
void        ft_free_tokens(t_tokens *tokens);
void        ft_free_all(t_master *master);

/* ---------- Environment Functions ---------- */

/* Environment Functions */
t_env       *ft_create_env_list(char **envp);   
t_env       *ft_add_variable(t_env **env, char *key, char *value); 
void        ft_delete_variable(t_env **env, char *key);
t_env       *ft_set_new_variable(char *key, char *value);
char        **ft_init_char_array(int total_size);
int         ft_init_env(t_env **env, char **env_array);
int         ft_add_next_variable(t_env **env, char *variable);
int         ft_init_empty_env(t_env **env);
char        *ft_getkey(char *var);         
char        *ft_getvalue(char *var);     
char        **ft_get_env_array(t_env *env);
int	        ft_env_length(t_env *env);
char        *ft_addlvl(char *stringlvl);      
t_env       *ft_getvar(t_env *env, char *key);

int ft_setpwd(t_env **env);

/* Tokenizing  Main Functions */
t_tokens *ft_init_tokens(int total_tokens);
int ft_fill_tokens_no_expansion(t_tokens *tokens, char *input);
t_tokens *ft_get_final_tokens(t_tokens *tokens, t_master *master);

/* Tokenizing Util 1 */
int ft_count_token_length(char *token);
int ft_count_tokens(char *input);
int ft_count_operation_length(char *str);
t_tokentype ft_get_token_type(char *token);
int ft_count_tokens_in_tokens(t_tokens *tokens);

/* Tokenizing Utils 2 */
t_tokens *ft_split_expanded_tokens(t_tokens *tokens);
int ft_fill_tokens_with_expansion(t_tokens *new_tokens, t_tokens *old_tokens);
char *ft_get_final_token(t_tokens *token);
int ft_erase_quotations(t_tokens *tokens);

/* Tokenizing Utils 2 */
int ft_process_token(t_tokens *token, char *input, t_tokentype tokentype);
int ft_split_input_into_tokens(t_tokens *tokens, char *input);

/* Parsing */
int ft_process_line(char *input, t_master *master);
t_tokens *ft_get_tokens(char *input, t_master *master);
t_command *ft_get_commands(t_tokens *tokens, t_master *master);
int ft_quoted_str_len_without(char *quoted_str, char quote_type);

int ft_open_heredocs(t_command *commands, t_master *master);
t_command *ft_get_commands(t_tokens *tokens, t_master *master);
void	ft_fill_commands(t_command *commands, t_tokens *tokens);
t_command	*ft_init_command(void);
t_command	*ft_init_commands(int total_commands);
int	ft_init_commands_values(t_command *commands, t_tokens *tokens);
int	ft_count_commands(t_tokens *tokens);
int	ft_count_operation(t_tokens *tokens, t_tokentype type);
void	ft_fill_infiles(t_command *command, t_tokens *token, bool reset);
char	**ft_allocate_str_arr(int total_count, int *failflag);

/* Parsing Variables Functions */
char *ft_expand_variables(char *input, t_master *master, bool consider_quotation);
int ft_expand_token_variables(t_tokens *tokens, t_master *master);
char *ft_concat_var(char *str, char *varvalue, int varstart, int varkeylen);
char *ft_get_expanded_variable(char *input, t_master *master);
int ft_varkey_len(char *input);

/* syntax check functions */
int ft_check_quote_syntax(char *str);
int ft_check_operation_syntax(t_tokens *tokens);

/* Signal Handling Functions */
void	    setup_signals(void);
void	    reset_signals(void);
int         check_signal(void);
void	setup_heredoc_signals(void);

/* Main Functions */
t_master        *init_master(void);                // Initialize master structure


/* Error Handling */
void            ft_printerror(char *cmd, char *errfile, char *errormsg); // Print error message


/* void print_tokens(t_tokens *head); */
#endif