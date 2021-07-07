#pragma once

#include "CRF.h"


namespace MLearn
{
	class PosTagCRF : public CRF
	{
	public:

		//vector of predefined tags for different types of words
		const std::vector<std::string> tags = 
		{
			"NUL",	//Placeholder for attemps to get non existing word
			"CC",	//Coordinating conjunction
			"CD",	//Cardinal number
			"DT",	//Determiner
			"EX",	//Existential there
			"FW",	//Foreign word
			"IN",	//Preposition or subordinating conjunction
			"JJ",	//Adjective
			"JJR",	//Adjective, comparative
			"JJS",	//Adjective, superlative
			"LS",	//List item marker
			"MD",	//Modal
			"NN",	//Noun, singular or mass
			"NNS",	//Noun, plural
			"NNP",	//Proper noun, singular
			"NNPS",	//Proper noun, plural
			"PDT",	//Predeterminer
			"POS",	//Possessive ending
			"PRP",	//Personal pronoun
			"PRP$",	//Possessive pronoun
			"RB",	//Adverb
			"RBR",	//Adverb, comparative
			"RBS",	//Adverb, superlative
			"RP",	//Particle
			"SYM",	//Symbol
			"TO",	//to
			"UH",	//Interjection
			"VB",	//Verb, base form
			"VBD",	//Verb, past tense
			"VBG",	//Verb, gerund or present participle
			"VBN",	//Verb, past participle
			"VBP",	//Verb, non - 3rd person singular present
			"VBZ",	//Verb, 3rd person singular present
			"WDT",	//Wh - determiner
			"WP",	//Wh - pronoun
			"WP$",	//Possessive wh - pronoun
			"WRB",	//Wh - adverb
			",",	//","
			".",	//"."
			"$",	//"$"
			":",	//no idea what this tag is
			";",	// ; apparently
			"LQU",  //quotes have their own tags, TIL
			"RQU",  //and the begining and end quotes have different tags :P
			"LRB",
			"RRB",  //same thing with brackets and parentheses i guess?
			"AS",   //alright i swear this databank is making up tags at this point
			"SO",   //thats what it is garanteed
			"START",//start tag for extra tag before sentence starts
			"STOP"  //stop tag for after sentence ends
		};

		//matrix to hold probabilities of transitions between tags
		Custom::Matrix::Fmatrix transitionMatrix = Custom::Matrix::Fmatrix(50, std::vector<float>(50, 0));

	public:

		//get features from tokens in a sentence
		std::vector<std::vector<std::string>> getFeatures(std::vector<std::string> tokens);

		//create set of probability matrices based on input features
		void createDataset(std::vector<std::vector<std::string>> features);

	};
}