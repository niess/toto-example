# Generic compiler flags.
CC=     gcc
CFLAGS= -O0 -g -Wall -std=c99

# OS dependent flags.
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    SOEXT=  dylib
    LIB=    libtoto.$(SOEXT)
    LD=     $(CC) -dynamiclib -Wl,-install_name,@rpath/$(LIB)
    RPATH=  -Wl,-rpath,@loader_path/../lib
else
    SOEXT=  so
    LIB=    libtoto.$(SOEXT)
    LD=     $(CC) -shared
    RPATH=  '-Wl,-rpath,$$ORIGIN/../lib'
endif

# Version flags.
VERSION_MAJOR= $(shell grep TOTO_VERSION_MAJOR src/toto.h | cut -d' ' -f3)
VERSION_MINOR= $(shell grep TOTO_VERSION_MINOR src/toto.h | cut -d' ' -f3)
VERSION_PATCH= $(shell grep TOTO_VERSION_PATCH src/toto.h | cut -d' ' -f3)

LIB_SHORTNAME= $(LIB).$(VERSION_MAJOR)
LIB_FULLNAME=  $(LIB_SHORTNAME).$(VERSION_MINOR).$(VERSION_PATCH)


# C library compilation.
LIB_CFLAGS= -fPIC $(CFLAGS)

.PHONY: lib
lib: lib/$(LIB_FULLNAME) \
     lib/$(LIB_SHORTNAME) \
     lib/$(LIB)

lib/$(LIB_FULLNAME): src/toto.c src/toto.h | libdir
	$(LD) -o $@ $(LIB_CFLAGS) $^

lib/$(LIB_SHORTNAME): lib/$(LIB_FULLNAME)
	@ln -fs $(LIB_FULLNAME) $@

lib/$(LIB): lib/$(LIB_SHORTNAME)
	@ln -fs $(LIB_SHORTNAME) $@

.PHONY: libdir
libdir:
	@mkdir -p lib


# Python package.
PYTHON=  python3
PACKAGE= wrapper.abi3.$(SOEXT)
OBJS=    src/wrapper.o

.PHONY: package
package: toto/$(PACKAGE) \
         toto/lib/$(LIB) \
         toto/include/toto.h

toto/$(PACKAGE): setup.py src/build-wrapper.py $(OBJS) lib/$(LIB)
	$(PYTHON) setup.py build --build-lib .
	@rm -rf build toto.egg-info

src/%.o: src/%.c src/%.h
	$(CC) $(LIB_CFLAGS) -c -o $@ $<

toto/lib/$(LIB): lib/$(LIB)
	@mkdir -p toto/lib
	@ln -fs ../../$< $@

toto/include/%.h: src/%.h
	@mkdir -p toto/include
	@ln -fs ../../$< $@


# C example(s) compilation.
examples: bin/img

EXAMPLES_CFLAGS= $(CFLAGS) -Isrc
EXAMPLES_LDFLAGS= -L$(PWD)/lib -Wl,-rpath,$(PWD)/lib -ltoto

bin/%: examples/%.c src/toto.h | lib/$(LIB) bindir
	$(CC) $(EXAMPLES_CFLAGS) -o $@ $< $(EXAMPLES_LDFLAGS)

.PHONY: bindir
bindir:
	@mkdir -p bin


# Cleaning.
.PHONY: clean
clean:
	rm -rf bin
	rm -rf build
	rm -rf lib
	rm -f src/*.o
	rm -rf toto/$(PACKAGE) toto/__pycache__ toto/version.py 
