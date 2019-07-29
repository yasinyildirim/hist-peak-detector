#pragma once
#include <vector>
#include <algorithm>

struct Peak
{
	int born, died, left, right;

	Peak( int startIdx):
		born(startIdx),
		left(startIdx),
		right(startIdx),
		died(-1)
	{		
	}
	
	int getPersistence(const std::vector<int>& signal) const
	{
		if (this->died != -1)
			return signal[born] - signal[died];
		else
			return *(std::max_element(signal.begin(), signal.end()));
	}
	
};

class DominantPeakFinder
{
public:
	std::vector<Peak> run( std::vector< int>& signal);
private:
	void performUnion(const std::vector< int>& signal,
		std::vector<Peak>& peaks,
		std::vector<int>& peakIndices,
		 int idx,
		int idxLeft,
		int idxRight);
	std::vector<Peak> outlierDetection(std::vector<Peak>& peaks,
		 std::vector<int>& hist, double cutoffValue);
	double calcMedian( std::vector<Peak>& seq,
		 std::vector<int>& hist);
	double calcMedianDev(const std::vector<Peak>& seq,
		const std::vector<int>& hist, double median);
};


inline void DominantPeakFinder::performUnion(const std::vector<int>& signal,
	std::vector<Peak>& peaks,
	std::vector<int>& peakIndices,
	 int idx,
	int idxLeft, int idxRight)
{
	if (signal[peaks[idxLeft].born] > signal[peaks[idxRight].born])
	{
		peaks[idxRight].died = idx;
		peaks[idxLeft].right = peaks[idxRight].right;
		peakIndices[idx] = idxLeft;
		peakIndices[peaks[idxLeft].right] = idxLeft;
	}
	else
	{
		peaks[idxLeft].died = idx;
		peaks[idxRight].left = peaks[idxLeft].left;
		peakIndices[idx] = idxRight;
		peakIndices[peaks[idxRight].left] = idxRight;
	}
}

