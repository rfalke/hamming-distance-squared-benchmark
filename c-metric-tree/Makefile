COMMON=-march=native -Wall -Werror

all: find-similar-gcc_O2 find-similar-gcc_O3 find-similar-clang_O2 find-similar-clang_O3 find-similar-clang_Ofast

find-similar-gcc_O2: find-similar.c
	gcc $(COMMON) -O2 find-similar.c -o $@

find-similar-gcc_O3: find-similar.c
	gcc $(COMMON) -O3 find-similar.c -o $@

find-similar-clang_O2: find-similar.c
	clang $(COMMON) -O2 find-similar.c -o $@
	
find-similar-clang_O3: find-similar.c
	clang $(COMMON) -O3 find-similar.c -o $@

find-similar-clang_Ofast: find-similar.c
	clang $(COMMON) -Ofast find-similar.c -o $@

format:
	clang-format -i find-similar.c 

clean:
	rm -f *~ find-similar-gcc* find-similar-clang*
