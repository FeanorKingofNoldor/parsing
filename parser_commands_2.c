/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_commands_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:39:05 by mhuthmay          #+#    #+#             */
/*   Updated: 2025/06/23 12:09:51 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_fill_commands(t_command *commands, t_tokens *tokens)
{
	t_command	*tmpcmd;
	t_tokens	*tmptok;

	tmpcmd = commands;
	tmptok = tokens;
	while (tmpcmd && tmptok)
	{
		if (!tmptok->prev)
			ft_fill_infiles(NULL, NULL, true);
		if (tmptok->type == INPUT || tmptok->type == OUTPUT
			|| tmptok->type == HEREDOC || tmptok->type == APPEND
			|| (tmptok->type == WORD && (!tmptok->prev
					|| tmptok->prev->type == WORD
					|| tmptok->prev->type == PIPE)))
		{
			ft_fill_infiles(tmpcmd, tmptok, false);
		}
		else if (!tmptok || tmptok->type == PIPE)
		{
			ft_fill_infiles(NULL, NULL, true);
			tmpcmd = tmpcmd->next;
		}
		tmptok = tmptok->next;
	}
}

int	ft_open_heredocs(t_command *commands, t_master *master)
{
	int	heredoc_fd;

	heredoc_fd = 0;
	while (commands)
	{
		if (commands->heredoc_delimiters)
		{
			heredoc_fd = ft_heredoc(commands, master);
			if (heredoc_fd == -1)
				return (-1);
			commands->heredoc_fd = heredoc_fd;
		}
		commands = commands->next;
	}
	return (0);
}

t_command	*ft_get_commands(t_tokens *tokens, t_master *master)
{
	t_command	*commands;

	commands = ft_init_commands(ft_count_commands(tokens));
	if (!commands)
		return (NULL);
	if (ft_init_commands_values(commands, tokens))
	{
		ft_free_commands(commands);
		return (NULL);
	}
	ft_fill_commands(commands, tokens);
	if (ft_open_heredocs(commands, master))
	{
		ft_free_commands(commands);
		return (NULL);
	}
	return (commands);
}

t_command	*ft_init_command(void)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->append = 0;
	command->args = NULL;
	command->cmdpath = NULL;
	command->heredoc_delimiters = NULL;
	command->infiles = NULL;
	command->is_heredoc = 0;
	command->outfiles = NULL;
	command->next = NULL;
	command->was_quoted = false;
	command->heredoc_fd = -1;
	return (command);
}

t_command	*ft_init_commands(int total_commands)
{
	t_command	*commandhead;
	t_command	*current;
	int			i;

	i = 0;
	commandhead = ft_init_command();
	if (!commandhead)
		return (NULL);
	current = commandhead;
	while (i < total_commands - 1)
	{
		current->next = ft_init_command();
		if (!current->next)
		{
			ft_free_commands(commandhead);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	return (commandhead);
}
