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
			"CCONJ",// 1 Conjuctions
			"NUM",	// 2 Cardinal number
			"DET",	// 3 Determiner / Article
			"X",	// 4 Extra words
			"ADP",	// 5 Adpositions
			"ADJ",	// 6 Adjective
			"NOUN",	// 7 Noun
			"PROPN",// 8 Proper noun
			"PRON", // 9 Pronoun
			"PART",	// 10 Particle
			"ADV",	// 11 Adverbs
			"SYM",	// 12 Symbol
			"VERB",	// 13 Verb
			"PUNCT",// 14 punctuation
			".",	// 15 Period
			"START",// 16 start tag
			"STOP"  // 17 stop tag
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