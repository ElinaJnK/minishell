/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:06:17 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/23 12:51:48 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

void	here_doc_q(char *line, t_token **lst_tok)
{
	int		i;
	t_token	*last;

	i = 0;
	last = last_elem(*lst_tok);
	while (line[i] && !is_op(line + i) && !is_fb(line + i))
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			last->type = DREDIR2_E;
			return ;
		}
		i++;
	}
}

void	here_child(int fd, t_token *t, t_all *all, t_token *tmp)
{
	int	ex;

	sig_child();
	signal_here_c();
	ex = read_stdin(fd, t, all->env);
	free_lst_tok(&tmp);
	free_all(all);
	exit(ex);
}

void	free_here(t_here *here)
{
	if (here)
	{
		if (here->line)
			free(here->line);
		free(here);
	}
}

int	init_here(t_here **her, int fd, int *ctrl)
{
	t_here	*here;
	int		len;
	char	*tmp;

	here = malloc(sizeof(t_here));
	if (!here)
		return (close(fd), EXIT_FAILURE);
	*ctrl = EXIT_SUCCESS;
	here->fd = fd;
	here->i = 1;
	here->line = readline("> ");
	if (here->line)
	{
		len = (int)ft_strlen(here->line);
		if (len > 0 && here->line[len - 1] != '\n')
		{
			tmp = here->line;
			here->line = ft_strjoin(tmp, "\n");
			free(tmp);
		}
	}
	ctrl_d(&here, ctrl);
	if (*ctrl != EXIT_SUCCESS)
		return (free_here(here), *ctrl);
	return ((*her) = here, EXIT_SUCCESS);
}
