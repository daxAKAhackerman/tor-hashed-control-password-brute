SHELL = /bin/bash
CC := gcc
CFLAGS := -lcrypto -Wall -Werror -Wextra -pedantic -std=c2x
PROGNAME := $(shell basename $(CURDIR))
SRC_FOLDER := src
BIN_FOLDER := bin

.PHONY: default build clean

default: build

build: $(PROGNAME)

debug: CFLAGS += -g
debug: $(PROGNAME)

$(PROGNAME):
	mkdir $(BIN_FOLDER) > /dev/null 2>&1 || true
	cd $(SRC_FOLDER) && $(CC) *.c $(CFLAGS) -o ../$(BIN_FOLDER)/$(PROGNAME)

clean:
	rm -rf $(BIN_FOLDER)
