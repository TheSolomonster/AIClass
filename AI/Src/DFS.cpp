#include "Precompiled.h"
#include "DFS.h"

using namespace AI;

bool DFS::Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY)
{
	//reset everything
	graph.ResetSearchParams();
	mOpenList.clear();
	mClosedList.clear();
	//add start node to open list
	auto node = graph.GetNode(startX, startY);
	auto endNode = graph.GetNode(endX, endY);
	mOpenList.push_back(node);
	node->opened = true;
	bool found = false;
	while (!found && mOpenList.size() > 0)
	{
		auto node = mOpenList.front();
		if (node == endNode) found = true;
		else
		{
			for (auto& node2 : node->neighbors)
			{
				if (node2 != nullptr && !node2->closed && !node2->opened)
				{
					node2->opened = true;
					node2->parent = node;
					mOpenList.push_front(node2);
				}
			}
			mClosedList.push_back(node);
			node->closed = true;
			mOpenList.remove(node);
		}
	}
	return found;
}