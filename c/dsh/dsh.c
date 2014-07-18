/*
 *
 * dsh - david's shell
 *
 */

#define _PROGRAM_NAME "dsh"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFFER_SIZE 256
#define HISTORY_SIZE 1024
#define JOBS_SIZE 5
#define FALSE 0
#define TRUE 1
#define FOREGROUND 0
#define BACKGROUND 1

#define HISTORY_FILE "shell_history"

typedef struct job_t {
	char *command;
	pid_t pid;
	time_t start_time;
	int id;
} job;

char **path; /* list of directories in PATH (null-terminated) */
char *history[HISTORY_SIZE]; /* list of recent commands */
int history_index; /* index of last command in history */
job *job_list[JOBS_SIZE]; /* list of current background processes */
int job_index; /* next free position in job_list */
int job_counter; /* keep track of max job id - prevents duplicate ids */
int num_jobs; /* number of jobs currently running */

/* handle the child-signal */
void signal_handler(int signal)
{
	pid_t pid;
	time_t end_time;
	double cpu_time;
	int i, status;

	switch (signal) {
	case SIGCHLD:
		pid = waitpid(WAIT_ANY, &status, WNOHANG | WUNTRACED);
		if (pid > 0) {
			for (i = 0; i < JOBS_SIZE; i++) {
				if (job_list[i]->pid == pid) {
					time(&end_time);
					cpu_time = (double)(end_time - job_list[i]->start_time);
					if (WIFSIGNALED(status)) {
						printf("[%d]+ Killed (%d) %.2f sec\t%s\n", job_list[i]->id, job_list[i]->pid, cpu_time, job_list[i]->command);
					} else {
						printf("[%d]+ Done (%d) %.2f sec\t%s\n", job_list[i]->id, job_list[i]->pid, cpu_time, job_list[i]->command);
					}
					/* remove job from list */
					free(job_list[i]->command);
					job_list[i]->command = NULL;
					job_list[i]->id = 0;
					job_index = i;
					num_jobs--;
					if (num_jobs == 0) {
						/* no more jobs running - reset job_counter */
						job_counter = 1;
					}
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}

/* given the input string, tokenize on the delimiters specified - returns a null-terminated array of strings */
char **tokenize(char *input, char *delims)
{
	char *s;
	char *token;
	char **result;
	int i, max = 10;

	/* Protect input string - strtok modifies string */
	s = malloc(sizeof(*s) * strlen(input) + 1);
	strncpy(s, input, strlen(input) + 1);

	result = malloc(sizeof(*result) * max);
	
	/* Tokenize string on blankspace */
	token = strtok(s, delims);
	for (i = 0;; i++) {
		if (i >= max) {
			result = realloc(result, sizeof(*result) * max * 2);
			max = max * 2;
		}
		if (token) {
			result[i] = malloc(sizeof(*result[i]) * strlen(token) + 1);
			strncpy(result[i], token, strlen(token) + 1);
		} else {
			result[i] = NULL;
			break;
		}

		token = strtok(NULL, delims);
	}
	free(s);
	return result;
}

/* free all members of a array of strings */
void free_strings(char **strings)
{
	char **temp_strings;
	char *temp;

	if (strings == NULL)
		return;
	
	for (temp_strings = strings; *temp_strings; temp_strings++) {
		temp = *temp_strings;
		free(temp);
	}
	free(strings);
}

/* initialize global variables on startup */
void init(void)
{
	int i;
	
	signal(SIGCHLD, &signal_handler);

	path = tokenize(getenv("PATH"), ":");

	for (i = 0; i < JOBS_SIZE; i++) {
		job_list[i] = malloc(sizeof(*job_list[i]));
		if (!job_list[i]) {
			fprintf(stderr, "%s: memory allocation failed\n", _PROGRAM_NAME);
		}
		job_list[i]->command = NULL;
		job_list[i]->id = 0;
	}
	job_index = 0;
	job_counter = 1;
	num_jobs = 0;

	for (i = 0; i < HISTORY_SIZE; i++) {
		history[i] = malloc(sizeof(*history[i]) * 8);
		if (!history[i]) {
			fprintf(stderr, "%s: memory allocation failed\n", _PROGRAM_NAME);
		}
		strncpy(history[i], "<empty>", 8);
	}
	history_index = 0;

	if (read_history(HISTORY_FILE) != 0) {
		fprintf(stderr, "%s: history file not found, will be created upon exit\n", _PROGRAM_NAME);
	}
}

/* free global variables on exit */
void cleanup(void)
{
	int i;
	
	for (i = 0; i < JOBS_SIZE; i++) {
		if (job_list[i]->command) {
			free(job_list[i]->command);
		}
		free(job_list[i]);
	}

	for (i = 0; i < HISTORY_SIZE; i++) {
		free(history[i]);
	}
	
	free_strings(path);
}

/* add string to the history list */
void history_add(char *input)
{
	/* update index to point to new command being added */
	history_index = (history_index + 1) % HISTORY_SIZE;
	/* resize memory for string and copy into history */
	history[history_index] = realloc(history[history_index], strlen(input) + 1);
	strncpy(history[history_index], input, strlen(input) + 1);
}

/* fill s with a prompt to be displayed to the user before command entry */
void prompt(char *s)
{
	static char *username;
	static char hostname[BUFFER_SIZE];
	static char abs_dir[BUFFER_SIZE];
	static char *dir;
	
	/* better to get username from env than with getlogin() - user may have set LOGNAME */
	if(!(username = getenv("LOGNAME"))) {
		fprintf(stderr, "%s: failed to obtain username\n", _PROGRAM_NAME);
	}

	if (gethostname(hostname, BUFFER_SIZE) != 0) {
		fprintf(stderr, "%s: failed to obtain hostname\n", _PROGRAM_NAME);
	}

	/* Get current directory */
	if (getcwd(abs_dir, BUFFER_SIZE) == 0) {
		fprintf(stderr, "%s: failed to obtain working directory\n", _PROGRAM_NAME);
	}

	dir = strrchr(abs_dir, '/');
	dir++;
	snprintf(s, BUFFER_SIZE, "[%s@%s %s]>>> ", username, hostname, dir);
}

/* verify that the file exists and is not a directory */
int is_command(char *file)
{
	struct stat fs;

	if (stat(file, &fs) == 0) {
		/* check that file is not directory - if not executable, execv will catch and throw an error */
		if (!S_ISDIR(fs.st_mode)) {
			return 0;
		}
	}
	return 1;
}

/* search all directories in PATH for command */
int search_path(char *command)
{
	char buf[BUFFER_SIZE], temp[BUFFER_SIZE];
	int i;

	/* command may be an absolute path */
	if (command[0] == '/') {
		printf("abs path given\n");
		if (is_command(command) == 0) {
			return 0;
		}
	}
	/* command may be a relative path */
	else if (strchr(command, '/')) {
		if (getcwd(temp, BUFFER_SIZE) == 0) {
			fprintf(stderr, "%s: failed to obtain working directory\n", _PROGRAM_NAME);
		}
		snprintf(buf, BUFFER_SIZE, "%s/%s", temp, command);
		if (is_command(buf) == 0) {
			return 0;
		}
	}
	/* check if file exists somewhere in PATH */
	else {
		for (i = 0; path[i]; i++) {
			snprintf(temp, BUFFER_SIZE, "%s/%s", path[i], command);
			if (is_command(temp) == 0) {
				/* command found - update command to absolute path */
				strncpy(command, temp, strlen(temp) + 1);
				return 0;
			}
		}
	}
	/* command not found */
	return 1;
}

/* execute the passed in command in a new child process - put in background if applicable */
void execute_command(char *command, char **args, int state)
{
	pid_t pid;
	job *new_job;
	int i, status;

	if ((pid = fork()) < 0) {
		perror(_PROGRAM_NAME ": fork");
		return;
	}
	switch (pid) {
	case 0:
		if (execv(args[0], args) < 0) {
			perror(_PROGRAM_NAME ": execute error");
			exit(EXIT_FAILURE);
		}
	default:
		if (state == FOREGROUND) {
			/* wait for child to finish executing */
			waitpid(pid, &status, 0);
		} else {
			/* add child to job_list */
			new_job = job_list[job_index];
			new_job->command = malloc(sizeof(*new_job->command) * strlen(command) + 1);
			if (new_job->command == NULL) {
				fprintf(stderr, "%s: memory allocation failed\n", _PROGRAM_NAME);
			}
			strncpy(new_job->command, command, strlen(command) + 1);
			new_job->pid = pid;
			time(&(new_job->start_time));
			new_job->id = job_counter++;
			num_jobs++;
			/* set job_index to next available position in job_list */
			if (num_jobs != JOBS_SIZE) {
				for (i = 0; i < JOBS_SIZE; i++) {
					job_index = (job_index + 1) % JOBS_SIZE;
					if (job_list[job_index]->command == NULL) {
						break;
					}
				}
			}
			printf("[%d] %d\n", new_job->id, new_job->pid);
		}
	}
}

/* pipe several commands together */
int pipe_command(char *input)
{
	char *command, *s, *t;
	char **temp_list;
	char ***cmd_list;
	int i, j, len;
	pid_t *pid;
	int **fd;
	int *status;
	
	/* copy input into command temp variable */
	command = malloc(sizeof(*command) * strlen(input) + 1);
	if (!command) {
		fprintf(stderr, "%s: error alocating memory\n", _PROGRAM_NAME);
	}
	strncpy(command, input, strlen(input) + 1);

	if ((s = strchr(command, '[')) == NULL) {
		fprintf(stderr, "%s: pipe: syntax error: missing \"[\"\n", _PROGRAM_NAME);
		return -1;
	}

	if ((t = strchr(command, ']')) == NULL) {
		fprintf(stderr, "%s: pipe: syntax error: missing \"]\"\n", _PROGRAM_NAME);
		return -1;
	}
	s++; /* s now points to beginning of first pipe command */
	*t = '\0'; /* remove trailing ']' */
	
	temp_list = tokenize(s, ",");
	for (len = 0; temp_list[len]; len++);

/*	if (len < 2 || len > 4) {
		fprintf(stderr, "%s: pipe: pipe length of %d unsupported\n", _PROGRAM_NAME, len);
		return -1;
	}
*/
	if (len < 2) {
		fprintf(stderr, "%s: pipe: pipe length of %d unsupported\n", _PROGRAM_NAME, len);
		return -1;
	}

	cmd_list = malloc(sizeof(*cmd_list) * len + 1);
	if (!cmd_list) {
		fprintf(stderr, "%s: error alocating memory\n", _PROGRAM_NAME);
	}
	for (i = 0; temp_list[i]; i++) {
		cmd_list[i] = tokenize(temp_list[i], " ");
	}
	cmd_list[len] = NULL;

	status = malloc(sizeof(*status) * len);
	if (!status) {
		fprintf(stderr, "%s: error alocating memory\n", _PROGRAM_NAME);
	}
	
	pid = malloc(sizeof(*pid) * len);
	if (!pid) {
		fprintf(stderr, "%s: error alocating memory\n", _PROGRAM_NAME);
	}

	fd = malloc(sizeof(*fd) * (len - 1));
	if (!fd) {
		fprintf(stderr, "%s: error alocating memory\n", _PROGRAM_NAME);
	}
	for (i = 0; i< (len - 1); i++) {
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i]) {
			fprintf(stderr, "%s: error alocating memory\n", _PROGRAM_NAME);
		}
	}
	/* set up pipe file descriptors */
	for (i = 0; i < (len - 1); i++) {
		if (pipe(fd[i]) != 0) {
			perror(_PROGRAM_NAME ": pipe");
			return -1;
		}
	}
	/* left command */
	pid[0] = fork();
	switch (pid[0]) {
	case -1:
		perror(_PROGRAM_NAME ": fork");
		return -1;
	case 0:
		close(1);
		dup(fd[0][1]);
		for (i = 0; i < (len - 1); i++) {
			close(fd[i][0]);
			close(fd[i][1]);
		}
		if (search_path(cmd_list[0][0]) == 0) {
			if (execv(cmd_list[0][0], cmd_list[0]) < 0) {
				perror(_PROGRAM_NAME ": execute error");
				exit(EXIT_FAILURE);
			}
		} else {
			fprintf(stderr, "%s: %s: command not found\n", _PROGRAM_NAME, cmd_list[0][0]);
			return -1;
		}
	}
	/* inner commands */
	for (i = 1; i <= (len - 2); i++) {
		pid[i] = fork();
		switch(pid[i]) {
		case -1:
			perror(_PROGRAM_NAME ": fork");
			return -1;
		case 0:
			close(0);
			dup(fd[i - 1][0]);
			close(1);
			dup(fd[i][1]);
			for (j = 0; j < (len - 1); j++) {
				close(fd[j][0]);
				close(fd[j][1]);
			}
			if (search_path(cmd_list[i][0]) == 0) {
				if (execv(cmd_list[i][0], cmd_list[i]) < 0) {
					perror(_PROGRAM_NAME ": execute error");
					exit(EXIT_FAILURE);
				}
			} else {
				fprintf(stderr, "%s: %s: command not found\n", _PROGRAM_NAME, cmd_list[i][0]);
				return -1;
			}
		}
	}
	/* right command */
	pid[len - 1] = fork();
	switch (pid[len - 1]) {
	case -1:
		perror(_PROGRAM_NAME ": fork");
		return -1;
	case 0:
		close(0);
		dup(fd[len - 2][0]);
		for (i = 0; i < (len - 1); i++) {
			close(fd[i][0]);
			close(fd[i][1]);
		}
		if (search_path(cmd_list[len - 1][0]) == 0) {
			if (execv(cmd_list[len - 1][0], cmd_list[len - 1]) < 0) {
				perror(_PROGRAM_NAME ": execute error");
				exit(EXIT_FAILURE);
			}
		} else {
			fprintf(stderr, "%s: %s: command not found\n", _PROGRAM_NAME, cmd_list[len - 1][0]);
			return -1;
		}
	}

	/* clean up and wait for children to return */
	for (i = 0; i < (len - 1); i++) {
		close(fd[i][0]);
		close(fd[i][1]);
		free(fd[i]);
	}
	free(fd);
	for (i = 0; i < len; i++) {
		waitpid(pid[i], &(status[i]), 0);
	}
	free(pid);
	free(status);
	for (i = 0; cmd_list[i]; i++) {
		free_strings(cmd_list[i]);
	}
	free(cmd_list);
	free_strings(temp_list);
	free(command);
	return 0;
}

/* check if command is a shell builtin */
int builtin(char *input, char **args)
{
	char buf[BUFFER_SIZE];
	char temp[BUFFER_SIZE];
	char *builtin_list[] = {"exit", "cd", "type", "pipe", "goback", "jobs", "kill", "history", "!", "help", NULL};
	char *builtin_help[] = {
			"quit program",
			"change directory",
			"write a description of command type",
			"pipe commands together",
			"runs jobs in background",
			"display jobs running in background",
			"terminate a background job",
			"display list of recent commands",
			"run a command from list of recents",
			"display this help",
			NULL};
	int update_history = TRUE;
	int i, j, n;

	if (strncmp(args[0], "exit\0", 5) == 0) {
		printf("exit\n");
		exit(0);
	}
	/* print help information */
	else if (strncmp(args[0], "help\0", 5) ==0) {
		printf("%s, David Luco 2014\n", _PROGRAM_NAME);
		for (i = 0; builtin_list[i] && builtin_help[i]; i++) {
			printf("  %-8s - %s\n", builtin_list[i], builtin_help[i]);
		}
	}
	/* change current directory */
	else if (strncmp(args[0], "cd\0", 3) == 0) {
		if (args[1]) {
			/* check for relative or absolute path */
			if (args[1][0] == '/') {
				/* absolute path */
				if (chdir(args[1]) < 0) {
					snprintf(buf, BUFFER_SIZE, "%s: cd: %s", _PROGRAM_NAME, args[1]);
					perror(buf);
				}
			}
			/* relative path */
			else {
				if (getcwd(temp, BUFFER_SIZE) == 0) {
					perror(_PROGRAM_NAME);
					return 0;
				}
				snprintf(buf, BUFFER_SIZE, "%s/%s", temp, args[1]);
				if (chdir(buf) < 0) {
					snprintf(temp, BUFFER_SIZE, "%s: cd: %s", _PROGRAM_NAME, args[1]);
					perror(temp);
				}
			}
		}
		/* no directory specified - go to home directory */ 
		else {
			if (chdir(getenv("HOME")) < 0) {
				snprintf(buf, BUFFER_SIZE, "%s: cd: %s", _PROGRAM_NAME, getenv("HOME"));
				perror(buf);
			}
		}
	}
	/* get type of command entered */
	else if (strncmp(args[0], "type\0", 5) == 0) {
		if (args[1]) {
			strncpy(buf, args[1], strlen(args[1]) + 1);
			if (search_path(buf) == 0) {
				printf("%s is %s\n", args[1], buf);
			} else {
				n = FALSE;
				for (i = 0; builtin_list[i]; i++) {
					if (strncmp(args[1], builtin_list[i], strlen(builtin_list[i]) + 1) == 0) {
						/* command found */
						printf("%s is a shell builtin\n", args[1]);
						n = TRUE;
						break;
					}
				}
				/* command is not a builtin */
				if (n == FALSE) {
					fprintf(stderr, "%s: type: %s: not found\n", _PROGRAM_NAME, args[1]);
				}
			}
		}
	}
	/* pipe commands together */
	else if (strncmp(args[0], "pipe\0", 5) == 0) {
		pipe_command(input);
	}
	/* run command in the background */
	else if (strncmp(args[0], "goback\0", 7) == 0) {
		if (args[1]) {
			if (search_path(args[1]) == 0) {
				if (num_jobs != JOBS_SIZE) {
					execute_command(input, args+1, BACKGROUND);
				} else {
					fprintf(stderr, "%s: goback: maximum number of background jobs runnning\n", _PROGRAM_NAME);
				}
			} else {
				fprintf(stderr, "%s: %s: command not found\n", _PROGRAM_NAME, args[1]);
			}
		} else {
			fprintf(stderr, "%s: goback: missing command argument\n", _PROGRAM_NAME);
		}
	}
	/* list running background processes */
	else if (strncmp(args[0], "jobs\0", 5) == 0) {
		if (num_jobs > 0) {
			for (i = 0, n = 1; i < num_jobs; n++) {
				for (j = 0; j < JOBS_SIZE; j++) {
					if (job_list[j]->id == n) {
						printf("[%d]\t%s\n", n, job_list[j]->command);
						i++;
						break;
					}
				}
			}
		}
	}
	/* kill a background process by id */
	else if (strncmp(args[0], "kill\0", 5) == 0) {
		if (args[1]) {
			n = atoi(args[1]);
			if (n > 0 && n < job_counter) {
				j = FALSE;
				for (i = 0; i < JOBS_SIZE; i++) {
					if (job_list[i]->id == n) {
						kill(job_list[i]->pid, SIGKILL);
						j = TRUE; /* use j to indicate that a job was found */
						break;
					}
				}
				if (j == FALSE) {
					fprintf(stderr, "%s: kill: invalid job id\n", _PROGRAM_NAME);
				}
			} else {
				fprintf(stderr, "%s: kill: invalid job id\n", _PROGRAM_NAME);
			}
		} else {
			fprintf(stderr, "%s: kill: job id not specified\n", _PROGRAM_NAME);
		}
	}
	/* display full history */
	else if (strncmp(args[0], "history\0", 8) == 0) {
		if (strncmp(history[(history_index + 1) % HISTORY_SIZE], "<empty>\0", 8) == 0) {
			/* history has not looped back to top - print history starting from index 0 */
			for (i = 0; i < HISTORY_SIZE; i++) {
				n = (i + 1) % HISTORY_SIZE;
				if (strncmp(history[n], "<empty>\0", 8) != 0) {
					printf("%5d %s\n", i+1, history[n]);
				}
			}
		} else {
			/* history has looped back to top - print history starting from history_index + 1 */
			for (i = 0; i < HISTORY_SIZE; i++) {
				n = ((history_index + i) + 1) % HISTORY_SIZE;
				printf("%5d %s\n", i+1, history[n]);
			}
		}
	}
	/* display list of most recent commands */
	else if (strncmp(args[0], "histlist\0", 9) == 0) {
		for (i = 0; i < 5; i++) {
			printf("%d %s\n", i+1, history[(((history_index - i) % HISTORY_SIZE) + HISTORY_SIZE) % HISTORY_SIZE]);
		}
		update_history = FALSE;
	}
	/* run command n from history */
	else if (strncmp(args[0], "histrun\0", 8) == 0 || strncmp(args[0], "!\0", 2) == 0) {
		if (!args[0][1]) {
			fprintf(stderr, "%s: history number not specified\n", _PROGRAM_NAME);
			return 0;
		}
//		n = atoi(args[1]);
		n = atoi(args[0] + 1);
		if (n < 1 || n > HISTORY_SIZE) {
			fprintf(stderr, "%s: invalid history number\n", _PROGRAM_NAME);
			return 0;	
		}
		i = (((history_index - (n-1)) % HISTORY_SIZE) + HISTORY_SIZE) % HISTORY_SIZE;
		/* set up args with command n from history */
		free_strings(args);
		args = tokenize(history[i], " ");
		printf("%s\n", history[i]);
		/* check if builtin */
		if (builtin(history[i], args) != 0) {
			/* not a shell builtin - try to execute command n */
			if (search_path(args[0]) == 0) {
				execute_command(history[i], args, FOREGROUND);
			} else {
				fprintf(stderr, "%s: %s: command not found\n", _PROGRAM_NAME, args[0]);
			}
			/* re-add command from history - prevents infinite loop of histrun calling itself */
			history_add(history[i]);
		}
		/* only add command from history if not a builtin - builtins will handle it themselves */
		update_history = FALSE;
	}
	/* command is not a shell builtin */
	else {
		return 1;
	}
	/* for commands other than histlist and histrun, update history */
	if (update_history) {
		history_add(input);
	}
	return 0;
}

/* decide if command is a shell builtin or a binary */
int parse(char *input, char **args)
{	
	if (builtin(input, args) != 0) {
		/* not a shell builtin - execute normally */
		if (search_path(args[0]) == 0) {
			execute_command(input, args, FOREGROUND);
		} else {
			fprintf(stderr, "%s: %s: command not found\n", _PROGRAM_NAME, args[0]);
		}
		history_add(input);
	}
	
	return 0;
}

/* write contents of history list to file on exit */
int history_write(void)
{
	FILE *fp;
	int i, j, start_index = 1;
	
	if ((fp = fopen(HISTORY_FILE, "a")) == NULL) {
		perror(_PROGRAM_NAME ": history file");
		return -1;
	}
	
	if (strncmp(history[0], "<empty>\0", 8) != 0) {
		/* history has wrapped around to beginning - set start_index to least recent command in history */
		start_index = (history_index + 1) % HISTORY_SIZE;
	}
	for (i = 0; i < HISTORY_SIZE; i++) {
		j = (start_index + i) % HISTORY_SIZE;
		if (strncmp(history[j], "<empty>\0", 8) != 0) {
			fprintf(fp, "%s\n", history[j]);
		} else {
			break;
		}
	}

	if (fclose(fp) != 0) {
		perror(_PROGRAM_NAME ": history file");
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[], char *envp[])
{
	char buf[BUFFER_SIZE];
	char *line;
	char **args;

	init();

	for (;;) {
		prompt(buf);

		if ((line = readline(buf)) == NULL) {
			printf("exit\n");
			break;
		}
		if (line[0] != 0) {
			add_history(line);
		} else {
			continue;
		}

		args = tokenize(line, " ");
		if (parse(line, args) != 0) {
			break;		
		}
		free(line);
		free_strings(args);
	}
	history_write();
	cleanup();
		
	return 0;
}
