int readArg(char *, FILE *);
int readLiadanArg(char *, FILE *);
int writeArg(char *, FILE *, int);
int getHidHostByDomain(char *, char *, char *, FILE *);
int getNextHost(char *, FILE *);
int getLabeledHost(char *, char *, FILE *);
int passHeader(FILE *, FILE *);
int checkmask(char *, FILE *);
void maskFeatures(char *, FILE *, FILE *);
