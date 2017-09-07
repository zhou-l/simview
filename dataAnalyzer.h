#pragma once
#include "KDtree.h"
#include "spatialDataPt.h"

class DataAnalyzer
{
public:
	DataAnalyzer();
	~DataAnalyzer();

	void buildKDtree(const std::vector<FLOATVECTOR3>& mesh, KD<spatialDataPt*>& kdtree);

	// Analyze ensemble
	void analyzeEnsembleRuns(const KD<spatialDataPt*>& kdtree, vector<vector<float> >& ensemble);

private:
	void kdtree_ensemble_analysis(BinNode<spatialDataPt*>* kdnode, vector<vector<float>>& ensemble); // recursive function
};

