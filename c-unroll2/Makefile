detected_OS := $(shell uname)

ifeq ($(detected_OS),Darwin)        # Mac OS X
    CC=gcc-11
endif
COMMON=-march=native -Wall -Werror

all: find-similar-gcc_O2 find-similar-gcc_O3 find-similar-clang_O2 find-similar-clang_O3 find-similar-clang_Ofast

find-similar-gcc_O2: find-similar.c search64.inc.c search256.inc.c
	$(CC) $(COMMON) -O2 find-similar.c -o $@

find-similar-gcc_O3: find-similar.c search64.inc.c search256.inc.c
	$(CC) $(COMMON) -O3 find-similar.c -o $@

find-similar-clang_O2: find-similar.c search64.inc.c search256.inc.c
	clang $(COMMON) -O2 find-similar.c -o $@

find-similar-clang_O3: find-similar.c search64.inc.c search256.inc.c
	clang $(COMMON) -O3 find-similar.c -o $@

find-similar-clang_Ofast: find-similar.c search64.inc.c search256.inc.c
	clang $(COMMON) -Ofast find-similar.c -o $@

format:
	clang-format -i find-similar.c
	clang-format -i search64.inc.c
	clang-format -i search256.inc.c

clean:
	rm -f *~ find-similar-gcc* find-similar-clang*
