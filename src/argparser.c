#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "argparser.h"

int validate_args(Args *args, int argc, char **argv)
{
    int opt;

    char *usage_string = "Usage: %s -t target_hash -w wordlist [-n number_of_processes]\n";

    // Required arguments
    int t_found, w_found;
    t_found = w_found = 0;

    // Default values
    args->num_processes_arg = 1;

    while ((opt = getopt(argc, argv, "t:w:n:")) != -1)
    {
        switch (opt)
        {
        case 't':
            char *hash_prefix = HASH_PREFIX;
            if (strlen(optarg) != THASH_ARG_LEN || memcmp(optarg, hash_prefix, 3) != 0)
            {
                fprintf(stderr, "[!] Incorrect hash format\n");
                return 1;
            }
            args->target_arg = optarg;
            t_found = 1;
            break;
        case 'w':
            args->wordlist_arg = optarg;
            w_found = 1;
            break;
        case 'n':
            args->num_processes_arg = atoi(optarg);
            break;
        default:
            fprintf(stderr, usage_string, argv[0]);
            return 1;
        }
    }

    if (!(t_found && w_found))
    {
        fprintf(stderr, "Missing argument\n");
        fprintf(stderr, usage_string, argv[0]);
        return 1;
    }

    return 0;
}
