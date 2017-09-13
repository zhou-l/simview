#pragma once
// the statistics information of a node

#include <vector>
#include "global.h"

class StatInfo
{
public:
	StatInfo():
		_mu(0.0f),
		_var(0.0f),
		_median(0.0f),
		_min(0.0f),
		_max(0.0f)
	{
		_outliers.resize(g_params.ensStatNumOutliers(), 0.0f);
		_statDists.resize(g_params.ensStatNumNeighbors(), 0.0f);
	}

	~StatInfo() {}


public:
	float _mu;
	float _var;
	float _median;
	float _min;
	float _max;

	std::vector<float> _outliers;
	// statistical differences to the node's neighbors
	std::vector<float> _statDists;
};