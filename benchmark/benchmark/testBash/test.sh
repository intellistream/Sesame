echo "---------start Testing CluStream on KDD99 Dataset---------"

echo  ">> Parameter Test: lastArrivingNum, timeWindow, clusterNumber, radiusFactor <<"
for a in $(seq 5 25 40)
do
        for T in $(seq 10 3 200)
        do
                for c in $(seq 30 500 100)
                do
                  for r in $(seq 2 5 8)
                        do
                        ./benchmark.exe -p 10000 -d 54 -c 12 -a $a -T $T -b 200 -C $c -r $r -O 0
                        done
                done
        done
done