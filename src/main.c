#define _POSIX_C_SOURCE 200809L

#include "../include/argparser.h"
#include "../include/color.h"
#include "../include/crypto.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>

void sigquit_handler(int sig) {
  (void)sig;
  _exit(1);
}

int main(int argc, char **argv) {
  Args args;
  unsigned char target[THASH_ARG_BIN_LEN];
  char thash[SHA1_DIGEST_LEN];
  char salt[SALT_LEN];
  char count_byte;
  char line[256];
  unsigned char hash_out[SHA1_DIGEST_LEN];
  int pid_status;

  if (validate_args(&args, argc, argv) != 0) {
    return 1;
  }

  args.target_arg += 3; // Get rid of the prefix

  // Load the hash as hex
  for (int i = 0; i < THASH_ARG_BIN_LEN; i++) {
    sscanf(args.target_arg + 2 * i, "%2hhx", &target[i]);
  }

  memcpy(thash, &target[9], SHA1_DIGEST_LEN); // Load the target hash
  memcpy(salt, target, SALT_LEN);             // Load the target salt
  memcpy(&count_byte, &target[SALT_LEN], 1);  // Load the iteration count byte

  int count =
      ((int)16 + (count_byte & 15))
      << ((count_byte >> 4) + EXPBIAS); // Convert the iteration byte to a
                                        // number as described in rfc4880

  // Open the file
  FILE *file = fopen(args.wordlist_arg, "rb");
  if (file == NULL) {
    fprintf(stderr, COLOR_BOLD_RED "[!] Unable to open file\n" COLOR_RESET);
    return 1;
  }
  int file_descriptor = fileno(file);

  printf(COLOR_BOLD_YELLOW "[-] Running dictionary attack...\n" COLOR_RESET);
  signal(SIGQUIT, sigquit_handler); // Register signal required to stop children
  for (int i = 0; i < args.num_processes_arg; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      while (1) {
        flock(file_descriptor, LOCK_EX);
        if (!fgets(line, sizeof(line), file)) {
          break;
        }
        flock(file_descriptor, LOCK_UN);
        hash_password(hash_out, line, strlen(line) - 1, salt, count);
        if (memcmp(thash, hash_out, SHA1_DIGEST_LEN) == 0) {
          printf(COLOR_BOLD_GREEN "[+] Password found: " COLOR_BOLD_BLUE
                                  "%s" COLOR_RESET,
                 line);
          _exit(0);
        }
      }
      _exit(1);
    }
  }

  signal(SIGQUIT,
         SIG_IGN); // Ensure the parent cannot get stopped by the SIGQUIT signal
  while (waitpid(0, &pid_status, 0) != -1) {
    if (WEXITSTATUS(pid_status) == 0) {
      kill(0, SIGQUIT);
      fclose(file);
      return 0;
    }
  }

  printf(COLOR_BOLD_RED
         "[!] Password was not found in word list\n" COLOR_RESET);
  fclose(file);
  return 1;
}
