#include "volblockanalyzer.h"

volBlockAnalyzer::volBlockAnalyzer()
{

}

float volBlockAnalyzer::blockCorr(const VolumeBlock *b1, const VolumeBlock *b2)
{
    return 0.0f;
}

float volBlockAnalyzer::blockMean(const VolumeBlock *b)
{
    float* dat = b->Data();
    UINT64VECTOR3 dim = b->Dim();
    return 0.0f;
}


