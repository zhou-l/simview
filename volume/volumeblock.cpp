#include "volumeblock.h"
#include "VolumeData.h"

VolumeBlock::VolumeBlock(const VolumeData* vol, UINT64VECTOR3 blockStart, UINT64VECTOR3 blockDim):
    _dim(blockDim)
{
   _data = new float[blockDim.volume()];
   UINT64VECTOR3 volDim = vol->getDim();
   for(UINT64 z = blockStart.z; z < blockStart.z + blockDim.z; z++)
   {
       for(UINT64 y = blockStart.y; y < blockStart.y + blockDim.y; y++)
       {
           for(UINT64 x = blockStart.x; x < blockStart.x + blockDim.x; x++)
           {
               UINT64 blockInd = ((z - blockStart.z) * blockDim.y + (y - blockStart.y)) * blockDim.x + (x - blockStart.x);
               float val = vol->getVoxel(UINT64VECTOR3(x,y,z));

               _data[blockInd] = val;
           }
       }
   }
}
