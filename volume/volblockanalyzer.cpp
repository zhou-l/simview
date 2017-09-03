#include "volblockanalyzer.h"
#include "scythe/scythestat/stat.h"

using namespace scythe;
volBlockAnalyzer::volBlockAnalyzer()
{

}

float volBlockAnalyzer::blockCorr(const VolumeBlock *b1, const VolumeBlock *b2)
{
//    Matrix<>;
//    scythe::mean();
    return 0.f;
}

float volBlockAnalyzer::blockMean(const VolumeBlock *b)
{
    float* dat = b->Data();
    UINT64VECTOR3 dim = b->Dim();
    return 0.f;
}


