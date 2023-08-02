#include "minishell.h"

int	is_paf(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

int	do_search_com(char *start, char *end, char *command, char *filepath)
{
	int	len;

	len = end - start;
	ft_strncpy(filepath, start, len);
	filepath[len] = '/';
	ft_strncpy(filepath + len + 1, command, ft_strlen(command));
	if (access(filepath, X_OK) != -1)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

char	*find_path(char *path, char *command)
{
	char	*start;
	char	*end;
	char	*filepath;

	start = path;
	while (*start)
	{
		end = start;
		while (*end && *end != ':')
			end++;
		filepath = (char *)malloc(sizeof(char)
				* ((end - start) + 2 + ft_strlen(command)));
		if (!filepath)
			return (NULL);
		if (do_search_com(start, end, command, filepath) == EXIT_SUCCESS)
			return (filepath);
		free(filepath);
		if (*end == ':')
			start = end + 1;
		else
			break ;
	}
	return (NULL);
}

char	*get_command_path(char *command, t_env *env)
{
	char	*path;
	t_env	*tmp;

	tmp = env;
	path = NULL;
	while (tmp)
	{
		if (strncmp(tmp->name, "PATH", 4) == 0)
		{
			path = tmp->value;
			break ;
		}
		tmp = tmp->next;
	}
	if (!path)
		return (NULL);
	return (find_path(path, command));
}
