#include "Mat.h"

Mat::V2f Mat::addOuterf(std::vector<float> v1, std::vector<float> v2)
{
	Mat::V2f returnMatrix(v1.size(), v2.size());

	for (int i = 0; i < v1.size(); i++)
	{
		for (int j = 0; j < v2.size(); j++)
		{
			returnMatrix[i][j] = v1[i] + v2[j];
		}
	}

	return returnMatrix;
}

Mat::V2i Mat::addOuteri(std::vector<int> v1, std::vector<int> v2)
{
	Mat::V2i returnMatrix(v1.size(), v2.size());

	for (int i = 0; i < v1.size(); i++)
	{
		for (int j = 0; j < v2.size(); j++)
		{
			returnMatrix[i][j] = v1[i] + v2[j];
		}
	}

	return returnMatrix;
}

Mat::V2d Mat::addOuterd(std::vector<double> v1, std::vector<double> v2)
{
	Mat::V2d returnMatrix(v1.size(), v2.size());

	for (int i = 0; i < v1.size(); i++)
	{
		for (int j = 0; j < v2.size(); j++)
		{
			returnMatrix[i][j] = v1[i] + v2[j];
		}
	}

	return returnMatrix;
}