#include "CRF.h"

float MLearn::CRF::getFeatureWeight(std::pair<std::string, std::string> featureFunction)
{
	if (!featureFunctionWeights.contains(featureFunction))
	{
		featureFunctionWeights[featureFunction] = 0.01f;
	}

	return featureFunctionWeights[featureFunction];
}

void MLearn::CRF::createProbabilityMatrix(std::vector<std::string>& features, const std::vector<std::string>* tags, int position, int tagOverride)
{
	//matrix of size tags.size() by tags.size()
	std::vector<std::vector<float>> matrix;


	//make sure tagOverride is -1 or points to a valid tag
	if (tagOverride < -1 or tagOverride >= (int)tags->size()) //apparently, for some flippity doo-daa reason, -1 is always > that the size of a vector, but not > than the size as an int
	{
		throw CRF_Error("CRF::createProbabilityMatrix(\"tagOverride Must be a Valid Tag Index in \"tags\" Vector or -1\")");
	}


	//vector of probabilities of the previous tag
	std::vector<float> prevTagProbs;

	//check if the current tag is predefined
	if (tagOverride == -1)
	{

//haHA WARNING DISABLE FOR THE WIN
#pragma warning(disable:26451)

		//get probabilities of previous tags by summing the columns in the previous probability matrix
		prevTagProbs = Custom::Matrix::sumColumns<float, Custom::Matrix::Fmatrix>(probabilityMatrices[position - 1]); 
		
		//I have no flipping idea why the heck Visual Studio thinks there is arithmatic overflow on this line but it's annoying me

#pragma warning(default:26451)


		//set each row in matrix equal to the probability of the previous matrix's respective column
		//remember that each index in a row is the same and adds up to the previous tag's probability * tags.size() 
		//and that each column is the normalized probability of all previous tags that adds up to 1
		for (int i = 0; i < tags->size(); i++)
		{
			matrix.push_back(std::vector<float>(tags->size(), prevTagProbs[i]));
		}


		//the sum of probabilities of previous tag should always add up to 1 * tags.size()
		//since each index in a row is the probability of the previous matrix's column
		//and the sum of each current column adds up to 1
		//so we start with that and add the new values

		//total value for normalizing the probabilities
		float totalValue = 1 * tags->size();


		//push back weights for feature functions to get probabilities at position 1 since position 0 is always NUL tag
		for (const auto& i : features)
		{
			for (int j = 0; j < tags->size(); j++)
			{
				//weight of feature function
				float temp = getFeatureWeight(std::pair<std::string, std::string>(tags->at(j), i));

				for (int k = 0; k < tags->size(); k++)
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
		//set the current matrix equal to a matrix of zeros with size of tags.size() by tags.size()
		matrix = std::vector<std::vector<float>>(tags->size(), std::vector<float>(tags->size(), 0));


		//get previous tag probabilities if current position is not 0
		if (position > 0)
		{

//stupid 26451 warning
#pragma warning(disable:26451)

			prevTagProbs = Custom::Matrix::sumColumns<float, Custom::Matrix::Fmatrix>(probabilityMatrices[position - 1]);

			for (int i = 0; i < tags->size(); i++)
			{
				matrix[i][tagOverride] = prevTagProbs[i];
			}

#pragma warning(default:26451)

		}
		else
		{
			for (int i = 0; i < tags->size(); i++)
			{
				matrix[i][tagOverride] = 1 / tags->size();
			}
		}
	}


	//add matrix to probability matrices
	probabilityMatrices.push_back(matrix);
}


