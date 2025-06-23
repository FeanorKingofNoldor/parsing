/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_utils_1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 02:22:40 by ncantona          #+#    #+#             */
/*   Updated: 2025/06/23 12:18:36 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_token_length(char *token)
{
	int	i;

	i = 0;
	if (ft_count_operation_length(token))
		return (ft_count_operation_length(token));
	while (token[i] && !ft_isspace(token[i]))
	{
		if (token[i] == '\"')
			i += ft_quoted_str_len_without(token + i + 1, '\"') + 2;
		else if (token[i] == '\'')
			i += ft_quoted_str_len_without(token + i + 1, '\'') + 2;
		else
			i++;
		if (ft_count_operation_length(token + i))
			return (i);
	}
	return (i);
}

int	ft_count_tokens(char *input)
{
	int	i;
	int	total_tokens;

	i = 0;
	total_tokens = 0;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
		{
			total_tokens++;
			i += ft_count_token_length(input + i);
		}
		else
			i++;
	}
	return (total_tokens);
}

int	ft_count_operation_length(char *str)
{
	if (!str)
		return (0);
	if ((str[0] == '<' && str[0 + 1] && str[0 + 1] == '<') || (str[0] == '>'
			&& str[0 + 1] && str[0 + 1] == '>'))
		return (2);
	if (str[0] == '<' || str[0] == '>' || str[0] == '|')
		return (1);
	return (0);
}

t_tokentype	ft_get_token_type(char *token)
{
	if (!ft_strcmp(token, "<"))
		return (INPUT);
	else if (!ft_strcmp(token, ">"))
		return (OUTPUT);
	else if (!ft_strcmp(token, "<<"))
		return (HEREDOC);
	else if (!ft_strcmp(token, ">>"))
		return (APPEND);
	else if (!ft_strcmp(token, "|"))
		return (PIPE);
	return (WORD);
}

int	ft_count_tokens_in_tokens(t_tokens *tokens)
{
	int			total_tokens;
	t_tokens	*tmp;

	total_tokens = 0;
	tmp = tokens;
	while (tmp)
	{
		if (!ft_strlen(tmp->token))
			total_tokens++;
		total_tokens += ft_count_tokens(tmp->token);
		tmp = tmp->next;
	}
	return (total_tokens);
}
