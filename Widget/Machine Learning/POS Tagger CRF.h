#pragma once

#include <algorithm>

#include "CRF.h"



namespace MLearn
{

	class PosTagCRF : public CRF
	{
	public:

		//vector of predefined tags for different types of words
		const std::vector<std::string> tags = 
		{
			"NUL",	// 0 Placeholder for attemps to get non existing word
			"CC",	// 1 Coordinating conjunction
			"CD",	// 2 Cardinal number
			"DT",	// 3 Determiner / Article
			"FW",	// 4 Foreign word
			"IN",	// 5 Preposition or subordinating conjunction
			"JJ",	// 6 Adjective
			"MD",	// 7 Modal
			"NN",	// 8 Noun, singular or mass
			"POS",	// 9 Possessive ending
			"PRP",	// 10 Personal pronoun
			"RB",	// 11 Adverb
			"SYM",	// 12 Symbol
			"VB",	// 13 Verb, base form
			",",	// 14 ","
			".",	// 15 "."
			"START",// 16 start tag for extra tag before sentence starts
			"STOP"  // 17 stop tag for after sentence ends
		};

		//matrix to hold probabilities of transitions between tags
		Custom::Matrix::Fmatrix transitionMatrix = Custom::Matrix::Fmatrix(tags.size(), std::vector<float>(tags.size(), 1.0f / tags.size()));


		//indexes are 
		//0 = true positive
		//1 = false positive
		//2 = false negative
		//3 = amount of times tag has bean seen
		std::map<std::string, std::array<int, 4>> tagF1Frequencies;

	public:

		PosTagCRF();

		//get features from tokens in a sentence
		std::vector<std::vector<std::string>> getFeatures(std::vector<std::string> tokens);

		//create set of probability matrices based on input features
		void createDataset(std::vector<std::vector<std::string>> features);

		void printF1Scores();

	};
}