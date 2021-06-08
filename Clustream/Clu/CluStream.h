#pragma once
#include "microcluster.h"
#include <vector>
#include <cmath>
#include <cassert>
#include <limits>
#include <cstdio>

const double double_max=std::numeric_limits<double>::max();

double distance(DataPoint& a,DataPoint& b);

class CluStream{
public:
	std::vector<Microclusters> microclusters;
	int time_window;
	int q;
	int t;

	CluStream(int h,int q,int t);
	void offline_cluster(DataPoint& datapoint,long timestamp);
};