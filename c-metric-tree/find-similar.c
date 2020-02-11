/*
 Code based on https://github.com/depp/metric-tree-demo
 as an answer to https://stackoverflow.com/questions/6389841/efficiently-find-binary-strings-with-low-hamming-distance-in-large-set
*/

/* Metric tree sample implementation.

   This generates a bunch of pseudorandom 32-bit integers, inserts
   them into an index, and queries the index for points within a
   certain distance of the given point.

   That is,

   Let S = { N pseudorandom 32-bit integers }
   Let d(x,y) be the (base-2) Hamming distance between x and y
   Let q(x,r) = { y in S : d(x,y) <= r }

   There are three implementations in here which can be selected at runtime.

   "bk" is a BK-Tree.  Each internal node has a center point, and each
   child node contains a set of all points a certain distance away
   from the center.

   "vp" is a VP-Tree.  Each internal node has a center point and two
   children.  The "near" child contains all points contained in a
   closed ball of a certain radius around the center, and the "far"
   node contains all other points.

   "linear" is a linear search.

   The tree implementations use a linear search for leaf nodes.  The
   maximum number of points in a leaf node is configurable at runtime,
   but 1000 is a good number.  If the number is low, say 1, then the
   memory usage of the tree implementations will skyrocket to
   unreasonable levels: more than 24 bytes per element.

   Note that VP trees are slightly faster than BK trees for this
   problem, and neither tree implementation significantly outperforms
   linear search (that is, by a factor of two or more) for r > 6.  */

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VERBOSE_CONSTRUCTION 0

__attribute__((malloc)) static void *xmalloc(size_t sz) {
  void *p;
  if (!sz)
    return NULL;
  p = malloc(sz);
  if (!p)
    err(1, "malloc");
  return p;
}

typedef struct {
  uint64_t hash;
  int file_id;
} bkey64_t;
enum { MAX_DISTANCE64 = 64 };

static inline unsigned distance(bkey64_t x, bkey64_t y) {
  return __builtin_popcountll(x.hash ^ y.hash);
}

static int max_linear;
static bkey64_t *data;
static int num_data;
static int size;
static int max_dist;

/* VP-tree ==================== */

struct vptree {
  union {
    struct {
      /* Closed ball (d = threshold is included) */
      bkey64_t vantage;
      struct vptree *near;
      struct vptree *far;
      unsigned short threshold;
    } tree;
    struct {
      bkey64_t *keys;
      unsigned count;
    } linear;
  } data;
  unsigned short is_linear;
};

static struct vptree *mktree_vp(const bkey64_t *restrict keys, size_t n,
                                int indent) {
  struct vptree *root;
  assert(n > 0);

  /* Build root */
  root = xmalloc(sizeof(*root));

  if (n <= max_linear || n <= 1) {
    root->is_linear = 1;
    bkey64_t *tmp = xmalloc(sizeof(*tmp) * n);
    memcpy(tmp, keys, sizeof(*tmp) * n);
    root->data.linear.count = n;
    root->data.linear.keys = tmp;
    if (VERBOSE_CONSTRUCTION) {
      printf("%*s", indent * 4, "");
      printf("linear node of size=%zu\n", n);
    }
    return root;
  } else {
    bkey64_t rootkey = keys[0];

    root->is_linear = 0;
    root->data.tree.threshold = 0;
    root->data.tree.vantage = rootkey;
    root->data.tree.near = NULL;
    root->data.tree.far = NULL;

    n -= 1;
    keys += 1;
    if (!n) {
      return root;
    } else {
      size_t dcnt[MAX_DISTANCE64 + 1];
      size_t i, all_so_far;
      bkey64_t *tmp;
      unsigned k;
      size_t median, nnear, nfar, inear, ifar;

      /* Count keys inside the given ball */
      for (i = 0; i <= MAX_DISTANCE64; ++i) {
        dcnt[i] = 0;
      }
      for (i = 0; i < n; ++i) {
        dcnt[distance(rootkey, keys[i])]++;
      }
      all_so_far = 0;
      for (i = 0; i <= MAX_DISTANCE64; ++i) {
        dcnt[i] = (all_so_far += dcnt[i]);
      }
      assert(all_so_far == n);

      median = dcnt[0] + (n - dcnt[0]) / 2;
      for (k = 1; k <= MAX_DISTANCE64; ++k) {
        if (dcnt[k] > median) {
          break;
        }
      }
      if (k != 1 && median - dcnt[k - 1] <= dcnt[k] - median) {
        k--;
      }
      nnear = dcnt[k] - dcnt[0];
      nfar = n - dcnt[k];
      if (VERBOSE_CONSTRUCTION) {
        printf("%*s", indent * 4, "");
        printf("keys: %zu; near: %zu; far: %zu; k=%u\n", n, nnear, nfar, k);
      }

      /* Sort keys into near and far sets */
      tmp = xmalloc(sizeof(*tmp) * (nnear + nfar));
      inear = 0;
      ifar = nnear;
      for (i = 0; i < n; ++i) {
        if (keys[i].file_id == rootkey.file_id) {
          continue;
        }
        if (distance(rootkey, keys[i]) <= k) {
          tmp[inear++] = keys[i];
        } else {
          tmp[ifar++] = keys[i];
        }
      }
      assert(inear == nnear);
      assert(ifar == nnear + nfar);

      root->data.tree.threshold = k;
      if (nnear) {
        root->data.tree.near = mktree_vp(tmp, nnear, indent + 1);
      }
      if (nfar) {
        root->data.tree.far = mktree_vp(tmp + nnear, nfar, indent + 1);
      }

      free(tmp);
      return root;
    }
  }
}

static void report(unsigned dist, bkey64_t key1, bkey64_t key2) {
  if (key1.file_id >= key2.file_id) {
    // do not output
    return;
  }

  printf("%d %d %d\n", dist, key1.file_id, key2.file_id);
}

static void query_vp(struct vptree *restrict root, bkey64_t ref) {

  if (root->is_linear) {
    const bkey64_t *restrict keys = root->data.linear.keys;
    unsigned i, n = root->data.linear.count;
    for (i = 0; i < n; ++i) {
      unsigned dist = distance(ref, keys[i]);
      if (dist <= max_dist) {
        report(dist, ref, keys[i]);
      }
    }
  } else {
    /* We are trying to find x that satisfy d(ref,x) <= maxd
       By triangle inequality, we know: d(root,x) <= d(root,ref) + d(ref,x)
       By algebra: d(root,x) - d(root,ref) <= d(ref,x)
       By transitivity: d(root,x) - d(root,ref) <= maxd
       By algebra: d(root,x) <= maxd + d(root,ref) */
    unsigned d = distance(root->data.tree.vantage, ref);
    unsigned thr = root->data.tree.threshold;
    if (d <= max_dist + thr) {
      if (root->data.tree.near) {
        query_vp(root->data.tree.near, ref);
      }
      if (d <= max_dist) {
        report(d, ref, root->data.tree.vantage);
      }
    }
    if (d + max_dist > thr && root->data.tree.far) {
      query_vp(root->data.tree.far, ref);
    }
  }
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

/* Main ==================== */

static void usage(const char *msg) {
  fprintf(stderr,
          "Usage: find-similar <input-file> <max-distance> <max_linear>\n");
  fprintf(stderr, "Problem: %s\n", msg);
  exit(1);
}

static void read_data(FILE *data_input) {
  char line_buffer[4096], *tmp;
  int line;

  tmp = fgets(line_buffer, 4096, data_input);
  assert(tmp == line_buffer);

  if (sscanf(line_buffer, "%d %d", &size, &num_data) != 2) {
    usage("wrong number of parameters in first line");
  }
  data = malloc(sizeof(*data) * num_data);
  assert(size == 64);

  for (line = 0; line < num_data; line++) {
    tmp = fgets(line_buffer, 4096, data_input);
    assert(tmp == line_buffer);
    assert(line_buffer[size / 4] == ' ');
    line_buffer[size / 4] = '\0';
    uint64_t val = simple_strtoul(line_buffer);
    data[line].hash = val;
    data[line].file_id = line;
  }
}

static void search_64() {
  struct vptree *root = mktree_vp(data, num_data, 0);

  for (int i = 0; i < num_data; i++) {
    query_vp(root, data[i]);
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
  if (max_dist >= MAX_DISTANCE64 || max_dist <= 0) {
    fprintf(stderr, "Distance should be in the range 1..%d\n", MAX_DISTANCE64);
    return 1;
  }

  if (sscanf(argv[3], "%d", &max_linear) != 1) {
    usage("max_linear is not an int");
  }

  read_data(data_input);
  if (size == 64) {
    search_64();
  } else {
    assert(0);
    //    search_256();
  }
  return 0;
}