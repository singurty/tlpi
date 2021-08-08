#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <wordexp.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/times.h>

int main(int argc, char *argv[])
{
	if (argc < 3 || !strcmp(argv[0], "--help")) {
		printf("Usage: %s <directory> <numer of files>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int numFiles = strtol(argv[2], NULL, 10);
	if (numFiles > 999999) {
		printf("maximum number of files is 999999\n");
		exit(EXIT_FAILURE);
	}
	wordexp_t directory;
	wordexp(argv[1], &directory, 0);
	char filename[strlen(*directory.we_wordv) + 9];
	filename[0] = '\0';
	strcat(filename, *directory.we_wordv);
	strcat(filename, "/");
	strcat(filename, "x");
	struct timeval init;
	struct timeval afterCreate;
	gettimeofday(&init, NULL);
	for (int i=0; i<numFiles; i++) {
		srand(time(NULL) + i);
		int num = rand() % 999999;
		char numStr[7];
		sprintf(numStr, "%d", num);
		filename[strlen(*directory.we_wordv)+2] = '\0';
		strcat(filename, numStr);
		int fd = open(filename, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
		if (fd == -1) {
			perror("file create error");
			exit(EXIT_FAILURE);
		}
		puts(filename);
		close(fd);
	}
	gettimeofday(&afterCreate, NULL);
	float interval = (afterCreate.tv_usec - init.tv_usec) * 0.000001;
	printf("time taken: %.03f\n", interval);
}