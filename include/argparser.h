#ifndef __ARGPARSER__
#define __ARGPARSER__

#define HASH_PREFIX "16:"
#define THASH_ARG_LEN 61
#define THASH_ARG_BIN_LEN 29

typedef struct Args {
  char *target_arg;
  char *wordlist_arg;
  int num_processes_arg;
} Args;

int validate_args(Args *args, int argc, char *argv[]);

#endif
