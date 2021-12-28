#include "CRF.h"

float MLearn::CRF::getFeatureWeight(std::string tag, std::string feature)
{

	std::pair<std::string, std::string> featureFunction(tag, feature);

	if (!featureFunctionWeights.contains(featureFunction))
	{
		featureFunctionWeights[featureFunction] = (float)rand() / RAND_MAX / 10;
	}

	return featureFunctionWeights[featureFunction];

}

float MLearn::CRF::getFeaturePrimeWeight(std::string tag, std::string feature)
{

	std::pair<std::string, std::string> featureFunction(tag, feature);


	if (!featureFunctionPrimeWeights.contains(featureFunction))
	{
		featureFunctionPrimeWeights[featureFunction] = (float)rand() / RAND_MAX / 10;
	}

	return featureFunctionPrimeWeights[featureFunction];

}

void MLearn::CRF::createProbabilityMatrix(std::vector<std::string> features)
{

	if (tagList->size() != tagAmount)
	{

		throw CRF_Error("CRF::createProbabilityMatrix(), tagList size not equal to variable tagAmount");

	}


	Matrix::Fmatrix matrix(1, std::vector<float>(tagAmount, 1.0f / tagAmount)); //matrix to add to probability matrices, starting with vector of 0s on first row

	Matrix::Fmatrix tagProbs(1, std::vector<float>(tagAmount, 0.0f)); //sum of feature weights associated with current tags

	Matrix::Fmatrix tagProbsPrime(1, std::vector<float>(tagAmount, 0.0f)); //sum of feature weights assosiated with previous tags


	//add feature weights for current tag
	for (int i = 0; i < tagAmount; i++)
	{
		for (int j = 0; j < features.size(); j++)
		{
			tagProbs[0][i] += getFeatureWeight(tagList->at(i), features.at(j)); //make matrix with each index in diagonal the sum of observed features for that tag
		}
	}

	//add feature weights for previous tag
	for (int i = 0; i < tagAmount; i++)
	{
		for (int j = 0; j < features.size(); j++)
		{
			tagProbsPrime[0][i] += getFeaturePrimeWeight(tagList->at(i), features.at(j));
		}
	}

	matrix = Matrix::addOuterMatrix(tagProbsPrime, tagProbs);

	probabilityMatrices.push_back(matrix);

}


Matrix::Fmatrix MLearn::CRF::calculateForwardVector(int position)
{
	/*
	
	calculate probabilities of previous tag at index t using forward algorithm

	f(t,y) - vector of probabilities for tag y in probability matrix at index t

	α(t,y) - probability of tag y in vector α(t)

	α(0) - 1d matrix of all 0s

	α(t,y) = logsumexp( α(t-1) + f(t,y) )
	
	*/
	
	if (position >= probabilityMatrices.size() or position < 0)
	{
		throw CRF_Error("calculateForwardVector(), Position Out Of Bounds For Current Set");
	}
	
	//current iteration's probabilities
	Custom::Matrix::Fmatrix tempMatrix = Custom::Matrix::Fmatrix(1, std::vector<float>(tagAmount, 0.0f));


	//iterate over probability matrices up to current position - 1
	for (int i = 0; i < position; i++)
	{
		
		//matrix to hold α(t-1)
		Matrix::Fmatrix tempMatrix2 = tempMatrix;

		//iterate over tags y
		for (int y = 0; y < tagAmount; y++)
		{

			//matrix to hold f(t,y)
			Matrix::Fmatrix prob = Matrix::Fmatrix(1, std::vector<float>(tagAmount, 0.0f));
			

			//create f(t,y)
			for (int k = 0; k < tagAmount; k++)
			{
				prob[0][k] = probabilityMatrices[i][k][y];
			}

			//add α(t-1) and f(t,y)
			prob = Matrix::addMatrix(tempMatrix2, prob);

			//logsumexp result
			tempMatrix[0][y] = logsumexp(Matrix::convertToVector(prob));

		}

	}
	
	return tempMatrix;

}



Matrix::Fmatrix MLearn::CRF::calculateBackwardVector(int position)
{
	
	/*
	
	calculate probabilities for current tag at index t using backwards algorithm

	f(t,yp) - vector of probabilities for previous tag yp in probability matrix at index t

	β(t,yp) - probability of previous tag yp at index t

	β(N+1) - 1d matrix holding all 1s

	β(t,yp) = logsumexp( β(t+1) + f(t,yp) )
	
	*/

	if (position >= probabilityMatrices.size() or position < 0)
	{
		throw CRF_Error("calculateBackwardVector(), Position Out Of Bounds For Current Set");
	}

	//current iteration's probabilities
	Custom::Matrix::Fmatrix tempMatrix = Custom::Matrix::Fmatrix(1, std::vector<float>(tagAmount, 1.0f));


	//iterate to position (t+1)
	for (int i = probabilityMatrices.size() - 1; i > position; i--)
	{
		
		//vector for β(n+1)
		Matrix::Fmatrix tempMatrix2 = tempMatrix;

		//iterate over previous tags yp
		for (int yp = 0; yp < tagAmount; yp++)
		{

			//matrix holding f(t,yp)
			Matrix::Fmatrix prob = Matrix::Fmatrix(1, probabilityMatrices[i][yp]);

			//add β(t+1) and f(t,yp)
			prob = Matrix::addMatrix(prob, tempMatrix2);

			//logsumexp the result
			tempMatrix[0][yp] = logsumexp(Matrix::convertToVector(prob));

		}

	}
	

	tempMatrix = tempMatrix;

	return tempMatrix;

}



std::vector<std::pair<int, int>> MLearn::CRF::viterbi()
{

	/*
	
	calculate most likely path through prediction matricies

	f(t,y) - vector of probabilities of current tag y in probability matrix t
	
	I(t,y) - matrix for most probable previous tag for each tag y

	l(t,yp) - probability of previous tag being yp at index t

	l(t,yp) = max( l(t-1,y) + f(t-1,y) )

	*/
	
	
	//matrix holding I(t,y)
	Matrix::Imatrix tagIndices = Matrix::Imatrix(probabilityMatrices.size(), std::vector<int>(tagAmount, 0));

	//matrix holding l(t-1,yp)
	std::vector<float> prevProbs = std::vector<float>(tagAmount, 0.0f);

	//matrix holding l(t,yp)
	std::vector<float> curProbs = std::vector<float>(tagAmount, 0.0f);

	//first tag is start tag
	prevProbs[startTag] = 1.0f;



	for (int i = 0; i < probabilityMatrices.size(); i++)
	{

		//iterate over each tag y
		for (int y = 0; y < tagAmount; y++)
		{

			std::vector<float> tempProbs = std::vector<float>(tagAmount, 0.0f);


			//iterate over previous tags yp
			for (int yp = 0; yp < tagAmount; yp++)
			{
				tempProbs[yp] = prevProbs[yp] + probabilityMatrices[i][yp][y]; //get l(t-1,y) + f(t-1,y)
			}


			
			//get max( l(t-1,y) + f(t-1,y) )
			auto argmax = std::max_element(tempProbs.begin(), tempProbs.end());

			tagIndices[i][y] = std::distance(tempProbs.begin(), argmax);

			//assign the most likely previous tag index to current tag index
			curProbs[y] = *argmax;

		}

		prevProbs = curProbs;

	}

	
	/*
	
	for selecting most probable tag sequence

	1. select end tag index at I(N,endTag) and get its contents (the index of the most likely previous tag)

	2. go to that index at I(N-1,y) and get its contents (again being the index of the most likely previous tag)

	3. repeat step 2 until you reach the first tag

	4. add the start tag as the first tag

	you now have a completed prediction set

	*/


	//set of predicted tags
	std::vector<std::pair<int, int>> tagPredictions;

	//last tag is end tag
	int nextTag = endTag;


	//go to index at position t-1 held in current position's index
	for (int i = probabilityMatrices.size() - 1; i > 0; i--)
	{

		tagPredictions.insert(tagPredictions.begin(), std::pair<int, int>(tagIndices[i][nextTag], nextTag));

		nextTag = tagIndices[i][nextTag];

	}

	//insert the start tag as first prediction
	tagPredictions.insert(tagPredictions.begin(), std::pair<int, int>(startTag, nextTag));

	return tagPredictions;

}

void MLearn::CRF::iterateWeights(std::vector<std::string> correctTags, float learnRate)
{
	
	//update weights for current tag
	std::thread t1(&CRF::updateCurWeights, this, correctTags, learnRate);

	//update weights for previous tag
	std::thread t2(&CRF::updatePrimeWeights, this, correctTags, learnRate);


	t1.join();
	t2.join();

}

float MLearn::CRF::logsumexp(std::vector<float> arr)
{

	/*
	
	max - max value in arr

	logsumexp(arr) = max + log( exp( arr[0] - max ) + exp( arr[1] - max ) ... )
	
	*/


	//get max element in arr
	auto max = std::max_element(arr.begin(), arr.end());


	float temp = 0;


	//sum of exp in all elements in arr minus max
	for (int i = 0; i < arr.size(); i++)
	{
		temp += exp(arr[i] - *max);
	}

	//return log of sum of exponentiated elements plus the max element
	return log(temp) + *max;

}

std::vector<float> MLearn::CRF::getTagProbs(int position, float normalizeFactor)
{
	
	/*
	
	normalized probabilities for each current tag for token at a position

	N - amount of tokens in current set

	N+1 - amount of probability matrices in current set

	f(t) - probability matrix at position t

	α(t) - forward vector for position t

	β(t) - backward vector for position t

	logz - normalization factor calculated using logsumexp( α(N) )

	p(y|yp;t) - matrix of unnormalized conditional probabilities for current tag y given previous tag yp at position t

	p(t,y) - set of probabilities for each current tag at position t

	FW(t) - forward and backward matrix combined

	addOuter(α,β) - add each element in vector α with entire vector β to make each row in new matrix equal to the size of α by the size of β

	sumColumns(m) - sum each element in each column in matrix m to create new vector


	FW(t) = addOuter( α(t-1) , β(t+1) )

	PFW(t) = exp( FW(t) + p(y|yp;t) - logz )

	p(t,y) = sumColumns( PFW(t) ) / e      - sum the columns and divide by Euler's number

	*/
	
	if (position >= probabilityMatrices.size() or position < 0)
	{
		throw CRF_Error("getTagProbs(), Position Out Of Bounds For Current Set");
	}

	//get probability matrix p(y|yp;t)
	Matrix::Fmatrix prediction = probabilityMatrices[position];


	//calculate the forward and backward vectors, α(t-1) and β(t+1) respectively
	auto forwardMatrix = std::async(&CRF::calculateForwardVector, this, position);
	auto backwardMatrix = std::async(&CRF::calculateBackwardVector, this, position);


	//get outer product of α(t-1) and β(t+1) to get FW(t)
	Matrix::Fmatrix FBMatrix = Matrix::addOuterMatrix(forwardMatrix.get(), backwardMatrix.get());


	//get PFW(t)
	prediction = Matrix::addMatrix(prediction, FBMatrix);
	prediction = Matrix::addMatrixWith(prediction, -1 * normalizeFactor);
	prediction = Matrix::expElementsInMatrix(prediction);


	
	//get p(t,y) without Euler's number
	std::vector<float> temp = Custom::Matrix::sumColumns(prediction);

	//divide each probability by Euler's number to normalize probabilities between 0 and 1
	for (int i = 0; i < temp.size(); i++)
	{
		temp[i] /= exp(1);
	}
	

	return temp;
}

std::vector<float> MLearn::CRF::getPrevTagProbs(int position, float normalizeFactor)
{
	
	/*

	normalized probabilities for each previous tag for token at a position

	N - amount of tokens in current set

	N+1 - amount of probability matrices in current set

	f(t) - probability matrix at position t

	α(t) - forward vector for position t

	β(t) - backward vector for position t

	logz - normalization factor calculated using logsumexp( α(N) )

	p(y|yp;t) - matrix of unnormalized conditional probabilities for previous tag y given previous tag yp at position t

	p(t,yp) - set of probabilities for each previous tag at position t

	FW(t) - forward and backward matrix combined

	addOuter(α,β) - add each element in vector α with entire vector β to make each row in new matrix equal to the size of α by the size of β

	sumRows(m) - sum each element in each row in matrix m to create new vector


	FW(t) = addOuter( α(t-1) , β(t+1) )

	PFW(t) = exp( FW(t) + p(y|yp;t) - logz )

	p(t,yp) = sumRows( PFW(t) ) / e        - sum rows and divide by Euler's number

	*/


	
	if (position >= probabilityMatrices.size() or position < 0)
	{
		throw CRF_Error("getPrevTagProbs(), Position Out Of Bounds For Current Set");
	}
	
	//get probability matrix p(y|yp;t)
	Matrix::Fmatrix prediction = probabilityMatrices[position];


	//calculate the forward and backward vectors, α(t-1) and β(t+1) respectively
	auto forwardMatrix = std::async(&CRF::calculateForwardVector, this, position);
	auto backwardMatrix = std::async(&CRF::calculateBackwardVector, this, position);

	//get outer product of α(t-1) and β(t+1)
	Matrix::Fmatrix FBMatrix = Matrix::addOuterMatrix(forwardMatrix.get(), backwardMatrix.get());


	//add the two matricies
	prediction = Matrix::addMatrix(prediction, FBMatrix);

	//subtract the normalization factor from each element in the prediction matrix
	prediction = Matrix::addMatrixWith(prediction, -1 * normalizeFactor);

	//exponentiate the matrix to get PWF(t)
	prediction = Matrix::expElementsInMatrix(prediction);


	//get p(t,yp) without Euler's number
	std::vector<float> temp = Custom::Matrix::sumRows(prediction);

	//divide by Euler's number to normalize probabilities between 0 and 1
	for (int i = 0; i < temp.size(); i++)
	{
		temp[i] /= exp(1);
	}
	

	return temp;
}



void MLearn::CRF::updateCurWeights(std::vector<std::string> correctTags, float learnRate)
{
	
	//get the normalization factor for current set
	float logz = logsumexp(Matrix::convertToVector(calculateForwardVector(probabilityMatrices.size())));


	//update weights for each feature
	for (int i = 0; i < correctTags.size(); i++)
	{

		//get probabilities of current tags for current token
		std::vector<float> tagProbs = getTagProbs(i, logz);

		//get index of current correct tag in tagList
		auto tagIndex = std::find(tagList->begin(), tagList->end(), correctTags[i]);
		auto probIndex = std::distance(tagList->begin(), tagIndex);


		//get the inverse of the probability the current tag is correct
		float inverseProb = (1 - tagProbs[probIndex]);

		if (inverseProb > 0) //only update the correct tag if it has a positive probability to avoid decreasing the features weights
		{
			for (const auto feature : features[i]) //iterate over each feature for current token
			{

				//subtract current probability with learn rate from weights for all tags' features
				for (int j = 0; j < tagList->size(); j++)
				{
					featureFunctionWeights[std::pair(tagList->at(j), feature)] -= tagProbs[j] * learnRate;
				}

				//add the same amount subtracted to weights for the correct tag's features to get net zero loss
				featureFunctionWeights[std::pair(correctTags[i], feature)] += tagProbs[probIndex] * learnRate;

				//increase the weights for the correct tag's feature
				featureFunctionWeights[std::pair(correctTags[i], feature)] += inverseProb * learnRate;

			}
		}
	}
}

void MLearn::CRF::updatePrimeWeights(std::vector<std::string> correctTags, float learnRate)
{
	
	//get the normalization factor for current set
	float logz = logsumexp(Matrix::convertToVector(calculateForwardVector(probabilityMatrices.size())));
	
	//update weights for each feature
	for (int i = 1; i < features.size(); i++)
	{

		//get probabilities of previous tags for current token
		std::vector<float> tagProbs = getPrevTagProbs(i, logz);

		//get index of previous correct tag in tagList
		auto tagIndex = std::find(tagList->begin(), tagList->end(), correctTags[i - 1]);
		auto probIndex = std::distance(tagList->begin(), tagIndex);


		//get the inverse of the probability the previous tag is correct
		float inverseProb = (1 - tagProbs[probIndex]);

		if (inverseProb > 0) //only update the correct tag if it has a positive probability to avoid decreasing the features weights
		{

			for (const std::string feature : features[i]) //iterate over each feature for current token
			{

				//subtract current probability with learn rate from weights for all tags' features
				for (int j = 0; j < tagAmount; j++)
				{
					featureFunctionPrimeWeights[std::pair(tagList->at(j), feature)] -= tagProbs[j] * learnRate;
				}

				//add the same amount subtracted to weights for the correct tag's features to get net zero loss
				featureFunctionPrimeWeights[std::pair(correctTags[i - 1], feature)] += tagProbs[probIndex] * learnRate;

				//increase the weights for the correct tag's feature
				featureFunctionPrimeWeights[std::pair(correctTags[i - 1], feature)] += inverseProb * learnRate;

			}
		}
	}
}

