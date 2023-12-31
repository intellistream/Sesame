#!/bin/bash
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -level) level="$2"; shift ;;
        -metric) metric="$2"; shift ;;
        -times) times="$2"; shift ;;
        -algo) algo="$2"; shift ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

for ((i=1; i<=$times; i++)); do
    ./build/benchmark/benchmark -level "$level" -metric "$metric" -algo "$algo"
done
