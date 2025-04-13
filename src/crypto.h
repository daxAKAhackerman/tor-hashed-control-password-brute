#ifndef __CRYPTO__
#define __CRYPTO__

#define SALT_LEN 8
#define SHA1_DIGEST_LEN 20

void hash_password(char *out, char *password, size_t password_len, char *salt, int count);

void print_hash(char *data);

#endif
