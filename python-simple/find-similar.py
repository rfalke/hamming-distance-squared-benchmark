#!/usr/bin/env python3

import sys


def read_data(inputfn):
    lines = open(inputfn).readlines()
    size, num_entries = [int(x) for x in lines[0].split(" ")]
    entries = []
    for line in lines[1:]:
        entry, id = line.split(" ")
        assert len(entry) == size / 4, [entry, len(entry), size]
        assert int(id) == len(entries)
        entries.append(int(entry, 16))
    assert len(entries) == num_entries
    return entries


def search(entries, max_dist):
    for i in range(len(entries)):
        e1 = entries[i]
        for j in range(i + 1, len(entries)):
            e2 = entries[j]
            dist = bin(e1 ^ e2).count("1")
            if dist <= max_dist:
                print("%d %d %d" % (dist, i, j))


def main():
    inputfn = sys.argv[1]
    max_dist = int(sys.argv[2])
    entries = read_data(inputfn)
    search(entries, max_dist)


if __name__ == "__main__":
    main()
