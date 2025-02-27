#include "util/print_help.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	main(int argc, char *argv[])
{
	char	*command;
	int		p_flag = 0, q_flag = 0, r_flag = 0, s_flag = 0;
	char	*s_value;
	int		opt;

	// 引数がなければ usage を表示
	if (argc < 2)
	{
		print_usage(argv[0]);
		return (EXIT_FAILURE);
	}
	// 最初の引数をコマンドとして判定
	command = argv[1];
	if (strcmp(command, "md5") != 0 && strcmp(command, "sha256") != 0)
	{
		print_invalid_command(command);
		return (EXIT_FAILURE);
	}
	// ft_ssl のコマンドとしては、argv[1] 以降がフラグと入力になる
	// getopt の解析開始位置を 2 に設定
	optind = 2;
	s_value = NULL;
	// 有効なフラグは -p, -q, -r, -s (s は引数付き)
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
	// 解析結果のデバッグ出力（ここは実際のハッシュ計算に置き換える）
	printf("Command: %s\n", command);
	printf("Flags:\n");
	printf("  -p: %s\n", p_flag ? "ON" : "OFF");
	printf("  -q: %s\n", q_flag ? "ON" : "OFF");
	printf("  -r: %s\n", r_flag ? "ON" : "OFF");
	if (s_flag)
		printf("  -s: %s\n", s_value);
	else
		printf("  -s: OFF\n");
	// 残りの引数はファイルまたは文字列として扱う
	if (optind < argc)
	{
		printf("File/other input(s):\n");
		for (int i = optind; i < argc; i++)
		{
			printf("  %s\n", argv[i]);
		}
	}
	else
	{
		printf("No file/string input provided. Reading from STDIN...\n");
	}
	// ここから実際の md5 / sha256 の処理へ繋げる
	return (EXIT_SUCCESS);
}
