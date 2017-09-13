#ifndef PARAMS_H
#define PARAMS_H
#include "vertex.h"
#include "KDtree.h"
#include "spatialDataPt.h"

class VolumeData;
class Params
{
public:
    Params();
    ~Params();

    const std::vector<std::vector<float> >& pointData() const;
    void setPointData(const std::vector<std::vector<float> > &pointData);

    const std::vector<Vertex>& vertices() const;
    void setVertices(const std::vector<Vertex> &vertices);

    KD<spatialDataPt*>& meshKDTree_unsafe()  { return _meshKDtree; }
    void meshKDTree(const KD<spatialDataPt*>& val) { _meshKDtree = val; }

    const std::vector<std::vector<float> >& ensembleData() const;
    void setEnsembleData(const std::vector<std::vector<float> > &val);
    
    std::vector<VolumeData*>& ensembleVols_unsafe() { return _ensembleVols; }
    const std::vector<VolumeData*>& ensembleVols() const { return _ensembleVols; }
    void setEnsembleVols(const std::vector<VolumeData*>& val) { _ensembleVols = val; }

    const int VolBlockSize() const { return _volBlockSize; }
    void VolBlockSize(int volBkSize) { _volBlockSize = MAX(volBkSize,2); } // min block size is 2*2*2!

    const int ensStatNumOutliers() const { return _ensStatNumOutliers; }
	void ensStatNumOutliers(int val) { _ensStatNumOutliers = val; }


    const int ensStatNumNeighbors() const { return _ensStatNumNeighbors; }
	void ensStatNumNeighbors(int val) { _ensStatNumNeighbors = val; }
private:
    std::vector<std::vector<float>> _pointData;
    std::vector<std::vector<float>> _ensembleData;
    std::vector<VolumeData*>        _ensembleVols; // converted from the ensemble point data

    std::vector<Vertex>             _vertices;
    KD<spatialDataPt*>              _meshKDtree;

    int                             _volBlockSize; // the size of a basic volume block for analysis
    // params for per-node statistics
    int                             _ensStatNumOutliers; // number of outliers 
    int                             _ensStatNumNeighbors;
};

#endif // PARAMS_H
