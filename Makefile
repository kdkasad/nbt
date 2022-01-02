#
# Copyright (c) 2021-2022 Kian Kasad
#
# This file is distributed under a modified BSD license. See the provided
# LICENSE file for details.
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

CFLAGS ?= -g -O0
CFLAGS += -Wall -pedantic
CPPFLAGS ?=
CPPFLAGS += -D_DEFAULT_SOURCE=1

BIN  = nbt
SRCS = $(wildcard src/*.c)
OBJS = $(subst .c,.o,$(SRCS))

COPYRIGHT_YEAR = 2021-2022
COPYRIGHT_NAME = Kian Kasad
CPPFLAGS += -DCOPYRIGHT_YEAR="\"$(COPYRIGHT_YEAR)\"" -DCOPYRIGHT_NAME="\"$(COPYRIGHT_NAME)\""

.PHONY: all
all: $(BIN)

.PHONY: print-flags
print-flags:
	@echo "CFLAGS:   $(CFLAGS)"
	@echo "CPPFLAGS: $(CPPFLAGS)"
	@echo "LDFLAGS:  $(LDFLAGS)"

.PHONY: clean
clean:
	@echo Removing objects...
	@rm -f $(OBJS)
	@echo Removing binaries...
	@rm -f $(BIN)

$(BIN): $(OBJS)
	@echo "LINK	$@"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.c.o:
	@echo "CC	$<"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
