#ifndef VOLBLOCKANALYZER_H
#define VOLBLOCKANALYZER_H

#include "MyVectors.h"
#include "volumeblock.h"
class volBlockAnalyzer
{
public:
    volBlockAnalyzer();

    float blockCorr(const VolumeBlock* b1, const VolumeBlock* b2);
    float blockMean(const VolumeBlock* b);
    float blockVar(const VolumeBlock* b);

private:


    UINT64VECTOR3 blockSize;
};

#endif // VOLBLOCKANALYZER_H
