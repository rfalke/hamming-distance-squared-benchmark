# Hamming distance squared benchmark

How fast can you compute the hamming distances between all elements of a large input list?

# Background: Perceptual Hashing

[Perceptual Hashing](https://en.wikipedia.org/wiki/Perceptual_hashing) constructs a hash for a media content (e.g. an image file). The computed hash (called phash) can than compared to other phashs for example by counting the number of different bits. If this number is small a similar media file is found. For counting the different bits the hamming distance can be used: `different_bits = hamming_distance(phash1 XOR phash2)`. 

# The Task

Given a list of 64bit or 256bit phash values compute all pairs which have a hamming distance less or equal than a given maximal distance.

# Results

All times are wall times. CPU is a [AMD Ryzen 3 1200 Quad-Core Processor](https://en.wikichip.org/wiki/amd/ryzen_3/1200).

## 64bit input

Implementation                     |      1k       |       10k     |    100k       |     1m
------------                       | ------------- |-------------  | ------------- | -------------
python-simple                      | 0.37s         | 35.25s        |               | |
c-[VP metric tree](https://en.wikipedia.org/wiki/Vantage-point_tree) (maxlinear=64)    | 0.00s         | 0.13s         | 13.72s        | |
java-simple                        | 0.11s         | 0.29s         | 12.85s        | |
c-simple                           | 0.00s         | 0.03s         | 3.29s         | 373.93s
c-unrolled (4 times)               | 0.00s         | 0.02s         | 2.46s         | 306.17s
c-unrolled (128 times)             | 0.00s         | 0.02s         | 1.86s         | 274.33s

The 256bit input variants are not shown since they usually have a running time of just 4 times higher.

## Observations

- The metric tree looked promising for finding all related phashs for *one* phash but isn't good for find all pairs of phash.
- Not surprising: C is faster than Java which is faster than Python
- Not shown: gcc generates faster code than clang
- The overhead (starting and reading the input of 100k lines) takes 0.01sec in c
- The running times for larger unrolled code varies a lot. Reason unknown.
- The different `-Ox` flags to the c compiler don't make a difference. Passing `-march=native` is important.
- For 100k input there are (n+1)*(n/2) = 5 billion comparisons (`popcnt`) to be made. If we assume one `popcnt` instruction per clock cycle and a clock speed of 3400 Mhz the best run time (ignoring other instructions) is (5/3.4) = 1.47s. 
- Going from 100k to 1m should increase the running time by a factor of 100. The measured increase is larger (120-150). The reason may be that the data doesn't fit into some cache anymore. 

## Ideas to improve speed

- Multiple threads. The problem can be easily executed in parallel after reading the input. Will Go be faster here?
- Different data structure. The VP metric tree wasn't so successful but maybe there are other better suited data structures.
- For 1m and larger input sizes: [Loop Tiling](https://en.wikipedia.org/wiki/Loop_nest_optimization)
- Working with probabilities using [Locality-sensitive hashing for Hamming distances](https://en.wikipedia.org/wiki/Locality-sensitive_hashing#Bit_sampling_for_Hamming_distance)
- [This](https://www.agner.org/optimize/blog/read.php?i=853#853) suggests that a CPU core can execute 4 `popcnt` per clock cycle. Why is this not visible?
- Using a SIMD implementation ([a paper](https://arxiv.org/pdf/1611.07612.pdf), [some code](https://github.com/WojciechMula/sse-popcount)) is not so easy because we have to xor with all the data first but maybe this can be made faster. 

# Benchmark Details

The input file starts with a single line containing the bit size and the number of phash values following. Each further line contain the phash (in hex without `0x`) a space and than the phash index. The phash index starts with 0 and increments for each line by one. You can assume that the phashs in the input file are increasing and are unique.
 
```
64 1000
009602e768ac4df2 0
00e87c214cffdd70 1
0119dc54102acf2e 2
....
```

The program takes the input file name and the max distance as a parameter. It may take additional parameters for tuning. The software writes the found pairs to stdout.

The output consists of pairs: first the distance, than the phash indices of the two involved phashs. The left phash index has to be lower than the right phash index. The order of the output lines does not matter. Each found phash pair should only be written once.


# Data Details

These aren't real phash but were generated using sha256. The input sizes are factors of 10. The max distance for each bit size and input size is chosen so that a few hundreds output lines are produced.
