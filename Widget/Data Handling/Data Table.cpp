#include "Data Table.h"

using namespace Data;

dataTable::dataTable(std::string dataFile, int target)
{
	//set target variable
	targetVar = target;

	//open file
	std::fstream file;
	file.open(dataFile, std::ios::in);

	
	//exception handling
	if (!file)
	{
		throw TABLE_ERROR("File Not Found");
	}
	

	//initialize temporary values
	std::string curLine;
	std::vector<int> curData;

	//current line itteration
	int curLineNum = 0;


	//iterate over lines in file
	while (std::getline(file, curLine))
	{
		//tokenize line into readable data
		curData = tokenize<int>(curLine);

		//temporary map to hold current variables
		std::map<int, int> temp;

		//iterate over variables in current line
		for (int i = 0; i < curData.size(); i++)
		{
			temp[i] = curData[i];
		}

		//push back temporary map
		data.push_back(temp);

		//increase counter
		curLineNum++;
	}

	file.close();

#ifdef _DEBUG
	std::cout << "Copied " << curLineNum << " Lines into memory\n\n";
#endif
}

Data::dataTable::dataTable(int target)
{
	targetVar = target;
}

int Data::dataTable::getTargetClass()
{
	return targetVar;
}

int Data::dataTable::getTableSize()
{
	return data.size();
}

bool Data::dataTable::checkOperate(int varNum)
{
	return (!data.empty() and data[0].contains(varNum));
}

std::map<int, int> Data::dataTable::getCounts(int varNum)
{
	std::map<int, int> counts;

	if (!checkOperate(varNum))
	{
		throw TABLE_ERROR("Cannot Get Counts of Variable");
	}

	for (int i = 0; i < data.size(); i++)
	{
		counts[data[i].at(varNum)]++;
	}

	return counts;
}

void Data::dataTable::removeCol(int varNum)
{
	
	if (!checkOperate(varNum))
	{
		throw TABLE_ERROR("Cannot Remove Column of Variables");
	}

	for (int i = 0; i < data.size(); i++)
	{
		data[i].erase(varNum);
	}
}

void Data::dataTable::addRow(std::map<int, int> row)
{
	if (data.size() != 0 and data.at(0).size() != row.size())
	{
		throw TABLE_ERROR("Cannot Add Row of Size Not Equal to Table");
	}

	data.push_back(row);
}

void Data::dataTable::removeRow(int lineNum)
{
	if (data.size() < lineNum - 1)
	{
		throw TABLE_ERROR("Invalid Line Index");
	}

	//why the heck is there no way to erase a vector element by index number?
	data.erase(data.begin() + lineNum - 1);
}

std::map<int, int> Data::dataTable::getRow(int lineNum)
{
	if (data.size() > lineNum - 1)
	{
		throw TABLE_ERROR("Invalid Line Index");
	}
	
	return std::map<int, int>(data[lineNum]);
}

dataTable Data::dataTable::splitData(int varNum, int varVal)
{
	dataTable temp(targetVar);

	if (!checkOperate(varNum))
	{
		throw TABLE_ERROR("Cannot Split Table Using Variable");
	}

	for (int i = 0; i < data.size(); i++)
	{
		if (data[i].at(varNum) == varVal)
		{
			temp.addRow(data[i]);
		}
	}

	temp.removeCol(varNum);

	return temp;
}

void Data::dataTable::removeData(int varNum, int varVal)
{
	if (!checkOperate(varNum))
	{
		throw TABLE_ERROR("Cannot Split Table Using Variable");
	}

	for (int i = 0; i < data.size(); i++)
	{
		if (data[i].at(varNum) == varVal)
		{
			removeRow(i);
		}
	}
}

TABLE_ERROR::TABLE_ERROR(char const* const message) : std::exception(message) {}

char const* TABLE_ERROR::what() const noexcept
{
	return exception::what();
}