#include "util/print_help.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

/* --- 入出力処理 --- */
// STDIN から全データを読み込む
char	*read_stdin(void)
{
	size_t	capacity;
	size_t	size;
	char	*buffer;
	size_t	n;
	char	*newbuf;

	capacity = BUFFER_SIZE;
	size = 0;
	buffer = malloc(capacity);
	if (!buffer)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	while ((n = fread(buffer + size, 1, BUFFER_SIZE, stdin)) > 0)
	{
		size += n;
		if (size + BUFFER_SIZE > capacity)
		{
			capacity *= 2;
			newbuf = realloc(buffer, capacity);
			if (!newbuf)
			{
				free(buffer);
				perror("realloc");
				exit(EXIT_FAILURE);
			}
			buffer = newbuf;
		}
	}
	buffer[size] = '\0';
	return (buffer);
}

// ファイルから全データを読み込む
char	*read_file(const char *filename)
{
	FILE	*fp;
	long	filesize;
	char	*buffer;
	size_t	n;

	fp = fopen(filename, "rb");
	if (!fp)
	{
		fprintf(stderr, "ft_ssl: %s: %s\n", filename, strerror(errno));
		return (NULL);
	}
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	rewind(fp);
	buffer = malloc(filesize + 1);
	if (!buffer)
	{
		perror("malloc");
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	n = fread(buffer, 1, filesize, fp);
	buffer[n] = '\0';
	fclose(fp);
	return (buffer);
}
