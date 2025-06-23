/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:30:00 by mhuthmay          #+#    #+#             */
/*   Updated: 2025/06/23 14:41:53 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Writes a line (including newline) to the given pipe file descriptor.
 *
 * @param line The string to write.
 * @param pipe_front The write end of the pipe.
 */
static void	ft_write_in_pipe(char *line, int pipe_front)
{
	write(pipe_front, line, ft_strlen(line));
	write(pipe_front, "\n", 1);
}

/**
 * @brief Expands variables in a heredoc line if not quoted.
 *
 * @param line The input line (will be freed if expanded).
 * @param currentcmd The current command structure.
 * @param master The master structure.
 *
 * @return
 * pointer to expanded line ( success )
 *
 * NULL ( malloc failure )
 */
static char	*ft_get_heredoc_expanded_line(char *line, t_command *currentcmd,
		t_master *master)
{
	char	*expanded;

	if (currentcmd->was_quoted)
		return (line);
	expanded = ft_expand_variables(line, master, false);
	free(line);
	return (expanded);
}

/**
 * @brief Processes a single line in the heredoc pipe.
 *
 * @param currentcmd The current command structure.
 * @param master The master structure.
 * @param pipe_fd The current pipe file descriptors.
 * @param delim_idx The index of the current delimiter.
 * @param line The current input line.
 *
 * @return
 * 0 ( continue )
 * 1 ( break the loop )
 * -1 ( error )
 */
static int	ft_process_heredoc_line(t_command *currentcmd, t_master *master,
		int pipe_fd[2], int delim_idx, char *line)
{
	char	*expanded_line;

	if (!line)
	{
		close(pipe_fd[1]);
		return (1);
	}
	if (!ft_strcmp(line, currentcmd->heredoc_delimiters[delim_idx]))
	{
		close(pipe_fd[1]);
		free(line);
		return (1);
	}
	expanded_line = ft_get_heredoc_expanded_line(line, currentcmd, master);
	if (!expanded_line)
		return (-1);
	ft_write_in_pipe(expanded_line, pipe_fd[1]);
	free(expanded_line);
	return (0);
}

/**

	* @brief Reads lines from STDIN and writes them to the pipe until the delimiter is found.
 *
 * @param currentcmd The current command structure.
 * @param master The master structure.
 * @param pipe_fd The current pipe file descriptors.
 * @param delim_idx The index of the current delimiter.
 *
 * @return
 * 0 ( success )
 *
 * -1 ( error )
 */
static int	ft_fill_heredoc_pipe(t_command *currentcmd, t_master *master,
		int pipe_fd[2], int delim_idx)
{
	char	*line;
	int		process_result;

	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		process_result = ft_process_heredoc_line(currentcmd, master, pipe_fd,
				delim_idx, line);
		if (process_result == 1)
			break ;
		if (process_result == -1)
			return (-1);
	}
	setup_signals();
	return (0);
}

/**

	* @brief Handles all heredoc delimiters for a command and returns the read end of the last pipe.
 *
 * @param currentcmd The current command structure.
 * @param master The master structure.
 *
 * @return
 * The read end of the last heredoc pipe ( success )
 *
 * -1 ( error )
 */
int	ft_heredoc(t_command *currentcmd, t_master *master)
{
	int	heredoc_pipe_fd[2];
	int	delim_idx;

	delim_idx = 0;
	while (currentcmd->heredoc_delimiters[delim_idx])
	{
		if (pipe(heredoc_pipe_fd) == -1)
			return (-1);
		if (ft_fill_heredoc_pipe(currentcmd, master, heredoc_pipe_fd,
				delim_idx))
		{
			close(heredoc_pipe_fd[0]);
			close(heredoc_pipe_fd[1]);
			return (-1);
		}
		if (currentcmd->heredoc_delimiters[delim_idx + 1])
			close(heredoc_pipe_fd[0]);
		delim_idx++;
	}
	return (heredoc_pipe_fd[0]);
}
