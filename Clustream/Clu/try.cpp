#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
 
typedef std::vector<std::vector<double>> Matrix;

struct timespec TIMER_START,TIMER_END;
long unsigned int timer(int f){
	if(f){
		clock_gettime(CLOCK_MONOTONIC_RAW, &TIMER_START);
		 
          printf("%ld \n",TIMER_START.tv_nsec); 
		return 0;  
	}
	clock_gettime(CLOCK_MONOTONIC_RAW, &TIMER_END);
  
      
           printf("%ld \n",TIMER_END.tv_nsec); 
	return (TIMER_END.tv_sec - TIMER_START.tv_sec) * 1e9 + (TIMER_END.tv_nsec - TIMER_START.tv_nsec);
}

int main(int argc, char const *argv[]){	
	if(argc!=2){
		fprintf(stderr, "[USAGE] %s [file.csv] \n", argv[0]);
		return 1;
	}
	//FILE* f=fopen(argv[1],"r");
	printf("%s \n",argv[1]);
	long timestamp=0;
	Matrix batch;
	timer(1);
		// if(timestamp==40000)break;
	//fclose(f);
}