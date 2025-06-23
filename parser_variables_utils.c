/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:42:33 by mhuthmay          #+#    #+#             */
/*   Updated: 2025/06/23 12:19:19 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* expands every variable in the given string if the flag set to true, if not
only the variables outside quotations or inside "" quotations
are getting expanded not variables inside '' */

static bool	ft_expand_variable(char **result, t_expand_state *state,
		t_master *master)
{
	char	*varvalue;

	varvalue = ft_get_expanded_variable((*result) + state->i + 1, master);
	if (!varvalue)
	{
		free(*result);
		return (false);
	}
	*result = ft_concat_var(*result, varvalue, state->i, ft_varkey_len((*result)
				+ state->i + 1));
	if (!*result)
		return (false);
	if (state->in_double_quotes)
		state->in_double_quotes = !state->in_double_quotes;
	state->i = 0;
	return (true);
}

bool	ft_process_char(char **result, t_expand_state *state, t_master *master,
		bool consider_quotation)
{
	if ((*result)[state->i] == '$' && ft_varkey_len((*result) + state->i
			+ 1) != 0)
	{
		if (!ft_expand_variable(result, state, master))
			return (false);
	}
	else if ((*result)[state->i] == '\"')
	{
		state->in_double_quotes = !state->in_double_quotes;
		state->i++;
	}
	else if ((*result)[state->i] == '\'' && consider_quotation
		&& !state->in_double_quotes)
	{
		state->i += ft_quoted_str_len_without((*result) + state->i + 1, '\'')
			+ 2;
	}
	else
		state->i++;
	return (true);
}

char	*ft_expand_variables(char *input, t_master *master,
		bool consider_quotation)
{
	char			*result;
	t_expand_state	state;

	result = ft_strdup(input);
	if (!result)
		return (NULL);
	state.i = 0;
	state.in_double_quotes = false;
	while (result[state.i])
	{
		if (!ft_process_char(&result, &state, master, consider_quotation))
			return (NULL);
	}
	return (result);
}
