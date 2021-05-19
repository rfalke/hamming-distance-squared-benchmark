#!/usr/bin/env bash

set -e

function mytime ()
{
    if type -p gtime > /dev/null; then
        gtime "$@";
    else
        /usr/bin/time "$@";
    fi
}

function testone () {
    timing=$(mktemp)
    cmd="$1"
    name="$2"
    maxdist="$3"
    extra="$4"

    inputfn="sample-data/${name}.input"

    echo "=== cmd='$cmd' name=$name maxdist=$maxdist extra=$extra"
    for i in $(seq 5)
    do
        mytime -f "%e" -o "$timing" $cmd "$inputfn" "$maxdist" $extra >/dev/null
        a=$(cat "$timing")
        echo "$extra $a" >>timing2
    done
    rm "$timing"
}


function testc () {
    name="$1"
    maxdist=$2

    rm -f timing2
    for threads in 1 2 3 4 5 6 7 8 9 10
    do
        testone "c-parallel/find-similar-gcc_O2" "$name" "$maxdist" $threads
    done
    mv timing2 "timing-results/$3"
}

testc "64_100k" 13 "timing_threads_64"
testc "256_100k" 87 "timing_threads_256"
