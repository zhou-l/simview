#pragma once
#include "KDtree.h"
#include "spatialDataPt.h"

class dataAnalyzer
{
public:
	dataAnalyzer();
	~dataAnalyzer();
private:
	// KD tree using 3D fp-coordinates as key, id as value?
	KD<spatialDataPt>*         _kdtree;
};

