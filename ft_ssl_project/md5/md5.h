#ifndef MD5_H
# define MD5_H

# include <stdint.h>
# include <stdio.h>
# include <string.h>

/* MD5で使用するマクロ定義 */
# define S11 7
# define S12 12
# define S13 17
# define S14 22
# define S21 5
# define S22 9
# define S23 14
# define S24 20
# define S31 4
# define S32 11
# define S33 16
# define S34 23
# define S41 6
# define S42 10
# define S43 15
# define S44 21

# define F(x, y, z) (((x) & (y)) | ((~(x)) & (z)))
# define G(x, y, z) (((x) & (z)) | ((y) & (~(z))))
# define H(x, y, z) ((x) ^ (y) ^ (z))
# define I(x, y, z) ((y) ^ ((x) | (~(z))))
# define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

# define FF(a, b, c, d, x, s, ac)                        \
	{                                                   \
		(a) += F((b), (c), (d)) + (x) + (uint32_t)(ac); \
		(a) = ROTATE_LEFT((a), (s));                    \
		(a) += (b);                                     \
	}

# define GG(a, b, c, d, x, s, ac)                        \
	{                                                   \
		(a) += G((b), (c), (d)) + (x) + (uint32_t)(ac); \
		(a) = ROTATE_LEFT((a), (s));                    \
		(a) += (b);                                     \
	}

# define HH(a, b, c, d, x, s, ac)                        \
	{                                                   \
		(a) += H((b), (c), (d)) + (x) + (uint32_t)(ac); \
		(a) = ROTATE_LEFT((a), (s));                    \
		(a) += (b);                                     \
	}

# define II(a, b, c, d, x, s, ac)                        \
	{                                                   \
		(a) += I((b), (c), (d)) + (x) + (uint32_t)(ac); \
		(a) = ROTATE_LEFT((a), (s));                    \
		(a) += (b);                                     \
	}

/* MD5コンテキスト */
typedef struct
{
	uint32_t state[4];        /* A, B, C, D */
	uint32_t count[2];        /* ビット数 (64bitとして) */
	unsigned char buffer[64]; /* 入力バッファ */
}		MD5_CTX;

void	MD5Init(MD5_CTX *context);
void	MD5Update(MD5_CTX *context, const unsigned char *input,
			unsigned int inputLen);
void	MD5Final(unsigned char digest[16], MD5_CTX *context);
void	md5_hash(const char *data, char *hash, size_t hash_size);

#endif
