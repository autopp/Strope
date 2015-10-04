CFLAGS=-Wall -pedantic -Werror -fPIC -shared --std=c99
CC=clang-3.6
TARGET=libstrope.so

TEST_CFLAGS=-Ispecc -I. -Lspecc -L. -Wall -Werror -fPIC --std=gnu99
TEST_CC=gcc
TEST_TARGET=strope_spec

VALGRIND=$(shell which valgrind 2> /dev/null)

ifdef VALGRIND
VALGRIND_LIST=valgrind --leak-check=full -q
else
VALGRIND_LIST=
endif

.PHONY=all clean test

all: $(TARGET)

$(TARGET): strope.c strope.h
	$(CC) $(CFLAGS) strope.c -o $@

clean:
	rm -f $(TARGET) $(TEST_TARGET)

test: libspecc all
	@$(TEST_CC) $(TEST_CFLAGS) strope_spec.c -g -lspecc -lstrope -o $(TEST_TARGET) 2>&1
	@env LD_LIBRARY_PATH=$(CURDIR):$(CURDIR)/specc:$${LD_LIBRARY_PATH} $(VALGRIND_LIST) ./$(TEST_TARGET) 2>&1

libspecc:
	@$(MAKE) -C specc --no-print-directory
