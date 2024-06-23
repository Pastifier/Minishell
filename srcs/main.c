#include "parser.h"
#include "interpreter.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

volatile sig_atomic_t	g_signal = 0;

static bool	init_envl(t_node **envl);
static bool	init_shlvl(t_node *envl);
static void	signal_handler(int signum);
int pipe_at_eol(char **line, t_node **envl, t_astnode **ast);
int pipe_at_eol_child(char **line, t_node **envl, int fd[2], t_astnode **ast);
int pipe_at_eol_parent(char **line, int fd[2]);

int	main(int argc, char **argv, char **envp)
{
	char 		*prompt = "$> ";
	char  		*line;
	t_astnode	*ast;
	t_token		*token_list;
	t_node		*envl;
	int			parse_ret;

	((void)argc, (void)argv, envl = NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_handler);
	if (!init_envl(&envl))
		return (EXIT_FATAL);
	if (!str_arr_dup_to_list(envp, &envl))
		return (EXIT_FATAL);
	if (!init_shlvl(envl))
		return (list_destroy(&envl), EXIT_FATAL);
	while (true)
	{
		signal(SIGINT, signal_handler);
		ast = NULL;
		token_list = NULL;
		line = readline(prompt);
		if (line == NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			break;
		}
		if (g_signal == 130)
			*(int*)envl->content = 130;
		if (line[0] != '\0')
		{
			parse_ret = init_tokenizer(line, &ast, &token_list, &envl);
			if (parse_ret == 4)
			{
				parse_ret = pipe_at_eol(&line, &envl, &ast);
				destroy_mini_shell(&token_list, &ast, parse_ret);
				if (!parse_ret)
					parse_ret = init_tokenizer(line, &ast, &token_list, &envl);
			}
			if (parse_ret)
			{
				destroy_mini_shell(&token_list, &ast, parse_ret);
				*(int *)envl->content = EXIT_FATAL;
				if (OS_IS_MAC)
					*(int *)envl->content = 258;
			}
			else
			{
				interpret(ast, envl);
				destroy_mini_shell(&token_list, &ast, 0);
			}
			add_history(line);
		}
		free(line);
		rl_on_new_line();
	}
	write(1, "exit\n", 5);
	int temp = *(int*)envl->content;
	list_destroy(&envl);
	clear_history();
	exit(temp);
}

// @author Emran BinJamaan
//
// @brief 	Initialize the environment list with the first node being
//			invisible and containing the exit-code; which will be used
//			later to determine the exit status of the last-executed command.
//
// @param	envl The environment list to initialize.
//
// @warning The environment list must be freed by the caller. If the allocation fails,
//			the function will free the allocated memory and set the
//			pointer to `NULL`.
static bool init_envl(t_node **envl)
{
	t_node	*to_append;

	to_append = node_create("");
	if (!to_append)
		return (false);
	to_append->visible = false;
	free(to_append->content);
	to_append->content = ft_calloc(1, sizeof(int));
	if (!to_append->content)
		return (node_destroy(to_append), false);
	*(int*)to_append->content = EXIT_SUCCESS;
	list_append(envl, to_append);
	return (true);
}

static bool init_shlvl(t_node *envl)
{
	t_node	*shlvl;
	char	*eql_addr;
	char	*shlvl_value_str;

	shlvl = find_variable(&envl, "SHLVL");
	if (!shlvl)
	{
		if (bltin_export(&envl, "SHLVL", "1"))
			return (false);
		return (true);
	}
	eql_addr = ft_strchr(shlvl->content, '=');
	if (ft_atoi(eql_addr + 1).value < 0)
	{
		if (bltin_export(&envl, "SHLVL", "0"))
			return (false);
		return (true);
	}
	shlvl_value_str = ft_itoa(ft_atoi(eql_addr + 1).value + 1);
	if (!shlvl_value_str)
		return (false);
	if (bltin_export(&envl, "SHLVL", shlvl_value_str))
		return (free(shlvl_value_str), false);
	free(shlvl_value_str);
	return (true);
}

int pipe_at_eol(char **line, t_node **envl, t_astnode **ast)
{
	int 	ret;
	pid_t	pid;
	int fd[2];

	if (pipe(fd) == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		close(fd[0]);
		ret = pipe_at_eol_child(line, envl, fd, ast);
		exit(0);
	}
	else
	{
		waitpid(pid, &ret, 0);
		if (WEXITSTATUS(ret) == 0)
			return (pipe_at_eol_parent(line, fd));
		return (close(fd[0]), close(fd[1]), 4);
	}
	return (ret);
}

int pipe_at_eol_child(char **line, t_node **envl, int fd[2], t_astnode **ast)
{
	int			ret;
	t_astnode	*new_ast;
	t_token		*token_list;

	new_ast = NULL;
	token_list = NULL;
	free(*line);
	destroy_mini_shell(NULL, ast, 0);
	while (true)
	{
		*line = readline("> ");
		if (*line == NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			exit(1);
		}
		if ((*line)[0] != '\0')
		{
			ret = init_tokenizer(*line, &new_ast, &token_list, envl);
			destroy_mini_shell(&token_list, &new_ast, ret);
			if (ret && ret != 4)
				exit(ret);		
			write(fd[1], " ", 1);
			write(fd[1], *line, ft_strlen(*line));
			if (!ret)
			{
				list_destroy(envl);
				exit(EXIT_SUCCESS);
			}
		}
		free(*line);
	}
	list_destroy(envl);
	exit	(EXIT_SUCCESS);
}

int	pipe_at_eol_parent(char **line, int fd[2])
{
	char	*temp;
	char	*new_line;

	close(fd[1]);
	temp = *line;
	new_line = (get_next_line(fd[0]).line); // need guard for GNL
	if (!new_line)
	{
		/*deal with it*/
	}
	close(fd[0]);
	if (!*line)
		return (free(temp), 1);
	*line = ft_strjoin(temp, new_line);
	free(temp);
	free(new_line);
	if (!*line)
		return (EXIT_FAILURE);
	return (0);
}

void	signal_handler(int signum)
{
	g_signal = signum;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
