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

	const KD<spatialDataPt*>& meshKDTree()  { return _meshKDtree; }
	void meshKDTree(const KD<spatialDataPt*>& val) { _meshKDtree = val; }
	
private:
    std::vector<std::vector<float>> _pointData;
    std::vector<Vertex>             _vertices;
	KD<spatialDataPt*>              _meshKDtree;

};

#endif // PARAMS_H
