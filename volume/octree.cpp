#include "octree.h"
#include "VolumeData.h"
#include "volumeblock.h"
#include "volblockanalyzer.h"
#include <iostream>

using namespace std;
octree::octree(OCT_TYPE type) :
	_type(type),
	_root(NULL)
{

}

octree::~octree()
{
	octreeNode* node = _root;
	if(node)
	{
		octreeDestroy(node);
	}
}

bool octree::build(int levels)
{
	// Subdivide the volume to blockSize
	cout << __FUNCTION__ << "...";
	// pad if necessary
	_tree_levels = levels;
	// we have padded volume now!
	_root = new octreeNode();
	_root->_level = _tree_levels;
	octreeBuild(_root);

	cout << "done!" << endl;
	return false;
}

void octree::fillInEnsembleData(std::vector<VolumeData*>& ensVols, int blockSize)
{
	if (_type != OCT_ENSEMBLE)
	{
		cout << "Error octree type! OCT_ENSEMBLE required!" << endl;
		return;
	}
	cout << __FUNCTION__ << "...";
	octreeFillEnsembleData(_root, ensVols, blockSize);
	cout << "done!" << endl;
}

void octree::analyzeEnsembleData(std::vector<VolumeData*>& ensVols, VolBlockAnalyzer * analyzer)
{
	if (_type != OCT_ENSEMBLE)
	{
		cout << "Error octree type! OCT_ENSEMBLE required!" << endl;
		return;
	}
	octreeAnalyzeEnsData(_root, ensVols, analyzer);
}

void octree::octreeBuild(octreeNode* node)
{
	// recursively build the octree
	if (node)
	{
		if (node->_level > 0)
		{
			float portion = 1.0f / float(2 << ( _tree_levels - node->_level - 1));
			for (int z = 0; z < 2; z++)
			{
				for (int y = 0; y < 2; y++)
				{
					for (int x = 0; x < 2; x++)
					{
						int i = (z * 2 + y) * 2 + x;
						node->_children[i] = new octreeNode();
						node->_children[i]->_parent = node;
						node->_children[i]->_level = node->_level - 1;
						node->_volPortion = portion * FLOATVECTOR3(float(x), float(y), float(z));
						// depth build
						octreeBuild(node->_children[i]);
					}
				}
			}
		}
		else
			return;		
	}
}

void octree::octreeDestroy(octreeNode * node)
{
	if (node == NULL)
		return;

	for (int i = 0; i < 8; i++)
	{
		octreeDestroy(node->_children[i]);
	}
	node->_parent = NULL;
	SAFE_DELETE(node);
		
}

void octree::octreeFillEnsembleData(octreeNode * node, std::vector<VolumeData*>& ensVols, int blockSize)
{
	if (node == NULL)
		return;
	for (int i = 0; i < 8; i++)
	{
		octreeFillEnsembleData(node->_children[i], ensVols, blockSize);
	}

	EnsembleVolBlock* blk = NULL;
	if(node->_level == 0) // leaf nodes
	{ 
		UINT64VECTOR3 volDim = ensVols[0]->getDim();
		UINT64VECTOR3 brickStart = UINT64VECTOR3(UINT64(float(volDim.x) * node->_volPortion.x),
			UINT64(float(volDim.y) * node->_volPortion.y), 
			UINT64(float(volDim.z) * node->_volPortion.z));
		blk = new EnsembleVolBlock(ensVols, brickStart, UINT64VECTOR3(blockSize, blockSize, blockSize));
	}
	node->_data = blk;

}

void octree::octreeAnalyzeEnsData(octreeNode * node, std::vector<VolumeData*>& ensVols, VolBlockAnalyzer * analyzer)
{
	if (node == NULL)
		return;
	for (int i = 0; i < 8; i++)
		octreeAnalyzeEnsData(node->_children[i], ensVols, analyzer);
	if (node->_level == 0)
	{
		// compute the root node statistics
		analyzer->ensemble_inBlockAnalysis(reinterpret_cast<EnsembleVolBlock*>(node->_data), node->_statInfo);
		analyzer->ensemble_neighborBlocksAnalysis(reinterpret_cast<EnsembleVolBlock*>(node->_data), ensVols, node->_volPortion);
	}
	else
	{
		vector<StatInfo> childrenStats(8); // gather stats from children nodes!
		for (int i = 0; i < 8; i++)
			childrenStats[i] = node->_children[i]->_statInfo;
		analyzer->ensemble_propStats(childrenStats, node->_statInfo);
	}
}