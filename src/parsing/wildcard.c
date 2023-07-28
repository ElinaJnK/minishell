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

void	process_wild(const char *pattern, const char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	//char			*subpath;

	dir = opendir(path);
	if (!dir)
	{
		perror("opendir");
		return ;
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (ft_strncmp(entry->d_name, ".", 1) != 0
			&& ft_strncmp(entry->d_name, "..", 2) != 0)
		{
			if (wildcard_match(entry->d_name, pattern))
				printf("%s\n", entry->d_name);
			// if (entry->d_type == DT_DIR)
			// {
			//     subpath = malloc(sizeof(char) * 100);
			//     snprintf(subpath, sizeof(subpath), "%s/%s", path,
			//              entry->d_name);
			//     process_wild(pattern, subpath);
			//     free(subpath);
			// }
		}
		entry = readdir(dir);
	}
	closedir(dir);
}

// int	main(void)
// {
// 	char	*pwd;

// 	pwd = getcwd(NULL, 0);
// 	printf("Match test1: %d\n", wildcard_match("hello", "h*lo"));
// 	printf("Match test2: %d\n", wildcard_match("hello", "he*o"));
// 	printf("Match test3: %d\n", wildcard_match("hello", "*"));
// 	printf("Match test4: %d\n", wildcard_match("hello", "he*llo*"));
// 	printf("Match test5: %d\n", wildcard_match("hello", "he*m"));
// 	process_wild("*ea*", pwd);
// 	return 0;
// }