#ifndef __CRYPTO__
#define __CRYPTO__

#define SALT_LEN 8
#define SHA1_DIGEST_LEN 20
#define EXPBIAS 6

void hash_password(char *out, char *password, size_t password_len, char *salt, int count);

#endif
