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
  fprintf(stderr, "Usage: find-similar <input-file> <max-distance>\n");
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

static void read_data(FILE *data_input) {
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

#define INNER64(X, Y)                                                          \
  {                                                                            \
    int sx = X;                                                                \
    int sy = Y;                                                                \
    uint64_t distance = __builtin_popcountll(to_compare ^ (data[sy]));         \
    if (unlikely(distance <= max_dist)) {                                      \
      printf("%" PRIu64 " %d %d\n", distance, sx, sy);                         \
    }                                                                          \
  }

static void __attribute__((noinline)) search_64() {
  for (int x = 0; x < num_data; x++) {
    uint64_t to_compare = data[x];
    int start = x + 1;
    int next_4 = min(((start - 1) / 4 + 1) * 4, num_data);
    int last_4 = max(num_data & (~3), start);
    for (int y = start; y < next_4; y++) {
      INNER64(x, y);
    }
    for (int y = next_4; y < last_4; y += 4) {
      INNER64(x, y + 0);
      INNER64(x, y + 1);
      INNER64(x, y + 2);
      INNER64(x, y + 3);
    }
    if (last_4 != start || next_4 != num_data) {
      for (int y = last_4; y < num_data; y++) {
        INNER64(x, y);
      }
    }
  }
}

#define INNER256(X, Y)                                                         \
  {                                                                            \
    int sx = X;                                                                \
    int sy = Y;                                                                \
    uint64_t distance =                                                        \
        (__builtin_popcountll(to_compare0 ^ data[4 * sy + 0]) +                \
         __builtin_popcountll(to_compare1 ^ data[4 * sy + 1]) +                \
         __builtin_popcountll(to_compare2 ^ data[4 * sy + 2]) +                \
         __builtin_popcountll(to_compare3 ^ data[4 * sy + 3]));                \
    if (unlikely(distance <= max_dist)) {                                      \
      printf("%" PRIu64 " %d %d\n", distance, sx, sy);                         \
    }                                                                          \
  }

static void __attribute__((noinline)) search_256() {
  for (int x = 0; x < num_data; x++) {
    uint64_t to_compare0 = data[4 * x + 0];
    uint64_t to_compare1 = data[4 * x + 1];
    uint64_t to_compare2 = data[4 * x + 2];
    uint64_t to_compare3 = data[4 * x + 3];

    int start = x + 1;
    int next_4 = min(((start - 1) / 4 + 1) * 4, num_data);
    int last_4 = max(num_data & (~3), start);
    for (int y = start; y < next_4; y++) {
      INNER256(x, y);
    }
    for (int y = next_4; y < last_4; y += 4) {
      INNER256(x, y + 0);
      INNER256(x, y + 1);
      INNER256(x, y + 2);
      INNER256(x, y + 3);
    }
    if (last_4 != start || next_4 != num_data) {

      for (int y = last_4; y < num_data; y++) {
        INNER256(x, y);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    usage("wrong number of arguments");
  }

  FILE *data_input = fopen(argv[1], "r");
  assert(data_input);
  if (sscanf(argv[2], "%d", &max_dist) != 1) {
    usage("max-dist is not an int");
  }

  read_data(data_input);
  if (size == 64) {
    search_64();
  } else {
    search_256();
  }
  return 0;
}
