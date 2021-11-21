#include "CRF.h"

float MLearn::CRF::getFeatureWeight(std::pair<std::string, std::string> featureFunction)
{
	if (!featureFunctionWeights.contains(featureFunction))
	{
		featureFunctionWeights[featureFunction] = (float)rand() / RAND_MAX / 10;
	}

	return featureFunctionWeights[featureFunction];
}

void MLearn::CRF::createProbabilityMatrix(std::vector<std::string>& features, Custom::Matrix::Fmatrix* transitionMatrix, const std::vector<std::string>* tags, int position, int startTag, int tagOverride)
{

	if (tags->size() != tagAmount)
	{

		throw CRF_Error("CRF::createProbabilityMatrix(\"std::vector<std::string>* tags size not equal to variable tagAmount\"");

	}


	Matrix::Fmatrix matrix(1, std::vector<float>(tagAmount, 0)); //matrix to add to probability matrices, starting with vector of 0s on first row

	std::vector<float> tagProbs(tagAmount, 0.0f); //sum of feature weights associated with each tag


	if (tagOverride != -1) //allow prediction to force specific tag as output
	{

		for (int i = 1; i < tagAmount; i++)
		{

			matrix.push_back(std::vector<float>(tagAmount, 0));

			matrix[i][tagOverride] = 1;

		}

		//matrix = Matrix::addMatrix(matrix, *transitionMatrix);

		matrix = Matrix::expElementsInMatrix(matrix);

		for (int i = 0; i < tagAmount; i++)
		{

			float sum = 0;

			for (int j = 0; j < tagAmount; j++)
			{
				sum += matrix[i][j];
			}

			for (int j = 0; j < tagAmount; j++)
			{
				matrix[i][j] /= sum;
			}

		}

		probabilityMatrices.push_back(matrix);

		return;

	}


	float sum = 0.0f; //sum of tagProbs for normalizing


	//add feature weights assosiated with a tag to the probability that the current tag is that tag
	for (int i = 1; i < tagAmount; i++)
	{

		for (int j = 0; j < features.size(); j++)
		{

			tagProbs[i] += getFeatureWeight(std::pair(tags->at(i), features.at(j))); //make matrix with each index in diagonal the sum of observed features for that tag

		}

		sum += tagProbs[i];

	}

	//normalize tag probabilities
	for (int i = 1; i < tagProbs.size(); i++)
	{

		tagProbs[i] /= sum;

	}


	
	if (position != 0) //push back tag probabilities for every possible previous tag if not the first matrix
	{

		for (int i = 1; i < tagAmount; i++)
		{

			matrix.push_back(tagProbs);

		}

	}
	else //if current matrix is the first matrix push back tagProbs only to the START tag as the previous tag
	{

		for (int i = 1; i < startTag; i++)
		{

			matrix.push_back(std::vector<float>(tagAmount, 1.0f / tagAmount));

		}

		matrix.push_back(tagProbs);

		for (int i = startTag + 1; i < tagAmount; i++)
		{

			matrix.push_back(std::vector<float>(tagAmount, 1.0f / tagAmount));

		}

	}
	
	//matrix = Matrix::addMatrix(matrix, *transitionMatrix);

	matrix = Matrix::expElementsInMatrix(matrix);

	/*for (int i = 0; i < tagAmount; i++)
	{
		
		float sum = 0;

		for (int j = 0; j < tagAmount; j++)
		{
			sum += matrix[i][j];
		}

		for (int j = 0; j < tagAmount; j++)
		{
			matrix[i][j] /= sum;
		}

	}*/

	probabilityMatrices.push_back(matrix);

}



Matrix::Fmatrix MLearn::CRF::calculateForwardVector(int position, int startTag)
{
	//vector for holding cumulative probabilities for previous tag
	Custom::Matrix::Fmatrix tempMatrix = Custom::Matrix::Fmatrix(1, std::vector<float>(tagAmount, 0.0f));
	
	tempMatrix[0][startTag] = 1.0f;


	for (int i = 0; i < position; i++)
	{

		tempMatrix = Matrix::multiplyMatrix(tempMatrix, tMatrix);

		tempMatrix = Matrix::multiplyMatrix(tempMatrix, probabilityMatrices[i]);
		

		float sum = 0.0f;

		for (int j = 0; j < tempMatrix[0].size(); j++)
		{

			sum += tempMatrix[0][j];

		}

		tempMatrix = Matrix::multiplyMatrixBy(tempMatrix, 1.0f / sum);

	}
	

	tempMatrix = Matrix::transposeMatrix(tempMatrix);

	return tempMatrix;
}



Matrix::Fmatrix MLearn::CRF::calculateBackwardVector(int position, int endTag)
{
	
	Custom::Matrix::Fmatrix tempMatrix = Custom::Matrix::Fmatrix(tagAmount, std::vector<float>(1, 1.0f));

	//tempMatrix[endTag][0] = 1.0f;


	for (int i = probabilityMatrices.size() - 1; i >= position; i--)
	{

		tempMatrix = Matrix::multiplyMatrix(probabilityMatrices[i], tempMatrix);

		tempMatrix = Matrix::multiplyMatrix(tMatrix, tempMatrix);


		float sum = 0.0f;

		for (int j = 0; j < tempMatrix.size(); j++)
		{

			sum += tempMatrix[j][0];

		}

		tempMatrix = Matrix::multiplyMatrixBy(tempMatrix, 1.0f / sum);
	}
	

	tempMatrix = Matrix::transposeMatrix(tempMatrix);

	return tempMatrix;
}

float MLearn::CRF::normalizeFactor(int startTagIndex, int endTagIndex)
{
	//create temporary matrix starting with the first probability matrix
	Custom::Matrix::Fmatrix tempMatrix = probabilityMatrices[0];
	

	//iterate over the probability matricex, multiplying them together
	for (int i = 1; i < probabilityMatrices.size() - 1; i++)
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

	
	//calculate the forward and backward vectors with multithreading
	auto forwardMatrix = std::async(&CRF::calculateForwardVector, this, position, startTag);
	auto backwardMatrix = std::async(&CRF::calculateBackwardVector, this, position, endTag);
	//auto normalFactor = std::async(&CRF::normalizeFactor, this, startTag, endTag);


	/*auto forwardMatrix = calculateForwardVector(position, startTag);
	auto backwardMatrix = calculateBackwardVector(position, endTag);*/

	
	
	prediction = Matrix::multiplyMatrix(Matrix::multiplyMatrix(forwardMatrix.get(), backwardMatrix.get()), prediction);

	//prediction = Matrix::multiplyMatrix(backwardMatrix.get(), forwardMatrix.get());

	
	//normalize each row to get conditional probability
	/*for (int i = 0; i < tagAmount; i++)
	{

		float sum = 0;

		for (int j = 0; j < tagAmount; j++)
		{
			sum += prediction[i][j];
		}


		for (int j = 0; j < tagAmount; j++)
		{
			prediction[i][j] /= sum;
		}

	}*/
	

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

std::vector<float> MLearn::CRF::getTagProbs(int position, int startTag, int endTag)
{
	//temporarily set prediction matrix to probability matrix at wanted position
	Matrix::Fmatrix prediction = probabilityMatrices[position];


	//calculate the forward and backward vectors with multithreading
	auto forwardMatrix = std::async(&CRF::calculateForwardVector, this, position, startTag);
	auto backwardMatrix = std::async(&CRF::calculateBackwardVector, this, position, endTag);
	//auto normalFactor = std::async(&CRF::normalizeFactor, this, startTag, endTag);

	/*auto forwardMatrix = calculateForwardVector(position, startTag);
	auto backwardMatrix = calculateBackwardVector(position, endTag);*/

	prediction = Matrix::multiplyMatrix(Matrix::multiplyMatrix(forwardMatrix.get(), backwardMatrix.get()), prediction);



	//prediction = Matrix::multiplyMatrix(backwardMatrix.get(), forwardMatrix.get());

	


	//normalize each row to get conditional probability
	/*for (int i = 1; i < tagAmount; i++)
	{

		float sum = 0;

		for (int j = 0; j < tagAmount; j++)
		{
			sum += prediction[i][j];
		}


		for (int j = 0; j < tagAmount; j++)
		{
			prediction[i][j] /= sum;
		}

	}*/



	std::vector<float> temp = Custom::Matrix::sumColumns(prediction);

	float sum = 0;
	float min = 0;



	for (int i = 0; i < temp.size(); i++)
	{
		sum += temp[i];
	}


	if (sum > 0)
	{
		for (int i = 0; i < temp.size(); i++)
		{
			temp[i] /= sum;
		}
	}

	

	return temp;
}

std::vector<float> MLearn::CRF::getPrevTagProbs(int position, int startTag, int endTag)
{
	//temporarily set prediction matrix to probability matrix at wanted position
	Matrix::Fmatrix prediction = probabilityMatrices[position];


	//calculate the forward and backward vectors with multithreading
	auto forwardMatrix = std::async(&CRF::calculateForwardVector, this, position, startTag);
	auto backwardMatrix = std::async(&CRF::calculateBackwardVector, this, position, endTag);
	//auto normalFactor = std::async(&CRF::normalizeFactor, this, startTag, endTag);



	prediction = Matrix::multiplyMatrix(Matrix::multiplyMatrix(forwardMatrix.get(), backwardMatrix.get()), prediction);


	//normalize each row to get conditional probability
	for (int i = 1; i < tagAmount; i++)
	{

		float sum = 0;

		for (int j = 0; j < tagAmount; j++)
		{
			sum += prediction[i][j];
		}


		for (int j = 0; j < tagAmount; j++)
		{
			prediction[i][j] /= sum;
		}

	}

	

	std::vector<float> temp = Custom::Matrix::sumRows(prediction);

	float sum = 0;
	float min = 0;



	for (int i = 0; i < temp.size(); i++)
	{
		sum += temp[i];
	}


	if (sum > 0)
	{
		for (int i = 0; i < temp.size(); i++)
		{
			temp[i] /= sum;
		}
	}

	

	return temp;
}


void MLearn::CRF::updateTransitionMatrix(std::vector<std::pair<std::vector<std::string>, std::string>> &featureTokens, const std::vector<std::string>* tags, Custom::Matrix::Fmatrix &transitionMatrix, std::array<float, 2> learnRates)
{
#define UnlearnRate learnRates[0]
#define LearnRate learnRates[1]
	
	//iterate over all tokens that are not the NUL or STOP tag because they will never come before a token in a sentence
	
	for (int i = 1; i <= tags->size() - 2; i++)
	{
		//iterate over all tags except the NUL tag for the same reason except it will come after
		for (int j = 1; j < tags->size() - 1; j++)
		{
			//decrease the transitional weight of all tags using the derivative of the sigmoid function
			//later we will increment the transitional weight of a true transition between tags by more than what we decrement
			//in order to skip O(n^2) comparisons (where n is the number of tags) for checking if it's a true transitional sequence
			
			transitionMatrix[i][j] -= UnlearnRate * (1.0f / (1.0f + exp(transitionMatrix[i][j]))) * (1.0f - (1.0f / (1.0f + exp(transitionMatrix[i][j]))));
			//transitionMatrix[i][j] -= UnlearnRate;
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
		//transitionMatrix[prevTagIndex - tags->begin()][curTagIndex - tags->begin()] += LearnRate;
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

		/*featureFunctionWeights[std::pair(token.second, token.first.front())] += WordLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[0])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[0])]))));
		featureFunctionWeights[std::pair(token.second, token.first[1])] += NextLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[1])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[1])]))));
		featureFunctionWeights[std::pair(token.second, token.first[2])] += PreviousLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[2])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[2])]))));
		featureFunctionWeights[std::pair(token.second, token.first[3])] += LastTwoLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[3])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[3])]))));
		featureFunctionWeights[std::pair(token.second, token.first[4])] += LastThreeLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[4])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[4])]))));*/

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
				//featureFunctionWeights[std::pair(tags->at(j), token.first[i])] += UnlearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(tags->at(j), token.first[i])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(tags->at(j), token.first[i])]))));
				featureFunctionWeights[std::pair(tags->at(j), token.first[i])] -= UnlearnRate;
			}

			//add the derivative of the sigmoid function with a slightly increased rate of growth for all feature functions containing the true tag
			
			//featureFunctionWeights[std::pair(token.second, token.first[i])] -= NormalLearnRate * (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[i])]))) * (1.0f - (1.0f / (1.0f + exp(featureFunctionWeights[std::pair(token.second, token.first[i])]))));
			featureFunctionWeights[std::pair(token.second, token.first[i])] += NormalLearnRate;
		}
	}
}


void MLearn::CRF::updateWeightsCLL(std::vector<std::pair<std::vector<std::string>, std::string>> featureTokens, const std::vector<std::string>* tags, float learnRate, int startTag, int endTag)
{
	
	
	for (int i = 0; i < featureTokens.size() - 1; i++)
	{

		std::vector<float> tagProbs = getTagProbs(i, startTag, endTag);


		auto tagIndex = std::find(tags->begin(), tags->end(), featureTokens[i].second);

		auto probIndex = std::distance(tags->begin(), tagIndex);

		//float max = 0;

		/*for (int i = 0; i < tagProbs.size(); i++)
		{
			if (tagProbs[i] > max)
			{
				max = tagProbs[i];
			}
		}

		if (max > 0)
		{
			for (int i = 0; i < tagProbs.size(); i++)
			{
				tagProbs[i] /= max;
			}
		}*/

		float inverseProb;

		float sum = 0;

		float max = 0;
		int maxIndex = 0;

		if (tagProbs[0] != tagProbs[0])
		{
			continue;
		}






		/*for (int i = 0; i < tagProbs.size(); i++)
		{

			sum += tagProbs[i];


		}

		for (int i = 0; i < tagProbs.size(); i++)
		{
			tagProbs[i] /= sum;
		}*/

		inverseProb = (1 - tagProbs[probIndex]);

		if (inverseProb > 0)
		{
			for (int j = 0; j < featureTokens[i].first.size(); j++)
			{

				for (int k = 1; k < tags->size(); k++)
				{
					featureFunctionWeights[std::pair(tags->at(k), featureTokens[i].first[j])] -= tagProbs[k] * learnRate;
				}

				featureFunctionWeights[std::pair(featureTokens[i].second, featureTokens[i].first[j])] += tagProbs[probIndex] * learnRate;

				featureFunctionWeights[std::pair(featureTokens[i].second, featureTokens[i].first[j])] += inverseProb * learnRate;
			}
		}

		/*for (int j = 0; j < featureTokens[i].first.size(); j++)
		{

			for (int k = 1; k < tags->size(); k++)
			{
				featureFunctionWeights[std::pair(tags->at(k), featureTokens[i].first[j])] -= -(0 - tagProbs[k]) * (tagProbs[k] * (1 - tagProbs[k])) * learnRate;
			}

			featureFunctionWeights[std::pair(tags->at(probIndex), featureTokens[i].first[j])] += -(0 - tagProbs[probIndex]) * (tagProbs[probIndex] * (1 - tagProbs[probIndex])) * learnRate;
			featureFunctionWeights[std::pair(tags->at(probIndex), featureTokens[i].first[j])] -= -(1 - tagProbs[probIndex]) * (tagProbs[probIndex] * (1 - tagProbs[probIndex])) * learnRate;


		}*/

	}
}

void MLearn::CRF::updatetransitionMatrixCLL(std::vector<std::pair<std::vector<std::string>, std::string>> featureTokens, const std::vector<std::string>* tags, Custom::Matrix::Fmatrix& transitionMatrix, float learnRate, int startTag, int endTag)
{
	for (int i = 0; i < featureTokens.size() - 1; i++)
	{
		/*auto prevTagProbs = std::async(&CRF::getTagProbs, this, i - 1, startTag, endTag);	
		auto curTagProbs = std::async(&CRF::getTagProbs, this, i, startTag, endTag);*/

		auto prevTagProbs = std::async(&CRF::calculateForwardVector, this, i, startTag);
		auto curTagProbs = std::async(&CRF::calculateBackwardVector, this, i, endTag);

		
		auto curTag = std::find(tags->begin(), tags->end(), featureTokens[i].second);
		auto curTagIndex = std::distance(tags->begin(), curTag);


		//auto prevTag = std::find(tags->begin(), tags->end(), featureTokens[i - 1].second);

		auto prevTag = std::find(tags->begin(), tags->end(), [](int k, std::vector<std::pair<std::vector<std::string>, std::string>> fTokens) { return (k == 0) ? std::string("START") : fTokens[k - 1].second; }(i, featureTokens));
		auto prevTagIndex = std::distance(tags->begin(), prevTag);
		

		auto prev = prevTagProbs.get();
		auto cur = curTagProbs.get();


		
		/*Custom::Matrix::Fmatrix prevTagTranspose(1, std::vector<float>(prev));
		prevTagTranspose = Custom::Matrix::transposeMatrix(prevTagTranspose);
		
		Custom::Matrix::Fmatrix tagProbs = Custom::Matrix::multiplyMatrix(prevTagTranspose, Custom::Matrix::Fmatrix(1, std::vector<float>(cur)));*/

		
		Matrix::Fmatrix tagProbs = Matrix::multiplyMatrix(prev, cur);

		float inverseProb;


		float max = 0;
		int curMaxIndex = 0;
		int prevMaxIndex = 0;

		if (cur[0] != cur[0])
		{
			return;
		}
		
		

		

		float sum = 0;

		/*for (int j = 0; j < tagProbs[0].size(); j++)
		{
			sum += tagProbs[prevTagIndex][j];
		}

		for (int j = 0; j < tagProbs[0].size(); j++)
		{
			tagProbs[prevTagIndex][j] /= sum;
		}*/

		
		


		inverseProb = 1.0f - tagProbs[prevTagIndex][curTagIndex];

		
		Custom::Matrix::multiplyMatrixByReferenceBy(tagProbs, learnRate);


		for (int i = 0; i < tagAmount; i++)
		{

			transitionMatrix[prevTagIndex][i] -= tagProbs[prevTagIndex][i];

		}

		transitionMatrix[prevTagIndex][curTagIndex] += tagProbs[prevTagIndex][curTagIndex];

		transitionMatrix[prevTagIndex][curTagIndex] += learnRate * abs(inverseProb);
		

		sum = 0;

		for (int i = 0; i < tagAmount; i++)
		{
			sum += transitionMatrix[prevTagIndex][i];
		}
		
		
		for (int i = 0; i < tagAmount; i++)
		{
			transitionMatrix[prevTagIndex][i] /= sum;
		}




	}
}


