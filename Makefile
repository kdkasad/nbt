CFLAGS ?= -g -O0
CFLAGS += -Wall -pedantic

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

