import hashlib


def generate(name, num_entries, size):
    entries = []
    hashobj = hashlib.sha256()
    hashobj.update("%d %d" % (num_entries, size))
    for i in range(num_entries):
        hashobj.update(str(i))
        entries.append(hashobj.hexdigest()[:size / 4])
    assert len(set(entries)) == num_entries
    entries.sort()
    f = open("sample-data/%d_%s.input" % (size, name), "w")
    f.write("%d %d\n" % (size, num_entries))
    for i in range(num_entries):
        f.write("%s %d\n" % (entries[i], i))
    f.close()


def main():
    for size in [64, 256]:
        generate("1k", 1000, size)
        generate("10k", 10000, size)
        generate("100k", 100000, size)
        generate("1m", 1000000, size)


if __name__ == "__main__":
    main()
