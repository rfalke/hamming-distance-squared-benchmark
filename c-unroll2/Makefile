detected_OS := $(shell uname)
detected_cpu := $(shell uname -m)

clangextra := -march=native

ifeq ($(detected_OS),Darwin)        # Mac OS X
    CC=gcc-11
    ifeq ($(detected_cpu),arm64)
	clangextra := -mcpu=lightning
    endif
endif


COMMON=-Wall -Werror
GCCFLAGS=$(COMMON) -march=native
CLANGFLAGS=$(COMMON) $(clangextra)

all: find-similar-gcc_O2 find-similar-gcc_O3 find-similar-clang_O2 find-similar-clang_O3 find-similar-clang_Ofast

find-similar-gcc_O2: find-similar.c search64.inc.c search256.inc.c
	$(CC) $(GCCFLAGS) -O2 find-similar.c -o $@

find-similar-gcc_O3: find-similar.c search64.inc.c search256.inc.c
	$(CC) $(GCCFLAGS) -O3 find-similar.c -o $@

find-similar-clang_O2: find-similar.c search64.inc.c search256.inc.c
	clang $(CLANGFLAGS) -O2 find-similar.c -o $@

find-similar-clang_O3: find-similar.c search64.inc.c search256.inc.c
	clang $(CLANGFLAGS) -O3 find-similar.c -o $@

find-similar-clang_Ofast: find-similar.c search64.inc.c search256.inc.c
	clang $(CLANGFLAGS) -Ofast find-similar.c -o $@

format:
	clang-format -i find-similar.c
	clang-format -i search64.inc.c
	clang-format -i search256.inc.c

clean:
	rm -f *~ find-similar-gcc* find-similar-clang*
