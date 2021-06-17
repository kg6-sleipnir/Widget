#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
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


		
		std::vector<float> weights;


		void addFeature(std::string feature);

		void addTag(std::string tag);

		void getFeaturesFromFile(std::string inputFile);

		void getTagsFromFile(std::string inputFile);

		void createWeights();

		std::vector<std::vector<std::vector<float>>> createProbabilityMatrices();

		std::vector<std::pair<std::string, std::string>> predictTags(std::vector<std::string> &tokens);

		

	};
}