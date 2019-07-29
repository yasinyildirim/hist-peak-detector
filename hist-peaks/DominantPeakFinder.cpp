#include "DominantPeakFinder.h"
#include <cmath>

std::vector<Peak> DominantPeakFinder::run( std::vector<int>& signal)
{
	auto peakArr = std::vector<Peak>();
	auto peakIndices = std::vector< int>(signal.size(), -1 );
	auto indices = std::vector<unsigned int>(signal.size());
	for(size_t i = 0; i < indices.size(); ++i )
	{
		indices[i] = i;
	}
	//sort it in descending order
	std::sort(indices.begin(), indices.end(),
		[&signal](unsigned int l, unsigned int r) {
			return signal[l] > signal[r]; });

	for(auto idx : indices)
	{
		const auto has_left = idx > 0 && peakIndices[idx - 1] != -1;
		const auto has_right = idx < signal.size() - 1 && peakIndices[idx + 1] != -1;
		auto peakIdLeft = -1, peakIdRight = -1;
		if (has_left)
			peakIdLeft = peakIndices[idx - 1];
		if (has_right)
			peakIdRight = peakIndices[idx + 1];

		if (has_left && has_right)
			performUnion(signal, peakArr, peakIndices, idx, peakIdLeft, peakIdRight);
		
		if (has_left && !has_right)
		{
			peakArr[peakIdLeft].right += 1;
			peakIndices[idx] = peakIdLeft;
		}

		if (!has_left && has_right)
		{
			peakArr[peakIdRight].left -= 1;
			peakIndices[idx] = peakIdRight;
		}

		if (!has_left && !has_right)
		{
			peakArr.push_back(Peak(idx));
			peakIndices[idx] = peakArr.size() - 1;
		}		
	}
	//sort according to its persistence
	std::sort(peakArr.begin(), peakArr.end(),
		[&signal](const Peak& r, const Peak& l)
		{
			return r.getPersistence(signal) > l.getPersistence(signal);
		});
	auto dominantPeaks = outlierDetection(peakArr, signal, 10.5);

	return dominantPeaks;
}

std::vector<Peak> DominantPeakFinder::outlierDetection(
	std::vector<Peak>& peaks,
	 std::vector<int>& hist, double cutoffValue)
{
	const double PERCENTILE_75 = 0.6745;
	auto dominantPeaks = std::vector<Peak>();

	auto median = calcMedian(peaks, hist);
	
	auto medianDev = calcMedianDev(peaks, hist, median);
	
	for (auto p : peaks)
	{
		double mz_score = medianDev > 0.001 ? PERCENTILE_75 * 
			(p.getPersistence(hist) - median) / medianDev :
		PERCENTILE_75 *
			(p.getPersistence(hist) - median);

		if(fabs(mz_score) > cutoffValue)
		{
			dominantPeaks.push_back(p);
		}
	}

	std::sort(dominantPeaks.begin(),
		dominantPeaks.end(),
		[&hist](const Peak& r, const Peak& l)
		{
			return r.getPersistence(hist) > l.getPersistence(hist);
		});
	return dominantPeaks;
}

double DominantPeakFinder::calcMedian( std::vector<Peak>& seq,
	 std::vector<int>& hist) {
	if (seq.empty())
		return 0.0;
	auto midIt = seq.begin() + seq.size() / 2;

	std::nth_element(seq.begin(), midIt, seq.end(),
		[ hist](const Peak& l, const Peak& r) {
			return l.getPersistence(hist) < r.getPersistence(hist); });

	if (seq.size() % 2 == 0)
	{
		auto midIt2 = std::max_element(seq.begin(), midIt, 
			[&hist](const Peak& l, const Peak& r) {
				return l.getPersistence(hist) < r.getPersistence(hist); });
		return (static_cast<double>(midIt2->getPersistence(hist))
			+ midIt->getPersistence(hist)) / 2.;
	}
	else 
	{
		return static_cast<double>(midIt->getPersistence(hist));
	}
}

double DominantPeakFinder::calcMedianDev(
	const std::vector<Peak>& seq,
	const std::vector<int>& hist, double median)
{
	double medianDev = 0.;
	std::vector<double> medianDiffs;
	for (auto p : seq)
	{
		medianDiffs.push_back(fabs(median - p.getPersistence(hist)));	
	}

	const auto midIt = medianDiffs.begin() + medianDiffs.size() / 2;
	
	std::nth_element(medianDiffs.begin(), midIt, medianDiffs.end());
	if (medianDiffs.size() % 2 == 0) {
		const auto midIt2 = std::max_element(medianDiffs.begin(), midIt);
		medianDev = (*midIt2
			+ *midIt) / 2;
	}
	else {
		medianDev = *midIt;
	}
	return medianDev;
}
