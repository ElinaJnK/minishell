/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_sig.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:04:11 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/21 20:34:05 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*exit_here(void)
{
	static int	here = 0;

	return (&here);
}

void	handle_here_c(int sig)
{
	(void)sig;
	close(0);
	*exit_here() = 130;
}

void	signal_here_c(void)
{
	struct sigaction	act;

	act.sa_handler = handle_here_c;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	get_line(t_here **h, int *ctrl)
{
	t_here	*here;
	char	*tmp;
	int		len;

	here = *h;
	if (write(here->fd, here->line, ft_strlen(here->line)) < 0)
		failure("write failed");
	free(here->line);
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
}
