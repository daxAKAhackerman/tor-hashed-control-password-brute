SHELL = /bin/bash
CC := gcc
CFLAGS := -lcrypto
PROGNAME := $(shell basename $(CURDIR))
SRC_FOLDER := src
BIN_FOLDER := bin

.PHONY: default clean

default: build

build: $(PROGNAME)

$(PROGNAME):
	mkdir $(BIN_FOLDER) > /dev/null 2>&1 || true
	cd $(SRC_FOLDER) && $(CC) *.c $(CFLAGS) -o ../$(BIN_FOLDER)/$(PROGNAME)

clean:
	rm -rf $(BIN_FOLDER)
