#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int makeargv(const char *s, const char *delimiters, char ***argvp) {
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

int main(int argc, char *argv[]) {
	char delim[] = " \t";
	int i;
	char **myargv;
	int numtokens;
	pid_t pid;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s string\n", argv[0]);
		return -1;
	}
	if ((numtokens = makeargv(argv[1], delim, &myargv)) == -1) {
		fprintf(stderr, "Failed to construct an argument array for %s\n", argv[1]);
		return -1;
	}
	/*printf("The argument array contains:\n");
	for (i = 0; i < numtokens; i++)
		printf("%d:%s\n", i, myargv[i]);*/
	if ((pid = fork()) < 0) {
		perror("\n Error in creating a child\n");
		return -2;
	}
	if (pid == 0) {
		if (execvp(*myargv, myargv) == -1) {
			perror("Fail to run the command or command doesn't exists\n");
			return -3;
		}
	} else {
		wait(NULL);
	}
	return 0;
}
