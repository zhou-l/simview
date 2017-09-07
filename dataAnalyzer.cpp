#include "DataAnalyzer.h"


DataAnalyzer::DataAnalyzer()
{
}


DataAnalyzer::~DataAnalyzer()
{
}

void DataAnalyzer::buildKDtree(const std::vector<FLOATVECTOR3>& mesh, KD<spatialDataPt*>& kdtree)
{
	kdtree = KD<spatialDataPt*>(3);
	vector<spatialDataPt*> dataMesh(mesh.size(), 0);
	for (vector<spatialDataPt*>::iterator IT = dataMesh.begin(); IT != dataMesh.end(); ++IT)
	{
		UINT64 id = IT - dataMesh.begin();
		*IT = new spatialDataPt(mesh[id], id);
	}
	kdtree.buildBalanceTree(dataMesh);
}
