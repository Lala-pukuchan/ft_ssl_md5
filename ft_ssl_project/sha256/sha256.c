#include "sha256.h"
#include <stdio.h>
#include <string.h>

/* SHA256の変換処理：512ビット(64バイト)のデータブロックごとに内部状態を更新 */
static void	sha256_transform(SHA256_CTX *ctx, const unsigned char data[])
{
	uint32_t a, b, c, d, e, f, g, h, t1, t2, m[64];
	int i, j;
	/* 16個の32ビットワードに変換 */
	for (i = 0, j = 0; i < 16; i++, j += 4)
	{
		m[i] = ((uint32_t)data[j] << 24) | ((uint32_t)data[j
				+ 1] << 16) | ((uint32_t)data[j + 2] << 8) | ((uint32_t)data[j
				+ 3]);
	}
	/* メッセージスケジュールの拡張 */
	for (; i < 64; i++)
	{
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
	}
	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];
	for (i = 0; i < 64; i++)
	{
		t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}
	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

/* SHA256初期化: 内部状態、ビットカウント、バッファを初期化 */
void	SHA256Init(SHA256_CTX *ctx)
{
	ctx->bitcount = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

/* SHA256 Update: 入力データを処理して、64バイト単位のブロックを変換 */
void	SHA256Update(SHA256_CTX *ctx, const unsigned char *data,
		unsigned int len)
{
	unsigned int	i;
	unsigned int	index;
	unsigned int	partLen;

	index = (ctx->bitcount / 8) % 64;
	ctx->bitcount += ((uint64_t)len * 8);
	partLen = 64 - index;
	if (len >= partLen)
	{
		memcpy(&ctx->buffer[index], data, partLen);
		sha256_transform(ctx, ctx->buffer);
		for (i = partLen; i + 63 < len; i += 64)
		{
			sha256_transform(ctx, &data[i]);
		}
		index = 0;
	}
	else
	{
		i = 0;
	}
	memcpy(&ctx->buffer[index], &data[i], len - i);
}

/* SHA256 Final: パディングを追加し、最終的なハッシュ値を生成 */
void	SHA256Final(unsigned char digest[32], SHA256_CTX *ctx)
{
	unsigned char	padding[64] = {0x80};
	unsigned char	bits[8];
	unsigned int	index;
	unsigned int	padLen;

	index = (ctx->bitcount / 8) % 64;
	padLen = (index < 56) ? (56 - index) : (120 - index);
	/* 64ビットのビットカウントをビッグエンディアンで格納 */
	for (int i = 0; i < 8; i++)
	{
		bits[i] = (unsigned char)((ctx->bitcount >> (56 - 8 * i)) & 0xff);
	}
	SHA256Update(ctx, padding, padLen);
	SHA256Update(ctx, bits, 8);
	/* 内部状態をビッグエンディアン形式でバイト列に変換 */
	for (int i = 0; i < 8; i++)
	{
		digest[i * 4] = (unsigned char)((ctx->state[i] >> 24) & 0xff);
		digest[i * 4 + 1] = (unsigned char)((ctx->state[i] >> 16) & 0xff);
		digest[i * 4 + 2] = (unsigned char)((ctx->state[i] >> 8) & 0xff);
		digest[i * 4 + 3] = (unsigned char)(ctx->state[i] & 0xff);
	}
	memset(ctx, 0, sizeof(*ctx));
}

/* sha256_hash: 文字列からSHA256ハッシュを16進数文字列に変換して返す */
void	sha256_hash(const char *data, char *hash, size_t hash_size)
{
	SHA256_CTX		ctx;
	unsigned char	digest[32];

	SHA256Init(&ctx);
	SHA256Update(&ctx, (const unsigned char *)data, strlen(data));
	SHA256Final(digest, &ctx);
	if (hash_size < 65)
		return ; // 出力バッファが小さい場合は何もしない
	for (int i = 0; i < 32; i++)
	{
		sprintf(hash + i * 2, "%02x", digest[i]);
	}
}
