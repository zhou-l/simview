#ifndef OCTREE_H
#define OCTREE_H

class VolumeData;
class octree
{
public:
    octree();

    bool build(VolumeData* vol);
private:
    octree* _children[8];
    float   _minval; // change to correlation measures?
    float   _maxval;
};

#endif // OCTREE_H
