#include "CRF.h"

float MLearn::CRF::getFeatureWeight(std::pair<std::string, std::string> featureFunction)
{
	if (!featureFunctionWeights.contains(featureFunction))
	{
		featureFunctionWeights[featureFunction] = (float)rand() / RAND_MAX / 10;
	}

	return featureFunctionWeights[featureFunction];
}

void MLearn::CRF::createProbabilityMatrix(std::vector<std::string>& features, const std::vector<std::string>* tags, int position, int tagOverride)
{
	//matrix of size tags.size() by tags.size()
	Matrix::Fmatrix matrix = std::vector<std::vector<float>>(tags->size(), std::vector<float>(tags->size(), 0));

	
	//make sure tagOverride is -1 or points to a valid tag
	if (tagOverride < -1 or tagOverride >= (int)tags->size()) //apparently, for some flippity doo-daa reason, -1 is always > that the size of a vector, but not > than the size as an int
	{
		throw CRF_Error("CRF::createProbabilityMatrix(\"tagOverride Must be a Valid Tag Index in \"tags\" Vector or -1\")");
	}

	
	//check if the current tag is not predefined
	if (tagOverride == -1)
	{
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
				}
			}
		}
	}
	else 
	{
		//get previous tag probabilities if current position is not 0
		if (position > 0)
		{

//haHA WARNING DISABLE FOR THE WIN
#pragma warning(disable:26451)

			//get probabilities of previous tags by summing the columns in the previous probability matrix
			std::vector<float> prevTagProbs = Custom::Matrix::sumColumns<Custom::Matrix::Fmatrix>(probabilityMatrices[position - 1]);

			//I have no flipping idea why the heck Visual Studio thinks there is arithmatic overflow on this line but it's annoying me

#pragma warning(default:26451)


			for (int i = 0; i < tags->size(); i++)
			{
				matrix[i][tagOverride] = prevTagProbs[i];
			}

		}
		else
		{
			for (int i = 0; i < tags->size(); i++)
			{
				matrix[tagOverride][i] = 1.0f / tags->size();
			}
		}
	}


	//add matrix to probability matrices
	probabilityMatrices.push_back(matrix);
}

Matrix::Fmatrix MLearn::CRF::calculateForwardVector(int position)
{
	//get second probability matrix since the first is the start tag
	Custom::Matrix::Fmatrix tempMatrix = Custom::Matrix::transposeMatrix(probabilityMatrices[0]);
	

	//α{i(x)}T = α{i−1(x)}T * M{i(x)}
	//where α{0} = 1 at start tag

	//iterate over matrices and multiply them together
	for (int i = 1; i <= position; i++)
	{
		tempMatrix = Custom::Matrix::multiplyMatrix(tempMatrix, probabilityMatrices[i]);
	}


	return Matrix::transposeMatrix(tempMatrix);
}

Matrix::Fmatrix MLearn::CRF::calculateBackwardVector(int position)
{
	//get second to last probability matrix since the last is the stop tag
	Matrix::Fmatrix tempMatrix = probabilityMatrices[probabilityMatrices.size() - 1];


	//β{i(x)} = M{i + 1(x)} * β{i + 1(x)}
	//where β{n + 1} = 1 at end tag

	//reverse iterate over the probability matrices and multiply them together
	for (int i = probabilityMatrices.size() - 2; i > position; i--)
	{
		tempMatrix = Matrix::multiplyMatrix(tempMatrix, probabilityMatrices[i]);
	}

	
	return tempMatrix;
}

float MLearn::CRF::normalizeFactor(int startTagIndex, int endTagIndex)
{
	//create temporary matrix starting with the first probability matrix
	Custom::Matrix::Fmatrix tempMatrix = probabilityMatrices[0];
	

	//iterate over the probability matricex, multiplying them together
	for (int i = 1; i < probabilityMatrices.size(); i++)
	{
		tempMatrix = Custom::Matrix::multiplyMatrix(tempMatrix, probabilityMatrices[i]);
	}


	//get the entry at (start, end) as normalization factor
	return tempMatrix[startTagIndex][endTagIndex];
}

std::pair<int, int> MLearn::CRF::predictTag(int position, int startTag, int endTag)
{
	//temporarily set prediction matrix to probability matrix at wanted position
	Matrix::Fmatrix prediction = probabilityMatrices[position];

	//multiply forward vector by the probability matrix and then the resulting matrix by the backward vector
	prediction = Matrix::multiplyMatrix(calculateForwardVector(position), prediction);
	prediction = Matrix::multiplyMatrix(prediction, calculateBackwardVector(position));

	
	//Matrix::printMatrix(Matrix::multiplyMatrixBy(prediction, normalizeFactor(startTag, endTag)));


	//temp values for holding entry in prediction with highest value
	int prevTagIndex = 0;
	int tagIndex = 0;
	float max = 0;

	//find highest value in prediction
	for (int i = 0; i < prediction.size(); i++)
	{
		for (int j = 0; j < prediction[0].size(); j++)
		{
			if (prediction[i][j] > max)
			{
				prevTagIndex = i;
				tagIndex = j;
				max = prediction[i][j];
			}
		}
	}

	return std::pair<int, int>(prevTagIndex, tagIndex);
}

void MLearn::CRF::updateWeights(std::vector<std::pair<std::string, std::string>> tokenAnswers)
{
}
