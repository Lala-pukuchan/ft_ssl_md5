#include "print_help.h"
#include <stdio.h>

void	print_usage(const char *progname)
{
	fprintf(stderr, "usage: %s command [flags] [file/string]\n", progname);
}

void	print_invalid_command(const char *cmd)
{
	fprintf(stderr, "ft_ssl: Error: '%s' is an invalid command.\n", cmd);
	fprintf(stderr, "Commands:\n");
	fprintf(stderr, "md5\n");
	fprintf(stderr, "sha256\n");
	fprintf(stderr, "Flags:\n");
	fprintf(stderr, "-p -q -r -s\n");
}