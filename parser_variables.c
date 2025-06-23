/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:39:46 by ncantona          #+#    #+#             */
/*   Updated: 2025/06/23 11:25:44 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* counts the length of the variable key for example:
USER = 4 | ? = 1 | 1n32u45 = 0 | hell! = 4 */

int	ft_varkey_len(char *input)
{
	int	len;

	len = 0;
	if (ft_isnum(input[0]))
		return (0);
	if (input[0] == '?')
		return (1);
	while (input[len])
	{
		if (!ft_isalpha(input[len]) && !ft_isnum(input[len])
			&& input[len] != '_')
			return (len);
		len++;
	}
	return (len);
}

/* takes the variable key and searches for the variable in env
if found returns the var value or empty string if not, special case for '$?' */

char	*ft_get_expanded_variable(char *input, t_master *master)
{
	char	*varkey;
	t_env	*variable;

	if (input[0] && input[0] == '?')
		return (ft_itoa(master->errorcode));
	varkey = ft_strndup(input, ft_varkey_len(input));
	if (!varkey)
		return (NULL);
	variable = ft_getvar(master->env, varkey);
	free(varkey);
	if (variable)
		return (ft_strdup(variable->value));
	return (ft_strdup(""));
}

/* concatinates the string before the $ of the input with the varvalue and
with the string after the variablekey for example hello$USER! | beginning = hello
| varvalue = "value of $USER" | end = ! */

char	*ft_concat_var(char *str, char *varvalue, int varstart, int varkeylen)
{
	char	*beginning;
	char	*end;
	char	*result;

	beginning = ft_strndup(str, varstart);
	if (!beginning)
	{
		free(str);
		free(varvalue);
		return (NULL);
	}
	end = ft_strdup(str + varstart + varkeylen + 1);
	if (!end)
	{
		free(str);
		free(beginning);
		free(varvalue);
		return (NULL);
	}
	result = ft_strjoin3(beginning, varvalue, end);
	free(beginning);
	free(end);
	free(varvalue);
	free(str);
	return (result);
}

/* expands the variables of the every token */

int	ft_expand_token_variables(t_tokens *tokens, t_master *master)
{
	t_tokens	*tmp;
	char		*expanded_token;

	tmp = tokens;
	while (tmp)
	{
		if (!tmp->prev || (tmp->prev && tmp->prev->type != HEREDOC))
		{
			expanded_token = ft_expand_variables(tmp->token, master, true);
			if (!expanded_token)
				return (-1);
			free(tmp->token);
			tmp->token = expanded_token;
		}
		tmp = tmp->next;
	}
	return (0);
}
