#pragma once

#include <algorithm>

#include "CRF.h"



namespace MLearn
{

	//Conditional Random Field for predicting parts of speech for words in a sentence
	class PosTagCRF : public CRF
	{
	private:

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


		//indexes are 
		//0 = true positive
		//1 = false positive
		//2 = false negative
		//3 = amount of times tag has bean seen
		std::map<std::string, std::array<int, 4>> tagF1Frequencies;

		//get features from tokens in a sentence
		std::vector<std::vector<std::string>> createFeatures(std::vector<std::string> tokens);
		
	public:

		PosTagCRF();


		//create dataset based on set of tokens
		void createDataset(std::vector<std::string> tokens);

		//print stored f1 scores
		void printF1Scores();

		//clear stored f1 scores
		void clearF1Scores();

		//increment f1 scores for tag
		void addToF1Scores(std::string correctTag, std::string predictedTag);

	};
}