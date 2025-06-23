/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:50:49 by ncantona          #+#    #+#             */
/*   Updated: 2025/06/23 12:17:34 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_for_empty_tokens(t_tokens *tokens)
{
	bool	isempty;

	isempty = true;
	while (tokens)
	{
		if (ft_strlen(tokens->token) > 0)
			isempty = false;
		tokens = tokens->next;
	}
	if (isempty)
		return (1);
	return (0);
}

static int	ft_validate_input(char *input, t_master *master)
{
	if (!ft_count_tokens(input))
		return (0);
	if (ft_check_quote_syntax(input))
		return (master->errorcode = 2, 2);
	return (1);
}

static t_tokens	*ft_process_tokens(char *input, t_master *master)
{
	t_tokens	*tokens;

	tokens = ft_get_tokens(input, master);
	if (!tokens)
	{
		ft_putstr_fd("Allocation failed on tokens!\n", 2);
		master->errorcode = 1;
		return (NULL);
	}
	if (ft_check_operation_syntax(tokens))
	{
		master->errorcode = 2;
		ft_free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}

int	ft_process_line(char *input, t_master *master)
{
	t_tokens	*tokens;
	t_command	*commands;
	int			validation_result;

	validation_result = ft_validate_input(input, master);
	if (validation_result != 1)
		return (validation_result);
	tokens = ft_process_tokens(input, master);
	if (!tokens)
		return (-1);
	commands = ft_get_commands(tokens, master);
	if (!commands)
	{
		ft_free_tokens(tokens);
		ft_printerror(NULL, NULL, "Allocation failed on commands!\n");
		master->errorcode = 1;
		return (-1);
	}
	master->cmds = commands;
	ft_free_tokens(tokens);
	ft_execute_commands(master);
	return (0);
}

t_tokens	*ft_get_tokens(char *input, t_master *master)
{
	t_tokens	*tokens;

	tokens = ft_init_tokens(ft_count_tokens(input));
	if (!tokens)
		return (NULL);
	if (ft_fill_tokens_no_expansion(tokens, input))
		return (NULL);
	tokens = ft_get_final_tokens(tokens, master);
	if (!tokens)
		return (NULL);
	return (tokens);
}
