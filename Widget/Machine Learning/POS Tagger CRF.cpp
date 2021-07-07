#include "POS Tagger CRF.h"

std::vector<std::vector<std::string>> MLearn::PosTagCRF::getFeatures(std::vector<std::string> tokens)
{
	//vector to hold features of all tokens separately
	std::vector<std::vector<std::string>> features;
	
	//push back a beginning tag for better processing in the CRF
	features.push_back(std::vector<std::string>({ "START_TAG_" }));

	//temporary string to hold previous word
	//starts out with the "BEGIN_TAG_" feature
	//that was just created
	std::string prevWord = features[0][0];

	
	//index number of current token
	int index = 0;

	//iterate over tokens
	for (const auto& token : tokens)
	{
		index++;

		//vector to hold features of current token
		std::vector<std::string> curFeatures;

		curFeatures.push_back(std::string("_WORD_").append(token));

#pragma warning(disable:26451)
		//append current token to previous tokens features with string constructor shenanigans (I use this a lot)
		features[index - 1].insert(features[index - 1].begin() + 1, std::string("NEXT_").append(token));
#pragma warning(default:26451)
		

		//push back previous word to features using more string constructor shenanigans
		curFeatures.push_back(std::string("PREVIOUS_").append(prevWord));

		//change previous word to current word for next token
		prevWord = token;


		//perform various checks inside separate scope to keep extra variables out of local scope
		{
			//check if word is capitalized
			if (isupper(token[0]))
			{
				curFeatures.push_back("CAPITALIZED_");
			}


			bool isUpper = true;
			bool hyphen = false;
			bool period = false;

			//check if token contains a hyphen or period and if entire word is uppercase
			for (const auto& c : token)
			{
				if (!isupper(c)) //check if not uppercase
				{
					if (c == '-') //check for hyphen
					{
						hyphen = true;

						if (isUpper == false) //if word is not uppercase and contains a hyphen, it will almost never if not never contain a period
						{
							break;
						}
					}
					else if (c == '.') //check for period inside word
					{
						period = true;
					}
					else
					{
						isUpper = false;
					}
				}
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


			//check if token contains a digit
			if (isdigit(token[0]))
			{
				curFeatures.push_back("IS_NUMBER_");
			}
		}


#pragma warning(disable:26819) //disable warning message for fallthrough in switch cause it annoys me
		
		//switch statement to get first and last 1, 2, and 3 letters of each token as features
		//depending on the length of the token, as well as the token itself as a feature
		switch (token.size())
		{
		default: //get first and last 4 letters if word is at least 4 letters long

			curFeatures.push_back(std::string("_FIRST_4_").append(token.begin(), token.begin() + 4));
			curFeatures.push_back(std::string("_LAST_4_").append(token.end() - 4, token.end()));

		case 3:  //get first and last 3 letters as features if word longer than 3 letters

			curFeatures.push_back(std::string("_FIRST_3_").append(token.begin(), token.begin() + 3));
			curFeatures.push_back(std::string("_LAST_3_").append(token.end() - 3, token.end()));

		case 2: //get first and last 2 letters if word length is at least 2 letters long

			curFeatures.push_back(std::string("_FIRST_2_").append(token.begin(), token.begin() + 2));
			curFeatures.push_back(std::string("_LAST_2_").append(token.end() - 2, token.end()));

		case 1: //get first and last letter of word and entire word if length is at least 1 letters long

			curFeatures.push_back(std::string("_FIRST_1_").append(token.begin(), token.begin() + 1));
			curFeatures.push_back(std::string("_LAST_1_").append(token.end() - 1, token.end()));
			
			break;
			
		case 0: //I don't think that this case would ever happen but just in case

			//error handling with SaRcAsM
			throw CRF_Error("Encountered Token With Length of 0 SoMeHoW IdK WhAt HaPpEnEd");
			break;
		}

#pragma warning(default:26819) //re-enable the warning


		features.push_back(curFeatures);
	}

	//push back an ending tag for better processing in the CRF
	features.push_back(std::vector<std::string>({ "END_TAG_" , prevWord }));


	return features;
}

void MLearn::PosTagCRF::createDataset(std::vector<std::vector<std::string>> features)
{
	//clear previous probability matrices
	probabilityMatrices.clear();
	
	//create probability matrix with starting tag
	createProbabilityMatrix(features[0], &transitionMatrix, &tags, 0, 48);

	//create probability matrices between start and end tag
	for (int i = 1; i < features.size() - 1; i++)
	{
		createProbabilityMatrix(features[i], &transitionMatrix, &tags, i);
	}

	//create probability matrix with ending tag
	createProbabilityMatrix(features[features.size() - 1], &transitionMatrix, &tags, features.size() - 1, 49);
}
