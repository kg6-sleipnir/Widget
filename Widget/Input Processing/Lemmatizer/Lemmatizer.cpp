#include "Lemmatizer.h"

InProc::Lemmatizer::Lemmatizer()
{

#define LemmaFile "./Input Processing/Lemmatizer/Lemma Dictionary.sav"

	std::fstream lemmaFile;

	lemmaFile.open(LemmaFile, std::ios::in);

	std::string curLine;

	while (std::getline(lemmaFile, curLine))
	{

#define wordTag std::pair<std::string, std::string>(tokens[0], tokens[1])
#define lemma tokens[2]

		std::vector<std::string> tokens = tokenize<std::string>(curLine);

		lemmaDict[wordTag] = lemma;

	}

}

std::string InProc::Lemmatizer::getLemma(std::string word, std::string tag)
{
	return lemmaDict[std::pair(word, tag)];
}
