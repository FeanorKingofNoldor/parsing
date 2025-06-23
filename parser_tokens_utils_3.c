/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_utils_3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:34:40 by ncantona          #+#    #+#             */
/*   Updated: 2025/06/23 11:27:12 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* takes the token and input and fills the token until the length of the token
is reached so it doesnt go further in the input char *, sets either the given
tokentype or the one it will get by comparing the token*/

int	ft_process_token(t_tokens *token, char *input, t_tokentype tokentype)
{
	int		len;
	char	*new_token;

	len = ft_count_operation_length(input);
	if (!len)
		len = ft_count_token_length(input);
	new_token = ft_strndup(input, len);
	if (!new_token)
		return (-1);
	token->token = new_token;
	if (tokentype == 6)
		token->type = ft_get_token_type(new_token);
	else
		token->type = tokentype;
	return (len);
}

/* splits the input into tokens */

int	ft_split_input_into_tokens(t_tokens *tokens, char *input)
{
	int			i;
	int			len;
	t_tokens	*tmp;

	i = 0;
	tmp = tokens;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
		{
			len = ft_process_token(tmp, input + i, 6);
			if (len == -1)
				return (-1);
			i += len;
			tmp = tmp->next;
		}
		else
			i++;
	}
	return (0);
}
