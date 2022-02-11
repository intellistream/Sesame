
       
  for j in $(seq 50 50 2500)
      do
              ./benchmark.exe -p 3000 -d 2 -a 0 -O 3 -c 9 -s $j -S 10 
      done
      
  
   for j in $(seq 10 5 30)
      do
            ./benchmark.exe -p 3000 -d 2 -a 1 -O 3 -D 0.6 -M $j -m 2
      done

   for j in $(seq 0.1 0.1 5)
      do
             ./benchmark.exe -p 3000 -d 2 -a 2 -O 3 -R $j -E 5 -q 0.001 -j 150
      done
      
      
   for j in $(seq 0.1 0.01 1.2)
      do
            ./benchmark.exe -p 3000 -d 2 -a 3 -O 3 -R $j -L 0.25 -C 50 -w 0.1 -P 0.4
      done
    
    
    
    
   for j in $(seq 1 1 16)
      do
             ./benchmark.exe -p 3000 -d 2 -a 4 -O 3 -c 9 -W 0 -t 4 -b 200 -N 60 -r $j -l 12 -T 200
      done
    
    
    
   for j in $(seq 0.1 0.1 3)
      do
             ./benchmark.exe -p 3000 -d 2 -a 5 -O 3 -b 200 -n 4 -e $j -L 0.25 -u 4 -q 0.25 -B 2
      done
      
      
      
   for j in $(seq 0.01 0.01 3)
      do
             ./benchmark.exe -p 3000 -d 2 -a 6 -O 3 -L 0.125 -q 0.3 -x 0.5 -X 0.001 -g $j
      done
                        