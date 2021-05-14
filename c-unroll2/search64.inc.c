
#define INNER64(X, Y)                                                          \
  {                                                                            \
    int sx = X;                                                                \
    int sy = Y;                                                                \
    uint64_t distance = __builtin_popcountll(to_compare ^ (data[sy]));         \
    if (unlikely(distance <= max_dist)) {                                      \
      printf("%" PRIu64 " %d %d\n", distance, sx, sy);                         \
    }                                                                          \
  }

static void __attribute__((noinline)) UNROLL_NAME() {
  for (int x = 0; x < num_data; x++) {
    uint64_t to_compare = data[x];
    int start = x + 1;
    int next_4 =
        min(((start - 1) / UNROLL_FACTOR + 1) * UNROLL_FACTOR, num_data);
    int last_4 = max(num_data & (~(UNROLL_FACTOR - 1)), start);
    for (int y = start; y < next_4; y++) {
      INNER64(x, y);
    }
    for (int y = next_4; y < last_4; y += UNROLL_FACTOR) {
      INNER64(x, y + 0);
      INNER64(x, y + 1);
#if UNROLL_FACTOR > 2
      INNER64(x, y + 2);
      INNER64(x, y + 3);
#if UNROLL_FACTOR > 4
      INNER64(x, y + 4);
      INNER64(x, y + 5);
      INNER64(x, y + 6);
      INNER64(x, y + 7);
#if UNROLL_FACTOR > 8
      INNER64(x, y + 8);
      INNER64(x, y + 9);
      INNER64(x, y + 10);
      INNER64(x, y + 11);
      INNER64(x, y + 12);
      INNER64(x, y + 13);
      INNER64(x, y + 14);
      INNER64(x, y + 15);
#if UNROLL_FACTOR > 16
      INNER64(x, y + 16);
      INNER64(x, y + 17);
      INNER64(x, y + 18);
      INNER64(x, y + 19);
      INNER64(x, y + 20);
      INNER64(x, y + 21);
      INNER64(x, y + 22);
      INNER64(x, y + 23);
      INNER64(x, y + 24);
      INNER64(x, y + 25);
      INNER64(x, y + 26);
      INNER64(x, y + 27);
      INNER64(x, y + 28);
      INNER64(x, y + 29);
      INNER64(x, y + 30);
      INNER64(x, y + 31);
#if UNROLL_FACTOR > 32
      INNER64(x, y + 32);
      INNER64(x, y + 33);
      INNER64(x, y + 34);
      INNER64(x, y + 35);
      INNER64(x, y + 36);
      INNER64(x, y + 37);
      INNER64(x, y + 38);
      INNER64(x, y + 39);
      INNER64(x, y + 40);
      INNER64(x, y + 41);
      INNER64(x, y + 42);
      INNER64(x, y + 43);
      INNER64(x, y + 44);
      INNER64(x, y + 45);
      INNER64(x, y + 46);
      INNER64(x, y + 47);
      INNER64(x, y + 48);
      INNER64(x, y + 49);
      INNER64(x, y + 50);
      INNER64(x, y + 51);
      INNER64(x, y + 52);
      INNER64(x, y + 53);
      INNER64(x, y + 54);
      INNER64(x, y + 55);
      INNER64(x, y + 56);
      INNER64(x, y + 57);
      INNER64(x, y + 58);
      INNER64(x, y + 59);
      INNER64(x, y + 60);
      INNER64(x, y + 61);
      INNER64(x, y + 62);
      INNER64(x, y + 63);
#if UNROLL_FACTOR > 64
      INNER64(x, y + 64);
      INNER64(x, y + 65);
      INNER64(x, y + 66);
      INNER64(x, y + 67);
      INNER64(x, y + 68);
      INNER64(x, y + 69);
      INNER64(x, y + 70);
      INNER64(x, y + 71);
      INNER64(x, y + 72);
      INNER64(x, y + 73);
      INNER64(x, y + 74);
      INNER64(x, y + 75);
      INNER64(x, y + 76);
      INNER64(x, y + 77);
      INNER64(x, y + 78);
      INNER64(x, y + 79);
      INNER64(x, y + 80);
      INNER64(x, y + 81);
      INNER64(x, y + 82);
      INNER64(x, y + 83);
      INNER64(x, y + 84);
      INNER64(x, y + 85);
      INNER64(x, y + 86);
      INNER64(x, y + 87);
      INNER64(x, y + 88);
      INNER64(x, y + 89);
      INNER64(x, y + 90);
      INNER64(x, y + 91);
      INNER64(x, y + 92);
      INNER64(x, y + 93);
      INNER64(x, y + 94);
      INNER64(x, y + 95);
      INNER64(x, y + 96);
      INNER64(x, y + 97);
      INNER64(x, y + 98);
      INNER64(x, y + 99);
      INNER64(x, y + 100);
      INNER64(x, y + 101);
      INNER64(x, y + 102);
      INNER64(x, y + 103);
      INNER64(x, y + 104);
      INNER64(x, y + 105);
      INNER64(x, y + 106);
      INNER64(x, y + 107);
      INNER64(x, y + 108);
      INNER64(x, y + 109);
      INNER64(x, y + 110);
      INNER64(x, y + 111);
      INNER64(x, y + 112);
      INNER64(x, y + 113);
      INNER64(x, y + 114);
      INNER64(x, y + 115);
      INNER64(x, y + 116);
      INNER64(x, y + 117);
      INNER64(x, y + 118);
      INNER64(x, y + 119);
      INNER64(x, y + 120);
      INNER64(x, y + 121);
      INNER64(x, y + 122);
      INNER64(x, y + 123);
      INNER64(x, y + 124);
      INNER64(x, y + 125);
      INNER64(x, y + 126);
      INNER64(x, y + 127);
#endif
#endif
#endif
#endif
#endif
#endif
    }
    if (last_4 != start || next_4 != num_data) {
      for (int y = last_4; y < num_data; y++) {
        INNER64(x, y);
      }
    }
  }
}

#undef NAME
