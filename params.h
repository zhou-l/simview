#ifndef PARAMS_H
#define PARAMS_H
#include "vertex.h"
class Params
{
public:
    Params();

    const std::vector<std::vector<float> >& pointData() const;
    void setPointData(const std::vector<std::vector<float> > &pointData);

    const std::vector<Vertex>& vertices() const;
    void setVertices(const std::vector<Vertex> &vertices);

private:
    std::vector<std::vector<float>> _pointData;
    std::vector<Vertex>             _vertices;

};

#endif // PARAMS_H
