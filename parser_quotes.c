/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:29:05 by ncantona          #+#    #+#             */
/*   Updated: 2025/06/23 11:24:20 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* counts the length of the string inside quotation of the given
quote type */

int	ft_quoted_str_len_without(char *quoted_str, char quote_type)
{
	int	len;

	len = 0;
	while (quoted_str[len])
	{
		if (quoted_str[len] == quote_type)
			return (len);
		len++;
	}
	return (-1);
}
