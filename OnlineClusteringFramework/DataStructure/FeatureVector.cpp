/*

In this folder we implement data structures for storing statistic summaries of data streams
 
 BIRCH, BICO and Clustream are incremental  and share a common idea for summarizing 
 the stream in microclusters, in order to cluster them in the offine phase.

This type of data struture in the stream clustering algorithms is called 'clustering feature (CF)' vectors , 
which is proposed in BIRCH, to represent microcluster.

In these algorithms, we assume dataset points are vectors of d real numbers.

In BIRCH, a feature vector is a triple (N, LS, SS) composed of the following items:

N: number of data points in the CF
LS: d-dimensional sum of the N data points
SS: d-dimensional sum of squares of the N data points.
Feature vectors have the following useful properties:


They have the following useful properties:

They are additive: CF1 + CF2 = (N1 + N2, LS1 + LS2, SS1 + SS2).
The distance from a point to a cluster, the average intercluster distance, 
and the average intracluster distance are easy to compute from the CF.


In CLUSTREAM, nodes of the CF tree maintain CF vectors of the form (N,LS,SS,LST,SST), 
which extend BIRCH CFs with two additional temporal features:

LST: sum of the timestamps
SST: sum of squares of the timestamps
*/
