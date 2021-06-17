#pragma once

#include <string>

//list of types of word
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
	"RRB", //same thing with brackets i guess?
	"AS", //alright i swear this databank is making up tags at this point
	"SO" //thats what it is garanteed
};

class words;
class sentence;

inline int tag_Exeption(std::string errorType);
inline int tagPos(std::string tag);
inline sentence initializeWords(std::vector<std::string> tokens);


class words
{
public:
	std::string word;
	int tag = 0;
	int prevTag = 0;
	int nextTag = 0;
	bool isFirst = false;
	bool isLast = false;
	bool hyphen = false;
	char pre1 = 0;
	char pre2 = 0;
	char pre3 = 0;
	char suf1 = 0;
	char suf2 = 0;
	char suf3 = 0;


	//get values for each variable
	void initializeWord()
	{
		pre1 = word[0];
		suf3 = word[word.size() - 1];

		if (word.size() >= 3)
		{
			pre3 = word[2];
			suf3 = word[word.size() - 3];
			pre2 = word[1];
			suf2 = word[word.size() - 2];
		}
		else if (word.size() >= 2)
		{
			pre2 = word[1];
			suf2 = word[word.size() - 2];
		}

		if (word.find('-') < word.size())
		{
			hyphen = true;
		}
		

		//if word is "to" set tag to "TO" as it is the only word in the "TO" tag
		if (word == "to")
		{
			tag = tagPos("TO");
		}
		else if (word == ".")
		{
			tag = tagPos(".");
		}
		else if (word == ",")
		{
			tag = tagPos(",");
		}
		else if (isdigit(pre1))
		{
			tag = tagPos("CD");
		}

	}
};

class sentence
{
public:
	std::vector<words> word;
};
//error handling
inline int tag_Exeption(std::string errorType)
{
	std::cout << "tagExeption(" << errorType << ") \n";
	return -1;
}

//returns position of tag in wordType[]
inline int tagPos(std::string tag)
{
	std::string *pos = std::find(begin(wordType), end(wordType), tag);

	if (pos != end(wordType))
	{
		return std::distance(begin(wordType), pos);
	}
	else
	{
		throw tag_Exeption("Unexpected Tag: tag not in dictionary");
	}
}

//creates tokens and initializes some variables for individual words
inline sentence initializeWords(std::vector<std::string> tokens)
{
	sentence sent;

	//create sentence from input tokens and initialize each word
	for (int i = 0; i < tokens.size(); i++)
	{
		words curWord;
		curWord.word = tokens[i];
		sent.word.push_back(curWord);
		if (i == 0)
		{
			sent.word[i].isFirst = true;
		}
		else if (i == tokens.size() - 1)
		{
			sent.word[i].isLast = true;
		}

		//initialize each word to create variables for machine learning
		sent.word[i].initializeWord();
	}

	return sent;
}