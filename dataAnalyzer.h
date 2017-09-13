#pragma once
#include "KDtree.h"
#include "spatialDataPt.h"
#include "volume\VolumeData.h"
#include "RegularGrid.h"


class DataAnalyzer
{
public:
	DataAnalyzer();
	~DataAnalyzer();

	void buildKDtree(const std::vector<FLOATVECTOR3>& mesh, KD<spatialDataPt*>& kdtree);
	void buildRegGrid(const std::vector<FLOATVECTOR3>& mesh, RegularGrid<vector<UINT64>>& grid);
	// generate a lookup table that converts mesh point id to 3D coordinates in a volume with dimensions of "gridDim"
	// volPtCnt -- a volume recording number of points in each cell
	void convertMeshIdToGridPos(std::vector<UINT64VECTOR3>& volCoord_lookupTbl, VolumeData** volPtCnt, const std::vector<FLOATVECTOR3>& mesh, UINT64VECTOR3 gridDim);
	// convert ensemble point data to volumes
	void convertEnsembleRunsToVol(std::vector<VolumeData*>& vols, const std::vector<std::vector<float> >& ensembles,
		const UINT64VECTOR3& volDim, const std::vector<UINT64VECTOR3>& posLookupTbl, const VolumeData& volPtCnt);
	// Analyze ensembles
	// Analyze using KD-tree (not helpful though...)
	void analyzeEnsembleRuns(const KD<spatialDataPt*>& kdtree, vector<vector<float> >& ensemble);
	// Analyze with neighboring node similarity(?)
	void analyzeEnsembleNodes(std::vector<VolumeData*>& ensembleVols);
	void createEnsembleOctree(std::vector<VolumeData*>& ensembleVols, octree* ensembleOctTree);

private:
	void kdtree_ensemble_analysis(BinNode<spatialDataPt*>* kdnode, vector<vector<float>>& ensemble); // recursive function
	// in-node analysis of the ensemble
	void inNodeAnalysis(std::vector<VolumeData*>& ensembleVols);
	// between-nodes analysis
	void betweenNodeAnalysis(std::vector<VolumeData*>& ensembleVols, int num_neighbors);
};

