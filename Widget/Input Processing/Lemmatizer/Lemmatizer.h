#pragma once


#include <assert.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "../Tokenizer.h"

namespace InProc
{

	class Lemmatizer
	{
	private:

		std::map<std::pair<std::string, std::string>, std::string> lemmaDict;

	public:

		Lemmatizer();

		std::string getLemma(std::string word, std::string tag);

	};

}