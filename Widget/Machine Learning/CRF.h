#pragma once

#include <eh.h>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <thread>
#include <time.h>
#include <vector>

#include "../Input Processing/Tokenizer.h"
#include "../Custom Libraries/Matrix.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace Custom;

namespace MLearn
{
	//creates a learning structure using a conditional random field algorithm
	//this is the base CRF without a prediction function
	//to use the CRF create a new class with an appropriate
	//prediction function for what you are trying to predict
	//
	//also create a const "std::vector<std::string> tags" variable with 
	//tags that you want to make predictions for, and a transition matrix
	//for holding transitional probabilities of size tags.size() by tags.size()
	//
	//make sure tags[0] is a NULL tag
	class CRF
	{
	public:

		//map of feature functions with their weights
		//contains map with key value <TAG, FEATURE> with a weight as a float
		//to get the probability of a tag
		//iterate over each feature associated with the tag
		//and add up the weights for those features
		std::map<std::pair<std::string, std::string>, float> featureFunctionWeights;
		
		//vector of matrices containing probabilities of current and previous tag
		//horizontal axis is current tag
		//vertical axis is previous tag
		std::vector<Matrix::Fmatrix> probabilityMatrices;



		//empty function that will be redefined in derived CRF objects
		//when redefined, it should get all features from a set of input tokens
		//and create the probability matrices for all tokens
		virtual void createDataset(std::vector<std::vector<std::string>> features) {};

		//empty function that will be redefined in derived CRF objects
		//when redefined, it shoul get all features from a set of input tokens
		virtual std::vector<std::vector<std::string>> getFeatures(std::vector<std::string> tokens) { return std::vector<std::vector<std::string>>(); };



		//gets the weight of the feature function if it exists
		//if it doesn't exist it will add it to featureFunctionWeights for future reference
		float getFeatureWeight(std::pair<std::string, std::string> featureFunction);

		//create matrices that determine the probability that an index in a sequence is a tag
		//use tagOverride to force the tag at a position to be a predetermined tag
		void createProbabilityMatrix(std::vector<std::string>& features, Custom::Matrix::Fmatrix* transitionMatrix, const std::vector<std::string>* tags, int position, int tagOverride = -1);
		
		//caclulate forward vector to get probabilities of tags for previous indexes in a sequence
		Matrix::Fmatrix calculateForwardVector(int position);

		//calculate backward vector to get probabilities of tags for following indexes in a sequence
		Matrix::Fmatrix calculateBackwardVector(int position);

		//get normalization factor (Z) for calculating probability
		float normalizeFactor(int startTagIndex, int endTagIndex);

		//get pair of tags with highest probability at a position
		std::pair<int, int> predictTag(int position, int startTag, int endTag);

		void updateTransitionMatrix(std::vector<std::pair<std::vector<std::string>, std::string>>& featureTokens, const std::vector<std::string>* tags, Custom::Matrix::Fmatrix& transitionMatrix, std::array<float, 2> learnrates);

		void updateWeights(std::vector<std::pair<std::vector<std::string>, std::string>> featureTokens, const std::vector<std::string>* tags, std::array<float, 7> learnRates);

	};


	//error handling object
	struct CRF_Error : public std::exception
	{
		//create message
		CRF_Error(char const* const message) : std::exception(message) {}

		//get message
		virtual char const* what() const noexcept
		{
			return std::exception::what();
		}
	};
}