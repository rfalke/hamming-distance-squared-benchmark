#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static uint64_t *data;
static int num_data;
static int size;
static int max_dist;
static int num_threads;

static void usage(const char *msg) {
  fprintf(stderr,
          "Usage: find-similar <input-file> <max-distance> <threads>\n");
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

static void *search_64(void *argp) {
  int *p = argp;
  int thread_id = *p;
  for (int x = thread_id; x < num_data; x += num_threads) {
    uint64_t to_compare = data[x];
    for (int y = x + 1; y < num_data; y++) {
      uint64_t distance = __builtin_popcountll(to_compare ^ data[y]);

      if (distance <= max_dist) {
        printf("%" PRIu64 " %d %d\n", distance, x, y);
      }
    }
  }
  return 0;
}

static void *search_256(void *argp) {
  int *p = argp;
  int thread_id = *p;
  for (int x = thread_id; x < num_data; x += num_threads) {
    uint64_t to_compare0 = data[4 * x + 0];
    uint64_t to_compare1 = data[4 * x + 1];
    uint64_t to_compare2 = data[4 * x + 2];
    uint64_t to_compare3 = data[4 * x + 3];
    for (int y = x + 1; y < num_data; y++) {
      uint64_t distance = (__builtin_popcountll(to_compare0 ^ data[4 * y + 0]) +
                           __builtin_popcountll(to_compare1 ^ data[4 * y + 1]) +
                           __builtin_popcountll(to_compare2 ^ data[4 * y + 2]) +
                           __builtin_popcountll(to_compare3 ^ data[4 * y + 3]));

      if (distance <= max_dist) {
        printf("%" PRIu64 " %d %d\n", distance, x, y);
      }
    }
  }
  return 0;
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
  if (sscanf(argv[3], "%d", &num_threads) != 1) {
    usage("threads is not an int");
  }

  read_data(data_input);

  pthread_t threads[num_threads];
  int thread_ids[num_threads];
  for (int i = 0; i < num_threads; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, (size == 64 ? search_64 : search_256),
                   &thread_ids[i]);
  }
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}
