#
# Copyright (c) 2021-2022 Kian Kasad
#
# This file is distributed under a modified BSD license. See the provided
# LICENSE file for details.
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#



# Set debug profile's *FLAGS
CFLAGS_debug = -g -O0
LDFLAGS_debug =

# Set release profile's *FLAGS
CFLAGS_release = -Os
LDFLAGS_release = -s

# If BUILD environment variable is defined, use the given build profile.
# Otherwise, use 'release' profile only if *FLAGS are not already set in the
# environment.
ifdef BUILD
CFLAGS += $(CFLAGS_$(BUILD))
LDFLAGS += $(LDFLAGS_$(BUILD))
else
CFLAGS ?= $(CFLAGS_release)
LDFLAGS ?= $(LDFLAGS_release)
endif

CFLAGS += -Wall -pedantic
CPPFLAGS ?=
CPPFLAGS += -std=c99

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
