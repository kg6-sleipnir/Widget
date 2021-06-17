#pragma once

#include <eh.h>
#include <string>
#include <vector>


//list of possible tags a word can have
static std::string wordType[] = {
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
	",", //","
	".", //"."
	"$", //"$"
	":", //no idea what this tag is
	";", // ; apparently
	"LQU", //quotes have their own tags, TIL
	"RQU", //and the begining and end quotes have different tags :P
	"LRB",
	"RRB", //same thing with brackets and parentheses i guess?
	"AS", //alright i swear this databank is making up tags at this point
	"SO" //thats what it is garanteed
};

class sentence
{
	class token
	{

		std::string word;

		int tag;

	};


	std::vector<token> tokens;

	//construct sentence object from string input
	sentence(std::string words);

};