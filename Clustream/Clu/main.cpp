#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>

#include "clustream.h"

typedef std::vector<std::vector<double>> Matrix; //construct matrix which contain values in double format

struct timespec TIMER_START,TIMER_END;
long unsigned int timer(int f){
	if(f){
		clock_gettime(CLOCK_MONOTONIC_RAW, &TIMER_START);
		 
		return 0;  
	}
	clock_gettime(CLOCK_MONOTONIC_RAW, &TIMER_END);
	return (TIMER_END.tv_sec - TIMER_START.tv_sec) * 1e9 + (TIMER_END.tv_nsec - TIMER_START.tv_nsec);
}

Matrix get_batch(FILE* f, unsigned int n){
	Matrix ans(n);
	int i=0;
	char buff[1024];
	char* splitted;
	while(fgets(buff,1024,f)){
		splitted=strtok(buff,",");
		do{
			ans[i].push_back(atof(splitted));
		}while(splitted=strtok(NULL,","));
		i++;
		if(i==n)break;
	}
	ans.resize(i);
	return ans;
}

int main(int argc, char const *argv[]){	
	if(argc!=2){
		fprintf(stderr, "[USAGE] %s [file.csv] \n", argv[0]);
		return 1;
	}
	FILE* f=fopen(argv[1],"r");
	printf("%s", argv[1]);
	long timestamp=0;
	Matrix batch;
	CluStream model(
		512, //h 512
		2000, //m 200
		2   //t
	);
	long pps_avg=0;
	while((batch=get_batch(f,1)).size()!=0 ){
		
		for(Point datapoint : batch){
			timer(1);
			model.offline_cluster(datapoint,timestamp);
			if (timestamp>model.m)
				pps_avg+= 1/(timer(0)/1e+9);
			timestamp++;
		}
		// if(timestamp==40000)break;
	}
	pps_avg/=(timestamp-model.m);
	printf("avg pps :%ld\n",pps_avg);
	fclose(f);
}