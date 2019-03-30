#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

#define SADS_BUFFSIZE_MAX 1000
char *SADS_builtin[] = {"cd", "clear", "exit"};

typedef enum {
	SADS_START,
	SADS_RESUME,
	SADS_END
}status_s;

int SADS_total_func () {
	return (sizeof (SADS_builtin) / sizeof (char *));
}

status_s SADS_clear (char **args) {
	system ("clear");
	return SADS_RESUME;
}

status_s SADS_cd (char **args) {
	if (args[0] == NULL) {
		printf ("Usage cd [dir name]\n");
	} else {
		if (chdir (args[1]) != 0) {
			perror ("Error in changing the directory\n");
		}
	}
	return SADS_RESUME;
}
status_s SADS_exit (char **args) {
	printf ("Exiting SADS shell\n");
	return SADS_END;
}

status_s (*SADS_func[]) (char **args)= {
	SADS_cd,
	SADS_clear,
	SADS_exit
};

status_s SADS_other_prog (char **args) {
	pid_t pid;
	pid = fork();
	int status;
	if (pid == -1) {
		perror ("Error in forking the process\n");
	}
	
	if (pid == 0) {
		execvp (args[0], args);
		perror("SADS error command not found\n");
	} else  {
		 wait(NULL);
	}
	return SADS_RESUME;
}

status_s SADS_exec (char **args) {
	if (args[0] == NULL) {
		return SADS_RESUME;
	} else {
		for (int i = 0; i < SADS_total_func (); i++) {
			if (strcmp (SADS_builtin[i], args[0]) == 0)
				return SADS_func[i](args);
		}
	}
	return SADS_other_prog (args);
}

int SADS_makeargv(const char *s,  char ***argvp) {
	const char *delimiters = " \n\t";
	char *snew = (char *)malloc(sizeof(char) * strlen(s) + 1);
	int t, i;
	strcpy(snew, s);
	if (strtok(snew, delimiters) == NULL)
		return -1;
	for (t = 1; strtok(NULL, delimiters) != NULL; t++);
	*argvp = (char **)malloc(sizeof(char **) * t + 1);
	strcpy(snew, s);
	**argvp = strtok(snew, delimiters);
	for (i = 1; i < t; i++)
		*(*argvp + i) = strtok(NULL, delimiters);
	*(*argvp + i) = NULL;
	return t;
}

int SADS_get_line (char **line) {
	int count = 0;
	int c;
	int size = SADS_BUFFSIZE_MAX;
	*line = (char *)malloc(size * sizeof(char));
	if (*line == NULL) {
		perror ("Error in SADS\n");
		exit (-1);
	}
	while (1) {
		c = getchar();
		if (c == EOF || c == '\n') {
			*(*line + count) = '\0';
			return count;
		} else {
			*(*line + count) = c;
			count++;
		}
		
		if (count > size) {
			size += SADS_BUFFSIZE_MAX;
			*line = (char *)realloc (*line, size);
			if (*line == NULL) {
				perror ("Error in SADS\n");
				exit (-1);
			}
		}
	}
	return count;
}

int main (int argc, char **argv) {
	char *line;
	char **args;
	status_s status = SADS_START;
	do {
		printf ("SADS>");
		SADS_get_line (&line);
		SADS_makeargv (line, &args);
		status = SADS_exec (args);
		//printf ("%s\n", line);
		/*for (int i = 0; args[i] != NULL; i++) {
			printf ("%s\n", args[i]);
		}*/
		free (line);
		free (args);
	} while(status != SADS_END);
}
