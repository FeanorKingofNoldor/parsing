/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:41:17 by ncantona          #+#    #+#             */
/*   Updated: 2025/06/23 12:14:39 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* checks the input for unclosed quotes */

int	ft_check_quote_syntax(char *input)
{
	int	i;
	int	quoted_str_len;

	i = 0;
	quoted_str_len = 0;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
		{
			quoted_str_len = ft_quoted_str_len_without(input + i + 1, input[i]);
			if (quoted_str_len == -1)
			{
				ft_printerror(NULL, NULL, "syntax error unclosed quotes\n");
				return (2);
			}
			else
				i += quoted_str_len + 2;
		}
		else
			i++;
	}
	return (0);
}

/* checks if an operation is followed by another operation without WORD
input in between, exception for PIPE */

static int	ft_check_consecutive_token_syntax(t_tokens *tmp)
{
	if ((tmp->type != WORD && tmp->next && tmp->next->type != WORD
			&& tmp->type != PIPE) || (tmp->type == PIPE && tmp->next
			&& tmp->next->type == PIPE))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
		ft_putstr_fd("`", 2);
		ft_putstr_fd(tmp->next->token, 2);
		ft_putstr_fd("\'\n", 2);
		return (2);
	}
	return (0);
}

int	ft_check_operation_syntax(t_tokens *tokens)
{
	t_tokens	*tmp;
	int			consecutive_token_result;

	tmp = tokens;
	if (tmp && tmp->type == PIPE)
		return (ft_putstr_fd("minishell: syntax error near \
			unexpected token `|\'\n", 2), 1);
	while (tmp)
	{
		consecutive_token_result = ft_check_consecutive_token_syntax(tmp);
		if (consecutive_token_result != 0)
			return (consecutive_token_result);
		if (tmp->type > WORD && tmp->type < PIPE && tmp->next
			&& !ft_strlen(tmp->next->token) && tmp->type != HEREDOC)
			return (ft_putstr_fd("minishell: ambigous redirect\n", 2), 1);
		else if ((tmp->type != WORD && !tmp->next))
			return (ft_putstr_fd("minishell: syntax error near \
				unexpected token `newline\'\n", 2), 1);
		tmp = tmp->next;
	}
	return (0);
}
