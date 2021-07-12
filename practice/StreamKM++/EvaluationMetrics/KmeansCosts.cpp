#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../Utility/featurepp.hpp"

double getDistance(double *, double *, int);	//no need for headers...

/* Arguments:
     - argv[1]: sourcefile of Liadans algorithm
     - argv[2]: file with centres produced by Liadan
 	 - argv[3]: dimension
	 - argv[4]: number of points
     - argv[5]: number of used centres */

main(int argc, char *argv[]) {
   
   FILE *fpsource, *fpcentres;
   double kmeansoverall = 0.0;
   double mincost = 0.0;
   double distance = 0.0;
   int centres = atoi(argv[5]);
   int nop = atoi(argv[4]);
   int dim = atoi(argv[3]);
   int i, j, b;

   char t[1000];                    // Buffer

//   printf("Calculating total Liadan-cost...\n");   

//   printf("Open: %s\n", argv[1]);
   if ((fpsource = fopen(argv[1], "r"))==NULL) {
      printf("error: can't open Sourcefile %s\n", argv[1]);
      exit(1);
   }

//   printf("Open: %s\n", argv[2]);
   if ((fpcentres = fopen(argv[2], "r"))==NULL) {
      printf("error: can't open Centresfile %s\n", argv[2]);
      exit(1);
   }

/*   printf("Open: %s\n", argv[3]);
   if ((fpoutput = fopen(argv[3], "w"))==NULL) {
      printf("error: can't open Outputfile %s\n", argv[3]);
      exit(1);
   }*/

//   printf("centres: %d\n", centres);

   double **c = (double **) calloc(centres, sizeof(double *));    // holds all centres
   for (i = 0; i < centres; i++) {
      readLiadanArg(t, fpcentres);                       //passing the leading weight
      readLiadanArg(t, fpcentres);                       //get 1st argument
      double *v = (double *) calloc(dim, sizeof(double));      
      v[0] = atof(t);
      j = 1;
      while(readLiadanArg(t, fpcentres)==' ')            // as long as no line break occurs
         v[j++] = atof(t);
      v[j] = atof(t);
      c[i] = v;
   }

   fclose(fpcentres);   // all centers are now saved

   /* start reading the sourcefile */
   double *p = (double *) calloc(dim, sizeof(double));   // the actual point;

   for (i = 0; i < nop; i++) {
      readLiadanArg(t, fpsource);         //passing the leading 1
      j = 0;
      while(readLiadanArg(t, fpsource)==' ')
            p[j++] = atof(t);         
      p[j] = atof(t);
      mincost = getDistance(p, c[0], dim);
      for (j = 0; j < centres; j++) {     // looking for the nearest center
         distance = getDistance(p, c[j], dim);
         if (distance < mincost)
            mincost = distance;
      }
      kmeansoverall += mincost;
   }

   fclose(fpsource);
   
 //  fprintf(fpoutput, "Results from Liadan on %s\n", argv[1]);
 //  fprintf(fpoutput, "-------------------------------------------------\n");
 //  fprintf(fpoutput, "centres: %d\n", centres);   
   printf("%f\n", kmeansoverall);   
   
	/* freeing */
	for (i = 0; i < centres; i++) {
		free(c[i]);
	}
	free(c);
	free(p);

}

double getDistance(double *p1, double *p2, int d) {
   int i;
   double f = 0.0;

   for (i = 0; i < d; i++) {
      f += pow((p1[i]-p2[i]), 2);
   }
   
   return f;
}
