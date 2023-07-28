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
		if (ft_strncmp(entry->d_name, ".", 2) != 0
			&& ft_strncmp(entry->d_name, "..", 3) != 0)
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

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <dirent.h>
// #include <errno.h>

// // Assuming you have wildcard_match function defined somewhere

// typedef struct {
//     char name[256]; // adjust size if necessary
//     unsigned char type;
// } DirEntry;

// void process_wild(const char *pattern, const char *path) {
//     DIR *dir;
//     struct dirent *entry;
//     char subpath[4096];  // Using a static buffer; adjust size as necessary
//     DirEntry entries[4096]; // Again, this is static and assumes max 4096 entries per dir; adjust if necessary
//     int entry_count = 0;

//     dir = opendir(path);
//     if (!dir) {
//         perror("opendir");
//         return;
//     }

//     // Store all entries
//     while ((entry = readdir(dir)) != NULL) {
//         if (strncmp(entry->d_name, ".", 1) != 0 && strncmp(entry->d_name, "..", 2) != 0) {
//             strncpy(entries[entry_count].name, entry->d_name, sizeof(entries[entry_count].name));
//             entries[entry_count].type = entry->d_type;
//             entry_count++;
//         }
//     }
//     closedir(dir);

//     // Process stored entries
//     for (int i = 0; i < entry_count; i++) {
//         // Process regular files
//         if (entries[i].type != DT_DIR && wildcard_match(entries[i].name, pattern)) {
//             printf("%s/%s\n", path, entries[i].name);
//         }
//         // Process sub-directories
//         else if (entries[i].type == DT_DIR) {
//             snprintf(subpath, sizeof(subpath), "%s/%s", path, entries[i].name);
//             process_wild(pattern, subpath);
//         }
//     }
// }

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