#include <iostream>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <unistd.h>    // for sleep()
#include"KMeans.h"
#include"Clustream.h"
using namespace std;
#include <vector>

//typedef std::vector<std::vector<double>> Matrix;

double** Matrix,** batch;//construct matrix which contain values in double format

bool isString(char * str) {
    if (str == NULL)
        return false;
    int len = strlen(str);
    if (len == 0)
        return false;
    bool isaNumber = false;
        for (int i = 0; i < len; i++)
    {
        if (isalpha(str[i])) isaNumber=true;
            break;
    }
    return isaNumber;
}

double** get_batch(FILE* f,unsigned  int dimension,unsigned int n){
    int i=0;
    char buff[1024];
    char* splitted;
    Matrix=new double*[n];
    while(fgets(buff,1024,f))
    {
        splitted=strtok(buff,",");
        Matrix[i] = new double [dimension];
        int z=0;
        do{
            if( isString(splitted)==false)
                {
                    Matrix[i][z]= atof(splitted);
                    z++;
                }
        }while(splitted=strtok(NULL,","));

      /*   for(int a=0; a<38;a++)
            {cout<<Matrix[i][a]<<",";}
        cout<<endl;*/

        i++;

        if(i==n)break;
    }

    return Matrix;
}

//a simple test for K-means class
int testKMeans()
{
   /* double data[] = {
            0,181,5450,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,8,8,0,0,0,0,1,0,0,9,9,1,0,0.11,0,0,0,0,0,
            0,239,486,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,8,8,0,0,0,0,1,0,0,19,19,1,0,0.05,0,0,0,0,0,
            0,235,1337,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,8,8,0,0,0,0,1,0,0,29,29,1,0,0.03,0,0,0,0,0,
            0,219,1337,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,1,0,0,39,39,1,0,0.03,0,0,0,0,0,
            0,217,2032,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,1,0,0,49,49,1,0,0.02,0,0,0,0,0,
            0,217,2032,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,1,0,0,59,59,1,0,0.02,0,0,0,0,0,
            0,212,1940,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,1,0,1,1,69,1,0,1,0.04,0,0,0,0,
            0,159,4087,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,1,0,0,11,79,1,0,0.09,0.04,0,0,0,0,
            0,210,151,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,8,8,0,0,0,0,1,0,0,8,89,1,0,0.12,0.04,0,0,0,0,
            0,212,786,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,8,8,0,0,0,0,1,0,0,8,99,1,0,0.12,0.05,0,0,0,0
    };*/

    const int size = 10; //Number of samples
    const int dim = 38;   //Dimension of feature
    const int cluster_num = 4; //Cluster number

    KMeans* kmeans = new KMeans(38,4);
    int* labels = new int[10];
    kmeans->SetInitMode(KMeans::InitUniform);
    kmeans->Cluster(batch,10,labels);
  /*  for(int i = 0; i < 10; ++i)
    {
        for(int j=0;j<38;++j)
            cout<<batch[i][j]<<",";
        printf(" belongs to %d cluster\n",  labels[i]);
    }*/

    delete []labels;
    delete kmeans;

    return 0;
}


int main() {

    FILE* f=fopen("C:/Users/1124a/CLionProjects/CluStream/data","r");
    if(f == NULL) {
        perror("Error opening file");
        return(-1);
    }

   double ** Init_batch= get_batch(f,38,30);
   batch=get_batch(f,38,300);
 //   fclose(f);

    if(sizeof(batch)>0)
    {
        std::cout<<"Read data successfully!"<<endl;
    }
    CluStream * cluStream  = new CluStream(0.03,20,1.6,38,30);
    std::cout<<"Start initializing....!"<<endl;
    cluStream->init_kernels_offline(Init_batch,30);

    for(int i=29; i<300;i++)
    {
        cluStream->online_cluster(batch[i]);

    }


    return 0;
/*
 *
    clock_t begin = clock();
    clock_t end = clock();
    double time_taken = ((double)(end-begin))/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("The program took %f seconds to execute", time_taken);
*/




}


/*
 NOTE: The below old code uses std::vector<std::vector<double> > as a matrix
 Still need to compare Vector and double **
 typedef std::vector<std::vector<double>> Matrix
 Matrix get_batch(FILE* f,unsigned int n){
    Matrix ans(n);
    int i=0;
    char buff[1024];
     char* splitted;

    while(fgets(buff,1024,f)){
        splitted=strtok(buff,",");
        do{
          if( isString(splitted)==false) ans[i].push_back(atof(splitted));
        }while(splitted=strtok(NULL,","));
     for(int a=0; a<ans[i].size();a++)
        {
            cout<<ans[i][a]<<" ";
        }
i++;
if(i==n)break;
}
ans.resize(i);
return ans;
}


*/