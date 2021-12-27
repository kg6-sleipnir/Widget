#include "CRF.h"

float MLearn::CRF::getFeatureWeight(std::pair<std::string, std::string> featureFunction)
{
	if (!featureFunctionWeights.contains(featureFunction))
	{
		featureFunctionWeights[featureFunction] = (float)rand() / RAND_MAX / 10;
	}

	return featureFunctionWeights[featureFunction];
}

float MLearn::CRF::getFeaturePrimeWeight(std::pair<std::string, std::string> featureFunction)
{
	if (!featureFunctionPrimeWeights.contains(featureFunction))
	{
		featureFunctionPrimeWeights[featureFunction] = (float)rand() / RAND_MAX / 10;
	}

	return featureFunctionPrimeWeights[featureFunction];
}

void MLearn::CRF::createProbabilityMatrix(std::vector<std::string>& features, int position, int tagOverride)
{

	if (tagList->size() != tagAmount)
	{

		throw CRF_Error("CRF::createProbabilityMatrix(\"std::vector<std::string>* tagList size not equal to variable tagAmount\"");

	}


	Matrix::Fmatrix matrix(1, std::vector<float>(tagAmount, 1.0f / tagAmount)); //matrix to add to probability matrices, starting with vector of 0s on first row

	std::vector<float> tagProbs(tagAmount, 0.0f); //sum of feature weights associated with each tag

	

	if (tagOverride != -1) //allow prediction to force specific tag as output
	{

		for (int i = 1; i < tagAmount; i++)
		{

			matrix.push_back(std::vector<float>(tagAmount, 0));

			matrix[i][tagOverride] = 1.0f;

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

			if (sum > 0)
			{
				for (int j = 0; j < tagAmount; j++)
				{
					matrix[i][j] /= sum;
				}
			}

		}

		probabilityMatrices.push_back(matrix);

		return;

	}




	//add feature weights assosiated with a tag to the probability that the current tag is that tag
	for (int i = 0; i < tagAmount; i++)
	{

		for (int j = 0; j < features.size(); j++)
		{

			tagProbs[i] += getFeatureWeight(std::pair(tagList->at(i), features.at(j))); //make matrix with each index in diagonal the sum of observed features for that tag

		}


	}



	Matrix::Fmatrix tagProbsPrime(1, std::vector<float>(tagAmount, 0.0f));


	for (int i = 0; i < tagAmount; i++)
	{

		for (int j = 0; j < features.size(); j++)
		{

			tagProbsPrime[0][i] += getFeaturePrimeWeight(std::pair(tagList->at(i), features.at(j)));
		
		}


	}





	matrix = Matrix::addOuterMatrix(tagProbsPrime, Matrix::convertToMatrix<Matrix::Fmatrix>(tagProbs));

	


	probabilityMatrices.push_back(matrix);

}



Matrix::Fmatrix MLearn::CRF::calculateForwardVector(int position)
{
	//vector for holding cumulative probabilities for previous tag
	Custom::Matrix::Fmatrix tempMatrix = Custom::Matrix::Fmatrix(1, std::vector<float>(tagAmount, 0.0f));
	

	/*for (int i = 0; i < tagAmount; i++)
	{
		tempMatrix[i][startTag] = 1.0f;
	}*/


	//tempMatrix[0][startTag] = 1.0f;


	for (int i = 0; i < position; i++)
	{

		//tempMatrix = Matrix::multiplyMatrix(tempMatrix, tMatrix);

		//tempMatrix = Matrix::multiplyMatrix(tempMatrix, probabilityMatrices[i]);
		
		Matrix::Fmatrix tempMatrix2 = tempMatrix;

		for (int j = 0; j < tagAmount; j++)
		{

			Matrix::Fmatrix prob = Matrix::Fmatrix(1, std::vector<float>(tagAmount, 0.0f));
			
			for (int k = 0; k < tagAmount; k++)
			{
				prob[0][k] = probabilityMatrices[i][k][j];
			}

			prob = Matrix::addMatrix(prob, tempMatrix2);

			tempMatrix[0][j] = logsumexp(prob);

		}


		float sum = 0.0f;


	}
	

	//tempMatrix = Matrix::transposeMatrix(tempMatrix);

	return tempMatrix;
}



Matrix::Fmatrix MLearn::CRF::calculateBackwardVector(int position)
{
	
	Custom::Matrix::Fmatrix tempMatrix = Custom::Matrix::Fmatrix(tagAmount, std::vector<float>(1, 1.0f));






	for (int i = probabilityMatrices.size() - 1; i > position; i--)
	{
		
		
		
		
		Matrix::Fmatrix tempMatrix2 = Matrix::transposeMatrix(tempMatrix);


		for (int j = 0; j < tagAmount; j++)
		{

			Matrix::Fmatrix prob = Matrix::Fmatrix(1, probabilityMatrices[i][j]);

			prob = Matrix::addMatrix(prob, tempMatrix2);

			tempMatrix[j][0] = logsumexp(prob);

		}



		float sum = 0.0f;


	}
	

	tempMatrix = Matrix::transposeMatrix(tempMatrix);

	return tempMatrix;
}



std::vector<std::pair<int, int>> MLearn::CRF::viterbi()
{

	Matrix::Imatrix tagIndices = Matrix::Imatrix(probabilityMatrices.size(), std::vector<int>(tagAmount, 0));

	std::vector<float> prevProbs = std::vector<float>(tagAmount, 0.0f);

	std::vector<float> curProbs = std::vector<float>(tagAmount, 0.0f);

	prevProbs[startTag] = 1.0f;



	for (int i = 0; i < probabilityMatrices.size(); i++)
	{


		for (int j = 0; j < tagAmount; j++)
		{

			std::vector<float> tempProbs = std::vector<float>(tagAmount, 0.0f);


			//sum each element in curProbs with column j in current probability matrix to get probability
			for (int k = 0; k < tagAmount; k++)
			{
				tempProbs[k] = prevProbs[k] + probabilityMatrices[i][k][j];
			}


			

			auto argmax = std::max_element(tempProbs.begin(), tempProbs.end());

			tagIndices[i][j] = std::distance(tempProbs.begin(), argmax);

			curProbs[j] = *argmax;

		}

		prevProbs = curProbs;

	}

	

	std::vector<std::pair<int, int>> tagPredictions;

	int nextTag = endTag;

	

	for (int i = probabilityMatrices.size() - 1; i > 0; i--)
	{

		tagPredictions.insert(tagPredictions.begin(), std::pair<int, int>(tagIndices[i][nextTag], nextTag));

		nextTag = tagIndices[i][nextTag];

	}

	tagPredictions.insert(tagPredictions.begin(), std::pair<int, int>(startTag, nextTag));

	return tagPredictions;
}

float MLearn::CRF::logsumexp(Custom::Matrix::Fmatrix arr)
{

	if (arr.size() != 1)
	{
		throw CRF_Error("logsumexp() must be 1 dimensional matrix as parameter");
	}

	

	float max = 0;

	for (int i = 0; i < arr[0].size(); i++)
	{
		if (arr[0][i] > max)
		{
			max = arr[0][i];
		}
	}


	float temp = 0;

	for (int i = 0; i < arr[0].size(); i++)
	{

		temp += exp(arr[0][i] - max);

	}

	return log(temp) + max;
}

std::vector<float> MLearn::CRF::getTagProbs(int position, float normalizeFactor)
{
	//temporarily set prediction matrix to probability matrix at wanted position
	Matrix::Fmatrix prediction = probabilityMatrices[position];


	//calculate the forward and backward vectors with multithreading
	auto forwardMatrix = std::async(&CRF::calculateForwardVector, this, position);
	auto backwardMatrix = std::async(&CRF::calculateBackwardVector, this, position);

	//Matrix::Fmatrix FBMatrix = Matrix::multiplyMatrix(backwardMatrix.get(), forwardMatrix.get());

	Matrix::Fmatrix FBMatrix = Matrix::addOuterMatrix(forwardMatrix.get(), backwardMatrix.get());




	prediction = Matrix::addMatrix(prediction, FBMatrix);



	prediction = Matrix::addMatrixWith(prediction, -1 * normalizeFactor);

	prediction = Matrix::expElementsInMatrix(prediction);

	std::vector<float> temp = Custom::Matrix::sumColumns(prediction);

	float sum = 0;
	float min = 0;



	


	
	for (int i = 0; i < temp.size(); i++)
	{
		temp[i] /= exp(1);
	}
	
	/*for (int i = 0; i < temp.size(); i++)
	{
		sum += temp[i];
	}*/
	

	return temp;
}

std::vector<float> MLearn::CRF::getPrevTagProbs(int position, float normalizeFactor)
{
	
	if (position >= probabilityMatrices.size())
	{
		return std::vector<float>(tagAmount, 1.0f / tagAmount);
	}
	
	//temporarily set prediction matrix to probability matrix at wanted position
	Matrix::Fmatrix prediction = probabilityMatrices[position];


	//calculate the forward and backward vectors with multithreading
	auto forwardMatrix = std::async(&CRF::calculateForwardVector, this, position);
	auto backwardMatrix = std::async(&CRF::calculateBackwardVector, this, position);



	//Matrix::Fmatrix FBMatrix = Matrix::multiplyMatrix(backwardMatrix.get(), forwardMatrix.get());

	Matrix::Fmatrix FBMatrix = Matrix::addOuterMatrix(forwardMatrix.get(), backwardMatrix.get());





	prediction = Matrix::addMatrix(prediction, FBMatrix);

	prediction = Matrix::addMatrixWith(prediction, -1 * normalizeFactor);

	prediction = Matrix::expElementsInMatrix(prediction);




	std::vector<float> temp = Custom::Matrix::sumRows(prediction);

	float sum = 0;
	float min = 0;



	


	
	for (int i = 0; i < temp.size(); i++)
	{
		temp[i] /= exp(1);
	}
	
	/*for (int i = 0; i < temp.size(); i++)
	{
		sum += temp[i];
	}*/


	return temp;
}



void MLearn::CRF::updateWeights(std::vector<std::pair<std::vector<std::string>, std::string>> featureTokens, float learnRate)
{
	
	float logz = logsumexp(calculateForwardVector(probabilityMatrices.size()));

	for (int i = 0; i < featureTokens.size() - 1; i++)
	{

		/*std::vector<float> tagProbs(tagAmount, 0.0f);

		for (int j = 0; j < tagAmount; j++)
		{
			for (const std::string feature : featureTokens[i].first)
			{

				tagProbs[j] += getFeatureWeight(std::pair(tagList->at(j), feature));

			}

			tagProbs[j] = exp(tagProbs[j]);

		}*/
		
		
		//std::vector<float> tagProbs = Matrix::sumColumns(probabilityMatrices[i]);
		
		std::vector<float> tagProbs = getTagProbs(i, logz);


		if (tagProbs != tagProbs)
		{
			continue;
		}

		float sum = 0;

		for (int i = 0; i < tagAmount; i++)
		{
			sum += tagProbs[i];
		}

		/*if (sum > 0)
		{
			for (int i = 0; i < tagAmount; i++)
			{
				tagProbs[i] /= sum;
			}
		}*/

		auto tagIndex = std::find(tagList->begin(), tagList->end(), featureTokens[i].second);

		auto probIndex = std::distance(tagList->begin(), tagIndex);

		

		

		


		float inverseProb = (1 - tagProbs[probIndex]);

		if (inverseProb > 0)
		{
			for (int j = 0; j < featureTokens[i].first.size(); j++)
			{

				for (int k = 0; k < tagList->size(); k++)
				{
					featureFunctionWeights[std::pair(tagList->at(k), featureTokens[i].first[j])] -= tagProbs[k] * learnRate;
				}

				featureFunctionWeights[std::pair(featureTokens[i].second, featureTokens[i].first[j])] += tagProbs[probIndex] * learnRate;

				featureFunctionWeights[std::pair(featureTokens[i].second, featureTokens[i].first[j])] += inverseProb * learnRate;
			}
		}


	}
}

void MLearn::CRF::updatePrimeWeights(std::vector<std::pair<std::vector<std::string>, std::string>> featureTokens, float learnRate)
{

	float logz = logsumexp(calculateForwardVector(probabilityMatrices.size()));

	for (int i = 1; i < featureTokens.size(); i++)
	{

		

		std::vector<float> tagProbs = getPrevTagProbs(i, logz);

		auto tagIndex = std::find(tagList->begin(), tagList->end(), featureTokens[i - 1].second);

		auto probIndex = std::distance(tagList->begin(), tagIndex);


		float sum = 0;

		for (int j = 0; j < tagAmount; j++)
		{
			sum += tagProbs[j];
		}

		

		float inverseProb = (1 - tagProbs[probIndex]);


		if (inverseProb > 0)
		{

			for (const std::string feature : featureTokens[i].first)
			{

				for (int j = 0; j < tagAmount; j++)
				{
					featureFunctionPrimeWeights[std::pair(tagList->at(j), feature)] -= tagProbs[j] * learnRate;
				}

				featureFunctionPrimeWeights[std::pair(tagList->at(probIndex), feature)] += tagProbs[probIndex] * learnRate;

				featureFunctionPrimeWeights[std::pair(tagList->at(probIndex), feature)] += inverseProb * learnRate;

			}

		}


	}

}

