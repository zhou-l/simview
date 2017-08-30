#ifndef VOLUMEBLOCK_H
#define VOLUMEBLOCK_H

class VolumeData;
class VolumeBlock
{
public:
    VolumeBlock(const VolumeData* vol, UINT64VECTOR3 blockStart, UINT64VECTOR3 blockDim);
    float* Data() const {return _data;}
    UINT64VECTOR3 Dim() const { return _dim;}
private:
    float*         _data;
    UINT64VECTOR3  _dim;
};

#endif // VOLUMEBLOCK_H
