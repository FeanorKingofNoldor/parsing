/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 02:21:08 by ncantona          #+#    #+#             */
/*   Updated: 2025/06/23 12:17:05 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* allocates memory for one token struct and sets everythin to NULL or 0 */

static t_tokens	*ft_init_token(void)
{
	t_tokens	*token;

	token = malloc(sizeof(t_tokens));
	if (!token)
		return (NULL);
	token->token = NULL;
	token->type = 0;
	token->was_quoted = false;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

/* allocotates as many tokens as the given argument tells */

t_tokens	*ft_init_tokens(int total_tokens)
{
	t_tokens	*tokens;
	t_tokens	*current_token;
	t_tokens	*new_token;
	int			i;

	i = 0;
	tokens = ft_init_token();
	if (!tokens)
		return (NULL);
	current_token = tokens;
	while (i++ < total_tokens - 1)
	{
		new_token = ft_init_token();
		if (!new_token)
		{
			ft_free_tokens(tokens);
			return (NULL);
		}
		new_token->prev = current_token;
		current_token->next = new_token;
		current_token = current_token->next;
	}
	return (tokens);
}

/* splits the input into tokens without expanding the variables in it
and gives each token a tokentype such as WORD or PIPE */

int	ft_fill_tokens_no_expansion(t_tokens *tokens, char *input)
{
	if (ft_split_input_into_tokens(tokens, input) == -1)
	{
		ft_free_tokens(tokens);
		return (-1);
	}
	return (0);
}

/* takes the tokens, expands the variables and splits the tokens again
to avoid cases when variables gets expanded to a space or tab, returns the final
tokens, expanded, quotations erased! */

t_tokens	*ft_get_final_tokens(t_tokens *tokens, t_master *master)
{
	t_tokens	*new_tokens;

	if (ft_expand_token_variables(tokens, master))
	{
		ft_free_tokens(tokens);
		return (NULL);
	}
	new_tokens = ft_split_expanded_tokens(tokens);
	if (!new_tokens)
		return (NULL);
	return (new_tokens);
}
