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
