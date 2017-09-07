#include "params.h"


Params::Params():
	_meshKDtree(3)
{
}

Params::~Params()
{
	_meshKDtree.clear();
}

const std::vector<std::vector<float> >& Params::pointData() const
{
    return _pointData;
}

void Params::setPointData(const std::vector<std::vector<float> > &pointData)
{
    _pointData = pointData;
}

const std::vector<Vertex>& Params::vertices() const
{
    return _vertices;
}

void Params::setVertices(const std::vector<Vertex> &vertices)
{
    _vertices = vertices;
}
