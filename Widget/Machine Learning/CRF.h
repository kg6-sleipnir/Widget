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
#include "../Custom Libraries/Mat.h"

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
	//tags that you want to make predictions for, and a transition matrix
	//for holding transitional probabilities of size tags.size() by tags.size()
	//
	//make sure tags[0] is a NULL tag
	class CRF
	{
	protected:

		int tagAmount;

		int startTag;

		int endTag;


		//set of current features for updating weights
		std::vector<std::vector<std::string>> features;


		//vector of matrices containing probabilities of current and previous tag
		//horizontal axis is current tag
		//vertical axis is previous tag
		std::vector<Mat::V2f> probabilityMatrices;



		//create matrices that determine the probability that a token in a sequence is a tag
		//use tagOverride to force the tag at a position to be a predetermined tag
		void createProbabilityMatrix(std::vector<std::string> features);
	
	private: 

		//list of feature functions with their weights
		//indexing is in pair of <TAG, FEATURE>
		std::map<std::pair<std::string, std::string>, float> featureFunctionWeights;


		//weights for features of current token associated with probability of previous tag
		//indexing is in pair of <TAG, FEATURE>
		std::map<std::pair<std::string, std::string>, float> featureFunctionPrimeWeights;


		//true when weights are frozen
		bool frozen = false;


		//get normalised probabilities of current tag using variant of forward backward algorithm from HMMs
		std::vector<float> getTagProbs(int position, float normalizeFactor);

		//get normalised probabilities of previous tag using variant of forward backward algorithm from HMMs
		std::vector<float> getPrevTagProbs(int position, float normalizeFactor);

		//gets or create the weight of the feature function associated with current token
		float getFeatureWeight(std::string tag, std::string feature);

		void modifyFeatureWeight(std::string tag, std::string feature, float amount);

		//gets or create the weight of the feature function associated with previous token
		float getFeaturePrimeWeight(std::string tag, std::string feature);

		void modifyFeaturePrimeWeight(std::string tag, std::string feature, float amount);

		//caclulate forward vector to get probabilities of previous tag
		std::vector<float> calculateForwardVector(int position);

		//calculate backward vector to get probabilities of current tag
		std::vector<float> calculateBackwardVector(int position);

		//get logsumexp of 1 dimensional matrix
		float logsumexp(std::vector<float> arr);



		//update the weights of current features related to current token
		//current features is already known from when the dataset was created
		void updateCurWeights(std::vector<std::string> correctTags, float learnRate);


		//update the weights of current features related to previous token
		//current features is already known from when the dataset was created
		void updatePrimeWeights(std::vector<std::string> correctTags, float learnRate);



	public:

		//list of tags to be defined in inherited cobject
		const std::vector<std::string>* tagList;



		//get set of predictions on current data based on Viterbi algorithm
		std::vector<std::pair<int, int>> viterbi();

		//update the weights for current features for current iteration
		//current features is already known from when current dataset was created
		void iterateWeights(std::vector<std::string> correctTags, float learnRate);

		//remove weights with values between a low and high number
		void removeWeights(float low, float high);

		//save weights to a file
		void saveWeights(std::string outputFile);

		//load weights from file
		void loadWeights(std::string inFile);

		//freeze weights to avoid updates or creation
		void freezeWeights();

		//unfreeze weights to allow for updates or creation
		void unfreezeWeights();

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