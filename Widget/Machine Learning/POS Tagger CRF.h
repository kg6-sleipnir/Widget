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
			"DT",	// 3 Determiner
			"EX",	// 4 Existential there
			"FW",	// 5 Foreign word
			"IN",	// 6 Preposition or subordinating conjunction
			"JJ",	// 7 Adjective
			"JJR",	// 8 Adjective, comparative
			"JJS",	// 9 Adjective, superlative
			"LS",	// 10 List item marker
			"MD",	// 11 Modal
			"NN",	// 12 Noun, singular or mass
			"NNS",	// 13 Noun, plural
			"NNP",	// 14 Proper noun, singular
			"NNPS",	// 15 Proper noun, plural
			"PDT",	// 16 Predeterminer
			"POS",	// 17 Possessive ending
			"PRP",	// 18 Personal pronoun
			"PRP$",	// 19 Possessive pronoun
			"RB",	// 20 Adverb
			"RBR",	// 21 Adverb, comparative
			"RBS",	// 22 Adverb, superlative
			"RP",	// 23 Particle
			"SYM",	// 24 Symbol
			"TO",	// 25 to
			"UH",	// 26 Interjection
			"VB",	// 27 Verb, base form
			"VBD",	// 28 Verb, past tense
			"VBG",	// 29 Verb, gerund or present participle
			"VBN",	// 30 Verb, past participle
			"VBP",	// 31 Verb, non - 3rd person singular present
			"VBZ",	// 32 Verb, 3rd person singular present
			"WDT",	// 33 Wh - determiner
			"WP",	// 34 Wh - pronoun
			"WP$",	// 35 Possessive wh - pronoun
			"WRB",	// 36 Wh - adverb
			",",	// 37 ","
			".",	// 38 "."
			"$",	// 39 "$"
			"QU",   // 40 quotes have their own tags, TIL
			"LRB",  // 41  
			"RRB",  // 42 same thing with brackets and parentheses i guess?
			"AS",   // 43 alright i swear this databank is making up tags at this point
			"SO",   // 44 thats what it is garanteed
			"START",// 45 start tag for extra tag before sentence starts
			"STOP"  // 46 stop tag for after sentence ends
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