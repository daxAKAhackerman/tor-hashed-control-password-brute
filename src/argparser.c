#include "../include/argparser.h"
#include "../include/color.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validate_args(Args *args, int argc, char **argv) {
  int opt;

  char *usage_string =
      "Usage: %s -t target_hash -w wordlist [-n number_of_processes]\n";
  char *usage_string_details =
      "  -t      The hash to crack. Looks like "
      "16:46CAB5E3FA5F7BF4606D132C27DA06F77B574F7C14B9D75657A74F59FB\n"
      "  -w      File containing the list of passwords to try\n"
      "  -n      The number of processes to spawn. Tune this to your number of "
      "logical processors for optimal speed. Defaults to 1\n";
  char *hash_prefix = HASH_PREFIX;

  // Required arguments
  int t_found, w_found;
  t_found = w_found = 0;

  // Default values
  args->num_processes_arg = 1;

  while ((opt = getopt(argc, argv, "t:w:n:h")) != -1) {
    switch (opt) {
    case 't':
      if (strlen(optarg) != THASH_ARG_LEN ||
          memcmp(optarg, hash_prefix, 3) != 0) {
        fprintf(stderr,
                COLOR_BOLD_RED "[!] Incorrect hash format\n" COLOR_RESET);
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
    case 'h':
      printf(usage_string, argv[0]);
      printf("\n");
      printf("%s", usage_string_details);
      return 1;
    default:
      fprintf(stderr, usage_string, argv[0]);
      return 1;
    }
  }

  if (!(t_found && w_found)) {
    fprintf(stderr, COLOR_BOLD_RED "[!] Missing argument\n" COLOR_RESET);
    fprintf(stderr, usage_string, argv[0]);
    return 1;
  }

  return 0;
}
