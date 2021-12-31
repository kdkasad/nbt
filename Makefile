#
# Copyright (c) 2021 Kian Kasad
#
# This file is distributed under a modified BSD license. See the provided
# LICENSE file for details.
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

CFLAGS ?= -g -O0
CFLAGS += -D_DEFAULT_SOURCE=1 -Wall -pedantic

BIN  = nbt
SRCS = $(wildcard src/*.c)
OBJS = $(subst .c,.o,$(SRCS))

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	@echo Removing objects...
	@rm -f $(OBJS)
	@echo Removing binaries...
	@rm -f $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

