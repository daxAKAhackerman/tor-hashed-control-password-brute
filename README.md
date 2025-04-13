# Tor HashedControlPassword Brute

C program to execute a dictionary attack against a Tor HashedControlPassword value

## Installation

Building requires `make`, `gcc` and `libssl-dev >= 3`. 

```bash
$ make
```

## Usage

```
Usage: bin/tor-hashed-control-password-brute -t target_hash -w wordlist [-n number_of_processes]

  -t      The hash to crack. Looks like 16:46CAB5E3FA5F7BF4606D132C27DA06F77B574F7C14B9D75657A74F59FB
  -w      File containing the list of passwords to try
  -n      The number of processes to spawn. Tune this to your number of logical processors for optimal speed. Defaults to 1
```
