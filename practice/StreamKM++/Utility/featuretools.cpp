#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "featurepp.hpp"

/* readArg: reads till first occurence of , or \n 
   returning it or EOF for end of file */
int readArg(char *s, FILE *fp) {

   int c;
   int i = -1;

   while ((s[++i] = c =  getc(fp)) != EOF) {
      if (c == ',' || c == '\n') {
         s[i] = '\0';
         return c;
      }
   }
   return EOF;
}

/* readLiadanArg: reads till first occurence of a blank or \n 
   returning it or EOF for end of file */
int readLiadanArg(char *s, FILE *fp) {

   int c;
   int i = -1;

   while ((s[++i] = c =  getc(fp)) != EOF) {
      if (c == ' ' || c == '\n') {
         s[i] = '\0';
         return c;
      }
   }
   s[i] = '\0';
   return EOF;
}




/* writeArg: writes Argument + seperator sep for ',' or '\n' or ' '
   returns EOF if can't write */
int writeArg(char *s, FILE *fp, int sep) {

   fputs(s, fp);
   if (sep == ',')
      fputs(",", fp);
   if (sep == '\n')
      fputs("\n", fp);
   if (sep == ' ')
      fputs(" ", fp);
   return 1;
}

/* getNextHost:   gets the next hostname in a labelfile, returns EOF
   if no host is left - hostname must be 1st entry in EVERY line */
int getNextHost(char *t, FILE *fp) {
   
   int c;
   int i = -1;
   
   while ((t[++i] = c = getc(fp)) != EOF) {
      if (c == ' ') {                  // read hostname in t
         t[i] = '\0';
         while (getc(fp) != '\n') {    // skip label
            ;
         }
         return i;
      }
   }
   return EOF;
}

/* getLabeledHost:   gets the label if hostname was read before! */
int getLabeledHost(char *hname, char *lb, FILE *fp) {

   int c;
   int i = -1;
   
   while ((hname[++i] = c = getc(fp)) != EOF) {
      if (c == ' ') {                  // read hostname in t
         hname[i] = '\0';
         break;
      }
   }
   i = -1;
   while ((lb[++i] = c = getc(fp)) != EOF) {
      if (c == ' ') {
         lb[i] = '\0';   
         while (getc(fp) != '\n')     // skip rest
            ;
         return i;
      } else if (c == '\n') {
         lb[i] = '\0';
         return i;
      }
   }
   return EOF;
}

/* getHidHostByDomain:    gets the next hostname in sourcefile + the hid, returns
   EOF if no host is left, just works for hosts with the same domain! */
int getHidHostByDomain(char *hid, char *name, char *domain, FILE *fp) {

   while (readArg(name, fp)!=EOF) {
      if (strstr(name, domain)==NULL) 
         strcpy(hid, name);
      else 
         return 1;
   }
   return EOF;
}

/* passHeader: puts the description of columns from the sourcefile 
   to the output (the 1st descriptive dataset) */
int passHeader(FILE *fpsource, FILE *fpoutput) {
   
   char t[1000];
   FILE *fptemp;
   
   while (readArg(t, fpsource)==',') {     /* passes header to new file */
         if (fputs(t, fpoutput)== EOF) {
             fprintf(stderr, "error in purgeUnlabeledHosts! Can't write!");
             return 0;
         }
         fputs(",", fpoutput);
   }
   fputs(t, fpoutput);
   fputs("\n", fpoutput);

   return 1;
}
