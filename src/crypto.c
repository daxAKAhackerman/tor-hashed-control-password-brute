#include <openssl/evp.h>
#include <string.h>
#include "crypto.h"


void hash_password(char *out, char *password, size_t password_len, char *salt, int count)
{
    EVP_MD_CTX *hash_ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex2(hash_ctx, EVP_sha1(), NULL);

    int data_len = password_len + SALT_LEN;
    char *data = malloc(data_len);
    memcpy(data, salt, SALT_LEN);
    memcpy(data + SALT_LEN, password, password_len);

    while (count)
    {
        if (count >= data_len)
        {
            EVP_DigestUpdate(hash_ctx, (void *)data, data_len);
            count -= data_len;
        }
        else
        {
            EVP_DigestUpdate(hash_ctx, (void *)data, count);
            count = 0;
        }
    }

    EVP_DigestFinal_ex(hash_ctx, out, NULL);

    EVP_MD_CTX_free(hash_ctx);
    free(data);
}

void print_hash(char *data)
{
    int i;

    printf("SHA-1 digest: ");
    for (i = 0; i < SHA1_DIGEST_LEN; i++)
        printf("%hhx", data[i]);
    printf("\n");
}
