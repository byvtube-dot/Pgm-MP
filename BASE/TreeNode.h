#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <memory>

template <typename T1, typename T2>
class TreeNode : public std::enable_shared_from_this<TreeNode<T1, T2>>
{
public:
	T1 logic;
	T2 list;
	string name;
	shared_ptr<TreeNode<T1, T2>> owner;
	vector<std::shared_ptr<TreeNode<T1, T2>>> nodes;

	TreeNode() : logic(T1()), list(T2()), name(""), owner(nullptr)
	{

	}

	TreeNode(T1 logicVal, T2 listVal, string nameVal) : logic(logicVal), list(listVal), name(nameVal), owner(nullptr)
	{

	}

	void Add(std::shared_ptr<TreeNode<T1, T2>> node)
	{
		node->owner = this->shared_from_this();
		nodes.push_back(node);
	}

	bool Remove(std::shared_ptr<TreeNode<T1, T2>> node)
	{
		auto it = std::find(nodes.begin(), nodes.end(), node);
		if (it != nodes.end())
		{
			(*it)->owner = nullptr; // Remove owner link
			nodes.erase(it);
			return true; // Removal successful
		}
		return false; // Node not found
	}

	void Clear()
	{
		// Clear the owner references of all child nodes
		for (auto& node : nodes)
		{
			if (node)
			{
				node->owner.reset(); // Reset shared_ptr
			}
		}
		nodes.clear(); // Clear the vector
	}
};