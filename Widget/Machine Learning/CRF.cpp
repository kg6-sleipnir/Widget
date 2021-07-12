#include "CRF.h"

float MLearn::CRF::getFeatureWeight(std::pair<std::string, std::string> featureFunction)
{
	if (!featureFunctionWeights.contains(featureFunction))
	{
		featureFunctionWeights[featureFunction] = 0.1f;
	}

	return featureFunctionWeights[featureFunction];
}

void MLearn::CRF::createProbabilityMatrix(std::vector<std::string>& features, Custom::Matrix::Fmatrix* transitionMatrix, const std::vector<std::string>* tags, int position, int tagOverride)
{
	//matrix of size tags.size() by tags.size() derived from a matrix holding probabilities for transitions between tags
	Matrix::Fmatrix matrix;

	
	//check if the current tag is not predefined
	if (tagOverride == -1)
	{
		//get sum of feature weights to normalize the matrix
		
		//float sumOfFeatures = 0;

		//for (const auto& i : features)
		//{
		//	for (int j = 1; j < tags->size(); j++)
		//	{
		//		//weight of feature function
		//		sumOfFeatures += getFeatureWeight(std::pair<std::string, std::string>(tags->at(j), i));
		//	}
		//}

		std::vector<float> tempVector(tags->size(), 0);

		//add normalized feature function weights to temporary vector at every tag index
		for (const auto& i : features)
		{
			for (int j = 1; j < tags->size(); j++)
			{
				//weight of feature function
				float temp = (getFeatureWeight(std::pair<std::string, std::string>(tags->at(j), i)) );

				tempVector[j] += temp;
			}
		}

		//repeatedly push back temp vector to matrix to get matrix of size tags->size() by tags->size()
		for (int i = 0; i < tags->size(); i++)
		{
			matrix.push_back(tempVector);
		}


		if (position > 1)
		{
			matrix = Matrix::addMatrix(matrix, Matrix::multiplyMatrixBy(Matrix::transposeMatrix(probabilityMatrices[position - 1]), 1.0f));
		}


		matrix = Custom::Matrix::addMatrix(*transitionMatrix, matrix);

		long double normalize = 0;

		for (int i = 1; i < matrix.size(); i++)
		{
			for (int j = 1; j < matrix[1].size(); j++)
			{
				normalize += abs(matrix[i][j]);
			}
		}

		//apparently, and int divided by 0 is an error, but a float divided by 0 is -nan(inf), weird
		if (normalize != 0)
		{
			Custom::Matrix::multiplyMatrixByReferenceBy(matrix, 0.5f / normalize);
		}

		//I don't know why but this increases the accuracy significantly
		//and prevents most sequences from defaulting to the NUL tag
		
		for (int i = 1; i < matrix.size(); i++)
		{
			for (int j = 1; j < matrix[1].size(); j++)
			{
				matrix[i][j] += 0.1f * ((matrix[i][j] + 1.0f) / (abs(matrix[i][j] + 1.0f)));
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

			matrix = std::vector<std::vector<float>>(tags->size(), std::vector<float>(tags->size(), 0));

			for (int i = 1; i < tags->size(); i++)
			{
				matrix[tagOverride][i] = prevTagProbs[i];
			}

			
		}
		else
		{
			matrix = std::vector<std::vector<float>>(tags->size(), std::vector<float>(tags->size(), 0));

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

	Custom::Matrix::transposeSquareMatrixByReference(tempMatrix);

	return tempMatrix;
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

	
	//calculate the forward and backward vectors with multithreading
	auto forwardMatrix = std::async(&CRF::calculateForwardVector, this, position);
	auto backwardMatrix = std::async(&CRF::calculateBackwardVector, this, position);
	//auto normalFactor = std::async(&CRF::normalizeFactor, this, startTag, endTag);


	//multiply forward vector by the probability matrix and then the resulting matrix by the backward vector
	prediction = Matrix::multiplyMatrix(forwardMatrix.get(), prediction);
	prediction = Matrix::multiplyMatrix(prediction, backwardMatrix.get());


	//Matrix::multiplyMatrixByReferenceBy(prediction, 1.0f / normalFactor.get());

	

	//temp values for holding entry in prediction with highest value
	int prevTagIndex = 0;
	int tagIndex = 0;
	float max = 0;

	//find highest value in prediction
	for (int i = 1; i < prediction.size(); i++)
	{
		for (int j = 1; j < prediction[0].size(); j++)
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

void MLearn::CRF::updateTransitionMatrix(std::vector<std::pair<std::vector<std::string>, std::string>> &featureTokens, const std::vector<std::string>* tags, Custom::Matrix::Fmatrix &transitionMatrix, std::array<float, 2> learnRates)
{
#define UnlearnRate learnRates[0]
#define LearnRate learnRates[1]
	
	//iterate over all tokens that are not the NUL or STOP tag because they will never come before a token in a sentence
	
	for (int i = 1; i <= 46; i++)
	{
		//iterate over all tags except the NUL tag for the same reason except it will come after
		for (int j = 1; j < tags->size(); j++)
		{
			//decrease the transitional weight of all tags using the derivative of the sigmoid function
			//later we will increment the transitional weight of a true transition between tags by more than what we decrement
			//in order to skip O(n^2) comparisons (where n is the number of tags) for checking if it's a true transitional sequence
			transitionMatrix[i][j] -= UnlearnRate * (1.0f / (1.0f + exp(transitionMatrix[i][j]))) * (1.0f - (1.0f / (1.0f + exp(transitionMatrix[i][j]))));
		}
	}
	
	//iterate over the features and tags of the sequence
	for (int i = 1; i < featureTokens.size(); i++)
	{
		//find the current and previous tags' index in tags vector
		auto prevTagIndex = std::find(tags->begin(), tags->end(), featureTokens[i - 1].second);
		auto curTagIndex = std::find(tags->begin(), tags->end(), featureTokens[i].second);


		//update the transitional sequence's weight by more than what we decremented earlier
		transitionMatrix[prevTagIndex - tags->begin()][curTagIndex - tags->begin()] += LearnRate * (1.0f / (1.0f + exp(transitionMatrix[prevTagIndex - tags->begin()][curTagIndex - tags->begin()]))) * (1.0f - (1.0f / (1.0f + exp(transitionMatrix[prevTagIndex - tags->begin()][curTagIndex - tags->begin()]))));
	}
}



void MLearn::CRF::updateWeights(std::vector<std::pair<std::vector<std::string>, std::string>> featureTokens, const std::vector<std::string>* tags, std::array<float, 7> learnRates)
{
#define WordLearnRate learnRates[0]
#define NextLearnRate learnRates[1]
#define PreviousLearnRate learnRates[2]
#define LastTwoLearnRate learnRates[3]
#define LastThreeLearnRate learnRates[4]
#define UnlearnRate learnRates[5]
#define NormalLearnRate learnRates[6]
	
	//erase the first and last feature vectors in the sequence's feature vectors since they are the START and STOP tag
	//and dont need to be updated due to them always being correct
	featureTokens.erase(featureTokens.begin());
	featureTokens.pop_back();

	//iterate through tokens
	for (const auto& token : featureTokens)
	{
		//update the _WORD_, NEXT_, PREVIOUS_, and LAST_x letters feature function weights respectively
		//since these features influence the tag prediction more than the others, we can increase them more than other features
		//to get more accurate results

		featureFunctionWeights[std::pair(token.second, token.first.front())] += WordLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[0])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[0])]))));
		featureFunctionWeights[std::pair(token.second, token.first[1])] += NextLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[1])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[1])]))));
		featureFunctionWeights[std::pair(token.second, token.first[2])] += PreviousLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[2])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[2])]))));
		featureFunctionWeights[std::pair(token.second, token.first[3])] += LastTwoLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[3])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[3])]))));
		featureFunctionWeights[std::pair(token.second, token.first[4])] += LastThreeLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[4])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[4])]))));

		//iterate through the features 
		for (int i = 0; i < token.first.size(); i++)
		{
			//iterate over every tag
			for (int j = 1; j < tags->size(); j++)
			{
				//modified perceptron algorithm to keep numbers between -1 and 1

				//w{j} = w{j} + αF{j}(x, y) for the true tag
				//w{j} = w{j} - αF{j}(x, y) for the all other tags

				//by subtracting the learning rate from the feature function's weight for all tags, 
				//it eliminates the need for O(Xn) comparisons (where X is the number of features over the entire sequence and n is the number of tags)
				//for checking which tag is the true tag, 
				//so long as we add more to the learning rate than we take away for the true tag 


				//subtract the derivative of a modified sigmoid function to keep numbers at reasonable values
				//this modified sigmoid function will keep all values positive, with a decreasing second derivative the closer it gets to 0
				//basically, it decrements the value with smaller decrements the closer it gets to 0 while making sure it never reaches 0 or a negative number
				
				//featureFunctionWeights[std::pair(tags->at(j), token.first[i])] -= 0.5f - ((learnRates[5] * exp((featureFunctionWeights[std::pair(tags->at(j), token.first[i])] / 2.0f))) / pow((exp((featureFunctionWeights[std::pair(tags->at(j), token.first[i])] / 2.0f)) + 1.0f), 2.0f));
				featureFunctionWeights[std::pair(tags->at(j), token.first[i])] -= UnlearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(tags->at(j), token.first[i])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(tags->at(j), token.first[i])]))));
			}

			//add the derivative of the sigmoid function with a slightly increased rate of growth for all feature functions containing the true tag
			featureFunctionWeights[std::pair(token.second, token.first[i])] += NormalLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[i])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[i])]))));
		}
	}
}
