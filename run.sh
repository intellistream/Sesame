# run.sh

if [ $# -ne 1 ]; then
    echo "Usage: $0 <executions>"
    exit 1
fi

executions=$1

for ((i=1; i<=$executions; i++)); do
    ./build/benchmark/benchmark
done
