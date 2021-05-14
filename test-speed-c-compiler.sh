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

    echo "=== cmd='$cmd' name=$name maxdist=$maxdist"
    for i in $(seq 3)
    do
        mytime -f "%e" -o "$timing" $cmd "$inputfn" "$maxdist" $extra >/dev/null
        t=$(cat "$timing")
        echo "$cmd $t" >>timing
    done
    rm "$timing"
}


function testc () {
    name="$1"
    maxdist=$2

    rm -f timing
    testone "c-simple/find-similar-gcc_O2" "$name" "$maxdist"
    testone "c-simple/find-similar-gcc_O3" "$name" "$maxdist"
    testone "c-simple/find-similar-clang_O2" "$name" "$maxdist"
    testone "c-simple/find-similar-clang_O3" "$name" "$maxdist"
    testone "c-simple/find-similar-clang_Ofast" "$name" "$maxdist"
    mv timing timing-results/timing_simple_$name

    testone "c-unroll/find-similar-gcc_O2" "$name" "$maxdist"
    testone "c-unroll/find-similar-gcc_O3" "$name" "$maxdist"
    testone "c-unroll/find-similar-clang_O2" "$name" "$maxdist"
    testone "c-unroll/find-similar-clang_O3" "$name" "$maxdist"
    testone "c-unroll/find-similar-clang_Ofast" "$name" "$maxdist"
    mv timing timing-results/timing_unroll_$name

    testone "c-unroll2/find-similar-gcc_O2" "$name" "$maxdist" 32
    testone "c-unroll2/find-similar-gcc_O3" "$name" "$maxdist" 32
    testone "c-unroll2/find-similar-clang_O2" "$name" "$maxdist" 32
    testone "c-unroll2/find-similar-clang_O3" "$name" "$maxdist" 32
    testone "c-unroll2/find-similar-clang_Ofast" "$name" "$maxdist" 32
    mv timing timing-results/timing_unroll2_32_$name
}

testc "64_100k" 13
testc "256_100k" 87
