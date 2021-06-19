#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <time.h>
#include <vector>

#include "../Input Processing/Tokenizer.h"
#include "../Custom Libraries/Matrix.h"

#define _USE_MATH_DEFINES
#include <cmath>


namespace MLearn
{
	//creates a learning structure using a conditional random field algorithm
	//this is the base CRF without a prediction function
	//to use the CRF create a new class with an appropriate
	//prediction function for what you are trying to predict
	//
	//also create a const "std::vector<std::string> tags" variable with 
	//tags that you want to make predictions for
	class CRF
	{
	public:

		const std::vector<std::string> tags;

		//map of feature functions with their weights
		//contains map with key value <TAG, FEATURE> with a weight as a float
		//to get the probability of a tag
		//iterate over each feature associated with the tag
		//and add up the weights for those features
		std::map<std::pair<std::string, std::string>, float> featureFunctionWeights;
		
		//vector of matrices containing probabilities of current and previous tag
		//horizontal axis is current tag
		//vertical axis is previous tag
		std::vector<std::vector<std::vector<float>>> probabilityMatrices;


		//gets the weight of the feature function if it exists
		//if it doesn't exist it will add it to featureFunctionWeights for future reference
		float getFeatureWeight(std::pair<std::string, std::string> featureFunction);

		void createProbabilityMatrix(std::vector<std::string> &features, int position);

		void updateWeights(std::vector<std::pair<std::string, std::string>> tokenAnswers);

	};
}