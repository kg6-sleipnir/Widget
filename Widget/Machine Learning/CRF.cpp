#include "CRF.h"

float MLearn::CRF::getFeatureWeight(std::pair<std::string, std::string> featureFunction)
{
	if (!featureFunctionWeights.contains(featureFunction))
	{
		featureFunctionWeights[featureFunction] = 0.01f;
	}

	return featureFunctionWeights[featureFunction];
}

void MLearn::CRF::createProbabilityMatrix(std::vector<std::string> &features, int position)
{
	std::vector<std::vector<float>> matrix;

	if (position != 1)
	{
		//get probabilities of previous tags by summing the columns in the previous probability matrix
		//I have no flipping idea why the heck Visual Studio thinks there is arithmatic overflow on this line but it's annoying me
		std::vector<float> prevTagProbs = Custom::Matrix::sumColumns<float, Custom::Matrix::Fmatrix>(probabilityMatrices[position - 1]);


		for (int i = 0; i < tags.size(); i++)
		{
			std::vector<float> temp;

			for (int j = 0; j < tags.size(); j++)
			{
				temp.push_back(prevTagProbs[i]);
			}

			matrix.push_back(temp);
		}
	}
	else
	{
		for (int i = 0; i < tags.size(); i++)
		{
			std::vector<float> temp;

			for (int j = 0; j < tags.size(); j++)
			{
				temp.push_back(0);
			}

			matrix.push_back(temp);
		}
	}


	if (position != 1)
	{
		//total value for normalizing the probabilities
		//sum of probabilities of previous tag should always add up to 1
		//so we start with that and add the new values
		float totalValue = 1;


		//push back weights for feature functions to get probabilities at position 1 since position 0 is always NUL tag
		for (const auto& i : features)
		{
			for (int j = 0; j < tags.size(); j++)
			{
				//weight of feature function
				float temp = getFeatureWeight(std::pair<std::string, std::string>(tags[j], i));

				for (int k = 0; k < tags.size(); k++)
				{
					//add to probability of tag being paired with previous tag
					matrix[k][j] += temp;

					//add to total value
					totalValue += temp;
				}
			}
		}


		//normalize probabilities
		for (auto& i : matrix)
		{
			for (auto& j : i)
			{
				j /= totalValue;
			}
		}
	}
	else
	{
		//total value for normalizing the probabilities
		float totalValue = 0;


		//push back weights for feature functions to get probabilities at position 1 since position 0 is always starting tag
		for (const auto& i : features)
		{
			for (int j = 0; j < tags.size(); j++)
			{
				//weight of feature function
				float temp = getFeatureWeight(std::pair<std::string, std::string>(tags[j], i));

				//add to probability of tag with previous tag being starting tag
				matrix[0][j] += temp;

				//add to total value
				totalValue += temp;
			}
		}


		//normalize the probabilities
		for (auto& i : matrix[0])
		{
			i /= totalValue;
		}
	}


	//add matrix to probability matrices
	probabilityMatrices.push_back(matrix);
}


