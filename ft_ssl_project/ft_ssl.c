#include "md5/md5.h"
#include "sha256/sha256.h"
#include "util/print_help.h"
#include "util/read.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* --- 関数ポインタ・ディスパッチ --- */
typedef void	(*hash_func_t)(const char *data, char *hash, size_t hash_size);

typedef struct
{
	const char	*name;
	hash_func_t	func;
}				hash_dispatch_t;

hash_dispatch_t	dispatch_table[] = {{"md5", md5_hash}, {"sha256", sha256_hash},
		{NULL, NULL}};

/* --- メイン処理 --- */
int	main(int argc, char *argv[])
{
	char		*command;
	int			p_flag = 0, q_flag = 0, r_flag = 0, s_flag = 0;
	char		*s_value;
	int			opt;
	char		hash[256];
	char		*stdin_data;
	char		*file_data;
	hash_func_t	hash_func;

	s_value = NULL;
	stdin_data = NULL;
	file_data = NULL;
	hash_func = NULL;
	if (argc < 2)
	{
		print_usage(argv[0]);
		return (EXIT_FAILURE);
	}
	command = argv[1];
	/* 関数ポインタによるディスパッチ */
	for (int i = 0; dispatch_table[i].name != NULL; i++)
	{
		if (strcmp(command, dispatch_table[i].name) == 0)
		{
			hash_func = dispatch_table[i].func;
			break ;
		}
	}
	if (!hash_func)
	{
		print_invalid_command(command);
		return (EXIT_FAILURE);
	}
	/* オプション解析（argv[1]以降） */
	optind = 2; // argv[0]: プログラム名, argv[1]: コマンド
	while ((opt = getopt(argc, argv, "pqrs:")) != -1)
	{
		switch (opt)
		{
		case 'p':
			p_flag = 1;
			break ;
		case 'q':
			q_flag = 1;
			break ;
		case 'r':
			r_flag = 1;
			break ;
		case 's':
			s_flag = 1;
			s_value = optarg;
			break ;
		case '?':
			fprintf(stderr, "ft_ssl: Error: Unknown flag '-%c'\n", optopt);
			return (EXIT_FAILURE);
		}
	}
	/* ① -p フラグ: STDIN を読み込み、入力内容をそのまま出力しつつハッシュ値を計算 */
	if (p_flag)
	{
		stdin_data = read_stdin();
		printf("%s", stdin_data);
		hash_func(stdin_data, hash, sizeof(hash));
		if (!q_flag)
		{
			if (r_flag)
				printf("%s\n", hash);
			else
				printf("(stdin)= %s\n", hash);
		}
		else
		{
			printf("%s\n", hash);
		}
		free(stdin_data);
	}
	/* ② -s フラグ: 文字列引数からハッシュ値を計算 */
	if (s_flag && s_value)
	{
		hash_func(s_value, hash, sizeof(hash));
		if (!q_flag)
		{
			if (r_flag)
				printf("%s \"%s\"\n", hash, s_value);
			else
				printf("%s (\"%s\") = %s\n", command, s_value, hash);
		}
		else
		{
			printf("%s\n", hash);
		}
	}
	/* ③ 残りの引数をファイル名として処理 */
	for (int i = optind; i < argc; i++)
	{
		file_data = read_file(argv[i]);
		if (!file_data)
			continue ;
		hash_func(file_data, hash, sizeof(hash));
		if (!q_flag)
		{
			if (r_flag)
				printf("%s %s\n", hash, argv[i]);
			else
				printf("%s (%s) = %s\n", command, argv[i], hash);
		}
		else
		{
			printf("%s\n", hash);
		}
		free(file_data);
	}
	/* ④ -p, -s 両方もなく、かつファイル指定もない場合は STDIN から読み込み */
	if (!p_flag && !s_flag && optind >= argc)
	{
		stdin_data = read_stdin();
		hash_func(stdin_data, hash, sizeof(hash));
		if (!q_flag)
		{
			if (r_flag)
				printf("%s\n", hash);
			else
				printf("(stdin)= %s\n", hash);
		}
		else
		{
			printf("%s\n", hash);
		}
		free(stdin_data);
	}
	return (EXIT_SUCCESS);
}
