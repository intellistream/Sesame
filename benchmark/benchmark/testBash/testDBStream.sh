echo "---------start Testing DBStream on the 5 selected Dataset---------"

echo  ">> Parameter Test: radius, lambda, clean_interval, min_weight, alpha <<"

for o in $(seq 0 4)
  do
    #Perform on Cover Type
    if [ $o == 0 ]; then
          for R in $(seq 180 20 200)
            do
              lambda=(0.125 0.25)
              for L in ${lambda[*]}
                do
                  for C in $(seq 5 6)
                    do
                      for w in $(seq 2 5)
                        do
                          alpha=(0.2 0.5)
                          for P in ${alpha[*]}
                            do
                          ./benchmark -p 5000 -d 54 -a 3 -O $o -R $R -L $L -C $C -w $w -P $P
                            done
                        done
                    done
                done
            done
    fi

 #Perform on KDD99
    if [ $o == 1 ]; then
          radius=(80 100 150)
          for R in ${radius[*]}
            do
              lambda=(0.125 0.25)
              for L in ${lambda[*]}
                do
                  for C in $(seq 5 6)
                    do
                      for w in $(seq 2 4)
                        do
                          alpha=(0.3)
                          for P in ${alpha[*]}
                            do
                          ./benchmark -p 5000 -d 41 -a 3 -O $o -R $R -L $L -C $C -w $w -P $P
                            done
                    done
                done
            done
         done
    fi
#Perform on Sensor
    if [ $o == 2 ]; then
        radius=(4 14)
          for R in $(seq 4 14)
            do
              lambda=(0.25 0.5)
              for L in ${lambda[*]}
                do
                  clean_interval=(1 2 5)
                  for C in  ${clean_interval[*]}
                    do
                      for w in $(seq 2 4)
                        do
                          alpha=(0.25 0.5)
                          for P in ${alpha[*]}
                            do
                          ./benchmark -p 5000 -d 5 -a 3 -O $o -R $R -L $L -C $C -w $w -P $P
                            done
                    done
                done
            done
         done
    fi
#Perform on Diamond
  :'  if [ $o == 3 ]; then
       radius=(0.4 0.5)
          for R in ${radius[*]}
            do
              lambda=(0.125 0.25)
              for L in ${lambda[*]}
                do
                  for C in $(seq 1 2)
                    do
                      for w in $(seq 2 5)
                        do
                          alpha=(0.2 0.5)
                          for P in ${alpha[*]}
                            do
                          ./benchmark -p 3000 -d 2 -a 3 -O $o -R $R -L $L -C $C -w $w -P $P
                        done
                    done
                done
            done
         done
    fi
#Perform on Zelnik
    if [ $o == 4 ]; then
      radius=(0.6 0.7)
          for R in ${radius[*]}
            do
              lambda=(0.1 0.0125 0.25)
              for L in ${lambda[*]}
                do
                  clean_interval=(3 5 6)
                  for C in ${clean_interval[*]}
                    do
                      for w in $(seq 1 2)
                        do
                          alpha=(0.3 0.5)
                          for P in ${alpha[*]}
                            do
                          ./benchmark -p 622 -d 2 -a 4 -O $o -R $R -L $L -C $C -w $w -P $P
                            done
                    done
                done
            done
         done
    fi'
  done








