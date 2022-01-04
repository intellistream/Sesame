echo "---------start Testing CluStream on the 5 selected Dataset---------"

echo  ">> Parameter Test: initBuffer, onlineClusterNumber, radiusFactor, lastArrivingNum, timeWindow <<"

for o in $(seq 0 4)
  do
    #Perform on Cover Type
    if [ $o == 0 ]; then
      for b in $(seq  500 500 1500)
        do
          for N in $(seq 60 20 150)
            do
              for r in $(seq 2 2 24)
                do
                  for l in $(seq 2 2 10)
                    do
                      for T in $(seq 1 2 5)
                        do
                          ./benchmark.exe -p 5000 -d 54 -a 4 -O $o -c 7 -W 0 -t 3 -b $b -N $N -r $r -l $l -T $T
                        done
                    done
                done
            done
         done
    fi

 #Perform on KDD99
    if [ $o == 1 ]; then
      for b in $(seq  500 500 1500)
        do
          for N in $(seq 60 20 150)
            do
              for r in $(seq 2 1 8)
                do
                  for l in $(seq 2 3 15)
                    do
                      for T in $(seq 1 2 5)
                        do
                          ./benchmark.exe -p 5000 -d 41 -a 4 -O $o -c 24 -W 0 -t 3 -b $b -N $N -r $r -l $l -T $T
                        done
                    done
                done
            done
         done
    fi
#Perform on Sensor
    if [ $o == 2 ]; then
      for b in $(seq  500 500 1000)
        do
          for N in $(seq 60 20 150)
            do
              for r in $(seq 2 2 15)
                do
                  for l in $(seq 2 2 10)
                    do
                      for T in $(seq 1 2 4)
                        do
                          ./benchmark.exe -p 5000 -d 5 -a 4 -O $o -c 24 -W 0 -t 3 -b $b -N $N -r $r -l $l -T $T
                        done
                    done
                done
            done
         done
    fi
       done
#Perform on Diamond
 :'   if [ $o == 3 ]; then
      for b in $(seq 500 500 1000)
        do
          for N in $(seq 60 20 150)
            do
              for r in $(seq  2 1 5)
                do
                  for l in $(seq 2 2 10)
                    do
                      for T in $(seq 1 2 3)
                        do
                          ./benchmark.exe -p 3000 -d 2 -a 4 -O $o -c 9 -W 0 -t 3 -b $b -N $N -r $r -l $l -T $T
                        done
                    done
                done
            done
         done
    fi'
#Perform on Zelnik
   :' if [ $o == 4 ]; then
      for b in $(seq 300 200 500 )
        do
          for N in $(seq 60 10 150)
            do
              for r in $(seq 2 1 5)
                do
                  for l in $(seq 2 2 8)
                    do
                      for T in $(seq 1 2)
                        do
                          ./benchmark.exe -p 622 -d 2 -a 4 -O $o -c 5 -W 0 -t 3 -b $b -N $N -r $r -l $l -T $T
                        done
                    done
                done
            done
         done
    fi
  done'








