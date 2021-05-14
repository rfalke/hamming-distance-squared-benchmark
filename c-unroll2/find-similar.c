#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

static uint64_t *data;
static int num_data;
static int size;
static int max_dist;
static int unroll_factor;

static int min(int x, int y) {
  if (x < y)
    return x;
  return y;
}

static int max(int x, int y) {
  if (x > y)
    return x;
  return y;
}

#define unlikely(x) __builtin_expect(!!(x), 0)

static void usage(const char *msg) {
  fprintf(stderr,
          "Usage: find-similar <input-file> <max-distance> <unroll-factor>\n");
  fprintf(stderr, "Problem: %s\n", msg);
  exit(1);
}

static inline uint64_t simple_strtoul(const char *cp) {
  const unsigned int base = 16;
  uint64_t result = 0;
  int i = 0;

  for (i = 0; i < 16; i++) {
    uint64_t value;

    assert(isxdigit(*cp));
    value = isdigit(*cp) ? *cp - '0' : toupper(*cp) - 'A' + 10;
    assert(value < base);
    result = result * base + value;
    cp++;
  }
  return result;
}

static void __attribute__((noinline)) read_data(FILE *data_input) {
  char line_buffer[4096], *tmp;
  int line;

  tmp = fgets(line_buffer, 4096, data_input);
  assert(tmp == line_buffer);

  if (sscanf(line_buffer, "%d %d", &size, &num_data) != 2) {
    usage("wrong number of parameters in first line");
  }
  data = malloc(size / 8 * num_data);
  int uint64_factor = size / 64;

  for (line = 0; line < num_data; line++) {
    tmp = fgets(line_buffer, 4096, data_input);
    assert(tmp == line_buffer);
    assert(line_buffer[size / 4] == ' ');
    line_buffer[size / 4] = '\0';
    for (int i = 0; i < uint64_factor; i++) {
      uint64_t val = simple_strtoul(line_buffer + 16 * i);
      data[uint64_factor * line + i] = val;
    }
  }
}

#define UNROLL_FACTOR 2
#define UNROLL_NAME search_64_2
#include "search64.inc.c"
#undef UNROLL_NAME
#define UNROLL_NAME search_256_2
#include "search256.inc.c"
#undef UNROLL_NAME
#undef UNROLL_FACTOR

#define UNROLL_FACTOR 4
#define UNROLL_NAME search_64_4
#include "search64.inc.c"
#undef UNROLL_NAME
#define UNROLL_NAME search_256_4
#include "search256.inc.c"
#undef UNROLL_NAME
#undef UNROLL_FACTOR

#define UNROLL_FACTOR 8
#define UNROLL_NAME search_64_8
#include "search64.inc.c"
#undef UNROLL_NAME
#define UNROLL_NAME search_256_8
#include "search256.inc.c"
#undef UNROLL_NAME
#undef UNROLL_FACTOR

#define UNROLL_FACTOR 16
#define UNROLL_NAME search_64_16
#include "search64.inc.c"
#undef UNROLL_NAME
#define UNROLL_NAME search_256_16
#include "search256.inc.c"
#undef UNROLL_NAME
#undef UNROLL_FACTOR

#define UNROLL_FACTOR 32
#define UNROLL_NAME search_64_32
#include "search64.inc.c"
#undef UNROLL_NAME
#define UNROLL_NAME search_256_32
#include "search256.inc.c"
#undef UNROLL_NAME
#undef UNROLL_FACTOR

#define UNROLL_FACTOR 64
#define UNROLL_NAME search_64_64
#include "search64.inc.c"
#undef UNROLL_NAME
#define UNROLL_NAME search_256_64
#include "search256.inc.c"
#undef UNROLL_NAME
#undef UNROLL_FACTOR

#define UNROLL_FACTOR 128
#define UNROLL_NAME search_64_128
#include "search64.inc.c"
#undef UNROLL_NAME
#define UNROLL_NAME search_256_128
#include "search256.inc.c"
#undef UNROLL_NAME
#undef UNROLL_FACTOR


static void __attribute__((noinline)) search_64() {
  if (unroll_factor == 2) {
    search_64_2();
  } else if (unroll_factor == 4) {
    search_64_4();
  } else if (unroll_factor == 8) {
    search_64_8();
  } else if (unroll_factor == 16) {
    search_64_16();
  } else if (unroll_factor == 32) {
    search_64_32();
  } else if (unroll_factor == 64) {
    search_64_64();
  } else if (unroll_factor == 128) {
    search_64_128();
  } else {
    usage("Unknown unroll factor");
  }
}

static void __attribute__((noinline)) search_256() {
  if (unroll_factor == 2) {
    search_256_2();
  } else if (unroll_factor == 4) {
    search_256_4();
  } else if (unroll_factor == 8) {
    search_256_8();
  } else if (unroll_factor == 16) {
    search_256_16();
  } else if (unroll_factor == 32) {
    search_256_32();
  } else if (unroll_factor == 64) {
    search_256_64();
  } else if (unroll_factor == 128) {
    search_256_128();
  } else {
    usage("Unknown unroll factor");
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    usage("wrong number of arguments");
  }

  FILE *data_input = fopen(argv[1], "r");
  assert(data_input);
  if (sscanf(argv[2], "%d", &max_dist) != 1) {
    usage("max-dist is not an int");
  }
  if (sscanf(argv[3], "%d", &unroll_factor) != 1) {
    usage("unroll-factor is not an int");
  }

  read_data(data_input);
  if (size == 64) {
    search_64();
  } else {
    search_256();
  }
  return 0;
}
