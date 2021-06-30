
#ifndef CLUT_KMEANS_H
#define CLUT_KMEANS_H
#include <fstream>

class KMeans
{
public:
    enum InitMode
    {
        InitRandom,
        InitManual,
        InitUniform,
    };

    KMeans(int dimension = 1, int cluster_Num = 1);
    ~KMeans();

    void SetMean(int i, const double* u){ memcpy(m_centroid[i], u, sizeof(double) * m_dimension); }
    void SetInitMode(int i)				{ m_initMode = i; }
    void SetMaxIterNum(int i)			{ m_maxIterNum = i; }
    void SetEndError(double f)			{ m_endError = f; }

    double* GetMean(int i)	{ return m_centroid[i]; }
    int GetInitMode()		{ return m_initMode; }
    int GetMaxIterNum()		{ return m_maxIterNum; }
    double GetEndError()	{ return m_endError; }


    /*	SampleFile: <size><dim><data>...
        LabelFile:	<size><label>...
    */
    void Cluster(double **Matrix, int N, int *Label);


    void Init(double **Matrix,int N);

    void Init(double *data, int N);
    void Cluster(double *data, int N, int *Label);
    void Init(std::ifstream& sampleFile);
    void Cluster(const char* sampleFileName, const char* labelFileName);

    friend std::ostream& operator<<(std::ostream& out, KMeans& kmeans);

private:
    int m_dimension;
    int m_clusterNum;
    double** m_centroid;//cluster centroid

    int m_initMode;
    int m_maxIterNum;		// The stopping criterion regarding the number of iterations
    double m_endError;		// The stopping criterion regarding the error
    double GetLabel(const double* x, int* label);
    double CalcDistance(const double* x, const double* u, int dimNum);
};

#endif //CLUT_KMEANS_H
