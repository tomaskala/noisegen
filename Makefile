.POSIX:
.SUFFIXES:
CC      = cc
CFLAGS  = -ansi -pedantic -Wall -Wextra -Werror -Og -g
LDFLAGS =
LDLIBS  =
PREFIX  = /usr/local

sources = src/noisegen.c src/cbuffer.c src/xoshiro.c src/util.c
headers = src/cbuffer.h src/xoshiro.h src/util.h
objects = $(sources:.c=.o)

noisegen: $(objects)
	$(CC) $(LDFLAGS) -o $@ $(objects) $(LDLIBS)

src/noisegen.o: src/noisegen.c
src/cbuffer.o: src/cbuffer.c src/cbuffer.h
src/xoshiro.o: src/xoshiro.c src/xoshiro.h
src/util.o: src/util.c src/util.h

.PHONY: clean
clean:
	rm -f noisegen $(objects)

.PHONY: install
install: noisegen
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 755 noisegen $(DESTDIR)$(PREFIX)/bin

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/noisegen

.PHONY: format
format:
	clang-format -i --style LLVM $(sources) $(headers)

.SUFFIXES: .c .o
.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
