.POSIX:
.SUFFIXES:
CC      = cc
CFLAGS  = -ansi -pedantic -Wall -Wextra -Werror -O3 -g
LDFLAGS =
LDLIBS  =
PREFIX  = /usr/local

sources = src/noisegen.c src/cbuffer.c src/xoshiro.c src/util.c
headers = src/noisegen.h src/cbuffer.h src/xoshiro.h src/util.h
objects = $(sources:.c=.o)

noisegen$(EXE): $(objects)
	$(CC) $(LDFLAGS) -o $@ $(objects) $(LDLIBS)

src/noisegen.o: src/noisegen.c src/noisegen.h
src/cbuffer.o: src/cbuffer.c src/cbuffer.h
src/xoshiro.o: src/xoshiro.c src/xoshiro.h
src/util.o: src/util.c src/util.h

.PHONY: clean
clean:
	rm -f noisegen$(EXE) $(objects)

.PHONY: install
install: noisegen$(EXE)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 755 noisegen$(EXE) $(DESTDIR)$(PREFIX)/bin

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/noisegen$(EXE)

.PHONY: format
format:
	clang-format -i --style Mozilla $(sources) $(headers)

.SUFFIXES: .c .o
.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
