/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_commands_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:03:06 by mhuthmay          #+#    #+#             */
/*   Updated: 2025/06/23 12:09:45 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_allocate_command_arrays(t_command *tmpcmd, t_tokens *tmptok,
		int *failflag)
{
	tmpcmd->infiles = ft_allocate_str_arr(ft_count_operation(tmptok, INPUT),
			failflag);
	if (*failflag)
		return (-1);
	tmpcmd->outfiles = ft_allocate_str_arr(ft_count_operation(tmptok, OUTPUT)
			+ ft_count_operation(tmptok, APPEND), failflag);
	if (*failflag)
		return (-1);
	return (0);
}

static int	ft_allocate_more_command_arrays(t_command *tmpcmd, t_tokens *tmptok,
		int *failflag)
{
	tmpcmd->heredoc_delimiters = ft_allocate_str_arr(ft_count_operation(tmptok,
				HEREDOC), failflag);
	if (*failflag)
		return (-1);
	tmpcmd->args = ft_allocate_str_arr(ft_count_operation(tmptok, WORD),
			failflag);
	if (*failflag)
		return (-1);
	return (0);
}

static t_tokens	*ft_advance_to_next_command(t_tokens *tmptok)
{
	while (tmptok && tmptok->type != PIPE)
		tmptok = tmptok->next;
	if (tmptok && tmptok->type == PIPE)
		tmptok = tmptok->next;
	return (tmptok);
}

int	ft_init_commands_values(t_command *commands, t_tokens *tokens)
{
	t_tokens	*tmptok;
	t_command	*tmpcmd;
	int			failflag;

	tmptok = tokens;
	tmpcmd = commands;
	failflag = 0;
	while (tmpcmd)
	{
		if (ft_allocate_command_arrays(tmpcmd, tmptok, &failflag) == -1)
			return (-1);
		if (ft_allocate_more_command_arrays(tmpcmd, tmptok, &failflag) == -1)
			return (-1);
		tmptok = ft_advance_to_next_command(tmptok);
		tmpcmd = tmpcmd->next;
	}
	return (0);
}
