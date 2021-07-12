/********
Copyright 2008 by Marcel R. Ackermann, Christiane Lammersen, Marcus Maertens, Christoph Raupach, 
Christian Sohler, Kamil Swierkot

Without any warranty!
*/

#define _FILE_OFFSET_BITS 64
#include <stdlib.h>
#include "DatasetFunctions.hpp"

// Prototypes
double readDouble(FILE*);

/**
returns the next point of a file (Gereons format) 
**/
void getNextPointGereon(FILE *file,struct point *p,int dimension){
	int l;
	for(l=0;l<dimension;l++){
		float nextNumber;
		char nextLine [256];
		fgets ( nextLine, 256, file );
		sscanf(nextLine,"%f",&nextNumber);
		p->coordinates[l] = nextNumber;
		p->squareSum += nextNumber * nextNumber;
	}
	p->weight = 1.0;
}


/*
 *  Returns the next point of a file(Liadan format)
 * */
void getNextPointLiadan(FILE* file, struct point *p, int dimension){
	
	// skip first number
	readDouble(file);
	
	for(int l = 0; l < dimension; l++){
		// read next number
		float nextNumber = readDouble(file);
		
		p->coordinates[l] = nextNumber;
		p->squareSum += nextNumber* nextNumber;
		
	}
	
	p->weight = 1.0;
}

/*
 * Reads a double. Ignores leading spaces.
 * 
 */
double readDouble(FILE* file){
	char c;
	static char buf[100];
	
	int i = 1;
	while((c = fgetc(file)) ==' ' && c != EOF){}
	
	buf[0] = c;
	    
	while((c = fgetc(file)) != '\n' && c != ' ' && c != EOF){
		buf[i++] = c;
	}
	buf[i]=0;
	
	return atof(buf);
}

/*
 *  saves the centers to a file with the separator ','
 * */
void saveCentersOldFormat(FILE* out, int numberOfCenters, struct point * centers,int dimension){
	for(int i=0;i<numberOfCenters;i++){
			int l;
			for(l=0;l<dimension-1;l++){
				fprintf(out,"%f,",centers[i].coordinates[l] / centers[i].weight);
			}
			fprintf(out,"%f",centers[i].coordinates[dimension-1] / centers[i].weight);
			fprintf(out,"\n");
		}	
}

/*
 *  saves the centers to a file with the separator ' ' and a the weight as the first row
 * */
void saveCentersLiadanFormat(FILE* out, int numberOfCenters, struct point * centers,int dimension){
	
	for(int i=0;i<numberOfCenters;i++){
				int l;
				fprintf(out,"%f ",centers[i].weight );
				for(l=0;l<dimension-1;l++){
					if(centers[i].weight != 0.0){
						fprintf(out,"%f ",centers[i].coordinates[l] / centers[i].weight);
					}else{
						fprintf(out,"%f ",centers[i].coordinates[l] );
					}
				}
				if(centers[i].weight != 0.0){
					fprintf(out,"%f",centers[i].coordinates[dimension-1] / centers[i].weight);
				}else{
					fprintf(out,"%f",centers[i].coordinates[dimension-1]);
				}
				fprintf(out,"\n");
			}	
	
}

