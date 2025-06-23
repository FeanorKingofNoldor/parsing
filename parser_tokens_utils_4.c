/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_utils_4.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:52:21 by mhuthmay          #+#    #+#             */
/*   Updated: 2025/06/23 11:27:05 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_handle_empty_token(t_token_state *state)
{
	if (!state->oldtmp->token || !state->oldtmp->token[state->i])
	{
		ft_process_token(state->newtmp, state->oldtmp->token + state->i,
			state->oldtmp->type);
		return (0);
	}
	return (1);
}

static int	ft_process_token_string(t_token_state *state)
{
	int	len;

	while (state->oldtmp->token[state->i])
	{
		if (!ft_isspace(state->oldtmp->token[state->i]))
		{
			len = ft_process_token(state->newtmp, state->oldtmp->token
					+ state->i, state->oldtmp->type);
			if (len == -1)
				return (-1);
			state->i += len;
			state->newtmp = state->newtmp->next;
		}
		else
			state->i++;
	}
	return (0);
}

int	ft_fill_tokens_with_expansion(t_tokens *new_tokens, t_tokens *old_tokens)
{
	t_token_state	state;

	state.newtmp = new_tokens;
	state.oldtmp = old_tokens;
	while (state.oldtmp)
	{
		state.i = 0;
		if (!ft_handle_empty_token(&state))
		{
			state.newtmp = state.newtmp->next;
			state.oldtmp = state.oldtmp->next;
			continue ;
		}
		if (ft_process_token_string(&state) == -1)
			return (-1);
		state.oldtmp = state.oldtmp->next;
	}
	return (0);
}
