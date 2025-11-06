CC = gcc
CFLAGS = -Wall -Wextra

EMPTY_TARGET = empty
EMPTY_TRACE = empty.trace
PART1_TARGET = part1
PART1_TRACE = part1.trace

all: $(EMPTY_TRACE) $(PART1_TRACE)

verify: $(EMPTY_TRACE) $(PART1_TRACE)
	@echo "System calls in empty.trace:"
	@wc -l $(EMPTY_TRACE)
	@echo "System calls in part1.trace (should be 5 more):"
	@wc -l $(PART1_TRACE)

$(EMPTY_TARGET): empty.c
	$(CC) $(CFLAGS) -o $@ $<

$(EMPTY_TRACE): $(EMPTY_TARGET)
	strace -o $@ ./$<

$(PART1_TARGET): part1.c
	$(CC) $(CFLAGS) -o $@ $<

$(PART1_TRACE): $(PART1_TARGET)
	strace -o $@ ./$<

clean:
	rm -f $(EMPTY_TARGET) $(PART1_TARGET) $(EMPTY_TRACE) $(PART1_TRACE)

.PHONY: all clean verify init
