#include "POS Tagger CRF.h"

std::vector<std::pair<std::string, std::string>> MLearn::PosTagCRF::makePrediction(std::vector<std::string> tokens)
{
	for (const auto& token : tokens)
	{
		std::vector<std::pair<std::string, std::string>> features;

		features.push_back();
	}

	return std::vector<std::pair<std::string, std::string>>();
}
