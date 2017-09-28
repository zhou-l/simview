#include "octree.h"
#include "VolumeData.h"
#include "volumeblock.h"
#include "volblockanalyzer.h"
#include "graph\massspringsystem.h"
#include <iostream>

using namespace std;
// helper functions
UINT64VECTOR3             ensVolBlkListDim;
vector<EnsembleVolBlock*> ensVolBlocks;

octree::octree(OCT_TYPE type) :
	_type(type),
	_root(NULL),
	_isWrite(false)
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
	_root->_level = _tree_levels - 1;
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
	// resize with correct number of blocks
	ensVolBlocks.clear();
	UINT64 numBlkPerDim = 1 << (_tree_levels - 1);
	ensVolBlkListDim = UINT64VECTOR3(numBlkPerDim, numBlkPerDim, numBlkPerDim);
	ensVolBlocks.resize(ensVolBlkListDim.volume());
	cout <<endl<< "Number of ensemble blocks = " << ensVolBlkListDim << endl;

	octreeFillEnsembleData(_root, ensVols, blockSize);
	//set global param
	g_params.ensVolBlkListDim(ensVolBlkListDim);
	g_params.ensVolBlocks(ensVolBlocks);
	cout << "done!" << endl;
}

void octree::analyzeEnsembleData(const std::vector<EnsembleVolBlock*>& ebList, const UINT64VECTOR3& ebListDim, VolBlockAnalyzer * analyzer)
{
	if (_type != OCT_ENSEMBLE)
	{
		cout << "Error octree type! OCT_ENSEMBLE required!" << endl;
		return;
	}
	cout << __FUNCTION__ << "...";
	octreeAnalyzeEnsData(_root, ebList, ebListDim, analyzer);
	cout << "done!" << endl;
}

void octree::setupMassSpringSys(MassSpringSystem** ppMassSpring)
{
	if (*ppMassSpring)
	{
		SAFE_DELETE(*ppMassSpring);
	}
	
	*ppMassSpring = new MassSpringSystem();
	cout << __FUNCTION__ << "...";
	octreeSetMassSpringSys(_root, ppMassSpring);
	cout << "done!" << endl;
}

void octree::beginOutputContent(std::string & filename)
{
	_ofs.open(filename.c_str(), std::ios_base::out);
	if (!_ofs.is_open())
	{
		cout << "Error opening file " << filename << " for writing!" << endl;
		return;
	}
	switch(_type)
	{
	case OCT_ENSEMBLE:
		_ofs << "Ensemble octree content = " << endl;
		break;
	case OCT_SINGLE:
		_ofs << "Single octree content = " << endl;
		break;
	default:
		_ofs << "Unknown type octree content = " << endl;
		break;
	}
	_isWrite = true;
	//octreeWriteContent();
}

void octree::endOutputContent()
{
	if (_ofs.is_open())
		_ofs.close();
	_isWrite = false;
	cout << "Write out octree to file done!" << endl;
}

void octree::octreeBuild(octreeNode* node)
{
	// recursively build the octree
	if (node)
	{
		if (node->_level > 0)
		{
			if (node->_level == _tree_levels - 1)
				node->_volStartPos = FLOATVECTOR3(0, 0, 0);
			float portion = 1.0f / float(1 << ( _tree_levels - node->_level));
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
						node->_children[i]->_volStartPos = node->_volStartPos + portion * FLOATVECTOR3(float(x), float(y), float(z));
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
		UINT64VECTOR3 brickStart = UINT64VECTOR3(UINT64(float(volDim.x) * node->_volStartPos.x),
			UINT64(float(volDim.y) * node->_volStartPos.y), 
			UINT64(float(volDim.z) * node->_volStartPos.z));
		blk = new EnsembleVolBlock(ensVols, brickStart, UINT64VECTOR3(blockSize, blockSize, blockSize));
		// Add to ensemble vol block list
		UINT64VECTOR3 brickPos = brickStart / UINT64(blockSize);
		UINT64 blkInd = (brickPos.z * ensVolBlkListDim.y + brickPos.y) * ensVolBlkListDim.x + brickPos.x;
		ensVolBlocks[blkInd] = blk;

	}
	node->_data = blk;

}

void octree::octreeAnalyzeEnsData(octreeNode * node, const std::vector<EnsembleVolBlock*>& ebList, const UINT64VECTOR3& ebListDim, VolBlockAnalyzer * analyzer)
{
	if (node == NULL)
		return;
	for (int i = 0; i < 8; i++)
		octreeAnalyzeEnsData(node->_children[i], ebList, ebListDim, analyzer);
	if (node->_level == 0)
	{
		// compute the leaf node statistics
		
		analyzer->ensemble_inBlockAnalysis(reinterpret_cast<EnsembleVolBlock*>(node->_data), node->_statInfo);
		analyzer->ensemble_neighborBlocksAnalysis(ebList, ebListDim,
			node->_volStartPos, node->_statInfo);

		if (_isWrite && _ofs.is_open())
		{
			_ofs << "Node level = " << node->_level << " start pos = " << node->_volStartPos << endl;
			_ofs << node->_statInfo << endl;
		}
	}
	else
	{
		vector<StatInfo> childrenStats(8); // gather stats from children nodes!
		for (int i = 0; i < 8; i++)
			childrenStats[i] = node->_children[i]->_statInfo;
		analyzer->ensemble_propStats(childrenStats, node->_statInfo);
		if (_isWrite && _ofs.is_open())
		{
			_ofs << "Node level = " << node->_level << " start pos = " << node->_volStartPos << endl;
			_ofs << node->_statInfo << endl;
		}
	}
}

void octree::octreeSetMassSpringSys(octreeNode * node, MassSpringSystem ** ppMassSpring)
{
	// For now, use leaf nodes only
	if (node == NULL)
		return;
	for (int i = 0; i < 8; i++)
	{
		octreeSetMassSpringSys(node->_children[i], ppMassSpring);
	}
	// set leaf node
	if (node->_level == 0)
	{
		FLOATVECTOR3 pos = node->_volStartPos;
		float mass = node->_statInfo._mu; // use mean value as mass?
		// A 3D graph???
		(*ppMassSpring)->addMass(mass, pos.x, pos.y, pos.z);
		(*ppMassSpring)->addSpring();
	}
}

void octree::octreeWriteContent(octreeNode * node, std::ofstream & file)
{
	if (node == NULL)
		return;
	for (int i = 0; i < 8; i++)
		octreeWriteContent(node->_children[i], file);
	file << node->_level << ": " << node->_volStartPos << endl
		<< node->_statInfo << endl;
}
