/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:48:58 by ncantona          #+#    #+#             */
/*   Updated: 2025/06/23 12:07:14 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_operation(t_tokens *tokens, t_tokentype type)
{
	int			total_operations;
	t_tokens	*tmp;

	total_operations = 0;
	tmp = tokens;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == type)
		{
			if (type != WORD)
				total_operations++;
			else if ((!tmp->prev || tmp->prev->type == WORD
					|| tmp->prev->type == PIPE) /*  && !tmp->was_quoted
				&& ft_strlen(tmp->token) */)
				total_operations++;
		}
		tmp = tmp->next;
	}
	return (total_operations);
}

int	ft_count_commands(t_tokens *tokens)
{
	int			total_commands;
	t_tokens	*tmp;

	total_commands = 1;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == PIPE)
			total_commands++;
		tmp = tmp->next;
	}
	return (total_commands);
}

char	**ft_allocate_str_arr(int total_count, int *failflag)
{
	int		i;
	char	**array;

	i = 0;
	if (!total_count)
		return (NULL);
	array = malloc(sizeof(char *) * (total_count + 1));
	if (!array)
	{
		*failflag = 1;
		return (NULL);
	}
	while (i < total_count + 1)
	{
		array[i] = NULL;
		i++;
	}
	return (array);
}
