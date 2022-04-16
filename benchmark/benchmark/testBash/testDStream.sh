echo "---------start Testing DStream on the 5 selected Dataset---------"

echo  ">> Parameter Test: lambda, beta, cm, cl, gridWidth <<"

for o in $(seq 0 4)
  do
    #Perform on Cover Type
    if [ $o == 0 ]; then
      lambda=(0.998 0.999)
          for L in ${lambda[*]}
            do
              beta=(0.001 0.3)
              for q in ${beta[*]}
                do
                  cm=(5 15 20)
                  for x in ${cm[*]}
                    do
                      cl=(0.8 0.001)
                      for X in ${cl[*]}
                        do
                          gridWidth=(13 14 15)
                          for g in ${gridWidth[*]}
                            do
                          ./benchmark -p 5000 -d 54 -a 6 -O $o -L $L -q $q -x $x -X $X -g $g
                            done
                        done
                    done
                done
            done
    fi

 #Perform on KDD99
    if [ $o == 1 ]; then
       lambda=(0.998 0.999)
         for L in ${lambda[*]}
            do
              beta=(0.001 0.3)
              for q in ${beta[*]}
                do
                  cm=(1.001 3.0)
                    for x in ${cm[*]}
                      do
                        cl=(0.8 0.999)
                          for X in ${cl[*]}
                            do
                            gridWidth=(6)
                            for g in ${gridWidth[*]}
                             do
                          ./benchmark -p 5000 -d 41 -a 6 -O $o -L $L -q $q -x $x -X $X -g $g
                            done
                    done
                done
            done
         done
    fi
#Perform on PowerSupply
    if [ $o == 2 ]; then
     lambda=(0.998 0.999)
         for L in ${lambda[*]}
            do
              beta=(0.001 0.3)
              for q in ${beta[*]}
                do
                  cm=(1 2)
                    for x in ${cm[*]}
                      do
                        cl=(0.001 0.002)
                          for X in ${cl[*]}
                            do
                            gridWidth=(0.5 0.6)
                            for g in ${gridWidth[*]}
                             do
                          ./benchmark -p 3000 -d 2 -a 6 -O $o -L $L -q $q -x $x -X $X -g $g
                            done
                    done
                done
            done
         done
    fi
#Perform on Diamond
    if [ $o == 3 ]; then
       lambda=(0.001)
         for L in ${lambda[*]}
            do
              beta=(0.3)
              for q in ${beta[*]}
                do
                  cm=(1.001 2)
                    for x in ${cm[*]}
                      do
                        cl=(0.001 0.01)
                          for X in ${cl[*]}
                            do
                            gridWidth=(0.5)
                            for g in ${gridWidth[*]}
                             do
                          ./benchmark -p 3000 -d 2 -a 6 -O $o -L $L -q $q -x $x -X $X -g $g
                        done
                    done
                done
            done
         done
    fi
#Perform on Zelnik
    if [ $o == 4 ]; then
     lambda=(0.001 0.998)
         for L in ${lambda[*]}
            do
              beta=(0.001 0.3)
              for q in ${beta[*]}
                do
                  cm=(1)
                    for x in ${cm[*]}
                      do
                        cl=(0.001)
                          for X in ${cl[*]}
                            do
                            gridWidth=(0.3 0.25)
                            for g in ${gridWidth[*]}
                             do
                          ./benchmark -p 622 -d 2 -a 6 -O $o -L $L -q $q -x $x -X $X -g $g
                            done
                    done
                done
            done
         done
    fi
  done








