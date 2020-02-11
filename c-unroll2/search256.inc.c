
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
      printf("%ld %d %d\n", distance, sx, sy);                                 \
    }                                                                          \
  }

static void __attribute__((noinline)) UNROLL_NAME() {
  for (int x = 0; x < num_data; x++) {
    uint64_t to_compare0 = data[4 * x + 0];
    uint64_t to_compare1 = data[4 * x + 1];
    uint64_t to_compare2 = data[4 * x + 2];
    uint64_t to_compare3 = data[4 * x + 3];
    int start = x + 1;
    int next_4 =
        min(((start - 1) / UNROLL_FACTOR + 1) * UNROLL_FACTOR, num_data);
    int last_4 = max(num_data & (~(UNROLL_FACTOR - 1)), start);
    for (int y = start; y < next_4; y++) {
      INNER256(x, y);
    }
    for (int y = next_4; y < last_4; y += UNROLL_FACTOR) {
      INNER256(x, y + 0);
      INNER256(x, y + 1);
#if UNROLL_FACTOR > 2
      INNER256(x, y + 2);
      INNER256(x, y + 3);
#if UNROLL_FACTOR > 4
      INNER256(x, y + 4);
      INNER256(x, y + 5);
      INNER256(x, y + 6);
      INNER256(x, y + 7);
#if UNROLL_FACTOR > 8
      INNER256(x, y + 8);
      INNER256(x, y + 9);
      INNER256(x, y + 10);
      INNER256(x, y + 11);
      INNER256(x, y + 12);
      INNER256(x, y + 13);
      INNER256(x, y + 14);
      INNER256(x, y + 15);
#if UNROLL_FACTOR > 16
      INNER256(x, y + 16);
      INNER256(x, y + 17);
      INNER256(x, y + 18);
      INNER256(x, y + 19);
      INNER256(x, y + 20);
      INNER256(x, y + 21);
      INNER256(x, y + 22);
      INNER256(x, y + 23);
      INNER256(x, y + 24);
      INNER256(x, y + 25);
      INNER256(x, y + 26);
      INNER256(x, y + 27);
      INNER256(x, y + 28);
      INNER256(x, y + 29);
      INNER256(x, y + 30);
      INNER256(x, y + 31);
#if UNROLL_FACTOR > 32
      INNER256(x, y + 32);
      INNER256(x, y + 33);
      INNER256(x, y + 34);
      INNER256(x, y + 35);
      INNER256(x, y + 36);
      INNER256(x, y + 37);
      INNER256(x, y + 38);
      INNER256(x, y + 39);
      INNER256(x, y + 40);
      INNER256(x, y + 41);
      INNER256(x, y + 42);
      INNER256(x, y + 43);
      INNER256(x, y + 44);
      INNER256(x, y + 45);
      INNER256(x, y + 46);
      INNER256(x, y + 47);
      INNER256(x, y + 48);
      INNER256(x, y + 49);
      INNER256(x, y + 50);
      INNER256(x, y + 51);
      INNER256(x, y + 52);
      INNER256(x, y + 53);
      INNER256(x, y + 54);
      INNER256(x, y + 55);
      INNER256(x, y + 56);
      INNER256(x, y + 57);
      INNER256(x, y + 58);
      INNER256(x, y + 59);
      INNER256(x, y + 60);
      INNER256(x, y + 61);
      INNER256(x, y + 62);
      INNER256(x, y + 63);
#endif
#endif
#endif
#endif
#endif
    }
    if (last_4 != start || next_4 != num_data) {
      for (int y = last_4; y < num_data; y++) {
        INNER256(x, y);
      }
    }
  }
}

#undef NAME