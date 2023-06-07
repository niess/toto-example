LIBNAME= toto
CFLAGS= -O0 -g -std=c99

lib/lib$(LIBNAME).so: src/$(LIBNAME).c src/wrapper.c src/$(LIBNAME).h | lib
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $^

lib:
	mkdir -p lib


.PHONY: clean
clean:
	rm -rf bin lib


examples: bin/img

EXAMPLES_CFLAGS= $(CFLAGS) -Isrc
EXAMPLES_LDFLAGS= -L$(PWD)/lib -Wl,-rpath,$(PWD)/lib -ltoto

bin/%: examples/%.c | bin lib/lib$(LIBNAME).so
	$(CC) $(EXAMPLES_CFLAGS) -o $@ $< $(EXAMPLES_LDFLAGS)

bin:
	mkdir -p bin
