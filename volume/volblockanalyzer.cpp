#include "volblockanalyzer.h"
#include "scythe/scythestat/stat.h"
#include <numeric>

using namespace std;
using namespace scythe;
VolBlockAnalyzer::VolBlockAnalyzer()
{

}

float VolBlockAnalyzer::blockCorr(const VolumeBlock *b1, const VolumeBlock *b2)
{
//    Matrix<>;
//    scythe::mean();
    return 0.f;
}

void VolBlockAnalyzer::InblockAnalysis(const VolumeBlock* b, StatInfo& stats)
{
    float* data = b->Data();
    UINT64VECTOR3 dim = b->Dim();
    // flatten the data: each line is a slice of the block
    scythe::Matrix<float> mat_b(dim.volume(), 1, data); // each data is scalar: rows , num of data; cols, data dim
    float mu = scythe::mean(mat_b);
    float median = scythe::median(mat_b);
    float var = scythe::var(mat_b);
    stats._mu = mu;
    stats._var = var;
    stats._median = median;
}

void VolBlockAnalyzer::ensemble_inBlockAnalysis(const EnsembleVolBlock* eb, StatInfo& propagated_block_stats)
{

    float* ens_data = eb->Data();
    UINT64VECTOR3 dim = eb->Dim();
    int runs = eb->numEnsembles();

    float block_mu = 0.0f;
    float block_med = 0.0f;
    float block_var = 0.0f;
    float block_min = numeric_limits<float>::max();
    float block_max = -numeric_limits<float>::max();

    for (UINT64 i = 0; i < dim.volume(); i++)
    {
        scythe::Matrix<float> dat(1, runs, &ens_data[i * runs]);
        float r_mu = mean(dat);
        float r_med = median(dat);
        float r_var = var(dat);
        float r_min = scythe::min(dat);
        float r_max = scythe::max(dat);
        // propagate to a single number for the whole block
        // Acoording to 
        //J.Görtler, C.Schulz, D.Weiskopf, O.Deussen
        //	Bubble Treemaps For Uncertainty Visualization
        //IEEE Transactions on Visualization and Computer Graphics (2018)
        // mu is simply added together, std aggregates using Euclidiean norm === sum up var
        // TODO: is the mu a sum???
        block_mu += r_mu; // is it true????
        block_var += r_var;
        block_min = MIN(r_min, block_min);
        block_max = MIN(r_max, block_max);
        // median has to be sorted
    }

    scythe::Matrix<float> mat_eb(dim.volume() * runs, 1, ens_data);
    block_med = median(mat_eb);

    propagated_block_stats._mu = block_mu;
    propagated_block_stats._median = block_med;
    propagated_block_stats._var = block_var;
    propagated_block_stats._max = block_max;
    propagated_block_stats._min = block_min;
}

void VolBlockAnalyzer::ensemble_propStats(const std::vector<StatInfo>& childrenStats, StatInfo & propagated_block_stats)
{
	float p_mu = 0.0f;
	float p_med = 0.0f;
	float p_var = 0.0f;
	float p_min = numeric_limits<float>::max();
	float p_max = -numeric_limits<float>::max();
	for (size_t i = 0; i < childrenStats.size(); i++)
	{
		p_mu += childrenStats[i]._mu;
		p_var += childrenStats[i]._var;
		p_min = MIN(childrenStats[i]._min, p_min);
		p_max = MAX(childrenStats[i]._max, p_max);
	}

	propagated_block_stats._max = p_max;
	propagated_block_stats._min = p_min;
	propagated_block_stats._median = p_med;
	propagated_block_stats._var = p_var;
}

void VolBlockAnalyzer::ensemble_neighborBlocksAnalysis(const std::vector<EnsembleVolBlock*>& ebList, const UINT64VECTOR3 & ebListDim, FLOATVECTOR3 blockPos, StatInfo & blockStats)
{
	// Compute similarity measures for a node's neighbors
	UINT64VECTOR3 curNodePos = UINT64VECTOR3(UINT64(float(ebListDim.x) * blockPos.x), 
		UINT64(float(ebListDim.y) * blockPos.y), UINT64(float(ebListDim.z) * blockPos.z));
	INTVECTOR3 iCurNodePos = INTVECTOR3(curNodePos.x, curNodePos.y, curNodePos.z);
	UINT64 curId = (curNodePos.z * ebListDim.y + curNodePos.y) * ebListDim.x + curNodePos.x;
	int numNeighbors = g_params.ensStatNumNeighbors();
	if (numNeighbors != 8 && numNeighbors != 27)
		numNeighbors = 8;
	blockStats._statDists.resize(numNeighbors);
	// check the neighborhood
	int cnt = 0;
	for (int nz = -1; nz <= 1; nz++)
	{
		for (int ny = -1; ny <= 1; ny++)
		{
			for (int nx = -1; nx <= 1; nx++)
			{
				if (nx == 0 && ny == 0 && nz == 0)
					continue;
				INTVECTOR3 npos = iCurNodePos + INTVECTOR3(nx, ny, nz);			
				if (numNeighbors == 8)
				{
					if(abs(nx) + abs(ny) + abs(nz) > 1)
						continue; // just consider voxels that are 1-voxel away from center
				}
				if (npos.minVal() < 0 || npos.x >= ebListDim.x || npos.y >= ebListDim.y || npos.z >= ebListDim.z) //if the neighbor is out of bound
				{
					blockStats._statDists[cnt].clear(); // similarity information doesn't exist.
				}
				else
				{
					UINT64 nId = (UINT64(npos.z) * ebListDim.y + UINT64(npos.y)) * ebListDim.x + UINT64(npos.x);
					vector<float> statDists = ensemble_betweenBlockAnalysis(ebList[curId], ebList[nId]);

					blockStats._statDists[cnt] = statDists;
				}
				cnt++;
			}
		}
	}

}


vector<float> VolBlockAnalyzer::ensemble_betweenBlockAnalysis(const EnsembleVolBlock * eb1, const EnsembleVolBlock * eb2)
{
	UINT64VECTOR3 ebDim = eb1->Dim();
	vector<float> ebDists(ebDim.volume(), 0.0f); // the array of statistical divergence
	float * peb1 = eb1->Data();
	float * peb2 = eb2->Data();
	int num_runs = eb1->numEnsembles();


	for (UINT64 z = 0; z < ebDim.z; z++)
	{
		for (UINT64 y = 0; y < ebDim.y; y++)
		{
			for (UINT64 x = 0; x < ebDim.x; x++)
			{
				UINT64 id = (z * ebDim.y + y) * ebDim.x + x;
				// compute KL divergence
		
				vector<float> p(&peb1[id * num_runs], &peb1[(id + 1)*num_runs]);
				vector<float> q(&peb2[id * num_runs], &peb2[(id + 1)*num_runs]);

				float sump = 0.f;
				float sumq = 0.f;

				vector<float> np = normalizeDistr(p, sump);
				vector<float> nq = normalizeDistr(q, sumq);
				// can use any divergence(distance) measurements
				if (sump * sumq == 0.0f)
					ebDists[id] = 0.f; // either of the arrays has probability of 0!!!
				else
					ebDists[id] = KLdivergence(np, nq);
			}
		}
	}
	return ebDists;
}

