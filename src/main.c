#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/file.h>
#include "crypto.h"
#include "argparser.h"

void sigquit_handler(int sig)
{
    _exit(1);
}

int main(int argc, char **argv)
{
    Args args;
    char target[THASH_ARG_BIN_LEN];
    char thash[SHA1_DIGEST_LEN];
    char salt[SALT_LEN];
    char count_byte;
    char line[256];
    char hash_out[SHA1_DIGEST_LEN];
    int pid_status;

    if (validate_args(&args, argc, argv) != 0)
    {
        return 1;
    }

    args.target_arg += 3; // Get rid of the prefix

    // Load the hash as hex
    for (int i = 0; i < THASH_ARG_BIN_LEN; i++)
    {
        sscanf(args.target_arg + 2 * i, "%2hhx", &target[i]);
    }

    memcpy(thash, &target[9], SHA1_DIGEST_LEN); // Load the target hash
    memcpy(salt, target, SALT_LEN);             // Load the target salt
    memcpy(&count_byte, &target[SALT_LEN], 1);  // Load the iteration count byte

    int count = ((int)16 + (count_byte & 15)) << ((count_byte >> 4) + EXPBIAS); // Convert the iteration byte to a number as described in rfc4880

    // Open the file
    FILE *file = fopen(args.wordlist_arg, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "[!] Unable to open file\n");
        return 1;
    }
    int file_descriptor = fileno(file);

    signal(SIGQUIT, sigquit_handler); // Register signal required to stop children
    for (int i = 0; i < args.num_processes_arg; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            while (1)
            {
                flock(file_descriptor, LOCK_EX);
                if (!fgets(line, sizeof(line), file))
                {
                    break;
                }
                flock(file_descriptor, LOCK_UN);
                hash_password(hash_out, line, strlen(line) - 1, salt, count);
                if (memcmp(thash, hash_out, SHA1_DIGEST_LEN) == 0)
                {
                    printf("[+] Password found: %s", line);
                    _exit(0);
                }
            }
            _exit(1);
        }
    }

    signal(SIGQUIT, SIG_IGN); // Ensure the parent cannot get stopped by the SIGQUIT signal
    while (waitpid(0, &pid_status, 0) != -1)
    {
        if (WEXITSTATUS(pid_status) == 0)
        {
            kill(0, SIGQUIT);
            fclose(file);
            return 0;
        }
    }

    printf("[!] Password was not found in word list\n");
    fclose(file);
    return 1;
}
