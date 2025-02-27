#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 4096

// ダミーのハッシュ計算関数（実際はMD5またはSHA-256の計算を行う）
void compute_hash(const char *data, char *hash, size_t hash_size) {
    // ここでは例として "dummyhash" を返す
    snprintf(hash, hash_size, "dummyhash");
}

// STDINから全データを読み込む関数（動的に確保）
char *read_stdin(void) {
    size_t capacity = BUFFER_SIZE;
    size_t size = 0;
    char *buffer = malloc(capacity);
    if (!buffer) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    size_t n;
    while ((n = fread(buffer + size, 1, BUFFER_SIZE, stdin)) > 0) {
        size += n;
        if (size + BUFFER_SIZE > capacity) {
            capacity *= 2;
            char *newbuf = realloc(buffer, capacity);
            if (!newbuf) {
                free(buffer);
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            buffer = newbuf;
        }
    }
    buffer[size] = '\0';
    return buffer;
}

// ファイルから全データを読み込む関数
char *read_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "ft_ssl: %s: %s\n", filename, strerror(errno));
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);
    char *buffer = malloc(filesize + 1);
    if (!buffer) {
        perror("malloc");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    size_t n = fread(buffer, 1, filesize, fp);
    buffer[n] = '\0';
    fclose(fp);
    return buffer;
}

int main(int argc, char *argv[]) {
    // 引数チェックとコマンド判定
    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    char *command = argv[1];
    if (strcmp(command, "md5") != 0 && strcmp(command, "sha256") != 0) {
        print_invalid_command(command);
        return EXIT_FAILURE;
    }

    // オプション解析（argv[1]以降）
    int p_flag = 0, q_flag = 0, r_flag = 0, s_flag = 0;
    char *s_value = NULL;
    optind = 2; // argv[0]はプログラム名、argv[1]はコマンドなので、解析はargv[2]から
    int opt;
    while ((opt = getopt(argc, argv, "pqrs:")) != -1) {
        switch (opt) {
            case 'p':
                p_flag = 1;
                break;
            case 'q':
                q_flag = 1;
                break;
            case 'r':
                r_flag = 1;
                break;
            case 's':
                s_flag = 1;
                s_value = optarg;
                break;
            case '?':
                fprintf(stderr, "ft_ssl: Error: Unknown flag '-%c'\n", optopt);
                return EXIT_FAILURE;
        }
    }

    char hash[256];

    /* ① -p フラグ: STDINを読み込み、入力内容をそのまま出力しつつ、ハッシュ値を計算して表示する */
    if (p_flag) {
        char *stdin_data = read_stdin();
        // STDINの内容をそのままエコー
        printf("%s", stdin_data);
        compute_hash(stdin_data, hash, sizeof(hash));
        if (!q_flag) {
            if (r_flag)
                printf("%s\n", hash);          // 逆出力: ハッシュ値のみまたは付加情報を後ろに
            else
                printf("(stdin)= %s\n", hash);   // 通常の出力フォーマット
        } else {
            printf("%s\n", hash);
        }
        free(stdin_data);
    }

    /* ② -s フラグ: 文字列引数からハッシュ値を計算して表示する */
    if (s_flag && s_value) {
        compute_hash(s_value, hash, sizeof(hash));
        if (!q_flag) {
            if (r_flag)
                printf("%s \"%s\"\n", hash, s_value);
            else
                printf("%s (\"%s\") = %s\n", command, s_value, hash);
        } else {
            printf("%s\n", hash);
        }
    }

    /* ③ 残りの引数をファイル名とみなして、各ファイルの内容からハッシュ値を計算して表示する */
    for (int i = optind; i < argc; i++) {
        char *file_data = read_file(argv[i]);
        if (!file_data)
            continue; // エラー時はスキップ
        compute_hash(file_data, hash, sizeof(hash));
        if (!q_flag) {
            if (r_flag)
                printf("%s %s\n", hash, argv[i]);
            else
                printf("%s (%s) = %s\n", command, argv[i], hash);
        } else {
            printf("%s\n", hash);
        }
        free(file_data);
    }

    /* ④ もし -p, -s 両方もなく、かつファイル指定もない場合は STDIN から読み込む */
    if (!p_flag && !s_flag && optind >= argc) {
        char *stdin_data = read_stdin();
        compute_hash(stdin_data, hash, sizeof(hash));
        if (!q_flag) {
            if (r_flag)
                printf("%s\n", hash);
            else
                printf("(stdin)= %s\n", hash);
        } else {
            printf("%s\n", hash);
        }
        free(stdin_data);
    }

    return EXIT_SUCCESS;
}
