#include <cstring>
#include "cstdlib"
#include <cmath>
#include <ctime>
#include <iostream>
#include <cassert>
#include "KMeans.h"
using namespace std;

KMeans::KMeans(int dimension, int cluster_Num)
{
    m_dimension = dimension;
    m_clusterNum = cluster_Num;
    m_centroid = new double*[m_clusterNum];
    for (int i = 0; i < m_clusterNum; i++)
    {
        m_centroid[i] = new double[m_dimension];
        memset(m_centroid[i], 0, sizeof(double) * m_dimension);

    }
    m_initMode = InitRandom;
    m_maxIterNum = 100;
    m_endError = 0.001;

}
//Release memory
KMeans::~KMeans()
{
    for (int i = 0; i < m_clusterNum; i++)
    {
        delete[] m_centroid[i];
    }
    delete[] m_centroid;
}
//Initialize K-means

void KMeans::Init(double **Matrix, int N)
{
    int size = N;
    if (m_initMode ==  InitRandom)
    {
        int interval = size / m_clusterNum;

        // Seed the random-number generator with current time
        srand((unsigned)time(NULL));

        for (int i = 0; i < m_clusterNum; i++)
        {
            int select = interval * i + (interval - 1) * rand() / RAND_MAX;
            memcpy(m_centroid[i], Matrix[select], sizeof(double) * m_dimension);
        }
    }
    else if (m_initMode == InitUniform)
    {
        for (int i = 0; i < m_clusterNum; i++)
        {
            int select = i * size / m_clusterNum;
            memcpy(m_centroid[i],Matrix[select], sizeof(double) * m_dimension);
        }
    }
    else if (m_initMode == InitManual)
    {
        // Do nothing
    }

}


void KMeans::Cluster(double **Matrix, int N, int *Label)
{
    int size = 0;
    size=N;
    assert(size >= m_clusterNum);

    // Initialize model
    Init(Matrix,N);

    // Recursion

    int label = -1;		// Class index
    double iterNum = 0;
    double lastCost = 0;
    double currCost = 0;
    int unchanged = 0;
    bool loop = true;
    int* counts = new int[m_clusterNum];//Total number of data points in each cluster
    double** next_means = new double*[m_clusterNum];	// New model for re-estimation
    for (int i = 0; i < m_clusterNum; i++)
    {
        next_means[i] = new double[m_dimension];
    }

    while (loop)
    {
        memset(counts, 0, sizeof(int) * m_clusterNum);
        for (int i = 0; i < m_clusterNum; i++)
        {
            memset(next_means[i], 0, sizeof(double) * m_dimension);
        }

        lastCost = currCost;
        currCost = 0;

        // Classification
        for (int i = 0; i < size; i++)
        {

            currCost += GetLabel(Matrix[i], &label);

            counts[label]++;//cumulate the number of cluster
            for (int d = 0; d < m_dimension; d++)
            {
                next_means[label][d] += Matrix[i][d];
            }
        }
        currCost /= size;

        // Re-estimation the centroid
        for (int i = 0; i < m_clusterNum; i++)
        {
            if (counts[i] > 0)
            {
                for (int d = 0; d < m_dimension; d++)
                {
                    next_means[i][d] /= counts[i];
                }
                memcpy(m_centroid[i], next_means[i], sizeof(double) * m_dimension);
            }
        }

        // Terminal conditions
        iterNum++;
        if (fabs(lastCost - currCost) < m_endError * lastCost)
        {
            unchanged++;
        }
        if (iterNum >= m_maxIterNum || unchanged >= 3)
        {
            loop = false;
        }

        //DEBUG
        //cout << "Iter: " << iterNum << ", Average Cost: " << currCost << endl;
    }

    // Output the label file
    for (int i = 0; i < size; i++)
    {
        GetLabel(Matrix[i], &label);
        Label[i] = label;
    }
    delete[] counts;
    for (int i = 0; i < m_clusterNum; i++)
    {
        delete[] next_means[i];
    }
    delete[] next_means;

}




//Calculate two data point distance

double KMeans::CalcDistance(const double* x, const double* u, int dimNum)
{
    double temp = 0;
    for (int d = 0; d < dimNum; d++)
    {
        temp += (x[d] - u[d]) * (x[d] - u[d]);
    }
    return sqrt(temp);
}

//Obtain the id of a datapoint's belonging cluster

double KMeans::GetLabel(const double* sample, int* label)
{
    double dist = -1;
    for (int i = 0; i < m_clusterNum; i++)
    {
        double temp = CalcDistance(sample, m_centroid[i], m_dimension);
        if (temp < dist || dist == -1)
        {
            dist = temp;
            *label = i;
        }
    }
    return dist;
}

ostream& operator<<(ostream& out, KMeans& kmeans)
{
    out << "<KMeans>" << endl;
    out << "<DimNum> " << kmeans.m_dimension << " </DimNum>" << endl;
    out << "<ClusterNum> " << kmeans.m_clusterNum << " </CluterNum>" << endl;

    out << "<Mean>" << endl;
    for (int i = 0; i < kmeans.m_clusterNum; i++)
    {
        for (int d = 0; d < kmeans.m_dimension; d++)
        {
            out << kmeans.m_centroid[i][d] << " ";
        }
        out << endl;
    }
    out << "</Mean>" << endl;
    out << "</KMeans>" << endl;
    return out;
}


void KMeans::Init(ifstream& sampleFile)
{
    int size = 0;
    sampleFile.seekg(0, ios_base::beg);
    sampleFile.read((char*)&size, sizeof(int));

    if (m_initMode ==  InitRandom)
    {
        int interval = size / m_clusterNum;
        double* sample = new double[m_dimension];

        // Seed the random-number generator with current time
        srand((unsigned)time(NULL));

        for (int i = 0; i < m_clusterNum; i++)
        {
            int select = interval * i + (interval - 1) * rand() / RAND_MAX;
            int offset = sizeof(int) * 2 + select * sizeof(double) * m_dimension;

            sampleFile.seekg(offset, ios_base::beg);
            sampleFile.read((char*)sample, sizeof(double) * m_dimension);
            memcpy(m_centroid[i], sample, sizeof(double) * m_dimension);
        }

        delete[] sample;
    }
    else if (m_initMode == InitUniform)
    {
        double* sample = new double[m_dimension];

        for (int i = 0; i < m_clusterNum; i++)
        {
            int select = i * size / m_clusterNum;
            int offset = sizeof(int) * 2 + select * sizeof(double) * m_dimension;

            sampleFile.seekg(offset, ios_base::beg);
            sampleFile.read((char*)sample, sizeof(double) * m_dimension);
            memcpy(m_centroid[i], sample, sizeof(double) * m_dimension);
        }

        delete[] sample;
    }
    else if (m_initMode == InitManual)
    {
        // Do nothing
    }
}

void KMeans::Cluster(const char* sampleFileName, const char* labelFileName)
{
    // Check the sample file
    ifstream sampleFile(sampleFileName, ios_base::binary);
    assert(sampleFile);

    int size = 0;
    int dim = 0;
    sampleFile.read((char*)&size, sizeof(int));
    sampleFile.read((char*)&dim, sizeof(int));
    assert(size >= m_clusterNum);
    assert(dim == m_dimension);

    // Initialize model
    Init(sampleFile);

    // Recursion
    double* x = new double[m_dimension];	// Sample data
    int label = -1;		// Class index
    double iterNum = 0;
    double lastCost = 0;
    double currCost = 0;
    int unchanged = 0;
    bool loop = true;
    int* counts = new int[m_clusterNum];
    double** next_means = new double*[m_clusterNum];	// New model for reestimation
    for (int i = 0; i < m_clusterNum; i++)
    {
        next_means[i] = new double[m_dimension];
    }

    while (loop)
    {
        memset(counts, 0, sizeof(int) * m_clusterNum);
        for (int i = 0; i < m_clusterNum; i++)
        {
            memset(next_means[i], 0, sizeof(double) * m_dimension);
        }

        lastCost = currCost;
        currCost = 0;

        sampleFile.clear();
        sampleFile.seekg(sizeof(int) * 2, ios_base::beg);

        // Classification
        for (int i = 0; i < size; i++)
        {
            sampleFile.read((char*)x, sizeof(double) * m_dimension);
            currCost += GetLabel(x, &label);

            counts[label]++;
            for (int d = 0; d < m_dimension; d++)
            {
                next_means[label][d] += x[d];
            }
        }
        currCost /= size;

        // Reestimation
        for (int i = 0; i < m_clusterNum; i++)
        {
            if (counts[i] > 0)
            {
                for (int d = 0; d < m_dimension; d++)
                {
                    next_means[i][d] /= counts[i];
                }
                memcpy(m_centroid[i], next_means[i], sizeof(double) * m_dimension);
            }
        }

        // Terminal conditions
        iterNum++;
        if (fabs(lastCost - currCost) < m_endError * lastCost)
        {
            unchanged++;
        }
        if (iterNum >= m_maxIterNum || unchanged >= 3)
        {
            loop = false;
        }
        //DEBUG
        //cout << "Iter: " << iterNum << ", Average Cost: " << currCost << endl;
    }

    // Output the label file
    ofstream labelFile(labelFileName, ios_base::binary);
    assert(labelFile);

    labelFile.write((char*)&size, sizeof(int));
    sampleFile.clear();
    sampleFile.seekg(sizeof(int) * 2, ios_base::beg);

    for (int i = 0; i < size; i++)
    {
        sampleFile.read((char*)x, sizeof(double) * m_dimension);
        GetLabel(x, &label);
        labelFile.write((char*)&label, sizeof(int));
    }

    sampleFile.close();
    labelFile.close();

    delete[] counts;
    delete[] x;
    for (int i = 0; i < m_clusterNum; i++)
    {
        delete[] next_means[i];
    }
    delete[] next_means;
}

void KMeans::Init(double *data, int N)
{
    int size = N;
    if (m_initMode ==  InitRandom)
    {
        int interval = size / m_clusterNum;
        double* sample = new double[m_dimension];

        // Seed the random-number generator with current time
        srand((unsigned)time(NULL));

        for (int i = 0; i < m_clusterNum; i++)
        {
            int select = interval * i + (interval - 1) * rand() / RAND_MAX;

            for(int j = 0; j < m_dimension; j++)
                sample[j] = data[select*m_dimension+j];
            memcpy(m_centroid[i], sample, sizeof(double) * m_dimension);
        }

        delete[] sample;
    }
    else if (m_initMode == InitUniform)
    {
        double* sample = new double[m_dimension];

        for (int i = 0; i < m_clusterNum; i++)
        {
            int select = i * size / m_clusterNum;

            for(int j = 0; j < m_dimension; j++)
            {
                sample[j] = data[select*m_dimension+j];}

            memcpy(m_centroid[i], sample, sizeof(double) * m_dimension);
        }

        delete[] sample;
    }
    else if (m_initMode == InitManual)
    {
        // Do nothing
    }
}
void KMeans::Cluster(double *data, int N, int *Label)
{
    int size = 0;
    size=N;

    assert(size >= m_clusterNum);

    // Initialize model
    Init(data,N);

    // Recursion
    double* x = new double[m_dimension];	// Sample data
    int label = -1;		// Class index
    double iterNum = 0;
    double lastCost = 0;
    double currCost = 0;
    int unchanged = 0;
    bool loop = true;
    int* counts = new int[m_clusterNum];//Total number of data points in each cluster
    double** next_means = new double*[m_clusterNum];	// New model for re-estimation
    for (int i = 0; i < m_clusterNum; i++)
    {
        next_means[i] = new double[m_dimension];
    }

    while (loop)
    {
        memset(counts, 0, sizeof(int) * m_clusterNum);
        for (int i = 0; i < m_clusterNum; i++)
        {
            memset(next_means[i], 0, sizeof(double) * m_dimension);
        }

        lastCost = currCost;
        currCost = 0;

        // Classification
        for (int i = 0; i < size; i++)
        {
            for(int j = 0; j < m_dimension; j++)//read data consequently from array
                x[j] = data[i*m_dimension+j];
            currCost += GetLabel(x, &label);

            counts[label]++;//cumulate the number of cluster
            for (int d = 0; d < m_dimension; d++)
            {
                next_means[label][d] += x[d];
            }
        }
        currCost /= size;

        // Reestimation
        for (int i = 0; i < m_clusterNum; i++)
        {
            if (counts[i] > 0)
            {
                for (int d = 0; d < m_dimension; d++)
                {
                    next_means[i][d] /= counts[i];
                }
                memcpy(m_centroid[i], next_means[i], sizeof(double) * m_dimension);
            }
        }

        // Terminal conditions
        iterNum++;
        if (fabs(lastCost - currCost) < m_endError * lastCost)
        {
            unchanged++;
        }
        if (iterNum >= m_maxIterNum || unchanged >= 3)
        {
            loop = false;
        }

        //DEBUG
        //cout << "Iter: " << iterNum << ", Average Cost: " << currCost << endl;
    }

    // Output the label file
    for (int i = 0; i < size; i++)
    {
        for(int j = 0; j < m_dimension; j++)
            x[j] = data[i*m_dimension+j];
        GetLabel(x, &label);
        Label[i] = label;
    }
    delete[] counts;
    delete[] x;
    for (int i = 0; i < m_clusterNum; i++)
    {
        delete[] next_means[i];
    }
    delete[] next_means;
}