#include "minishell.h"

int	wildcard_match(const char *str, const char *pattern)
{
	if (*pattern == '\0')
		return (*str == '\0');
	if (*pattern == '*')
	{
		pattern++;
		while (*str)
		{
			if (wildcard_match(str, pattern))
				return (1);
			str++;
		}
		return (wildcard_match(str, pattern));
	}
	else if (*str == '\0')
		return (0);
	else if (*str == *pattern)
		return (wildcard_match(str + 1, pattern + 1));
	return (0);
}

void	process_wild(const char *pattern, const char *path, t_token **tok,
	int *flag)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(path);
	if (!dir)
	{
		perror("opendir");
		return ;
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (ft_strncmp(entry->d_name, ".", 2) != 0
			&& ft_strncmp(entry->d_name, "..", 3) != 0)
		{
			if (wildcard_match(entry->d_name, pattern))
			{
				add_back_tok(tok, new_token(ft_strdup(entry->d_name), CMD));
				*flag = 1;
			}
		}
		entry = readdir(dir);
	}
	closedir(dir);
}
