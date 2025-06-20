#include "../include/crypto.h"
#include <openssl/evp.h>
#include <string.h>

void hash_password(unsigned char *out, char *password, size_t password_len,
                   char *salt, int count) {
  EVP_MD_CTX *hash_ctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex2(hash_ctx, EVP_sha1(), NULL);

  int data_len = password_len + SALT_LEN;
  char *data = malloc(data_len);
  memcpy(data, salt, SALT_LEN);
  memcpy(data + SALT_LEN, password, password_len);

  while (count) {
    if (count >= data_len) {
      EVP_DigestUpdate(hash_ctx, (void *)data, data_len);
      count -= data_len;
    } else {
      EVP_DigestUpdate(hash_ctx, (void *)data, count);
      count = 0;
    }
  }

  EVP_DigestFinal_ex(hash_ctx, out, NULL);

  EVP_MD_CTX_free(hash_ctx);
  free(data);
}
