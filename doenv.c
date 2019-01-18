#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
extern char **environ;
int main(int argc, char *argv[]) {
	int opt;
	int i;
	if (argc == 1) {
		for (int i = 0; environ[i] != NULL; i++) {
			printf("%s\n", environ[i]);
		}
		return 0;
	}
	if (argv[1][0] != '-') {
		system(argv[1]);
	}
	while((opt = getopt(argc, argv, "i:")) != -1) {
		switch(opt) {
			case 'i':
				for (i = optind - 1; i < argc; i++) {
					if (!putenv(argv[i])) {
						printf("Success\n");
					}
					else
						printf("Usage %s [:utility:] [-i name1=value1 name2=value2 ...]\n",
							argv[0]);
				}
				optind = i - 1;
				break;
			default:
				printf("%s: no %c such option found!!\n", argv[0], opt);
				break;
		}
	}
	return 0;
}
