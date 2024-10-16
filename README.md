# Tor HashedControlPassword Brute

Small Python script to execute a dictionary attack against a Tor HashedControlPassword value

## Installation

```bash
$ python -m venv .venv
$ source .venv/bin/activate
$ pip install -r requirements.txt
```

## Usage

```
usage: main.py [-h] -t HASH -w WORDLIST_FILE

Execute a dictionary attack on the value of the HashedControlPassword line of a torrc file

options:
  -h, --help            show this help message and exit
  -t HASH, --target HASH
                        The hash to crack. Looks like 16:46CAB5E3FA5F7BF4606D132C27DA06F77B574F7C14B9D75657A74F59FB
  -w WORDLIST_FILE, --wordlist WORDLIST_FILE
                        File containing the list of passwords to try

Hack the planet!
```
