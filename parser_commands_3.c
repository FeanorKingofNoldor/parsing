/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_commands_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuthmay <mhuthmay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:40:22 by mhuthmay          #+#    #+#             */
/*   Updated: 2025/06/23 12:09:48 by mhuthmay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Handle input type tokens */
static void	ft_handle_input_token(t_command *command, t_tokens *token,
		t_token_positions *pos)
{
	command->is_heredoc = 0;
	command->infiles[pos->inpos] = token->next->token;
	token->next->token = NULL;
	pos->inpos++;
}

/* Handle output and append type tokens */
static void	ft_handle_output_token(t_command *command, t_tokens *token,
		t_token_positions *pos)
{
	if (token->type == APPEND)
		command->append = 1;
	else
		command->append = 0;
	command->outfiles[pos->outpos] = token->next->token;
	token->next->token = NULL;
	pos->outpos++;
}

/* Handle heredoc type tokens */
static void	ft_handle_heredoc_token(t_command *command, t_tokens *token,
		t_token_positions *pos)
{
	command->was_quoted = token->next->was_quoted;
	command->is_heredoc = 1;
	command->heredoc_delimiters[pos->hdpos] = token->next->token;
	token->next->token = NULL;
	pos->hdpos++;
}

/* Reset function to zero out all position trackers */
static void	ft_reset_positions(t_token_positions *pos)
{
	pos->inpos = 0;
	pos->outpos = 0;
	pos->hdpos = 0;
	pos->argpos = 0;
}

/* Main function to process tokens */
void	ft_fill_infiles(t_command *command, t_tokens *token, bool reset)
{
	static t_token_positions	pos = {0};

	if (reset)
	{
		ft_reset_positions(&pos);
		return ;
	}
	if (token->type == INPUT)
		ft_handle_input_token(command, token, &pos);
	else if (token->type == OUTPUT || token->type == APPEND)
		ft_handle_output_token(command, token, &pos);
	else if (token->type == HEREDOC)
		ft_handle_heredoc_token(command, token, &pos);
	else if (token->type == WORD)
	{
		if (!ft_strlen(token->token) && !token->was_quoted)
			return ;
		command->args[pos.argpos] = token->token;
		token->token = NULL;
		pos.argpos++;
	}
}
