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
	class CRF
	{
#ifndef _DEBUG
	private:
#else
	public:
#endif

		std::vector<std::string> features;
		std::vector<std::string> tags;

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


		void addFeature(std::string feature);

		void addTag(std::string tag);

		void getFeaturesFromFile(std::string inputFile);

		void getTagsFromFile(std::string inputFile);

		void createFeatureFunctionWeights();

		void createProbabilityMatrix(std::vector<std::pair<std::string, std::string>> &featureFunctions, int position);

		

		

	};
}