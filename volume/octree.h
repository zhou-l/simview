#ifndef OCTREE_H
#define OCTREE_H
#include "MyVectors.h"
#include "octreenode.h"
#include <fstream>
class VolBlockAnalyzer;
class VolumeData;

enum OCT_TYPE
{
	OCT_ENSEMBLE = 0,
	OCT_SINGLE
};

class octree
{
public:
    octree(OCT_TYPE type);
	~octree();

    bool build(int levels);
	void fillInEnsembleData(std::vector<VolumeData*>& ensVols, int blockSize);
	void analyzeEnsembleData(const std::vector<EnsembleVolBlock*>& ebList, const UINT64VECTOR3& ebListDim, VolBlockAnalyzer* analyzer);
	void writeContent(std::string& filename);

protected:
	// recursive functions
	void octreeBuild(octreeNode* node);
	void octreeDestroy(octreeNode* node);
	void octreeFillEnsembleData(octreeNode* node, std::vector<VolumeData*>& ensVols, int blockSize);
	void octreeAnalyzeEnsData(octreeNode* node, const std::vector<EnsembleVolBlock*>& ebList,
		const UINT64VECTOR3& ebListDim, VolBlockAnalyzer* analyzer);
	void octreeWriteContent(octreeNode* node, std::ofstream& file);

    octreeNode* _root;

	int      _tree_levels;
	OCT_TYPE _type;
};



#endif // OCTREE_H
