#ifdef _WIN32
  #error ONLY *NIX OS‘ ARE SUPPORTED.
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


const size_t MAX_LINE_LEN = 4096;

int readline(int fd, char* buf, size_t bufsize);

_Bool is_empty_line(char* line);

int main(int argc, char** argv) {
	if (argc < 2) {
		char* warn = "Usage: lc FILENAME\n";
		write(fileno(stderr), warn, strlen(warn));
		return -1;
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		char* warn = "Failed to open file.\n";
		write(fileno(stderr), warn, strlen(warn));
		return -1;
	}
	long long count[2] = {0};
	char* buf = (char*) malloc(MAX_LINE_LEN);
	while (1) {
		int n = readline(fd, buf, MAX_LINE_LEN);
		if (n == EOF) {
			break;
		}
		count[is_empty_line(buf)] += 1;
	}
	printf("%lld %lld\n", count[0], count[1]);
	free(buf);
	return 0;
}

int readline(int fd, char* buf, size_t bufsize) {
	_Bool eol = 0;
	_Bool eof = 0;
	char *head = buf;
	while (!eol) {
		if (read(fd, buf, 1) == 0) {
			eof = 1;
			break;
		}
		if (*buf == '\n') {
			*buf = 0;
			break;
		}
		buf += 1;
		if (buf - head > bufsize) {
			return -2;
		}
	}
	if (eof) {
		return EOF;
	}
	return buf - head;
}

_Bool is_empty_line(char* line) {
	while (1) {
		if (*line == '\t' || *line == ' ') {
			line += 1;
		} else if (*line == 0) {
			return 1;
		} else {
			return 0;
		}
	}
}

