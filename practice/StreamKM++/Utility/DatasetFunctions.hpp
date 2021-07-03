/**
 * 
 * Changelog 
 *  2008/04/25
 *  - added functions to read datasets in Liadan format
 *  - added functions to save centers in old format(separated by a ',') and Liadan format
 * */

#ifndef DATASETFUNCTIONS_H
#define DATASETFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>


#include <string.h>
#include "../DataStructure/Point.hpp"


#define		TRUE			1
#define		FALSE			0

/**
returns the next point of a file (Gereons format) 
**/
void getNextPointGereon(FILE *file,struct point *p,int dimension);
/*
 * Returns the next point of a file (Liadan format)
 * */
void getNextPointLiadan(FILE* file, struct point *p, int dimension);

/*
 *  saves the centers to a file with the separator ','
 * */
void saveCentersOldFormat(FILE* out, int numberOfCenters, struct point * centers,int dimension);

/*
 *  saves the centers to a file with the separator ' ' and a the weight as the first row
 * */
void saveCentersLiadanFormat(FILE* out, int numberOfCenters, struct point * centers,int dimension);

#endif 
