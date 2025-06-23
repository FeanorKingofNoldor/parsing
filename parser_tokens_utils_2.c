/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_utils_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:31:23 by ncantona          #+#    #+#             */
/*   Updated: 2025/06/23 12:20:03 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* takes the tokens that are not expanded and expands
the variables in each token, then splits the tokens again
into more tokens if necessary and last but not least
the outer quotations in the tokens are getting erased */

t_tokens	*ft_split_expanded_tokens(t_tokens *tokens)
{
	t_tokens	*new_tokens;

	new_tokens = ft_init_tokens(ft_count_tokens_in_tokens(tokens));
	if (!new_tokens)
	{
		ft_free_tokens(tokens);
		return (NULL);
	}
	if (ft_fill_tokens_with_expansion(new_tokens, tokens))
	{
		ft_free_tokens(new_tokens);
		ft_free_tokens(tokens);
		return (NULL);
	}
	if (ft_erase_quotations(new_tokens))
	{
		ft_free_tokens(new_tokens);
		ft_free_tokens(tokens);
		return (NULL);
	}
	ft_free_tokens(tokens);
	return (new_tokens);
}

static int	ft_handle_quote(char **tmp, int *x, t_tokens *token)
{
	int		quote_length;
	char	*result;

	token->was_quoted = true;
	quote_length = ft_quoted_str_len_without(*tmp + *x + 1, (*tmp)[*x]) + 1;
	(*tmp)[*x] = '\0';
	(*tmp)[*x + quote_length] = '\0';
	result = ft_strjoin3(*tmp, *tmp + *x + 1, *tmp + *x + quote_length + 1);
	(*tmp)[*x] = 'X';
	(*tmp)[*x + quote_length] = 'X';
	free(*tmp);
	if (!result)
		return (0);
	*tmp = result;
	*x += quote_length - 1;
	return (1);
}

char	*ft_get_final_token(t_tokens *token)
{
	int		x;
	char	*tmp;

	tmp = ft_strdup(token->token);
	if (!tmp)
		return (NULL);
	x = 0;
	while (tmp[x])
	{
		if (tmp[x] == '\"' || tmp[x] == '\'')
		{
			if (!ft_handle_quote(&tmp, &x, token))
			{
				free(tmp);
				return (NULL);
			}
		}
		else
			x++;
	}
	return (tmp);
}

/* calls get final token on each token to erase all outer quotations */

int	ft_erase_quotations(t_tokens *tokens)
{
	t_tokens	*tmp;
	char		*final_token;

	tmp = tokens;
	while (tmp)
	{
		final_token = ft_get_final_token(tmp);
		if (!final_token)
			return (-1);
		free(tmp->token);
		tmp->token = final_token;
		tmp = tmp->next;
	}
	return (0);
}
