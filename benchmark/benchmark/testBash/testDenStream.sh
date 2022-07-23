echo "---------start Testing DenStream on the 5 selected Dataset---------"

echo  ">> Parameter Test: buf_size, min_points, epsilon, lambda, mu, beta <<"

for o in $(seq 0 4)
  do
    #Perform on Cover Type or KDD99
    if [ $o == 0 ] || [ $o == 1 ]; then
      for b in $(seq 500 500 1000)
        do
          for n in $(seq 40 10 50)
            do
              epsilon=(25 30 40)
              for e in ${epsilon[*]}
                do
                  lambda=(0.125 0.25)
                  for L in ${lambda[*]}
                    do
                      mu=(2 5 10)
                      for u in ${mu[*]}
                        do
                         # beta=(0.125 0.25)
                         # for q in ${beta[*]}
                          #  do
                          if [ $o == 0 ]; then
                                ./benchmark -p 3000 -d 54 -a 5 -B 2 -O $o -b $b -n $n -e $e -L $L -u $u -q 0.25
                          fi
                          if [ $o == 1 ]; then
                                ./benchmark -p 3000 -d 41 -a 5 -B 2 -O $o -b $b -n $n -e $e -L $L -u $u -q 0.25
                          fi
                          #done
                        done
                    done
                done
            done
         done
    fi
   #Perform on Sensor
      if [ $o == 2 ] ; then
        for b in $(seq 300 200 500)
          do
            min_points=(3 5 8)
            for n in ${min_points[*]}
              do
                epsilon=(0.1 0.2 6)
                for e in ${epsilon[*]}
                  do
                    lambda=(0.125 0.25)
                    for L in ${lambda[*]}
                      do
                        mu=(8 10)
                        for u in ${mu[*]}
                          do
                            beta=(0.125 0.25)
                            for q in ${beta[*]}
                              do
                                  ./benchmark -p 5000 -d 5 -a 5 -B 2 -O $o -b $b -n $n -e $e -L $L -u $u -q $q
                            done
                          done
                      done
                  done
              done
           done
      fi
#Perform on Diamond
  :'  if [ $o == 3 ] ; then
        for b in $(seq 150 150 500)
          do
            min_points=(5 8 15)
            for n in ${min_points[*]}
              do
                epsilon=(0.8)
                for e in ${epsilon[*]}
                  do
                    lambda=(0.125 0.25)
                    for L in ${lambda[*]}
                      do
                        mu=(4 6 8)
                        for u in ${mu[*]}
                          do
                           # beta=(0.125 0.25)
                           # for q in ${beta[*]}
                            #  do
                                  ./benchmark -p 3000 -d 2 -a 5 -B 2 -O $o -b $b -n $n -e $e -L $L -u $u -q 0.25
                            #done
                          done
                      done
                  done
              done
           done
      fi
#Perform on Zelnik
      if [ $o == 4 ] ; then
        for b in $(seq 100 100 200)
          do
            min_points=(5 8 10)
            for n in ${min_points[*]}
              do
                epsilon=(0.125)
                for e in ${epsilon[*]}
                  do
                    lambda=(0.125 0.25)
                    for L in ${lambda[*]}
                      do
                        mu=(3 8 15)
                        for u in ${mu[*]}
                          do
                           # beta=(0.125 0.25)
                           # for q in ${beta[*]}
                            #  do
                                  ./benchmark -p 622 -d 2 -a 5 -B 2 -O $o -b $b -n $n -e $e -L $L -u $u -q 0.125
                            #done
                          done
                      done
                  done
              done
           done
      fi'
  done








