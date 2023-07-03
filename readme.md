# MINISHELL

## Etapes a realiser
	1. 




echo "Today is $(date)" || &&


A forgotten love story

## Rules (by Doja playing in the background)

- Afficher un prompt en l’attente d’une nouvelle commande.

## All commands authorized explained

Thank you to the chatgpt lords :D

- readline : - get a line from a user with editing
```c
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

char *readline (const char *prompt);
```
will read a line from the terminal and return it.

- rl_clear_history() : - clear the history list that stores previously entered command lines

```c
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void) {
  // Add some commands to the history list
  add_history("command1");
  add_history("command2");
  add_history("command3");

  // Get the current length of the history list
  int length_before_clearing = history_length;
  printf("Length before clearing: %d\n", length_before_clearing);

  // Clear the history list
  rl_clear_history();

  // Get the new length of the history list
  int length_after_clearing = history_length;
  printf("Length after clearing: %d\n", length_after_clearing);

  return 0;
}
```
Be careful to add 
```sh
gcc your_program.c -o your_program -lreadline
```
to your makefile (already added :))

- rl_on_newline() : is used to indicate to Readline that a new command line has been entered and that it should reset the history search state. This function should be called after you've finished processing the current command line and are ready to start reading the next one.

```c
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void) {
  char *command_line;

  while ((command_line = readline("> ")) != NULL) {
    // Process the command line
    printf("You entered: %s\n", command_line);

    // Free the command line memory
    free(command_line);

    // Reset the history search state
    rl_on_newline();
  }

  return 0;
}
```
can be used for 
-> search the history (not really useful for only reading lines)

- rl_replace_line : is used to replace the current command line with a new string. This function is often used in conjunction with rl_redisplay() to update the user's display after modifying the current command line.

```c
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void) {
  char *command_line;

  while ((command_line = readline("> ")) != NULL) {
    // Process the command line
    printf("You entered: %s\n", command_line);

    // Modify the command line
	// value of 0 indicates the entire new command shoudl be used
	// modifies the line that we had
    rl_replace_line("New command line", 0);

    // Redisplay the modified command line
    rl_redisplay();

    // Free the command line memory
    free(command_line);
  }

  return 0;
}
```

- rl_redisplay : see up

- add_history : 

- printf : a function that calls your mom and tells her that you are a failure

- malloc/free/write/access/open/read/close/fork/wait/waitpid : oh so you didn't do pipex ?

- wait3 : is a system call in C that suspends the execution of the calling process until one of its child processes terminates. It is similar to the wait() system call, but provides more information about the child process's resource usage.
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  pid_t pid = fork();

  if (pid == 0) {
    // Child process
    printf("Child process running...\n");
    sleep(5);
    printf("Child process exiting...\n");
    exit(0);
  } else if (pid > 0) {
    // Parent process
    printf("Parent process waiting for child...\n");
    int status;
    struct rusage usage;
    pid_t child_pid = wait3(&status, 0, &usage);
    printf("Child process %d exited with status %d\n", child_pid, status);
    printf("CPU time used by child process: %ld.%06ld seconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("Max resident set size of child process: %ld bytes\n", usage.ru_maxrss);
  } else {
    // Error forking
    perror("fork");
    exit(1);
  }

  return 0;
}
```

- wait4 : same as wait 3 use it as such
```c
int status;
    struct rusage usage;
    struct timespec timeout = {5, 0}; // 5 second timeout
	// If the child process has not exited within 5 seconds, wait4() will return with an error.
    pid_t child_pid = wait4(pid, &status, 0, &usage);
    printf("Child process %d exited with status %d\n", child_pid, status);
    printf("CPU time used by child process: %ld.%06ld seconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("Max resident set size of child process: %ld bytes\n", usage.ru_maxrss);
```

- signal : is a function in C that allows you to register a signal handler for a particular signal. Signals are a form of inter-process communication in Unix-like operating systems that allow processes to receive notifications about certain events, such as interrupts or errors.

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int signal_number) {
  printf("Received signal %d\n", signal_number);
}

int main(void) {
  // Register the signal handler for SIGINT (Ctrl-C)
  if (signal(SIGINT, signal_handler) == SIG_ERR) {
    perror("signal");
    exit(1);
  }

  // Wait for a signal to be received
  printf("Waiting for signal...\n");
  pause();

  return 0;
}
```

- sigaction : more control over the behavior of the signal handler -> better than signal
```c
// Set up the sigaction structure
  struct sigaction sa;
  //specfies the signal_handler function that we defined earlier
  sa.sa_handler = signal_handler;
  // empty sets should be blocked
  sigemptyset(&sa.sa_mask);
  // systems interrupted by the signal should automatically be restarted
  sa.sa_flags = SA_RESTART;

  // Register the signal handler for SIGINT (Ctrl-C)
  if (sigaction(SIGINT, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }
  // suspend the calling process until a signal is called
pause(); // or wait in our case since pause is not allowed
```
- sigemptyset : see sigaction ^
- sigaddset : is a function in C that allows you to add a signal to a set of signals. Signal sets are used in conjunction with signal masks to specify which signals should be blocked or unblocked in a process.
```c
// in main
sigset_t mask;
  sigemptyset(&mask);
  // set up the signal mask to block SIGINT
  sigaddset(&mask, SIGINT);

  // Set the signal mask for this process
  if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
    perror("sigprocmask");
    exit(1);
  }

  // Register the signal handler for SIGTERM (terminate signal)
  struct sigaction sa;
  sa.sa_handler = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  if (sigaction(SIGTERM, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  // Wait for a signal to be received
  printf("Waiting for signal...\n");
  pause();
```

- kill/exit : yeah

- getcwd : get the current working directory of a process
```c
#include <unistd.h>

char *getcwd(char *buf, size_t size);
// buf : pointer to where the current directory will be stored
// size : size of the buffer
// NULL if did not work
int main() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
    return 0;
}
``` 
-> if the working directory is bigger than 1024, we need to find a way to store it ? 
-> not sure
-> find the maximum path length basically
-> use the get_command_path we defined in pipex and but with PATH_MAX (?) (limits.h -> don't know if we can do that lol)

- chdir : change the current working directory of the process
```c
int chdir(const char *path);
// example on how to use it
#include <unistd.h>
#include <stdio.h>

int main() {
    char *newdir = "/home/user/Documents";
    if (chdir(newdir) == 0) {
        printf("Current working directory changed to: %s\n", newdir);
    } else {
        perror("chdir() error");
        return 1;
    }
    return 0;
}
```
- stat : retrieve information about a file or directory, such as its size, permissions, modification time, and more.
```c
int stat(const char *path, struct stat *buf);
// path -> contains the path of what we want
// buf, filled with the information
#include <sys/stat.h>
#include <stdio.h>

int main() {
    char *filename = "example.txt";
    struct stat filestat;
    if (stat(filename, &filestat) == 0) {
        printf("File size: %ld bytes\n", filestat.st_size);
        printf("Permissions: %o\n", filestat.st_mode & 0777);
        printf("Last modified: %s", ctime(&filestat.st_mtime));
    } else {
        perror("stat() error");
        return 1;
    }
    return 0;
}
```
- lstat
- fstat
- unlink/execve/dup/dup2/pipe : yeah
- opendir
- readdir
- closedir
- strerror/perror : send the string associated to an error message. perror will transform error codes into readable error messages
- isatty

## Notes
($?) Expands to the exit status of the most recently executed foreground pipeline.

### 3.5 Shell Expansions
Expansion is performed on the command line after it has been split into tokens.

### 3.5.8 Filename Expansion : *
Bash scans each word for the characters ‘*’, ‘?’, and ‘[’. If one of these characters appears, and is not quoted, then the word is regarded as a pattern, and replaced with an alphabetically sorted list of filenames matching the pattern.

 If no matching filenames are found, and the shell option nullglob is disabled, the word is left unchanged. If the nullglob option is set, and no matches are found, the word is removed. If the failglob shell option is set, and no matches are found, an error message is printed and the command is not executed. If the shell option nocaseglob is enabled, the match is performed without regard to the case of alphabetic characters.

 ### 3.5.8.1 Pattern Matching
 The special pattern characters have the following meanings:

*
Matches any string, including the null string. When the globstar shell option is enabled, and ‘*’ is used in a filename expansion context, two adjacent ‘*’s used as a single pattern will match all files and zero or more directories and subdirectories. If followed by a ‘/’, two adjacent ‘*’s will match only directories and subdirectories.

*(pattern-list)
Matches zero or more occurrences of the given patterns.

### 3.5.9 Quote Removal

After the preceding expansions, all unquoted occurrences of the characters ‘\’, ‘'’, and ‘"’ that did not result from one of the above expansions are removed.
```sh
Ne pas interpréter de quotes (guillemets) non fermés ou de caractères spéciaux non demandés dans le sujet, tels que \ (le backslash) ou ; (le point-virgule). -> Doit-on enlever le \ et ; de la string????
```

### 3.6 Redirections
In the following descriptions, if the file descriptor number is omitted, and the first character of the redirection operator is ‘<’, the redirection refers to the standard input (file descriptor 0). If the first character of the redirection operator is ‘>’, the redirection refers to the standard output (file descriptor 1).

The word following the redirection operator in the following descriptions, unless otherwise noted, is subjected to brace expansion, tilde expansion, parameter expansion, command substitution, arithmetic expansion, quote removal, filename expansion, and word splitting. If it expands to more than one word, Bash reports an error. -> nous on aparameter expansion

Bash handles several filenames specially when they are used in redirections, as described in the following table. If the operating system on which Bash is running provides these special files, bash will use them; otherwise it will emulate them internally with the behavior described below.

/dev/fd/fd
If fd is a valid integer, file descriptor fd is duplicated.

/dev/stdin
File descriptor 0 is duplicated.

/dev/stdout
File descriptor 1 is duplicated.

/dev/stderr
File descriptor 2 is duplicated.

/dev/tcp/host/port
If host is a valid hostname or Internet address, and port is an integer port number or service name, Bash attempts to open the corresponding TCP socket.

/dev/udp/host/port
If host is a valid hostname or Internet address, and port is an integer port number or service name, Bash attempts to open the corresponding UDP socket.

!!! Redirections using file descriptors greater than 9 should be used with care, as they may conflict with file descriptors the shell uses internally. !!!

### 3.6.1 Redirecting Input

Redirection of input causes the file whose name results from the expansion of word to be opened for reading on file descriptor n, or the standard input (file descriptor 0) if n is not specified.

The general format for redirecting input is:

```sh
[n]<word
```

### 3.6.2 Redirecting Output

Redirection of output causes the file whose name results from the expansion of word to be opened for writing on file descriptor n, or the standard output (file descriptor 1) if n is not specified. If the file does not exist it is created; if it does exist it is truncated to zero size.

The general format for redirecting output is:

```sh
[n]>[|]word
```

If the redirection operator is ‘>’, and the noclobber option to the set builtin has been enabled, the redirection will fail if the file whose name results from the expansion of word exists and is a regular file. If the redirection operator is ‘>|’, or the redirection operator is ‘>’ and the noclobber option is not enabled, the redirection is attempted even if the file named by word exists.

### 3.6.3 Appending Redirected Output

Redirection of output in this fashion causes the file whose name results from the expansion of word to be opened for appending on file descriptor n, or the standard output (file descriptor 1) if n is not specified. If the file does not exist it is created.

The general format for appending output is:

[n]>>word

### 3.6.6 Here Documents

This type of redirection instructs the shell to read input from the current source until a line containing only word (with no trailing blanks) is seen. All of the lines read up to that point are then used as the standard input (or file descriptor n if n is specified) for a command.

The format of here-documents is:

[n]<<[-]word
        here-document
delimiter


```sh
No parameter and variable expansion, command substitution, arithmetic expansion, or filename expansion is performed on word. If any part of word is quoted, the delimiter is the result of quote removal on word, and the lines in the here-document are not expanded. If word is unquoted, all lines of the here-document are subjected to parameter expansion, command substitution, and arithmetic expansion, the character sequence \newline is ignored, and ‘\’ must be used to quote the characters ‘\’, ‘$’, and ‘`’.
```

### 3.7.2 Command Search and Execution

After a command has been split into words, if it results in a simple command and an optional list of arguments, the following actions are taken.

If the command name contains no slashes, the shell attempts to locate it. If there exists a shell function by that name, that function is invoked as described in Shell Functions.

If the name does not match a function, the shell searches for it in the list of shell builtins. If a match is found, that builtin is invoked.

If the name is neither a shell function nor a builtin, and contains no slashes, Bash searches each element of $PATH for a directory containing an executable file by that name. Bash uses a hash table to remember the full pathnames of executable files to avoid multiple PATH searches (see the description of hash in Bourne Shell Builtins). A full search of the directories in $PATH is performed only if the command is not found in the hash table. If the search is unsuccessful, the shell searches for a defined shell function named command_not_found_handle. If that function exists, it is invoked in a separate execution environment with the original command and the original command’s arguments as its arguments, and the function’s exit status becomes the exit status of that subshell. If that function is not defined, the shell prints an error message and returns an exit status of 127.

If the search is successful, or if the command name contains one or more slashes, the shell executes the named program in a separate execution environment. Argument 0 is set to the name given, and the remaining arguments to the command are set to the arguments supplied, if any.

If this execution fails because the file is not in executable format, and the file is not a directory, it is assumed to be a shell script and the shell executes it as described in Shell Scripts.

If the command was not begun asynchronously, the shell waits for the command to complete and collects its exit status.

### 3.7.5 Exit Status

The exit status of an executed command is the value returned by the waitpid system call or equivalent function. Exit statuses fall between 0 and 255, though, as explained below, the shell may use values above 125 specially. Exit statuses from shell builtins and compound commands are also limited to this range. Under certain circumstances, the shell will use special values to indicate specific failure modes.

The exit status of the last command is available in the special parameter $? (see Special Parameters).