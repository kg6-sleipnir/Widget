#include "POS Tagger CRF.h"

SenProc::PosTagCRF::PosTagCRF()
{

	tagAmount = tags.size();

	startTag = tags.size() - 2;
	endTag = tags.size() - 1;

	tagList = &tags;

}

std::vector<std::vector<std::string>> SenProc::PosTagCRF::createFeatures(std::vector<std::string> tokens)
{
	//vector to hold features of all tokens separately
	std::vector<std::vector<std::string>> features;
	
	//string that stores current word for next iteration's previous word
	std::string prevWord = "";

	//index number of current token
	int index = 0;

	//iterate over tokens
	for (auto& token : tokens)
	{

		//vector to hold features of current token
		std::vector<std::string> curFeatures;


		
		//check if word is capitalized
		if (65 <= token[0] and token[0] <= 90)
		{
			curFeatures.push_back("CAPITALIZED_");
		}

		bool isAlNum = true;
		bool isUpper = true;
		bool hyphen = false;
		bool period = false;
			

		//check if token contains a hyphen or period and if entire word is uppercase
		for (const auto& c : token)
		{

			if (97 < c or c < 122) //check if char is not uppercase - ASCII 97-122 is lowercase letters
			{
				isUpper = false;
			}
			else if (48 <= c and c <= 57) //check if char is number - ASCII 48-57 is numbers
			{
				curFeatures.push_back("CONTAINS_NUMBER_");
			}
			else if (c < 65 or 90 < c) //check if char is not uppercase, lowercase, or number - ASCII 65-90 is uppercase letters
			{
				isAlNum = false;
			}

		}

		//add features based on types of characters in token
		if (isAlNum == true)
		{
			curFeatures.push_back("IS_ALNUM");
		}
		else
		{
			curFeatures.push_back("HAS_SYMBOL");
		}

		if (isUpper == true)
		{
			curFeatures.push_back("ALL_UPPERCASE_");
		}

		if (hyphen == true)
		{
			curFeatures.push_back("HYPHEN_");
		}

		if (period == true)
		{
			curFeatures.push_back("PERIOD_");
		}
			

		//push back features signifying first and last token
		if (&token == &tokens.front())
		{
			curFeatures.push_back("IS_FIRST_");
		}
		else if (&token == &tokens.back())
		{
			curFeatures.push_back("IS_LAST_");
		}
		

		//transform token to lowercase
		std::transform(token.begin(), token.end(), token.begin(), ::tolower);

		//push back token as feature
		curFeatures.push_back(std::string("WORD_").append(token));


		//add full tokens as features to previous and current feature sets
		if (index != 0)
		{

			//append current token to previous token's feature set as next word feature
			features[index - 1].insert(features[index - 1].begin() + 1, std::string("NEXT_").append(token));

			//push back previous token to current token's feature set as previous word feature
			curFeatures.push_back(std::string("PREVIOUS_").append(prevWord));

		}

		//set previous token as current token
		prevWord = token;


		//switch statement to get first and last 1, 2, 3, and 4 letters of each token as features
		//depending on the length of the token, as well as the token itself as a feature
		switch (token.size())
		{
		default: //get first and last 4 letters if word is at least 4 letters long

			curFeatures.push_back(std::string("_LAST_4_").append(token.end() - 4, token.end()));
			curFeatures.push_back(std::string("_FIRST_4_").append(token.begin(), token.begin() + 4));

		case 3:  //get first and last 3 letters as features if word longer than 3 letters

			curFeatures.push_back(std::string("_LAST_3_").append(token.end() - 3, token.end()));
			curFeatures.push_back(std::string("_FIRST_3_").append(token.begin(), token.begin() + 3));

		case 2: //get first and last 2 letters if word length is at least 2 letters long

			curFeatures.push_back(std::string("_LAST_2_").append(token.end() - 2, token.end()));
			curFeatures.push_back(std::string("_FIRST_2_").append(token.begin(), token.begin() + 2));

		case 1: //get first and last letter of word and entire word if length is at least 1 letters long

			curFeatures.push_back(std::string("_LAST_1_").append(token.end() - 1, token.end()));
			curFeatures.push_back(std::string("_FIRST_1_").append(token.begin(), token.begin() + 1));
			break;

		case 0: //I don't think that this case would ever happen but just in case

			//error handling with SaRcAsM
			throw MLearn::CRF_Error("Encountered Null Token When Getting First/Last letter features");
			break;
		}

		//increment index of current token
		index++;


		//push back the current features of current token
		features.push_back(curFeatures);

	}

	//push back feature indicating there is no next token to last token
	features.back().push_back(std::string("NEXT_END_TAG_"));

	//push back an ending tag for better processing in the CRF
	features.push_back(std::vector<std::string>({ "END_TAG_" , std::string("PREVIOUS_").append(prevWord) }));


	return features;
}

void SenProc::PosTagCRF::createDataset(std::vector<std::string> tokens)
{
	
	//clear previous probability matrices
	probabilityMatrices.clear();


	//create current set of features for predictions of updating weights
	//defined in CRF.h
	features = createFeatures(tokens);


	//create probability matrices based on features
	for (const auto feature : features)
	{
		createProbabilityMatrix(feature);
	}

}

void SenProc::PosTagCRF::printF1Scores()
{
	int totalTruePositive = 0;
	int totalFalsePositive = 0;
	int totalFalseNegative = 0;
	int TotalTagCount = 0;

	for (const auto& i : tagF1Frequencies)
	{

#define TruePositive i.second[0]
#define FalsePositive i.second[1]
#define FalseNegative i.second[2]
#define TagCount i.second[3]


		totalTruePositive += TruePositive;
		totalFalsePositive += FalsePositive;
		totalFalseNegative += FalseNegative;
		TotalTagCount += TagCount;


		float precision = (float)TruePositive / (TruePositive + FalsePositive);
		float recall = (float)TruePositive / (TruePositive + FalseNegative);

		std::cout << i.first << ": " << precision << ",   " << recall << ",   " << 2 * ((precision * recall) / (precision + recall)) << ",   " << TagCount << "\n";
	}

	float totalPrecision = (float)totalTruePositive / (totalTruePositive + totalFalsePositive);
	float totalRecall = (float)totalTruePositive / (totalTruePositive + totalFalseNegative);


	std::cout << "Total scores: " << totalPrecision << ",   " << totalRecall << ",   " << 2 * ((totalPrecision * totalRecall) / (totalPrecision + totalRecall)) << ",   " << TotalTagCount << "\n\n";
}

void SenProc::PosTagCRF::clearF1Scores()
{
	tagF1Frequencies.clear();
}

void SenProc::PosTagCRF::addToF1Scores(std::string correctTag, std::string predictedTag)
{

	if (correctTag == predictedTag) //check if predicted tag is the correct tag
	{
		tagF1Frequencies[correctTag][0]++; //correct tag true positive
		tagF1Frequencies[correctTag][3]++; //correct tag amount of times seen
	}
	else //check if predicted tag is not the correct tag
	{
		tagF1Frequencies[predictedTag][1]++; //predicted tag false positive
		tagF1Frequencies[predictedTag][3]++; //predicted tag amount of time seen

		tagF1Frequencies[correctTag][2]++; //correct tag false negative
		tagF1Frequencies[correctTag][3]++; //correct tag amount of times seen
	}

}
