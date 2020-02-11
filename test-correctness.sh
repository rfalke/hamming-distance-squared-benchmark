#!/usr/bin/env bash

function testone () {
    timing=$(mktemp)
    cmd="$1"
    name="$2"
    maxdist="$3"
    extra="$4"

    inputfn="sample-data/${name}.input"
    expectedfn="sample-data/${name}.${maxdist}.output"

    echo "=== cmd='$cmd' name=$name maxdist=$maxdist"
    /usr/bin/time -f "wall=%es user=%Us" -o "$timing" $cmd "$inputfn" "$maxdist" $extra | sort -n >out
    if test -f "$expectedfn"; then
        if cmp -s "$expectedfn" out; then
            t=$(cat "$timing")
            echo "  pass $t"
            rm out
        else
            diff -u "$expectedfn" out
            exit 1
       fi
    else
        echo "The file $expectedfn is missing. Will use output"
        mv out $expectedfn
    fi
    rm "$timing"
}

function testpython () {
    testone "python python-simple/find-similar.py" "$1" "$2"
}

function testc () {
    testone "c-simple/find-similar-gcc_O3" "$1" "$2"
    testone "c-unroll/find-similar-gcc_O3" "$1" "$2"
    testone "c-unroll2/find-similar-gcc_O3" "$1" "$2" 128
    if [[ $1 == "64_"* ]]; then
        testone "c-metric-tree/find-similar-gcc_O3" "$1" "$2" 64
    fi
}

function testjava () {
    testone "java -cp java-simple FindSimilar " "$1" "$2"
}

#### Java
if true; then
    testjava 64_test_01 1
    testjava 64_test_01 2
    testjava 64_test_01 3
    testjava 256_test_01 1
    testjava 256_test_01 2
    testjava 256_test_01 3

    testjava 64_1k 15
    testjava 256_1k 100
    testjava 64_10k 15
    testjava 256_10k 98
    testjava 64_100k 13
    testjava 256_100k 87
#    testjava 64_1m 10
#    testjava 256_1m 100
fi

#### C
if true; then
    testc 64_test_01 1
    testc 64_test_01 2
    testc 64_test_01 3
    testc 256_test_01 1
    testc 256_test_01 2
    testc 256_test_01 3

    testc 64_1k 15
    testc 256_1k 100
    testc 64_10k 15
    testc 256_10k 98
    testc 64_100k 13
    testc 256_100k 87
#    testc 64_1m 10
#    testc 256_1m 83
fi

#### Python
if true; then
    testpython 64_test_01 1
    testpython 64_test_01 2
    testpython 64_test_01 3
    testpython 256_test_01 1
    testpython 256_test_01 2
    testpython 256_test_01 3
fi

if true; then
    testpython 64_1k 15
    testpython 64_10k 15

    testpython 256_1k 100
    testpython 256_10k 98
fi
