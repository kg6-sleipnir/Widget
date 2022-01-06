#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>


#include "../../Data Handling/Data Table.h"

using namespace Data;


namespace MLearn
{
	class DecisionTree
	{
#ifndef _DEBUG
	private:
#else
	public:
#endif

		dataTable* table = nullptr;

		class Node;
		class LeafNode;
		class InteriorNode;
		class RootNode;

		typedef std::variant<RootNode, InteriorNode, LeafNode> childNode;


		enum class NodeType
		{
			ROOT,
			INTERIOR,
			LEAF
		};


		class Node
		{
		protected:

			//node's most likely predictive value if there is no match for next variable
			int value = 0;

			//the type of node
			//types are: 
			//ROOT, INTERIOR, LEAF
			NodeType type = NodeType::ROOT;

			//which variable the node corresponds to
			int varNum = 0;

			std::map<int, Node> children;

		public:

			NodeType getType();

			int getVarNum();

			int getValue();

			void makeChild(childNode child, int varValue);

			std::map<int, Node>* getChildren();

		};

		class LeafNode : public Node
		{
		public:

			LeafNode(int val);

		};

		class InteriorNode : public Node
		{
		public:

			InteriorNode(int var, int val);

			InteriorNode() { type = NodeType::INTERIOR; }

			InteriorNode(InteriorNode const& N, int var, int val) : InteriorNode(N) { varNum = var; value = val; type = NodeType::INTERIOR; }

		};

		class RootNode : public Node
		{
		public:

			RootNode(int var, int val);

			RootNode() { type = NodeType::ROOT; }

			RootNode(InteriorNode const& N, int var, int val) : Node(N) { varNum = var; value = val; type = NodeType::ROOT; }

		};

		//get entropy of variable's value
		double entropy(dataTable T, int varNum, int varVal);

		//get entropy of variable
		double entropy(Data::dataTable& T, int varNum);

		//get entropy of entire tree
		double entropy(dataTable& T);

		//get information gain based on parent and child entropies
		double infoGain(dataTable& T, double parentEnt, double childEnt);

		//gets variable with highest gain from table
		int highestGain(Data::dataTable& T, double parentEnt);

		//creates the tree structure using recursion
		std::variant<RootNode, InteriorNode, LeafNode> createTree(Data::dataTable T, int recursion, int maxRecursion);

		//query function for recursion
		int query(std::vector<int> variables, Node& N);

		//IDE/compiler wants this to be under the rootNode definition so it will be sad and alone down here
		//final tree structure
		RootNode tree;


	public:

		//make prediction using tree
		int query(std::vector<int> variables);

		//get tree as readable text
		std::string treeText(Node& N);

		//save tree to file
		void saveTree(std::string fileName);

		//load saved tree from file
		DecisionTree(std::string fileName);

		//construct a decision tree from a dataFile and a target class
		DecisionTree(std::string dataFile, int targetClass, int numRecursions);
	};
}