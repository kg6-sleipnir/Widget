#include "Decision Tree.h"

using namespace MLearn;


DecisionTree::NodeType DecisionTree::Node::getType()
{
	return type;
}

int DecisionTree::Node::getVarNum()
{
	return varNum;
}

int DecisionTree::Node::getValue()
{
	return value;
}

void DecisionTree::Node::makeChild(childNode child, int varValue)
{
	if (std::holds_alternative<InteriorNode>(child))
	{
		children[varValue] = std::get<InteriorNode>(child);
	}
	else
	{
		children[varValue] = std::get<LeafNode>(child);
	}
}

std::map<int, DecisionTree::Node>* DecisionTree::Node::getChildren()
{
	return &children;
}

//construct leaf node with prediction value and node type
DecisionTree::LeafNode::LeafNode(int val)
{
	varNum = NULL;
	value = val;
	type = NodeType::LEAF;
}

//construct an interior node with its node type
DecisionTree::InteriorNode::InteriorNode(int var, int val)
{
	varNum = var;
	value = val;
	type = NodeType::INTERIOR;
}

DecisionTree::RootNode::RootNode(int var, int val)
{
	varNum = var;
	value = val;
	type = NodeType::ROOT;
}

//get entropy of a variable's value
double DecisionTree::entropy(dataTable T, int varNum, int varVal)
{
	double ent = 0;
	int denominator = 0;


	//get unique value counts of target class that are associated with the value varVal in the variable varNum
	std::map<int, int> m = [&T, varNum, varVal, &denominator] ()
	{
		dataTable temp = T.splitData(varNum, varVal);
		denominator = temp.getTableSize();
		return temp.getCounts(temp.getTargetClass());
	}();


	//get entropy from probability using the denominator
	for (auto it = m.begin(); it != m.end(); it++)
	{
		double temp = (double)it->second / denominator;
		ent -= log2(temp) * temp;
	}


	return ent;
}

//get entropy of a variable
double DecisionTree::entropy(dataTable &T, int varNum)
{
	std::map<int, int> vars = T.getCounts(varNum);
	

	//temporary values to hold total number of target values and total entropy
	int denominator = 0;
	double totalEnt = 0;

	
	for (auto it = vars.begin(); it != vars.end(); it++)
	{
		int temp = denominator;
		
		denominator += T.splitData(varNum, it->first).getTableSize();

		//we can use magical math laws to multiply the numerator of the probability now
		//and divide by the denominator later to get the correct entropy
		//since we dont yet know what the final denominator is yet
		totalEnt += ((double)denominator - temp) * entropy(T, varNum, it->first);
	}

	//magical math laws
	totalEnt /= denominator;

	return totalEnt;
}

//get entropy of entire tree
double DecisionTree::entropy(dataTable &T)
{
	double ent = 0;
	int denominator = T.getTableSize();

	std::map<int, int> m = T.getCounts(T.getTargetClass());

	for (auto it = m.begin(); it != m.end(); it++)
	{
		double temp = (double)it->second / denominator;
		ent -= temp * log2(temp);
	}

	return ent;
}

double DecisionTree::infoGain(dataTable &T, double parentEnt, double childEnt)
{
	return parentEnt - entropy(T, childEnt);
}

int DecisionTree::highestGain(dataTable &T, double parentEnt)
{
	std::map<int, int> vars = T.getRow(0);
	vars.erase(T.getTargetClass());

	double maxVal = -1;
	int maxIndex = 0;

	for (auto it = vars.begin(); it != vars.end(); it++)
	{
		double temp = infoGain(T, parentEnt, it->first);

		if (temp > maxVal)
		{
			maxVal = temp;
			maxIndex = it->first;
		}
	}

	return maxIndex;
}


std::variant<DecisionTree::RootNode, DecisionTree::InteriorNode, DecisionTree::LeafNode> DecisionTree::createTree(dataTable T, int recursion, int maxRecursion)
{
	std::map<int, int> targetCounts = T.getCounts(T.getTargetClass());
	
	//if there is only 1 value remaining in the possible answer values then return it
	if (targetCounts.size() == 1)
	{
		return LeafNode(targetCounts.begin()->first);
	}
	else if (targetCounts.size() == 0)
	{
#ifdef _DEBUG
		std::cout << "NULL BRANCH";
#endif

		return LeafNode(0);
	}
	else if (T.getRow(0).size() == 1 or recursion == maxRecursion) //check if there are no more variables left to compare
	{
		//set temporary values
		int max = -1;
		std::map<int, int> m = targetCounts;


		//check if there are more than one possible answer values left
		if (m.size() > 1)
		{
			for (std::map<int, int>::iterator i = m.begin(); i != m.end(); i++)
			{
				if (i->first > max)
				{
					max = i->first; //return the answer value with the highest probability
				}
			}
		}
		else if (m.size() == 1)
		{
			max = m.begin()->first;
		}


		return LeafNode(max);
	}
	else if (recursion == 0) //begin creating the tree with a root node
	{
		//add to recursion counter
		recursion++;
		
		//index of variable with highest gain
		int max = highestGain(T, entropy(T));

		int maxVal = -1;
		std::map<int, int> m = targetCounts;


		//set most likely answer for queries with values that dont exist
		if (m.size() > 1)
		{
			for (std::map<int, int>::iterator i = m.begin(); i != m.end(); i++)
			{
				if (i->first > maxVal)
				{
					maxVal = i->first; //return the answer value with the highest probability
				}
			}
		}
		else if (m.size() == 1)
		{
			maxVal = m.begin()->first;
		}


		//create a root node and set its variable number
		RootNode R(max, maxVal);

		//get a vector of all the values the variable has
		std::map<int, int> maxVar = T.getCounts(max);


		//iterate over the values of the variable
		for (auto it = maxVar.begin(); it != maxVar.end(); it++)
		{
#ifdef _DEBUG
			std::cout << "\nCreating Node at Recursion: " << recursion << "\n";
#endif


			//get a temporary table and the frequencies from current variable's value
			dataTable temp = T.splitData(max, it->first);
			
			
			//call for recursive tree creation
			auto n = createTree(temp, recursion, maxRecursion);

			R.makeChild(n, it->first);
		}

		return R;
	}
	else
	{
		//add to recursion counter
		recursion++;

		//index of variable with highest gain
		int max = highestGain(T, entropy(T));

		int maxVal = -1;
		std::map<int, int> m = targetCounts;


		//set most likely answer for queries with values that dont exist
		if (m.size() > 1)
		{
			for (std::map<int, int>::iterator i = m.begin(); i != m.end(); i++)
			{
				if (i->first > maxVal)
				{
					maxVal = i->first; //return the answer value with the highest probability
				}
			}
		}
		else if (m.size() == 1)
		{
			maxVal = m.begin()->first;
		}
		
		
		//create an interior node and set its variable number
		InteriorNode I(max, maxVal);

		//get a vector of all the values the variable has
		std::map<int, int> maxVar = T.getCounts(max);


		//iterate over the values of the variable
		for (auto it = maxVar.begin(); it != maxVar.end(); it++)
		{
			//get a temporary table and the frequencies from current variable's value
			dataTable temp = T.splitData(max, it->first);

#ifdef _DEBUG
			std::cout << "\nCreating Node at Recursion: " << recursion << "\n";
			std::cout << "Table Size: " << temp.getTableSize() << "\n";
#endif


			//call for recursive tree creation
			auto n = createTree(temp, recursion, maxRecursion);

			I.makeChild(n, it->first);
		}
		return I;
	}	
}

int DecisionTree::query(std::vector<int> variables)
{
	return query(variables, tree);
}


int DecisionTree::query(std::vector<int> variables, Node &N)
{
	int value = variables[N.getVarNum()];
	std::map<int, Node>* children = N.getChildren();

	try
	{
		if (children->at(value).getType() == NodeType::LEAF)
		{
			return children->at(value).getValue();
		}
		else
		{
			return query(variables, children->at(value));
		}
	}
	catch (...)
	{
		return N.getValue();
	}
}

std::string MLearn::DecisionTree::treeText(Node &N)
{
	//different punctuation signify different things about enclosed data

	//a leaf node's value is encased by "<" and ">"
	//an interior node's variable number and children is enclosed in the characters "(" and ")"
	//an entire node is signified with the node's variable number followed by "{" and the contents of each index, and ends with "}"
	//the placeholder character "|" separates every node by coming before the variable number for ease of iterating through text during deserialization

	//a leaf node with the value of 5 at index 2 is signified as |2<5>
	//an interior node with the same leaf node and a variable number of 3 is shown by |3{|2<5>}
	//a root node with variable number 4 that holds the previous interior node at index 1 is |4{1(|3{|2<5>})}
	//and the same root node with a leaf node of value 7 at index 6 is |4{1(|3{|2<5>})|6<7>}

	//the first number in the line of text always signifies the variable number of the root node

	//when the decision tree encounters new data it hasn't yet seen, it will be unable to make a prediction due to its structure
	//therefor, in these circumstances, the tree will pass through the variables it already knows about and will then 
	//make a prediction based on the value that occurs most frequently, when the tree is saved, this value will be stored after the 
	//variable number enclosed in "[" and "]"

	//hopefully this can explain it to future me :P
	


	//temp return value to hold current text
	std::string text;

	//map of node N's children with "int" being the index and "node" being the child
	std::map<int, Node>* children = N.getChildren();


	//add placeholder character
	text.append("|");

	//append the variable number and "{" to signify the beginning of a new node
	text.append(std::to_string(N.getVarNum()));

	//add variables most likely value to text
	text.append("[");
	text.append(std::to_string(N.getValue()));
	text.append("]");

	text.append("{");


	for (auto it = children->begin(); it != children->end(); it++)
	{
		//add placeholder character
		text.append("|");

		//append the variable value
		text.append(std::to_string(it->first));

		//check if leaf node to see what signifying character to use
		if (it->second.getType() == NodeType::LEAF)
		{
			//a leaf node is signified by being in-between the characters "<" and ">"

			text.append("<");

			text.append(std::to_string(it->second.getValue()));

			text.append(">");
		}
		else
		{
			//an interior node is signified by being in-between the characters "(" and ")"

			text.append("(");

			text.append(treeText(it->second));

			text.append(")");
		}
	}

	text.append("}");

	return text;
}


void MLearn::DecisionTree::saveTree(std::string fileName)
{
	//initialize file
	std::fstream file;
	file.open(fileName, std::ios::out);

	//get tree as text
	std::string text = treeText(tree);

	//write to file
	file << text;
}

MLearn::DecisionTree::DecisionTree(std::string fileName)
{
	std::fstream file;
	file.open(fileName, std::ios::in);

	//get line of text that holds the saved tree
	std::string savedData;
	std::getline(file, savedData);
	file.close();

#ifdef _DEBUG
	std::cout << savedData;
#endif


	//temp value to hold the current depth we are reading at
	//this should correspond to a node in the vector "nodes"
	//since it is incremented to correspond to an element in an array, it starts out at -1
	//so that the first time it's incremented it should correspond to "nodes[0]", which is the 
	//highest node in the hierarchy
	int depth = -1;

	//temporary string to hold numbers
	std::string token;

	//vector of node pointers to store temporary nodes
	//elements closer to "nodes[0]" are higher up in the hierarchy
	std::vector<InteriorNode*> nodes;


	//reverse iterate through saved data instead of normal iteration because
	//this way we can create a list of nodes to add to the heirarchy from the bottom up
	for (auto it = savedData.rbegin(); it != savedData.rend(); it++)
	{
		
		switch (*it)
		{
		case '>': //check if the data is a leaf node
		{
			//reverse iterate through savedData until list of leaf nodes is completed
			while (*it != '|')
			{
				//reverse iterate over the digits between '<' and '>', inserting them at the beginning of token to get full number
				while (*++it != '<')
				{
					token.insert(0, 1, *it);
				}


				//create a new leaf node with whole number in token as its value and reset token
				LeafNode* temp = new LeafNode(std::stoi(token));
				token.erase();


				//get index position of leaf node
				while (isdigit(*++it))
				{
					token.insert(0, 1, *it);
				}


				//set 'temp' as child node at 'token' index in node 'N'
				nodes.back()->makeChild(*temp, std::stoi(token));
				delete temp;
				token.erase();
			}
		}
		break;

		case '}': //check if the data is the end of a node and create a new node at the beginning of the list
			depth++;
			nodes.push_back(new InteriorNode);
		break;

		case ']': //check if data is beginning of a node
		{
			depth--;
			

			while (*++it != '[')
			{
				token.insert(0, 1, *it);
			}

			int val = std::stoi(token);
			token.erase();


			while (isdigit(*++it))
			{
				token.insert(0, 1, *it);
			}

			int varNum = std::stoi(token);
			token.erase();


			if (depth != -1)
			{
				it++;

				while (isdigit(*++it))
				{
					token.insert(0, 1, *it);
				}

				int index = std::stoi(token);
				token.erase();
				
				nodes.at(depth)->makeChild(InteriorNode(*nodes.back(), varNum, val), index);
				delete nodes.back();
				nodes.pop_back();
			}
			else
			{
				tree = RootNode(*nodes[0], varNum, val);
				delete nodes[0];
			}
		}
		break;
		}
	}
}


DecisionTree::DecisionTree(std::string dataFile, int targetClass, int numRecursions)
{
	//set table
	table = new dataTable(dataFile, targetClass);
	
	//create tree
	tree = std::get<RootNode>(createTree(*table, 0, numRecursions));

	//check if debug mode
#ifndef _DEBUG
	//delete table
	delete table;
	table = nullptr;
#endif

}
