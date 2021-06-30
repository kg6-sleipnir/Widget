#include "CRF.h"

float MLearn::CRF::getFeatureWeight(std::pair<std::string, std::string> featureFunction)
{
	if (!featureFunctionWeights.contains(featureFunction))
	{
		featureFunctionWeights[featureFunction] = (float)rand() / RAND_MAX;
	}

	return featureFunctionWeights[featureFunction];
}

void MLearn::CRF::createProbabilityMatrix(std::vector<std::string>& features, const std::vector<std::string>* tags, int position, int tagOverride)
{
	//matrix of size tags.size() by tags.size()
	Matrix::Fmatrix matrix = std::vector<std::vector<float>>(tags->size(), std::vector<float>(tags->size(), 0));

	
	
	

	
	//check if the current tag is not predefined
	if (tagOverride == -1)
	{
		//get sum of feature weights to normalize the matrix
		float sumOfFeatures = 0;

		for (const auto& i : features)
		{
			for (int j = 1; j < tags->size(); j++)
			{
				//weight of feature function
				sumOfFeatures += getFeatureWeight(std::pair<std::string, std::string>(tags->at(j), i));
			}
		}

		
		//push back weights for feature functions to get probabilities at position 1 since position 0 is always NUL tag
		for (const auto& i : features)
		{
			for (int j = 1; j < tags->size(); j++)
			{
				//weight of feature function
				float temp = (getFeatureWeight(std::pair<std::string, std::string>(tags->at(j), i)) / sumOfFeatures);

				for (int k = 1; k < tags->size(); k++)
				{
					//add to probability of tag being paired with previous tag
					matrix[k][j] += temp;
				}
			}
		}
	}
	else if (tagOverride < -1 or tagOverride >= (int)tags->size()) //apparently, for some flippity doo-daa reason, -1 is always > that the size of a vector, but not > than the size as an int
	{
		//error checking here is more efficient since we check if tagOverride is equal to -1 in the first if statement,
		//eliminating the need to do two additional checks for < -1 and >= tags->size() every iteration

		//make sure tagOverride is -1 or points to a valid tag
		throw CRF_Error("CRF::createProbabilityMatrix(\"tagOverride Must be a Valid Tag Index in \"tags\" Vector or -1\")");
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


			for (int i = 1; i < tags->size(); i++)
			{
				matrix[i][tagOverride] = prevTagProbs[i];
			}

		}
		else
		{
			for (int i = 1; i < tags->size(); i++)
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
	//I have encountered my first real
	//"This shouldn't work but it does and I don't know why" moment.
	//the program creates two additional matrices,
	//one at the beginning for the start tag and one at the end for the stop tag
	//
	//thing is, the start tag dissapears after processing, but
	//the first prediction always has the start tag as the previous tag
	//AND SOMEHOW still has the correct current tag
	//meanwhile, there are two tags at the end, 
	//the second to last has the correct previous and current tag after processing,
	//but the last matrix always has the previous and current tag as the end tag after processing,
	//and if I don't create the last matrix before processing it doesn't work

	
	//temporarily set prediction matrix to probability matrix at wanted position
	Matrix::Fmatrix prediction = probabilityMatrices[position];

	


	auto forwardMatrix = std::async(&CRF::calculateForwardVector, this, position);
	auto backwardMatrix = std::async(&CRF::calculateBackwardVector, this, position);

	//multiply forward vector by the probability matrix and then the resulting matrix by the backward vector
	prediction = Matrix::multiplyMatrix(forwardMatrix.get(), prediction);
	prediction = Matrix::multiplyMatrix(prediction, backwardMatrix.get());

	

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

void MLearn::CRF::updateWeights(std::vector<std::pair<std::vector<std::string>, std::string>> featureTokens, const std::vector<std::string>* tags, float learningRate)
{
	featureTokens.pop_back();
	featureTokens.erase(featureTokens.begin());

	//iterate through tokens
	for (const auto& token : featureTokens)
	{
		//iterate through the features 
		for (const auto& feature : token.first)
		{
			//iterate over every tag
			for (const auto& tag : *tags)
			{
				//modified perceptron algorithm to keep numbers between -1 and 1

				//w{j} = w{j} + αF{j}(x, y) for the true tag
				//w{j} = w{j} - αF{j}(x, y) for the all other tags

				//by subtracting the learning rate from the feature function's weight for all tags, 
				//it eliminates the need for O(Xn) comparisons (where X is the number of features over the entire sequence)
				//for checking which tag is the true tag, 
				//so long as we add more to the learning rate than we take away for the true tag 


				//subtract the derivative of the sigmoid function to keep numbers at reasonable values
				featureFunctionWeights[std::pair(tag, feature)] -= 0.05f * learningRate;
			}

			//add the derivative of the sigmoid function
			//I did some math and found out if you subtract the derivative of the sigmoid function 
			//and then add the derivative of the sigmoid function of the result to the result it will be
			//larger than what it started out as
			featureFunctionWeights[std::pair(token.second, feature)] += 1.0f * learningRate;
		}
	}
}
