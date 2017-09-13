#ifndef OCTREE_H
#define OCTREE_H
#include "MyVectors.h"
#include "octreenode.h"
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
	void analyzeEnsembleData(std::vector<VolumeData*>& ensVols, VolBlockAnalyzer* analyzer);

protected:
	// recursive functions
	void octreeBuild(octreeNode* node);
	void octreeDestroy(octreeNode* node);
	void octreeFillEnsembleData(octreeNode* node, std::vector<VolumeData*>& ensVols, int blockSize);
	void octreeAnalyzeEnsData(octreeNode* node, std::vector<VolumeData*>& ensVols, VolBlockAnalyzer* analyzer);

    octreeNode* _root;

	int      _tree_levels;
	OCT_TYPE _type;
};



#endif // OCTREE_H
