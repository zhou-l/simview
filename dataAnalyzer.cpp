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

void DataAnalyzer::analyzeEnsembleRuns(const KD<spatialDataPt*>& kdtree, vector<vector<float>>& ensemble)
{
	BSTNode<int, spatialDataPt*>* root = dynamic_cast<BSTNode<int,spatialDataPt*>*>(kdtree.rootNode());
	BinNode<spatialDataPt*>* node = root;
	// Traverse the tree
	while (node != NULL)
	{
		kdtree_ensemble_analysis(node->left(), ensemble);
		kdtree_ensemble_analysis(node->right(), ensemble);
	}

}

void DataAnalyzer::kdtree_ensemble_analysis(BinNode<spatialDataPt*>* kdnode, vector<vector<float>>& ensemble)
{
	if (!kdnode->isLeaf())
	{
		kdtree_ensemble_analysis(kdnode->left(), ensemble);
		kdtree_ensemble_analysis(kdnode->left(), ensemble);
	}
	else
	{
		if (kdnode == NULL)
			return; 
		// compute ensemble statistics

	}
}