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
    for i in $(seq 3)
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
    for maxlin in 256 512 1024 4096 10000 100000
    do
        testone "c-metric-tree/find-similar-gcc_O2" "$name" "$maxdist" $maxlin
    done
    mv timing2 timing-results/timing_metric_max_linear
}

testc "64_100k" 13
