#ifndef _MD5_H_
#define _MD5_H_

// Pràctica 1 de Sistemes Operatius
// llibreria
// md5.h -- from /usr/include/sys/md5.h, FreeBSD 6.2
// 12 NOV 2007 -- 20:27

#define MD5_BLOCK_LENGTH		64
#define MD5_DIGEST_LENGTH		16
#define LENGTH				16	// <=== afegit
#define MD5_DIGEST_STRING_LENGTH	(MD5_DIGEST_LENGTH * 2 + 1)

#include <sys/cdefs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* MD5 context. */
typedef struct MD5Context
{
  uint32_t state[4];		/* state (ABCD) */
  uint32_t count[2];		/* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];	/* input buffer */
} MD5_CTX;

void MD5Init (MD5_CTX *);
void MD5Update (MD5_CTX *, const unsigned char *, unsigned int);
void MD5Final (unsigned char[16], MD5_CTX *);
char *MD5End (MD5_CTX *, char *);
char *MD5File (const char *, char *);
char *MD5FileChunk (const char *, char *, off_t, off_t);
char *MD5Data (const void *, unsigned int, char *);

#endif
