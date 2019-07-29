
#include "DominantPeakFinder.h"
#include <string>
#include "libbmp/libbmp.h"
#include <iostream>

//driver program for running the Dominant peak detection method.

//calculate histogram of the image
std::vector<int> calcHist(const std::string& imgPath, int channel)
{
	std::vector<int> histogram(256, 0);
	BmpImg img;
	img.read(imgPath);
	
	const int width = img.get_width() ;
	const int height = img.get_height() ;
	// Count the amount of black and white pixels:
	// Use negative loops to improve performance
	for (int y = height-1, x; y >= 0; y--)
	{
		for (x = width-1; x >= 0; x--)
		{

			switch (channel)
			{
			case 0:
			{
				histogram[img.red_at(x, y)] += 1;
			}
			break;
			case 1:
			{
				histogram[img.green_at(x, y)] += 1;
			}
			break;
			case 2:
			{
				histogram[img.blue_at(x, y)] += 1;
			}
			default:
				break;
			}
		}
	}
	return histogram;
}

int main(char argc, char** argv)
{
	std::string imgPath = "data/test1.bmp";
	if (argc > 1)
		imgPath = std::string(argv[1]);
	
	auto redHist = calcHist(imgPath, 0);
	auto greenHist = calcHist(imgPath, 1);
	auto blueHist = calcHist(imgPath, 2);

	DominantPeakFinder dpf;
	auto peaks = dpf.run(redHist);
	std::cout << "Result from red channel histogram: " << std::endl;
	std::cout << "peaks found at:" << std::endl;
	int idx = 1;
	for(auto p : peaks)
	{
		std::cout << idx << ". peak index: " <<p.born 
			<< " persistence: "
			<< p.getPersistence(redHist)
			<< std::endl ;
		++idx;
	}

	return 0;
}