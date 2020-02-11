#!/usr/bin/env bash

function testone () {
    timing=$(mktemp)
    cmd="$1"
    name="$2"
    maxdist="$3"
    extra="$4"

    inputfn="sample-data/${name}.input"
    expectedfn="sample-data/${name}.${maxdist}.output"

    echo "=== cmd='$cmd' name=$name maxdist=$maxdist extra=$extra"
    for i in $(seq 5)
    do
        /usr/bin/time -f "%e" -o "$timing" $cmd "$inputfn" "$maxdist" $extra >/dev/null
        a=$(cat "$timing")
        echo "$extra $a" >>timing2
    done
    rm "$timing"
}


function testc () {
    name="$1"
    maxdist=$2

    rm -f timing2
    for factor in 2 4 8 16 32 64 128
    do
        testone "c-unroll2/find-similar-gcc_O2" "$name" "$maxdist" $factor
    done
    mv timing2 "timing-results/$3"
}

testc "64_100k" 13 "timing_unroll2_64"
testc "256_100k" 87 "timing_unroll2_256"
