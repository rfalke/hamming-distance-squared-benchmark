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
    for i in $(seq 1)
    do
        mytime -f "%e" -o "$timing" $cmd "$inputfn" "$maxdist" $extra >out
        t=$(cat "$timing")
        b=$(wc -l <out)
        echo "$maxdist $t $b" >>timing2
        rm out
    done
    rm "$timing"
}


function testc () {
    name="$1"
    maxdist=$2

    maxlin=64
    rm -f timing2
    for maxdist in $(seq 16)
    do
        testone "c-metric-tree/find-similar-gcc_O2" "$name" "$maxdist" $maxlin
    done
    mv timing2 timing-results/timing_metric_max_dist
}

testc "64_100k" 13
