
srcdir=./src/
builddir=./build/

CC=gcc
CFLAGS=-std=gnu99 -Wall

test:
	$(CC) -I. -I$(srcdir) -lm $(CFLAGS) $(srcdir)ssdb_geom.c $(srcdir)test.c -o $(builddir)ssdb_test
