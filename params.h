#ifndef PARAMS_H
#define PARAMS_H
#include "vertex.h"
#include "KDtree.h"
#include "spatialDataPt.h"
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
	
private:
    std::vector<std::vector<float>> _pointData;
	std::vector<std::vector<float>> _ensembleData;

    std::vector<Vertex>             _vertices;
	KD<spatialDataPt*>              _meshKDtree;

};

#endif // PARAMS_H
