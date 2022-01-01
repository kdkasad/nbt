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
SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(subst .c,.o,$(SRCS))

TBIN = test
TESTDIR = tests
TSRCS = $(wildcard $(TESTDIR)/*.c)
TOBJS = $(filter-out $(SRCDIR)/main.o,$(OBJS)) $(subst .c,.o,$(TSRCS))

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
clean: clean-tests
	@echo Removing objects...
	@rm -f $(OBJS)
	@echo Removing binaries...
	@rm -f $(BIN)

.PHONY: clean-tests
clean-tests:
	@echo Removing test objects...
	@rm -f $(filter $(TESTDIR)/%,$(TOBJS))
	@echo Removing test binaries...
	@rm -f $(TBIN)

$(BIN): $(OBJS)
	@echo "LINK	$@"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.c.o:
	@echo "CC	$<"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(TBIN): $(TOBJS)
	@echo "LINK	$@"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

tests/%.o: tests/%.c
	@echo "CC	$<"
	@$(CC) $(CPPFLAGS) -iquote $(SRCDIR) $(CFLAGS) -c -o $@ $^
