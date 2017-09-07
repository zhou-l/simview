#pragma once
#include "KDtree.h"
#include "spatialDataPt.h"

class DataAnalyzer
{
public:
	DataAnalyzer();
	~DataAnalyzer();

	void buildKDtree(const std::vector<FLOATVECTOR3>& mesh, KD<spatialDataPt*>& kdtree);

//	KD<spatialDataPt>* KDtree() const { return _kdtree; }
//private:
//	// KD tree using 3D fp-coordinates as key, id as value?
//	KD<spatialDataPt>*         _kdtree;
};

